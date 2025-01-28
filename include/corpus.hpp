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

class Corpus {
    public :
        vector<shared_ptr<Document>> Documents;
        map<string, int> df; //  document frequency 
        set<string> tokens_;
        int num_gram ;
        Corpus(){};
        Corpus(vector<shared_ptr<Document>>docs, int ngram):Documents(docs),num_gram(ngram){
                compute_df();
            };
        explicit Corpus(string FolderName){};
        auto begin() { return Documents.begin(); }
        auto end() { return Documents.end(); }
        auto begin() const { return Documents.begin(); }
        auto end() const { return Documents.end(); }
        size_t size() const {
            return Documents.size();
        }
        void add_document(const std::shared_ptr<Document>& doc);
        void compute_df();
        map<string,double>  compute_tf_idf(const Document& doc)const;
};
#endif