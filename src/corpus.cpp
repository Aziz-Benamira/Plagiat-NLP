#include "../include/corpus.hpp"


void Corpus::add_document(const std::shared_ptr<Document>& doc){
    Documents.push_back(doc);
    for(const auto& token_pair: doc->tf){
        tokens_.insert(token_pair.first);
        df[token_pair.first]++; // mettre à jour df
    } 
}

void Corpus::compute_df(){
    int N = Documents.size();
    int nb=0;
    df.clear();
    for(const auto& doc: Documents){
        // TODO : à changer utiliser les ngram de TF des docs
        for(const auto& token_pair: doc->tf){
            df[token_pair.first]++;
        }
    }

}


map<string,double> Corpus::compute_tf_idf(const Document& doc) const{
    map<string,double> tf_idf;
    for(const auto& term_pair: doc.tf){
        int dff = 1;
        if(tokens_.find(term_pair.first) != tokens_.end()){
            dff= df.at(term_pair.first);
        }
        double idf = log10(Documents.size()/(double)(dff));
        tf_idf[term_pair.first] = doc.tf.at(term_pair.first) * idf;
    }
    return tf_idf;
}