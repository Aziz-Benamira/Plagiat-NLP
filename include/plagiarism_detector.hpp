#ifndef PLAGIARISM_DETECTOR_H
#define PLAGIARISM_DETECTOR_H
#include "similarity_analyzer.hpp"

using namespace std;
class PlagiarismDetector{
    public:
        SimilarityAnalyzer Analyzer;
        int min_ngram;
        int max_ngram;
        explicit PlagiarismDetector(SimilarityAnalyzer leanalyseur, int ngram_range=4): Analyzer(leanalyseur),min_ngram(ngram_range),max_ngram(ngram_range){
    
        };

        explicit PlagiarismDetector(Corpus Lecorpus,int ngram_range=3) : Analyzer(Lecorpus),min_ngram(ngram_range),max_ngram(ngram_range) {
        };
        PlagiarismDetector(Corpus Lecorpus,int min_ngram, int max_ngram) : Analyzer(Lecorpus),min_ngram(min_ngram),max_ngram(max_ngram) {
        };
        map<shared_ptr<Document>,double>check_plagiarism(Document& Doc_to_check){
            double max_score = 0;
            int ngram_range = max_ngram;
            for(int i=min_ngram;i<=max_ngram;i++){
                max_score+= 1.0/(ngram_range-i+1);
        
            }
            map<shared_ptr<Document>,double> result;
            for(int ngram = min_ngram;ngram<=max_ngram;ngram++){
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
   double get_final_score(Document& doc_to_test,vector<std::shared_ptr<Document>> top_documents,std::map<string, int>& ngram_intensity,int n = 3) {
    
    // Get n-grams from test document once
    std::vector<std::string> test_ngrams;
    if(doc_to_test.get_ngram()!=n){
        test_ngrams = doc_to_test.create_ngrams(n);
    }else{
        test_ngrams.reserve(doc_to_test.tf.size());
        for (const auto& [key, value] : doc_to_test.tf) {
            test_ngrams.emplace_back(key);
    }
    }
    
    
    if (test_ngrams.empty() || top_documents.empty()) {
        return 0.0;
    }

    // First loop through documents to avoid repeated ngram creation
    for (const auto& doc : top_documents) {
        vector<string> compare_ngrams;
        if(doc->get_ngram()!=n){
            compare_ngrams=doc->create_ngrams(n);
        }else{
            compare_ngrams.reserve(doc->tf.size());
            for (const auto& [key, value] : doc->tf) {
                compare_ngrams.emplace_back(key);
            }
        }
        
        // Compare each test ngram against current document's ngrams
        for (const auto& test_ngram : test_ngrams) {
            for (const auto& compare_ngram : compare_ngrams) {
                // Character similarity check
                int matches = 0;
                int total_length = test_ngram.length();
                
                for (size_t i = 0; i < test_ngram.length() && i < compare_ngram.length(); ++i) {
                    if (tolower(test_ngram[i]) == tolower(compare_ngram[i])) {
                        matches++;
                    }
                }
                
                double similarity = total_length > 0 ? 
                    static_cast<double>(matches) / total_length : 0.0;
                
                if (similarity > 0.9) {  // 90% threshold
                    ngram_intensity[test_ngram]++;
                    break; // Move to next test_ngram once match is found
                }
            }
        }
    }

    return (static_cast<double>(ngram_intensity.size()) / test_ngrams.size());
}


std::map<std::string, int> get_plagiarized_words_with_intensity(map<string,int> gram_intensity) {
    // Obtenir les 3-grams avec leur intensité
    // auto gram_intensity = get_plagiarized_ngrams_with_intensity(doc_to_test,top_documents);

    // Créer une carte pour stocker les mots uniques et leur intensité
    std::map<std::string, int> word_intensity;

    // Parcourir les 3-grams plagiés
    for (const auto& [gram, intensity] : gram_intensity) {
        if (intensity > 0) { // Seulement les 3-grams qui apparaissent dans le corpus
            // Découper le 3-gram en mots individuels
            std::istringstream stream(gram);
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