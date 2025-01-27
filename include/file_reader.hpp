#ifndef FILE_READER_H
#define FILE_READER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <memory>
#include <filesystem>
#include <stdexcept>
#include <codecvt> // For encoding conversions
#include "document.hpp"
#include "corpus.hpp"

namespace fs = std::filesystem;

class FileReader {
public:
    // Method to read a single file and return a shared pointer to a Document
    std::shared_ptr<Document> readDocument(const std::string& filePath, types type = ANGLAIS, int ngram = 1) {
        // Check if the file exists and is a regular file
        if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
            throw std::runtime_error("File does not exist or is not a regular file: " + filePath);
        }

        std::ifstream file(filePath, std::ios::binary); // Open in binary mode to avoid encoding issues
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open file: " + filePath);
        }

        try {
            // Read file content
            std::ostringstream contentStream;
            contentStream << file.rdbuf();
            std::string content = contentStream.str();

            // Debug: Log file size
            // std::cerr << "File: " << filePath << ", Size: " << content.size() << " bytes" << std::endl;

            // Sanitize content
            // content = sanitizeContent(content);

            // Extract file name as title
            std::string title = fs::path(filePath).filename().string();
            // Extract file name without extension
            title = fs::path(filePath).stem().string();

            // Return a Document object
            return std::make_shared<Document>(content, type, ngram, title);
        } catch (const std::exception& e) {
            throw std::runtime_error("Error reading file: " + filePath + " - " + e.what());
        }
    }

    // Method to read all files in a folder and return a Corpus
    Corpus readCorpus(const std::string& folderPath, int ngram = 1) {
        std::vector<std::shared_ptr<Document>> documents;

        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                std::string extension = entry.path().extension().string();
                types fileType;

                // Determine file type based on the extension
                if (extension == ".en") {
                    fileType = ANGLAIS;
                } else if (extension == ".fr") {
                    fileType = FRANCAIS;
                }else if (extension == ".cpp") {
                    fileType = CPP;
                } else if (extension == ".py") {
                    fileType = PYTHON;
                }else {
                    // Skip unsupported file types
                    continue;
                }

                try {
                    // Attempt to read the file and add it to the document list
                    documents.push_back(readDocument(entry.path().string(), fileType, ngram));
                    // cout<<documents[documents.size()-1]->tf.size()<<endl;
                } catch (const std::exception& e) {
                    std::cerr << "Error reading file: " << entry.path() << " - " << e.what() << std::endl;
                }
            }
        }

        // Create and return a Corpus object
        cout<<documents.size()<<" document pour corpus lu."<<endl;
        return Corpus(documents, ngram);
    }

private:
    // Helper method to sanitize content by removing non-printable characters
    std::string sanitizeContent(const std::string& content) {
        std::string sanitized;
        for (char ch : content) {
            if (std::isprint(static_cast<unsigned char>(ch)) || std::isspace(static_cast<unsigned char>(ch))) {
                sanitized += ch;
            }
        }
        return sanitized;
    }
};

#endif
