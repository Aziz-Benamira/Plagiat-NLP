#ifndef DOCUMENT_H
#define DOCUMENT_H
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <set>
#include <sstream>
using namespace std;

enum types {FRANCAIS, ANGLAIS, PYTHON, CPP};
class Document{
    public:
        string text;
        string title;
        types type;
        vector<string> tokens;
        map<string,double> tf; // la frequence de chaque token
        explicit Document(const std::string& t) : text(t) {}
        
        void tokenization(set<string> stopwords);
        vector<string> create_ngrams(int n) const;
        void compute_tf(vector<string>& ngrams) ;
        void compute_tf(int n) ;
        friend ostream& operator<<(ostream& os, const Document& doc) {
        os << "Document Title: " << doc.title << endl;
        os << "Document Type: ";
        switch (doc.type) {
            case types::FRANCAIS: os << "FRANCAIS"; break;
            case types::ANGLAIS: os << "ANGLAIS"; break;
            case types::PYTHON: os << "PYTHON"; break;
            case types::CPP: os<<"CPP"; break;
            default: os << "Unknown"; break;
        }
        os << "\nText: " << doc.text << endl;
        os << "Nombre de tokens: " << doc.tokens.size()<<endl ;
        // os << "Term Frequencies:"<<endl;
        // for (const auto& pair : doc.tf) {
        //     os << pair.first << ": " << pair.second << endl;
        // }
        std::vector<std::pair<std::string, double>> sorted_tf(doc.tf.begin(), doc.tf.end());
        std::sort(sorted_tf.begin(), sorted_tf.end(), [](const auto& a, const auto& b) {return a.second > b.second;});

        os << "Top 5 most frequent terms:\n";
        for (int i = 0; i < std::min(5, int(sorted_tf.size())); ++i) {
            os << sorted_tf[i].first << ": " << sorted_tf[i].second * 100 << "%\n";
        }
            return os;
        }
};



#endif

