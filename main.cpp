#include "include/document.hpp"



int main(int argc, char* argv[]){
    set<string> stopwords = {"the", "is", "in", "and", "of", "to", "a", "it", "on", "for", "as", "at", "with", "by", "an", "be", "this", "that"};
    Document doc1("AI AI AI machine machine LOL LOL Valo valo , is good life ");
    doc1.title= "Machine learning a threat ? ";
    doc1.type = ANGLAIS;
    doc1.tokenization(stopwords);
    int n = atoi(argv[1]);

    vector<string> ngrams = doc1.create_ngrams(n);
    for (auto i : ngrams){
        cout<<i<<",";
    }
    cout<<endl;
    doc1.compute_tf(ngrams);
    cout<<doc1<<endl;
    // ngram : frequency , : 
    return 0;
}