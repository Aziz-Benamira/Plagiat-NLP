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
                    unique_tokens.insert(word);
                }
        }
}
void Document::tokenization(){
    set<string> stopwords;
    // Assign stopwords based on document type
    switch (type) {
        case FRANCAIS:
            stopwords = {
    "et", "ou", "mais", "donc", "car", "en", "de", "la", "le", "les", "un", "une", 
    "à", "au", "aux", "du", "des", "par", "pour", "sur", "avec", "sans", "ne", "pas", 
    "ce", "cette", "ces", "dans", "tout", "tous", "il", "elle", "ils", "elles", 
    "lui", "leur", "nous", "vous", "je", "tu", "moi", "toi", "que", "qui", "quoi"};
    break;

    case ANGLAIS:
        stopwords={ "the", "and", "is", "in", "at", "of", "a", "an", "on", "for", "to", "with", 
    "by", "that", "this", "it", "from", "not", "or", "but", "so", "because", 
    "these", "those", "be", "been", "being", "have", "has", "had", "do", "does", 
    "did", "I", "you", "he", "she", "we", "they", "me", "him", "her", "us", "them", 
    "my", "your", "his", "their"};
    break;
    
    case PYTHON:
        stopwords = {
    "import", "from", "def", "return", "if", "else", "elif", "for", "while", 
    "in", "as", "with", "try", "except", "raise", "assert", "and", "or", "not", 
    "class", "pass", "lambda", "yield", "break", "continue", "global", "nonlocal", 
    "True", "False", "None"};
    break;    

    case CPP:
        stopwords = {
    "alignas", "alignof", "and", "and_eq", "asm", "auto", "bitand", "bitor", 
    "bool", "break", "case", "catch", "char", "class", "const", "constexpr", 
    "continue", "decltype", "default", "delete", "do", "double", "dynamic_cast", 
    "else", "enum", "explicit", "export", "extern", "false", "float", "for", 
    "friend", "goto", "if", "inline", "int", "long", "mutable", "namespace", 
    "new", "noexcept", "not", "nullptr", "operator", "or", "or_eq", "private", 
    "protected", "public", "register", "reinterpret_cast", "return", "short", 
    "signed", "sizeof", "static", "static_assert", "struct", "switch", "template", 
    "this", "throw", "true", "try", "typedef", "typeid", "typename", "union", 
    "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", 
    "xor", "xor_eq"};
    break;
        default:
            stopwords = {
                "the", "and", "is", "in", "at", "of", "a", "an", "on", "for",
                "to", "with", "by", "that", "this", "it", "from"
                
            };
            break;
    }
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
                        unique_tokens.insert(word);
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

