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
        set<string> stopwords;
        map<string, int> df; //  document frequency 
        set<string> tokens_;
        int num_gram ;
        Corpus(){};
        Corpus(vector<shared_ptr<Document>>docs,set<string> stopwords= {
                "the", "and", "is", "in", "at", "of", "a", "an", "on", "for",
                "to", "with", "by", "that", "this", "it", "from"
                
            }):Documents(docs),stopwords(stopwords){};
        explicit Corpus(string FolderName){};
        void add_document(const std::shared_ptr<Document>& doc);
        void compute_df();
    

    
        map<string,double>  compute_tf_idf(Document& doc)const;

        





};
#endif