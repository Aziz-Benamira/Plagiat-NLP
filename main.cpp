#include "include/document.hpp"
#include "include/corpus.hpp"
#include <memory>
// g++ main.cpp -I./include -o main src/*.cpp

static double compute_cosine_similarity(map<string,double> tf_idf_1, map<string,double> tf_idf_2) { // APPROVED
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
int main(int argc, char* argv[]){
    set<string> stopwords = {"the", "is", "in", "and", "of", "to", "a", "it", "on", "for", "as", "at", "with", "by", "an", "be", "this", "that"};
    auto  doc1= make_shared<Document>("Machine learning and artificial intelligence are revolutionizing the way we analyze data. These technologies enable predictive modeling, improve decision-making processes, and create opportunities for innovation. AI applications include natural language processing, computer vision, and robotics.",ANGLAIS, "ML 1");
    auto doc2 =make_shared<Document>("Machine learning and artificial intelligence are transforming data analysis. These technologies enable predictive modeling, improve decision-making processes, and drive innovation. Applications of AI include natural language processing, computer vision, and robotics.",ANGLAIS, "ML 2");
    auto doc3=make_shared<Document>("Machine learning and artificial intelligence have revolutionized data analysis. These technologies allow predictive modeling, improve decision-making, and foster innovation. AI applications include natural language processing, computer vision, and robotics.",ANGLAIS, "ML 3");
    auto doc4= make_shared<Document>("Aziz Loves Machine learning and find that adam is also passionate about it so they work together to build a project",ANGLAIS, "ML 4");
    int ngram = atoi(argv[1]);
    cout<<"ngram : "<<ngram<<endl;
    doc1->tokenization();
    doc2->tokenization();
    doc3->tokenization();
    doc4->tokenization();
    doc1->compute_tf(ngram);
    doc2->compute_tf(ngram);
    doc3->compute_tf(ngram);
    doc4->compute_tf(ngram);
    Corpus DOCS;
    DOCS.add_document(doc2);
    DOCS.add_document(doc3);
    DOCS.add_document(doc4);
    DOCS.compute_df();
    map<string,double> doc1_tfidf = DOCS.compute_tf_idf(*doc1);
    
    map<string,double> doc2_tfidf = DOCS.compute_tf_idf(*doc2);
    for(auto terms: doc2_tfidf){
        cout<<terms.first << ":" <<terms.second<<endl;
    }
    cout<<"FINAL SCORE "<<endl;
    cout<<compute_cosine_similarity(doc1_tfidf, doc2_tfidf)<<endl;
    cout<<"Check : " <<  compute_cosine_similarity(doc1_tfidf,doc1_tfidf);
    return 0;

}
// inside docs
// about:0.0367016
// adam:0.0367016
// also:0.0367016
// aziz:0.0367016
// build:0.0367016
// find:0.0367016
// learning:0.0367016
// loves:0.0367016
// machine:0.0367016
// passionate:0.0367016
// project:0.0367016
// together:0.0367016
// work:0.0367016

// about:0.0231562
// adam:0.0231562
// also:0.0231562
// aziz:0.0231562
// build:0.0231562
// find:0.0231562
// learning:0.0231562
// loves:0.0231562
// machine:0.0231562
// passionate:0.0231562
// project:0.0231562
// together:0.0231562
// work:0.0231562