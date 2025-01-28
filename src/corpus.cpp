#include "../include/corpus.hpp"

// Ajoute un document au corpus et met à jour les tokens et la fréquence documentaire (df)
void Corpus::add_document(const std::shared_ptr<Document>& doc) {
    Documents.push_back(doc); // Ajoute le document à la liste des documents
    for (const auto& token_pair : doc->tf) {
        tokens_.insert(token_pair.first); // Ajoute le token à l'ensemble des tokens
        df[token_pair.first]++; // Met à jour la fréquence documentaire (df) pour ce token
    }
}

// Calcule la fréquence documentaire (df) pour tous les tokens dans le corpus
void Corpus::compute_df() {
    df.clear(); // Réinitialise la fréquence documentaire
    for (const auto& doc : Documents) {
        for (const auto& token_pair : doc->tf) {
            df[token_pair.first]++; // Met à jour la fréquence documentaire pour chaque token
        }
    }
}

// Calcule le score TF-IDF pour chaque terme d'un document donné
map<string, double> Corpus::compute_tf_idf(const Document& doc) const {
    map<string, double> tf_idf; // Map pour stocker les scores TF-IDF

    for (const auto& term_pair : doc.tf) {
        int dff = 1; // Fréquence documentaire par défaut
        if (tokens_.find(term_pair.first) != tokens_.end()) {
            dff = df.at(term_pair.first); // Récupère la fréquence documentaire réelle si le token existe
        }

        // Calcule l'IDF (Inverse Document Frequency)
        double idf = log10(Documents.size() / (double)(dff));

        // Calcule le score TF-IDF et l'ajoute à la map
        tf_idf[term_pair.first] = doc.tf.at(term_pair.first) * idf;
    }

    return tf_idf; // Retourne la map des scores TF-IDF
}