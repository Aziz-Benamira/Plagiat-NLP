#include "./include/httplib.h"
#include "./include/nlohmann/json.hpp"
#include "include/document.hpp"
#include "include/corpus.hpp"
#include "include/similarity_analyzer.hpp"
#include "include/file_reader.hpp"
#include "include/plagiarism_detector.hpp"
#include <memory>
#include <fstream>

using namespace std;
int main() {
    
    FileReader fileReader;
    Corpus corpus;
    string corpus_path = "./Corpus";
    try {
       corpus = fileReader.readCorpus(corpus_path,1);
    } catch (const std::exception& e) {
        std::cerr << "Erreur : " << e.what() << std::endl;
        return 1;
    }
    shared_ptr<Document> doc;

    httplib::Server server;
    server.set_mount_point("/", "./interface Graphique");
    // Handle CORS preflight requests to work security bla bla bla
    server.Options("/detect_plagiarism", [&](const httplib::Request &req, httplib::Response &res) {
        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_header("Access-Control-Allow-Headers", "Content-Type");
        res.set_header("Access-Control-Allow-Methods", "POST, GET, OPTIONS");
        res.set_content("", "text/plain");
    });
    // Handle POST requests to /detect_plagiarism
    server.Post("/detect_plagiarism", [&](const httplib::Request &req, httplib::Response &res) {
        // Parse JSON input
        if (doc) {
            doc.reset();
        }
        cout<<"got a message"<<endl;
        nlohmann::json request_json = nlohmann::json::parse(req.body);
        string type = request_json["type"];
        types t;
            if (type == "anglais") {
                t = ANGLAIS;
            } else if (type == "fr") {
                t = FRANCAIS;
            } else if (type == "c++") {
                t = CPP;
            } else if (type == "py") {
                t = PYTHON;
            } else {
                t = ANGLAIS;
            }
        doc = make_shared<Document>(request_json["text"],t);
        int ngram = 4;
        SimilarityAnalyzer analyzer(corpus);
        PlagiarismDetector detector(analyzer, ngram);
        auto result = detector.check_plagiarism(*doc);
        // pour scores
        std::vector<std::pair<string, double>> sortedResult_string;
        for (const auto& entry : result) {
            sortedResult_string.emplace_back(entry.first->title, entry.second);
        }
        std::sort(sortedResult_string.begin(), sortedResult_string.end(),
              [](const auto& a, const auto& b) {
                  return a.second > b.second;
              });
        
        // pour highlitghted text
        std::vector<std::pair<std::shared_ptr<Document>, double>> sortedResult(result.begin(), result.end());
std::sort(sortedResult.begin(), sortedResult.end(),
          [](const auto& a, const auto& b) {
              return a.second > b.second;
          });
          
         std::vector<std::shared_ptr<Document>> top_documents;
    for (const auto& [doc, score] : sortedResult) {
        top_documents.push_back(doc);
        if (top_documents.size() >= 5) break; // Limiter à 5 documents
    }   
        map<string,int> gram_intensity;
        double final_score = detector.get_final_score(*doc,top_documents,gram_intensity);
        auto word_intensity = detector.get_plagiarized_words_with_intensity(gram_intensity);
    std::string highlighted_text;
    
        highlighted_text = doc->highlight_plagiarism_in_processed_text(word_intensity);
        // Generate and send JSON response
        nlohmann::json response_json;  
        response_json["scores"]= sortedResult_string;
        response_json["final_score"] = final_score;
        response_json["highlighted_text"] = highlighted_text;

        res.set_header("Access-Control-Allow-Origin", "*");
        res.set_content(response_json.dump(), "application/json");
    });
    cout<<"server prêt"<<endl;
    cout<<"lien : "<< "http://localhost:8080/"<<endl;
    server.listen("0.0.0.0", 8080);
    return 0;
}

