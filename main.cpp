#include "include/document.hpp"
#include "include/corpus.hpp"
#include "include/similarity_analyzer.hpp"
#include "include/file_reader.hpp"
#include "include/plagiarism_detector.hpp"
#include <memory>
// g++ main.cpp -I./include -o main src/*.cpp

int main(int argc, char* argv[]){
    File_reader F(argv[2]);
    cout<<F.content<<endl;
    set<string> stopwords = {"the", "is", "in", "and", "of", "to", "a", "it", "on", "for", "as", "at", "with", "by", "an", "be", "this", "that"};
    int ngram = atoi(argv[1]);

    auto  doc1= make_shared<Document>("Machine learning and artificial intelligence are revolutionizing the way we analyze data. These technologies enable predictive modeling, improve decision-making processes, and create opportunities for innovation. AI applications include natural language processing, computer vision, and robotics.",ANGLAIS, ngram,"ML 1");
    auto doc2 =make_shared<Document>("Machine learning and artificial intelligence are transforming data analysis. These technologies enable predictive modeling, improve decision-making processes, and drive innovation. Applications of AI include natural language processing, computer vision, and robotics.",ANGLAIS, ngram,"ML 2");
    auto doc3=make_shared<Document>("Machine learning and artificial intelligence have revolutionized data analysis. These technologies allow predictive modeling, improve decision-making, and foster innovation. AI applications include natural language processing, computer vision, and robotics.",ANGLAIS,ngram, "ML 3");
    auto doc4= make_shared<Document>("Aziz Loves Machine learning and find that adam is also passionate about it so they work together to build a project",ANGLAIS,ngram, "ML 4");
    
    cout<<"ngram : "<<ngram<<endl;
    Corpus DOCS;
    DOCS.add_document(doc2);
    DOCS.add_document(doc3);
    DOCS.add_document(doc4);
    DOCS.compute_df();
    SimilarityAnalyzer Analyzer(DOCS);
    PlagiarismDetector detector(Analyzer,ngram);
    map<shared_ptr<Document>,double> result =detector.check_plagiarism(*doc1);
    for(auto couple:result){
        cout<<couple.first->title<<": "<<couple.second<<endl;
    }

    // SimilarityAnalyzer analyzer(DOCS);
    // cout<<"FINAL SCORE "<<endl;
    // double cosine = analyzer.compute_similarity(*doc1,*doc4 , SimilarityAnalyzer::COSINE);
    // double jaccard = analyzer.compute_similarity(*doc1, *doc4, SimilarityAnalyzer::JACCARD);
    // double euclidean = analyzer.compute_similarity(*doc1, *doc4, SimilarityAnalyzer::EUCLIDEAN);
    // double manhattan = analyzer.compute_similarity(*doc1, *doc4, SimilarityAnalyzer::MANHATTAN);

    // cout << "Cosine Similarity: " << cosine << endl;
    // cout << "Jaccard Similarity: " << jaccard << endl;
    // cout << "Euclidean Distance: " << euclidean << endl;
    // cout << "Manhattan Distance: " << manhattan << endl;
    // cout<< "Final score "<<analyzer.compute_score(*doc1,*doc4)<<endl;
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