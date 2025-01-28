#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
#include <regex>
#include <set>
#include <sstream>
using namespace std;

// Enumération des types de documents
enum types {FRANCAIS, ANGLAIS, PYTHON, CPP};

// Classe Document pour représenter un document texte
class Document {
    public:
        string text;                // Contenu textuel du document
        types type;                 // Type de document (FRANCAIS, ANGLAIS, etc.)
        string title;               // Titre du document
        vector<string> tokens;      // Liste des tokens (mots) après tokenisation
        map<string, double> tf;     // Fréquence de chaque token (Term Frequency)

        // Constructeur du document
        explicit Document(const std::string& text, types type = types::ANGLAIS, int ngram = 1, const std::string& title = "Unknown")
            : text(text), type(type), title(title), ngram(ngram) {
            tokenization();          // Tokenisation du texte
            compute_tf(ngram);       // Calcul de la fréquence des termes
        }

        // Tokenisation avec des stopwords (mots à ignorer)
        void tokenization(set<string> stopwords);

        // Tokenisation sans stopwords
        void tokenization();

        // Crée des n-grammes à partir des tokens
        vector<string> create_ngrams(int n) const;

        // Calcule la fréquence des termes (TF) à partir des n-grammes
        void compute_tf(vector<string>& ngrams);

        // Calcule la fréquence des termes (TF) pour un n-gramme donné
        void compute_tf(int n,int top_ngram=500);

        // Surcharge de l'opérateur << pour afficher les informations du document
        friend ostream& operator<<(ostream& os, const Document& doc) {
            os << "Titre du document: " << doc.title << endl;
            os << "Type de document: ";
            switch (doc.type) {
                case types::FRANCAIS: os << "FRANCAIS"; break;
                case types::ANGLAIS: os << "ANGLAIS"; break;
                case types::PYTHON: os << "PYTHON"; break;
                case types::CPP: os << "CPP"; break;
                default: os << "Inconnu"; break;
            }
            os << "\nTexte: " << doc.text << endl;
            os << "Nombre de tokens: " << doc.tokens.size() << endl;

            // Tri des termes par fréquence décroissante
            std::vector<std::pair<std::string, double>> sorted_tf(doc.tf.begin(), doc.tf.end());
            std::sort(sorted_tf.begin(), sorted_tf.end(), [](const pair<string, double>& a, const pair<string, double>& b) {
                return a.second > b.second;
            });

            // Affichage des 5 termes les plus fréquents
            os << "Top 5 des termes les plus fréquents:\n";
            for (int i = 0; i < std::min(5, int(sorted_tf.size())); ++i) {
                os << sorted_tf[i].first << ": " << sorted_tf[i].second * 100 << "%\n";
            }
            return os;
        }

        // Met en évidence les parties plagiées dans le texte traité
        std::string highlight_plagiarism_in_processed_text(const std::map<std::string, int>& word_intensity) const;

        // Met en évidence les parties plagiées dans le terminal
        std::string highlight_plagiarism_in_terminal(const std::map<std::string, int>& word_intensity) const;

        // Retourne la valeur de n-gramme utilisée
        int get_ngram() const { return ngram; }

    private:
        int ngram;  // Taille des n-grammes utilisés pour l'analyse
};

#endif