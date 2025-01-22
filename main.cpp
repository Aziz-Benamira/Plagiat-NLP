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

    auto doc1 = make_shared<Document>("Machine learning and artificial intelligence are revolutionizing the way we analyze data. These technologies enable predictive modeling, improve decision-making processes, and create opportunities for innovation. AI applications include natural language processing, computer vision, and robotics.", ANGLAIS, ngram, "ML 1");
    auto doc2 = make_shared<Document>("Machine learning is a subset of artificial intelligence that focuses on building systems that learn from data. It is widely used in applications like recommendation systems and fraud detection.", ANGLAIS, ngram, "ML 2");
    auto doc3 = make_shared<Document>("Computer vision is a field of artificial intelligence that enables machines to interpret and understand visual data. It is used in applications like facial recognition and autonomous vehicles.", ANGLAIS, ngram, "ML 3");
    auto doc4 = make_shared<Document>("Aziz is passionate about machine learning and enjoys working on projects that involve data analysis and predictive modeling.", ANGLAIS, ngram, "ML 4");
    auto doc5 = make_shared<Document>("Innovation in technology is driving changes across various industries. From healthcare to finance, new tools and methods are being developed to solve complex problems.", ANGLAIS, ngram, "ML 5");
    auto doc6 = make_shared<Document>("Data science is an interdisciplinary field that uses scientific methods to extract knowledge from data. It combines elements of statistics, machine learning, and domain expertise.", ANGLAIS, ngram, "ML 6");
    auto doc7 = make_shared<Document>("Robotics is a branch of engineering that involves the design and construction of robots. These machines can perform tasks autonomously or with minimal human intervention.", ANGLAIS, ngram, "ML 7");

    cout << "ngram : " << ngram << endl;

    // Créer le corpus
    Corpus DOCS;
    DOCS.add_document(doc2);
    DOCS.add_document(doc3);
    DOCS.add_document(doc4);
    DOCS.add_document(doc5);
    DOCS.add_document(doc6);
    DOCS.add_document(doc7);
    DOCS.compute_df();

    // Créer le détecteur de plagiat
    SimilarityAnalyzer Analyzer(DOCS);
    PlagiarismDetector detector(Analyzer, ngram);

    // map<shared_ptr<Document>,double> result =detector.check_plagiarism(*doc1);
    // for(auto couple:result){
    //     cout<<couple.first->title<<": "<<couple.second<<endl;
    // }
    auto word_intensity = detector.get_plagiarized_words_with_intensity(*doc1);

    // Afficher les résultats
    cout << "Words from doc_to_test with intensity in corpus:\n";
    for (const auto& [word, intensity] : word_intensity) {
        cout << word << ": " << intensity << endl;
    }

    // Highlight les parties plagiées dans le texte traité
    std::string highlighted_text = doc1->highlight_plagiarism_in_processed_text(word_intensity);
    cout << "Highlighted Text (Processed):\n" << highlighted_text << endl;







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