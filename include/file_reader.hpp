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
#include <codecvt>
#include <locale>
using namespace std;


using namespace std;
namespace fs = std::filesystem;

class FileReader {
public:
    // Method to read a single file and return a shared pointer to a Document
    shared_ptr<Document> readDocument(const string& filePath,types type = ANGLAIS, int ngram = 1) {
        ifstream file(filePath);
        if (!file.is_open()) {
            throw runtime_error("Failed to open file: " + filePath);
        }

        string content((istreambuf_iterator<char>(file)), istreambuf_iterator<char>());
        file.close();

        // Extract the file name (title)
        string title = fs::path(filePath).filename().string();

        // Create and return a Document object
        return make_shared<Document>(content, type, ngram, title);
    }

    // Method to read all text files in a folder and return a Corpus
   Corpus readCorpus(const string& folderPath, int ngram) {

    vector<shared_ptr<Document>> documents;

    for (const auto& entry : fs::directory_iterator(folderPath)) {
        if (entry.is_regular_file()) {
            string extension = entry.path().extension().string();
            types fileType;

            if (extension == ".en") {
                fileType = ANGLAIS;
            }else if (extension == ".fr") {
                fileType = FRANCAIS;
            }else if (extension == ".cpp") {
                fileType = CPP;
            } else if (extension == ".py") {
                fileType = PYTHON;
            } else {
                // Skip unsupported file types
                continue;
            }

            try {
                documents.push_back(readDocument(entry.path().string(), fileType,ngram));
            } catch (const exception& e) {
                cerr << "Error reading file: " << entry.path() << " - " << e.what() << endl;
            }
        }
    }

    // Create and return a Corpus object
    return Corpus(documents, ngram);
}
};
#endif