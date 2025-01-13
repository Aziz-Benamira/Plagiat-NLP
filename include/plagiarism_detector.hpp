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
            for(int i=1;i++;i=ngram_range){
                max_score+= 1/i;
            }
            map<shared_ptr<Document>,double> result;
            for(int ngram = 1;ngram++; ngram<=ngram_range){
                cout<<"NGRAM : "<<ngram<<endl;
                Doc_to_check.compute_tf(ngram);
                for(auto doc: Analyzer.LeCorpus.Documents){
                    doc->compute_tf(ngram);
                }
                Analyzer.LeCorpus.compute_df();
                for(auto doc: Analyzer.LeCorpus.Documents){
                    result[doc] += Analyzer.compute_score(Doc_to_check,*doc)/(ngram_range-ngram);
                }
            }
            for(auto doc: Analyzer.LeCorpus.Documents){
                    result[doc] /= max_score;
                }
            return result;
        }




};



#endif