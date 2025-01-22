#ifndef PLAGIARISM_DETECTOR_H
#define PLAGIARISM_DETECTOR_H
#include "similarity_analyzer.hpp"

using namespace std;
class PlagiarismDetector{
    public:
        SimilarityAnalyzer Analyzer;
        int ngram_range;
        explicit PlagiarismDetector(SimilarityAnalyzer leanalyseur, int ngram_range=4): Analyzer(leanalyseur),ngram_range(ngram_range){
            cout<<"CREATED"<<endl;
        };

        explicit PlagiarismDetector(Corpus Lecorpus,int ngram_range=4) : Analyzer(Lecorpus),ngram_range(ngram_range) {
        };
        
        map<shared_ptr<Document>,double>check_plagiarism(Document& Doc_to_check){
            double max_score = 0;
            for(int i=1;i<=ngram_range;i++){
                max_score+= 1.0/(ngram_range-i+1);
        
            }
            map<shared_ptr<Document>,double> result;
            for(int ngram = 1;ngram<=ngram_range;ngram++){
                // cout<<"NGRAM : "<<ngram<<endl;
                Doc_to_check.compute_tf(ngram);
                for(auto& doc: Analyzer.LeCorpus.Documents){
                    doc->compute_tf(ngram);
                }
                Analyzer.LeCorpus.compute_df();
                for(auto& doc: Analyzer.LeCorpus.Documents){
                    result[doc] += Analyzer.compute_score(Doc_to_check,*doc)/(ngram_range-ngram+1);
                }
            }
            for(auto& doc: Analyzer.LeCorpus.Documents){
                    result[doc] /= max_score;
                }
            return result;
        }


        std::map<std::string, int> get_plagiarized_3grams_with_intensity(Document& doc_to_test) { 
  // Étape 1 : Extraire les 3-grams du document testé
    auto test_3grams = doc_to_test.create_ngrams(3);

    // Étape 2 : Initialiser la carte des 3-grams avec une intensité de 0
    std::map<std::string, int> three_gram_intensity;
    for (const auto& three_gram : test_3grams) {
        three_gram_intensity[three_gram] = 0;
    }

    // Étape 3 : Parcourir les documents du corpus
    for (const auto& doc : Analyzer.LeCorpus.Documents) {
        // Générer les 3-grams du document du corpus
        auto corpus_3grams = doc->create_ngrams(3);

        // Mettre à jour l'intensité des 3-grams du document testé
        for (const auto& three_gram : test_3grams) {
            if (std::find(corpus_3grams.begin(), corpus_3grams.end(), three_gram) != corpus_3grams.end()) {
                three_gram_intensity[three_gram]++;
            }
        }
    }

    // Étape 4 : Retourner la carte des 3-grams avec leur intensité
    return three_gram_intensity;
}

std::map<std::string, int> get_plagiarized_words_with_intensity(Document& doc_to_test) {
    // Obtenir les 3-grams avec leur intensité
    auto three_gram_intensity = get_plagiarized_3grams_with_intensity(doc_to_test);

    // Créer une carte pour stocker les mots uniques et leur intensité
    std::map<std::string, int> word_intensity;

    // Parcourir les 3-grams plagiés
    for (const auto& [three_gram, intensity] : three_gram_intensity) {
        if (intensity > 0) { // Seulement les 3-grams qui apparaissent dans le corpus
            // Découper le 3-gram en mots individuels
            std::istringstream stream(three_gram);
            std::string word;
            while (stream >> word) {
                word_intensity[word] += intensity; // Mettre à jour l'intensité du mot
            }
        }
    }

    return word_intensity;
}
};



#endif