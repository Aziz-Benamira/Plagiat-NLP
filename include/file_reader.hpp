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
#include <codecvt> // Pour les conversions d'encodage
#include "document.hpp"
#include "corpus.hpp"

namespace fs = std::filesystem;

// Classe FileReader pour lire des fichiers et des dossiers, et créer des objets Document ou Corpus
class FileReader {
public:
    // Méthode pour lire un fichier et retourner un pointeur partagé vers un Document
    std::shared_ptr<Document> readDocument(const std::string& filePath, types type = ANGLAIS, int ngram = 1) {
        // Vérifie si le fichier existe et est un fichier régulier
        if (!fs::exists(filePath) || !fs::is_regular_file(filePath)) {
            throw std::runtime_error("Le fichier n'existe pas ou n'est pas un fichier régulier : " + filePath);
        }

        std::ifstream file(filePath, std::ios::binary); // Ouvre en mode binaire pour éviter les problèmes d'encodage
        if (!file.is_open()) {
            throw std::runtime_error("Échec de l'ouverture du fichier : " + filePath);
        }

        try {
            // Lit le contenu du fichier
            std::ostringstream contentStream;
            contentStream << file.rdbuf();
            std::string content = contentStream.str();

            // Debug : Affiche la taille du fichier
            // std::cerr << "Fichier : " << filePath << ", Taille : " << content.size() << " octets" << std::endl;

            // Nettoie le contenu (optionnel)
            // content = sanitizeContent(content);

            // Extrait le nom du fichier comme titre
            std::string title = fs::path(filePath).filename().string();
            // Extrait le nom du fichier sans extension
            title = fs::path(filePath).stem().string();

            // Retourne un objet Document
            return std::make_shared<Document>(content, type, ngram, title);
        } catch (const std::exception& e) {
            throw std::runtime_error("Erreur lors de la lecture du fichier : " + filePath + " - " + e.what());
        }
    }

    // Méthode pour lire tous les fichiers d'un dossier et retourner un Corpus
    Corpus readCorpus(const std::string& folderPath, int ngram = 1) {
        std::vector<std::shared_ptr<Document>> documents;

        // Parcourt tous les fichiers du dossier
        for (const auto& entry : fs::directory_iterator(folderPath)) {
            if (entry.is_regular_file()) {
                std::string extension = entry.path().extension().string();
                types fileType;

                // Détermine le type de fichier en fonction de l'extension
                if (extension == ".en") {
                    fileType = ANGLAIS;
                } else if (extension == ".fr") {
                    fileType = FRANCAIS;
                } else if (extension == ".cpp") {
                    fileType = CPP;
                } else if (extension == ".py") {
                    fileType = PYTHON;
                } else {
                    // Ignore les types de fichiers non supportés
                    continue;
                }

                try {
                    // Tente de lire le fichier et l'ajoute à la liste des documents
                    documents.push_back(readDocument(entry.path().string(), fileType, ngram));
                    // cout << documents[documents.size() - 1]->tf.size() << endl; // Debug : Affiche la taille du TF
                } catch (const std::exception& e) {
                    std::cerr << "Erreur lors de la lecture du fichier : " << entry.path() << " - " << e.what() << std::endl;
                }
            }
        }

        // Crée et retourne un objet Corpus
        return Corpus(documents, ngram);
    }

private:
    // Méthode utilitaire pour nettoyer le contenu en supprimant les caractères non imprimables
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