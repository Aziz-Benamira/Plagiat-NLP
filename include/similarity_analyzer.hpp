#ifndef SIMILARITY_ANALYZER_H
#define SIMILARITY_ANALYZER_H

#include "corpus.hpp"
#include "document.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <map>
#include <algorithm>
#include <regex>
#include <set>
#include <sstream>
#include <math.h>

using namespace std;

class SimilarityAnalyzer {
public:
    // Méthodes de calcul de similarité disponibles
    enum Method {
        COSINE,      // Similarité cosinus
        JACCARD,     // Similarité de Jaccard
        BHATTACHARYYA, // Distance de Bhattacharyya
        EUCLIDEAN,   // Distance euclidienne
        MANHATTAN    // Distance de Manhattan
    };

    Corpus LeCorpus; // Corpus de documents

    // Constructeurs
    SimilarityAnalyzer() {};
    explicit SimilarityAnalyzer(Corpus LeCorpus) : LeCorpus(LeCorpus) {};

    // Fonction principale pour calculer la similarité entre deux documents
    double compute_similarity(const Document& doc1, const Document& doc2, Method method) {
        switch (method) {
        case COSINE:
            return compute_cosine_similarity(doc1, doc2);
        case JACCARD:
            return compute_jaccard_similarity(doc1, doc2);
        case EUCLIDEAN:
            return compute_euclidean_similarity(doc1, doc2);
        case MANHATTAN:
            return compute_manhattan_distance(doc1, doc2);
        case BHATTACHARYYA:
            return compute_bhattacharyya_distance(doc1, doc2);
        default:
            return -1; // Retourne -1 si la méthode n'est pas reconnue
        }
    }

    // Fonction pour calculer un score global de similarité en combinant plusieurs méthodes
    double compute_score(const Document& doc1, const Document& doc2) {
        double result = 0.0;
        for (int method = COSINE; method <= BHATTACHARYYA; ++method) {
            double temp = compute_similarity(doc1, doc2, static_cast<Method>(method));
            result += temp;
        }
        return result / 3; // Retourne la moyenne des scores de similarité
    }

private:
    // Fonction pour calculer la similarité cosinus entre deux documents
    double compute_cosine_similarity(const Document& doc1, const Document& doc2) {
        map<string, double> tf_idf_1 = LeCorpus.compute_tf_idf(doc1);
        map<string, double> tf_idf_2 = LeCorpus.compute_tf_idf(doc2);
        double dot_product = 0.0, magnitude_doc1 = 0.0, magnitude_doc2 = 0.0;

        // Filtrage des termes communs
        map<string, double> filtered_tf_idf_2;
        for (const auto& pair : tf_idf_2) {
            if (tf_idf_1.find(pair.first) != tf_idf_1.end()) {
                filtered_tf_idf_2[pair.first] = pair.second;
            }
        }

        // Application de la fonction softmax
        tf_idf_2 = apply_softmax(filtered_tf_idf_2);
        tf_idf_1 = apply_softmax(tf_idf_1);

        // Calcul du produit scalaire et des magnitudes
        for (const auto& ngram : tf_idf_1) {
            if (tf_idf_2.find(ngram.first) != tf_idf_2.end()) {
                dot_product += ngram.second * tf_idf_2.at(ngram.first);
                magnitude_doc2 += tf_idf_2.at(ngram.first) * tf_idf_2.at(ngram.first);
            }
            magnitude_doc1 += ngram.second * ngram.second;
        }

        if (magnitude_doc1 == 0 || magnitude_doc2 == 0) {
            return 0.0;
        }

        return dot_product / (sqrt(magnitude_doc1) * sqrt(magnitude_doc2));
    }

    // Fonction pour appliquer la fonction softmax à un map de tf-idf
    map<string, double> apply_softmax(const map<string, double>& tf_idf) {
        map<string, double> softmax_result;
        double sum_exp = 0.0;

        // Calcul de la somme des exponentielles
        for (const auto& pair : tf_idf) {
            sum_exp += exp(pair.second);
        }

        // Application de la formule softmax
        for (const auto& pair : tf_idf) {
            softmax_result[pair.first] = exp(pair.second) / sum_exp;
        }

        return softmax_result;
    }

    // Fonction pour calculer la distance de Bhattacharyya entre deux documents
    double compute_bhattacharyya_distance(const Document& doc1, const Document& doc2) {
        map<string, double> tf_idf_1 = apply_softmax(LeCorpus.compute_tf_idf(doc1));
        map<string, double> tf_idf_2 = LeCorpus.compute_tf_idf(doc2);

        // Filtrage des termes communs
        map<string, double> filtered_tf_idf_2;
        for (const auto& pair : tf_idf_2) {
            if (tf_idf_1.find(pair.first) != tf_idf_1.end()) {
                filtered_tf_idf_2[pair.first] = pair.second;
            }
        }

        tf_idf_2 = apply_softmax(filtered_tf_idf_2);
        double result = 0.0;

        // Calcul de la distance de Bhattacharyya
        for (const auto& ngram : tf_idf_1) {
            if (tf_idf_2.find(ngram.first) != tf_idf_2.end()) {
                result += sqrt(ngram.second * tf_idf_2.at(ngram.first));
            }
        }

        return 1 + max(-1.0, log(result));
    }

    // Fonction pour calculer la similarité de Jaccard entre deux documents
    static double compute_jaccard_similarity(const Document& doc1, const Document& doc2) {
        std::set<std::string> ngrams1, ngrams2;

        // Récupération des n-grams des deux documents
        for (const auto& ngram : doc1.tf) {
            ngrams1.insert(ngram.first);
        }
        for (const auto& ngram : doc2.tf) {
            ngrams2.insert(ngram.first);
        }

        // Calcul de l'intersection
        std::set<std::string> intersection;
        std::set_intersection(ngrams1.begin(), ngrams1.end(), ngrams2.begin(), ngrams2.end(),
                              std::inserter(intersection, intersection.begin()));

        return (double)intersection.size() / ngrams1.size();
    }

    // Fonction pour calculer la distance euclidienne entre deux documents
    double compute_euclidean_similarity(const Document& doc1, const Document& doc2) {
        map<string, double> tf_idf_1 = LeCorpus.compute_tf_idf(doc1);
        map<string, double> tf_idf_2 = LeCorpus.compute_tf_idf(doc2);
        set<string> a;
        double result = 0.0;

        // Calcul de la distance euclidienne
        for (const auto& tf_term : tf_idf_1) {
            a.insert(tf_term.first);
            if (tf_idf_2.find(tf_term.first) != tf_idf_2.end()) {
                result += (tf_term.second - tf_idf_2.at(tf_term.first)) * (tf_term.second - tf_idf_2.at(tf_term.first));
            } else {
                result += tf_term.second * tf_term.second;
            }
        }

        for (const auto& tf_term : tf_idf_2) {
            if (tf_idf_1.find(tf_term.first) == tf_idf_1.end()) {
                result += tf_term.second * tf_term.second;
                a.insert(tf_term.first);
            }
        }

        double distance = sqrt(result);
        double norm = sqrt(1.0 / a.size());
        return 1 - pow(distance / norm, 1.0 / 6);
    }

    // Fonction pour calculer la distance de Manhattan entre deux documents
    double compute_manhattan_distance(const Document& doc1, const Document& doc2) {
        map<string, double> tf_idf_1 = LeCorpus.compute_tf_idf(doc1);
        map<string, double> tf_idf_2 = LeCorpus.compute_tf_idf(doc2);
        double result = 0.0;

        // Calcul de la distance de Manhattan
        for (const auto& tf_term : tf_idf_1) {
            if (tf_idf_2.find(tf_term.first) != tf_idf_2.end()) {
                result += abs(tf_term.second - tf_idf_2.at(tf_term.first));
            } else {
                result += tf_term.second;
            }
        }

        for (const auto& tf_term : tf_idf_2) {
            if (tf_idf_1.find(tf_term.first) == tf_idf_1.end()) {
                result += tf_term.second;
            }
        }

        return 1 - pow(result, 1.0 / 6);
    }
};

#endif