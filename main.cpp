#include "include/document.hpp"
#include "include/corpus.hpp"
#include "include/similarity_analyzer.hpp"
#include "include/file_reader.hpp"
#include "include/plagiarism_detector.hpp"
#include <memory>
#include <fstream>
using namespace std;
// Define the toString function for the types enumeration

string toString(types t) {
    switch (t) {
        case ANGLAIS: return "anglais";
        case FRANCAIS: return "français";
        case CPP: return "c++";
        case PYTHON: return "python";
        default: return "unknown";
    }
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        std::cerr << "Usage: " << argv[0] << " <test_file> <type> <corpus_path> <output_mode>\n";
        std::cerr << "Output mode: 'terminal' or 'file'\n";
        return 1;
    }

    std::string test_file = argv[1];
    std::string type = argv[2];
    std::string corpus_path = argv[3];
    std::string output_mode = argv[4];

    types t;
    if (type == "anglais") {
        t = ANGLAIS;
    } else if (type == "français") {
        t = FRANCAIS;
    } else if (type == "c++") {
        t = CPP;
    } else if (type == "py") {
        t = PYTHON;
    } else {
        t = ANGLAIS;
    }

    // Initialize file reader and corpus
    FileReader fileReader;
    Corpus corpus;
    shared_ptr<Document> doc;

    try {
        doc = fileReader.readDocument(test_file, t);
        corpus = fileReader.readCorpus(corpus_path,t);
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }

    // Redirect output to file if needed
    std::ofstream output_file;
    if (output_mode == "file") {
        output_file.open("output.txt", std::ios::out | std::ios::binary);
        if (!output_file.is_open()) {
            std::cerr << "Erreur : Impossible d'ouvrir le fichier output.txt" << std::endl;
            return 1;
        }
        output_file << "\xEF\xBB\xBF"; // UTF-8 BOM
        std::cout.rdbuf(output_file.rdbuf());
    }

    // -----------------------------
    // Generate output
    // -----------------------------
    std::cout << "\n";
    std::cout << "# Détecteur De Plagiat\n\n";
    std::cout << "### Langue du document à tester : " << toString(t) << "\n";
    std::cout << "### Titre du document à tester : " << doc->title << "\n";

    int ngram = 4;
    SimilarityAnalyzer analyzer(corpus);
    PlagiarismDetector detector(analyzer, ngram);

    auto result = detector.check_plagiarism(*doc);
    std::cout << "### Résultats de plagiat\n\n";
    for (const auto& couple : result) {
        cout << "- Document : " << couple.first->title 
                  << " (" << couple.second * 100 << "% de similitude)\n";
    }

    auto word_intensity = detector.get_plagiarized_words_with_intensity(*doc);
    std::string highlighted_text;

    if (output_mode == "file") {
        highlighted_text = doc->highlight_plagiarism_in_processed_text(word_intensity);
    } else {
        highlighted_text = doc->highlight_plagiarism_in_terminal(word_intensity);
    }

    std::cout << "\n\n\n";
    std::cout << "### Ceci est le schéma des couleurs utilisé pour surligner les mots plagiés :\n";
    if (output_mode == "file") {
        std::cout << "<red>rouge</red>, pour une intensité élevée\n";
        std::cout << "<yellow>doré</yellow>, pour une intensité moyenne\n";
        std::cout << "<magenta>magenta</magenta>, pour une intensité faible.\n\n";
    } else {
        std::cout << "\033[31mrouge\033[0m, pour une intensité élevée\n";
        std::cout << "\033[33mdoré\033[0m, pour une intensité moyenne\n";
        std::cout << "\033[35mmagenta\033[0m, pour une intensité faible.\n\n";
    }

    cout << "\n### Texte surligné\n\n";
    cout << highlighted_text << "\n";

    // Close the file if needed
    if (output_mode == "file") {
        output_file.close();
    }

    return 0;
}


    // Créer le corpus

    // Corpus DOCS;
    // DOCS.add_document(doc2);
    // DOCS.add_document(doc3);
    // DOCS.add_document(doc4);
    // DOCS.add_document(doc5);
    // DOCS.add_document(doc6);
    // DOCS.add_document(doc7);
    // DOCS.compute_df();
    // doc1->compute_tf(2);
    // doc4->compute_tf(2);
    // DOCS.compute_df();
    // auto test = DOCS.compute_tf_idf(*doc1);
    // auto test1 = DOCS.compute_tf_idf(*doc4);
    // Créer le détecteur de plagiat
    // SimilarityAnalyzer analyzer(DOCS);
    // PlagiarismDetector detector(analyzer, ngram);
    // double resultt = 0.0;
    // // for (int method = COSINE; method <= MANHATTAN; ++method) {
    // for (int method = SimilarityAnalyzer::COSINE; method <= SimilarityAnalyzer::BHATTACHARYYA; ++method) {
    //     resultt =analyzer.compute_similarity(*doc1, *doc5, static_cast<SimilarityAnalyzer::Method>(method));
    //     cout<<method <<":"<<resultt<<endl;
    // }
    // return result/4;
    
    // map<shared_ptr<Document>,double> result =detector.check_plagiarism(*doc1);
    // for(auto couple:result){
    //     cout<<couple.first->title<<": "<<couple.second<<endl;
    // }
    // auto word_intensity = detector.get_plagiarized_words_with_intensity(*doc1);

    // // Afficher les résultats
    // cout << "Words from doc_to_test with intensity in corpus:\n";
    // for (const auto& [word, intensity] : word_intensity) {
    //     cout << word << ": " << intensity << endl;
    // }

    


// auto doc1 = make_shared<Document>("Machine learning and artificial intelligence are revolutionizing the way we analyze data. These technologies enable predictive modeling, improve decision-making processes, and create opportunities for innovation. AI applications include natural language processing, computer vision, and robotics.", ANGLAIS, ngram, "ML 1");
    // auto doc2 = make_shared<Document>("Machine learning and artificial intelligence are transforming the way we analyze data. These technologies enable predictive analytics, improve decision-making, and drive innovation. Applications of AI include natural language processing, computer vision, and robotics.",ANGLAIS, ngram, "ML 2");
    // auto doc3 = make_shared<Document>("Artificial intelligence and machine learning are revolutionizing data analysis. These technologies allow predictive modeling, improve decision-making, and foster innovation. AI applications include natural language processing, computer vision, and robotics.",ANGLAIS, ngram, "ML 3");
    // auto doc4 = make_shared<Document>("Aziz is passionate about machine learning and enjoys working on projects that involve data analysis and predictive modeling.", ANGLAIS, ngram, "ML 4");
    // auto doc5 = make_shared<Document>("Machine learning and artificial intelligence are revolutionizing the way we analyze data. These technologies enable predictive modeling, improve decision-making processes, and create opportunities for innovation. AI applications include natural language processing, computer vision, and robotics.", ANGLAIS, ngram, "ML 5");
    // auto doc6 = make_shared<Document>("Data science is an interdisciplinary field that uses scientific methods to extract knowledge from data. It combines elements of statistics, machine learning, and domain expertise.", ANGLAIS, ngram, "ML 6");
    // auto doc7 = make_shared<Document>("Robotics is a branch of engineering that involves the design and construction of robots. These machines can perform tasks autonomously or with minimal human intervention.", ANGLAIS, ngram, "ML 7");




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

    // Highlight les parties plagiées dans le texte traité
    // string highlighted_text = doc1->highlight_plagiarism_in_processed_text(word_intensity);
    // cout << "Highlighted Text (Processed):\n" << highlighted_text << endl;
    
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