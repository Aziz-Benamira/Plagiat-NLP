#include "../include/corpus.hpp"


void Corpus::add_document(const std::shared_ptr<Document>& doc){
    Documents.push_back(doc);
    for(const auto& token: doc->tokens){
        tokens_.insert(token);
    } 
}

void Corpus::compute_df(){
    int N = Documents.size();
    int nb=0;
    for(const auto& doc: Documents){
        for(const auto& token: doc->unique_tokens){
            df[token]++;
        }
    }

}


map<string,double> Corpus::compute_tf_idf(Document& doc) const{
    map<string,double> tf_idf;
    for(const auto& term: doc.tokens){
        int dff = 0;
        if(tokens_.find(term) != tokens_.end()){
            dff== df.at(term);
        }
        double idf = log10(Documents.size()/(1.0+dff));
        tf_idf[term] = doc.tf[term] * idf;
    }
    return tf_idf;
}