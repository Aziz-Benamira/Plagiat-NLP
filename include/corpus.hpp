#ifndef CORPUS_H
#define CORPUS_H

#include <iostream>
#include <vector>
#include "document.hpp"
#include <memory>
#include <map>
#include <algorithm>
#include <regex>
#include <set>
#include <sstream>
#include <math.h>
using namespace std;

// Classe Corpus pour représenter une collection de documents
class Corpus {
    public:
        vector<shared_ptr<Document>> Documents;  // Liste des documents dans le corpus
        map<string, int> df;                     // Fréquence des termes dans les documents (Document Frequency)
        set<string> tokens_;                     // Ensemble des tokens uniques dans le corpus
        int num_gram;                            // Taille des n-grammes utilisés pour l'analyse

        // Constructeur par défaut
        Corpus(){};

        // Constructeur avec une liste de documents et une taille de n-gramme
        Corpus(vector<shared_ptr<Document>> docs, int ngram) : Documents(docs), num_gram(ngram) {
            compute_df();  // Calcule la fréquence des termes dans les documents
        };

        // Constructeur explicite pour charger un corpus à partir d'un dossier
        explicit Corpus(string FolderName){};
        
        // Méthodes pour itérer sur les documents
        auto begin() { return Documents.begin(); }
        auto end() { return Documents.end(); }
        auto begin() const { return Documents.begin(); }
        auto end() const { return Documents.end(); }
        size_t size() const {
            return Documents.size();
        }
        void add_document(const std::shared_ptr<Document>& doc);

        // Calcule la fréquence des termes dans les documents (Document Frequency)
        void compute_df();
        map<string,double>  compute_tf_idf(const Document& doc)const;
};
#endif