#include "../include/document.hpp"


void Document::tokenization(set<string> stopwords){
            // miniscule
            transform(text.begin(), text.end() ,text.begin() , ::tolower);
            // transform(stopwords.begin(), stopwords.end() ,stopwords.begin() , ::tolower);
            
            // suppression des ponctuations
            text = regex_replace(text, regex(R"([\.,!?\-;:\"'\(\)\[\]])"), " ");
            
            // 
            tokens.clear();
            std::istringstream stream(text);
            std::string word;
            std::string clean_text;

            while (stream >> word) {
                if (stopwords.find(word) == stopwords.end()) {
                    tokens.push_back(word);
                }
        }
}

vector<string>  Document::create_ngrams(int n) const{
    vector<string> ngrams;
    for (size_t i = 0; i + n <= tokens.size(); ++i) {
        string ngram = tokens[i];
        for (int j = 1; j < n; ++j) {
            ngram += " " + tokens[i + j];
        }
        ngrams.push_back(ngram);
    }
    return ngrams;
}

void Document::compute_tf(vector<string>& ngrams){
    tf.clear();
    for(const auto& ngram : ngrams) {
            tf[ngram]++;
        }
    for (const auto& ngram : tf) {
            tf[ngram.first]/= tokens.size();
    }
}

void Document::compute_tf(int n) {
        
        tf.clear();
        vector<string> ngrams = create_ngrams(n);

        for (const auto& ngram : ngrams) {
            tf[ngram]++;
        }
        for (const auto& ngram : tf) {
            tf[ngram.first]/= tokens.size();
        }
       
    }

