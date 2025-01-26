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
    // TODO : euclidien calculator
    // TODO : Manhattan calculator
    enum Method {
        COSINE,
        JACCARD,
        BHATTACHARYYA,
        EUCLIDEAN,
        MANHATTAN
    };
    Corpus LeCorpus;
    SimilarityAnalyzer(){};
    explicit SimilarityAnalyzer(Corpus LeCorpus): LeCorpus(LeCorpus){};
    double compute_similarity(const Document& doc1, const Document& doc2, Method method) {
        switch (method) {
        case COSINE:
            return compute_cosine_similarity(doc1, doc2);
        case JACCARD:
            return compute_jaccard_similarity(doc1, doc2);
        case EUCLIDEAN:
            return compute_euclidean_similarity(doc1,doc2);
        case MANHATTAN:
            return compute_manhattan_distance(doc1,doc2);
        case BHATTACHARYYA:
            return compute_bhattacharyya_distance(doc1,doc2);
        default:
            return -1;
        }
    }
    double compute_score(const Document& doc1, const Document& doc2){
        double result = 0.0;
        // for (int method = COSINE; method <= MANHATTAN; ++method) {
        for (int method = COSINE; method <= BHATTACHARYYA; ++method) {
            result +=compute_similarity(doc1, doc2, static_cast<Method>(method));
        }
        // return result/4;
        return result/3; 
    }
    
private:
    double compute_cosine_similarity(const Document& doc1, const Document& doc2) { // APPROVED
        map<string,double> tf_idf_1 = LeCorpus.compute_tf_idf(doc1);
        map<string,double> tf_idf_2 = LeCorpus.compute_tf_idf(doc2);
        double dot_product = 0.0, magnitude_doc1 = 0.0, magnitude_doc2 = 0.0;

        for (const auto& ngram : tf_idf_1) {
            if (tf_idf_2.find(ngram.first) != tf_idf_2.end()) { // if the ngram is common
                dot_product += ngram.second * tf_idf_2.at(ngram.first);
            }
            magnitude_doc1 += ngram.second * ngram.second;
        }

        for (const auto& ngram : tf_idf_2) {
            magnitude_doc2 += ngram.second * ngram.second;
        }

        if (magnitude_doc1 == 0 || magnitude_doc2 == 0) {
            return 0.0;
        }

        return dot_product / (sqrt(magnitude_doc1) * sqrt(magnitude_doc2));
    }
    map<string, double> apply_softmax(const map<string, double>& tf_idf) {
        map<string, double> softmax_result;

        // Step 1: Calculate the sum of exponentials of tf-idf values
        double sum_exp = 0.0;
        for (const auto& pair : tf_idf) {
            sum_exp += exp(pair.second);
        }

        // Step 2: Apply the softmax formula (exp(tf-idf) / sum(exp(tf-idf)))
        for (const auto& pair : tf_idf) {
            softmax_result[pair.first] = exp(pair.second) / sum_exp;
        }

        return softmax_result;
    }
    double compute_bhattacharyya_distance (const Document& doc1, const Document& doc2) { // APPROVED
        map<string,double> tf_idf_1 = apply_softmax(LeCorpus.compute_tf_idf(doc1));
        map<string,double> tf_idf_2 = apply_softmax(LeCorpus.compute_tf_idf(doc2));
        double dot_product = 0.0;
        double result=0.0;

        for (const auto& ngram : tf_idf_1) {
            if (tf_idf_2.find(ngram.first) != tf_idf_2.end()) { // if the ngram is common
                result+=  sqrt(ngram.second * tf_idf_2.at(ngram.first));
            }
        }
        // cout <<doc1.title<< ": " <<doc1.title <<" : result:"<< log(result) << "score " << 1 +max(-1.0,log(result))<<endl;
        return 1 +max(-1.0,log(result));
    }
    static double compute_jaccard_similarity(const Document& doc1, const Document& doc2) { // APPROVED
        std::set<std::string> ngrams1, ngrams2;

        for (const auto& ngram : doc1.tf) {
            ngrams1.insert(ngram.first);
        }
        for (const auto& ngram : doc2.tf) {
            ngrams2.insert(ngram.first);
        }

        std::set<std::string> intersection, union_set;
        std::set_intersection(ngrams1.begin(), ngrams1.end(), ngrams2.begin(), ngrams2.end(),
                              std::inserter(intersection, intersection.begin()));
        std::set_union(ngrams1.begin(), ngrams1.end(), ngrams2.begin(), ngrams2.end(),
                       std::inserter(union_set, union_set.begin()));

        return (double)intersection.size() / union_set.size();
    }
    
    double compute_euclidean_similarity(const Document& doc1, const Document& doc2) {
        map<string,double> tf_idf_1 = LeCorpus.compute_tf_idf(doc1);
        map<string,double> tf_idf_2 = LeCorpus.compute_tf_idf(doc2);
        set<string> a;
        double result = 0.0;

        for (const auto& tf_term : tf_idf_1) {
            a.insert(tf_term.first);
            if (tf_idf_2.find(tf_term.first) != tf_idf_2.end()) { // if the ngram is common
                // cout<<tf_term.first<<"First doc : "<<tf_term.second<<" Second doc : "<<tf_idf_2.at(tf_term.first)<<endl; 
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
        double norm = sqrt(1.0/a.size());
        // return 1 / (1 + distance); 
        return 1-pow(distance/(norm),1.0/6);
    }
    double compute_manhattan_distance(const Document& doc1, const Document& doc2){
        map<string,double> tf_idf_1 = LeCorpus.compute_tf_idf(doc1);
        map<string,double> tf_idf_2 = LeCorpus.compute_tf_idf(doc2);
        double result = 0.0;
       

        for (const auto& tf_term : tf_idf_1) {
           
            if (tf_idf_2.find(tf_term.first) != tf_idf_2.end()) { // if the ngram is common
                result+= abs(tf_term.second - tf_idf_2.at(tf_term.first));
            }else{
                result += tf_term.second ;
            }
        }

        for (const auto& tf_term : tf_idf_2) {
            if (tf_idf_1.find(tf_term.first) == tf_idf_1.end()){
                result += tf_term.second;
            
            }
        }

        // return 1/(1+result);
        return  1-pow(result,1.0/6);
    }
    
};














#endif