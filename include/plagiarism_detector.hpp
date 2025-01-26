#ifndef PLAGIARISM_DETECTOR_H
#define PLAGIARISM_DETECTOR_H
#include "similarity_analyzer.hpp"

using namespace std;
class PlagiarismDetector{
    public:
        SimilarityAnalyzer Analyzer;
        int ngram_range;
        explicit PlagiarismDetector(SimilarityAnalyzer leanalyseur, int ngram_range=4): Analyzer(leanalyseur),ngram_range(ngram_range){
    
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
                    double temp_result = Analyzer.compute_score(Doc_to_check,*doc);
                    result[doc] += temp_result/(ngram_range-ngram+1);
                    // cout<<doc->title<<": ngram :"<<ngram<<":"<< result[doc]<<endl;
                }
            }
            for(auto& doc: Analyzer.LeCorpus.Documents){
                    result[doc] /= max_score;
                }
            
            return result;
        }


      std::map<std::string, int> get_plagiarized_ngrams_with_intensity(Document& doc_to_test) {
    std::map<std::string, int> ngram_intensity;

    // Obtenir les 5 premiers documents avec les scores de similarité les plus élevés
    auto similarity_scores = check_plagiarism(doc_to_test);
    std::vector<std::shared_ptr<Document>> top_documents;
    for (const auto& [doc, score] : similarity_scores) {
        top_documents.push_back(doc);
        if (top_documents.size() >= 5) break; // Limiter à 5 documents
    }

    // Essayer d'abord avec n = 4
    int n = 4;
    doc_to_test.compute_tf(n);
    for (auto& doc : top_documents) {
        doc->compute_tf(n);
    }
    Analyzer.LeCorpus.compute_df();

    // Vérifier s'il y a des correspondances avec n = 4
    bool found_match = false;
    for (const auto& doc : top_documents) {
        for (const auto& ngram : doc_to_test.tf) {
            if (doc->tf.find(ngram.first) != doc->tf.end()) {
                found_match = true;
                break;
            }
        }
        if (found_match) break;
    }

    // Si aucune correspondance n'est trouvée avec n = 4, essayer avec n = 3
    if (!found_match) {
        n = 3;
        doc_to_test.compute_tf(n);
        for (auto& doc : top_documents) {
            doc->compute_tf(n);
        }
        Analyzer.LeCorpus.compute_df();

        // Vérifier s'il y a des correspondances avec n = 3
        found_match = false;
        for (const auto& doc : top_documents) {
            for (const auto& ngram : doc_to_test.tf) {
                if (doc->tf.find(ngram.first) != doc->tf.end()) {
                    found_match = true;
                    break;
                }
            }
            if (found_match) break;
        }
    }

    // Si aucune correspondance n'est trouvée avec n = 3, essayer avec n = 2
    if (!found_match) {
        n = 2;
        doc_to_test.compute_tf(n);
        for (auto& doc : top_documents) {
            doc->compute_tf(n);
        }
        Analyzer.LeCorpus.compute_df();
    }

    // Calculer l'intensité des n-grams
    for (const auto& doc : top_documents) {
        for (const auto& ngram : doc_to_test.tf) {
            if (doc->tf.find(ngram.first) != doc->tf.end()) {
                ngram_intensity[ngram.first]++;
            }
        }
    }

    return ngram_intensity;
}


std::map<std::string, int> get_plagiarized_words_with_intensity(Document& doc_to_test) {
    // Obtenir les 3-grams avec leur intensité
    auto three_gram_intensity = get_plagiarized_ngrams_with_intensity(doc_to_test);

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