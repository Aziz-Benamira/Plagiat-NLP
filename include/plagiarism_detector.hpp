#ifndef PLAGIARISM_DETECTOR_H
#define PLAGIARISM_DETECTOR_H

#include "similarity_analyzer.hpp"

using namespace std;

class PlagiarismDetector {
public:
    SimilarityAnalyzer Analyzer;  // Analyseur de similarité
    int min_ngram;  // Taille minimale des n-grams
    int max_ngram;  // Taille maximale des n-grams

    // Constructeur avec un analyseur de similarité et une taille de n-gram par défaut
    explicit PlagiarismDetector(SimilarityAnalyzer leanalyseur, int ngram_range = 4)
        : Analyzer(leanalyseur), min_ngram(ngram_range), max_ngram(ngram_range) {}

    // Constructeur avec un corpus et une taille de n-gram par défaut
    explicit PlagiarismDetector(Corpus Lecorpus, int ngram_range = 3)
        : Analyzer(Lecorpus), min_ngram(ngram_range), max_ngram(ngram_range) {}

    // Constructeur avec un corpus et une plage de tailles de n-grams
    PlagiarismDetector(Corpus Lecorpus, int min_ngram, int max_ngram)
        : Analyzer(Lecorpus), min_ngram(min_ngram), max_ngram(max_ngram) {}

    /**
     * Vérifie le plagiat d'un document en le comparant avec les documents du corpus.
     * @param Doc_to_check Le document à vérifier.
     * @return Une map des documents du corpus avec leur score de similarité.
     */
    map<shared_ptr<Document>, double> check_plagiarism(Document& Doc_to_check) {
        double max_score = 0;
        int ngram_range = max_ngram;

        // Calcul du score maximal possible
        for (int i = min_ngram; i <= max_ngram; i++) {
            max_score += 1.0 / (ngram_range - i + 1);
        }

        map<shared_ptr<Document>, double> result;

        // Calcul des scores de similarité pour chaque taille de n-gram
        for (int ngram = min_ngram; ngram <= max_ngram; ngram++) {
            Doc_to_check.compute_tf(ngram);
            for (auto& doc : Analyzer.LeCorpus.Documents) {
                doc->compute_tf(ngram);
            }
            Analyzer.LeCorpus.compute_df();

            for (auto& doc : Analyzer.LeCorpus.Documents) {
                double temp_result = Analyzer.compute_score(Doc_to_check, *doc);
                result[doc] += temp_result / (ngram_range - ngram + 1);
            }
        }

        // Normalisation des scores
        for (auto& doc : Analyzer.LeCorpus.Documents) {
            result[doc] /= max_score;
        }

        return result;
    }

    /**
     * Calcule le score final de similarité entre un document et une liste de documents.
     * @param doc_to_test Le document à tester.
     * @param top_documents Les documents à comparer.
     * @param ngram_intensity Une map pour stocker l'intensité des n-grams.
     * @param n La taille des n-grams.
     * @return Le score final de similarité.
     */
    double get_final_score(Document& doc_to_test, vector<std::shared_ptr<Document>> top_documents, std::map<string, int>& ngram_intensity, int n = 3) {
        // Récupère les n-grams du document à tester
        std::vector<std::string> test_ngrams;
        if (doc_to_test.get_ngram() != n) {
            test_ngrams = doc_to_test.create_ngrams(n);
        } else {
            test_ngrams.reserve(doc_to_test.tf.size());
            for (const auto& [key, value] : doc_to_test.tf) {
                test_ngrams.emplace_back(key);
            }
        }

        if (test_ngrams.empty() || top_documents.empty()) {
            return 0.0;
        }

        // Compare les n-grams du document à tester avec ceux des autres documents
        for (const auto& doc : top_documents) {
            vector<string> compare_ngrams;
            if (doc->get_ngram() != n) {
                compare_ngrams = doc->create_ngrams(n);
            } else {
                compare_ngrams.reserve(doc->tf.size());
                for (const auto& [key, value] : doc->tf) {
                    compare_ngrams.emplace_back(key);
                }
            }

            // Vérifie la similarité entre les n-grams
            for (const auto& test_ngram : test_ngrams) {
                for (const auto& compare_ngram : compare_ngrams) {
                    int matches = 0;
                    int total_length = test_ngram.length();

                    for (size_t i = 0; i < test_ngram.length() && i < compare_ngram.length(); ++i) {
                        if (tolower(test_ngram[i]) == tolower(compare_ngram[i])) {
                            matches++;
                        }
                    }

                    double similarity = total_length > 0 ? static_cast<double>(matches) / total_length : 0.0;

                    if (similarity > 0.9) {  // Seuil de 90%
                        ngram_intensity[test_ngram]++;
                        break;  // Passe au n-gram suivant une fois qu'une correspondance est trouvée
                    }
                }
            }
        }

        return (static_cast<double>(ngram_intensity.size()) / test_ngrams.size());
    }

    /**
     * Récupère les mots plagiés avec leur intensité.
     * @param gram_intensity Une map des n-grams avec leur intensité.
     * @return Une map des mots plagiés avec leur intensité.
     */
    std::map<std::string, int> get_plagiarized_words_with_intensity(map<string, int> gram_intensity) {
        std::map<std::string, int> word_intensity;

        // Parcourt les n-grams plagiés et les décompose en mots
        for (const auto& [gram, intensity] : gram_intensity) {
            if (intensity > 0) {
                std::istringstream stream(gram);
                std::string word;
                while (stream >> word) {
                    word_intensity[word] += intensity;
                }
            }
        }

        return word_intensity;
    }
};

#endif