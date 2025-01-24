#ifndef FILE_READER_H
#define FILE_READER_H
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <set>
#include "document.hpp"
#include "corpus.hpp"

using namespace std;
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <set>

using namespace std;
namespace fs = std::filesystem;

class FileReader {
public:
    // Method to read a single file and return a shared pointer to a Document
    shared_ptr<Document> readDocument(const string& filePath,types type = ANGLAIS, int tf_num = 1) {
        ifstream file(filePath);
        if (!file.is_open()) {
            throw runtime_error("Failed to open file: " + filePath);
        }

        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        // Extract the file name (title)
        string title = fs::path(filePath).filename().string();

        // Create and return a Document object
        return make_shared<Document>(content, type, tf_num, title);
    }

    // Method to read all text files in a folder and return a Corpus
   Corpus readCorpus(const string& folderPath, set<string> stopwords = {
        "the", "and", "is", "in", "at", "of", "a", "an", "on", "for",
        "to", "with", "by", "that", "this", "it", "from"}) {

    vector<shared_ptr<Document>> documents;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            string extension = entry.path().extension().string();
            types fileType;

            if (extension == ".txt") {
                fileType = ANGLAIS;
            } else if (extension == ".cpp") {
                fileType = CPP;
            } else if (extension == ".py") {
                fileType = PYTHON;
            } else {
                // Skip unsupported file types
                continue;
            }

            try {
                documents.push_back(readDocument(entry.path().string(), fileType,1));
            } catch (const exception& e) {
                cerr << "Error reading file: " << entry.path() << " - " << e.what() << endl;
            }
        }
    }

    // Create and return a Corpus object
    return Corpus(documents, stopwords);
}
};
#endif