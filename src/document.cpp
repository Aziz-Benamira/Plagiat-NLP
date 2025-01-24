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
    "my", "your", "his", "their", "are", "was", "were", "will", "would",};
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
    "asm", "auto", "bool", "break", "case", "catch", "char", "class", "const", "constexpr", 
    "continue", "default", "delete", "do", "double", "dynamic_cast", "else", "enum", "explicit", 
    "export", "extern", "false", "float", "for", "friend", "goto", "if", "inline", "int", "long", 
    "mutable", "namespace", "new", "noexcept", "not", "not_eq", "nullptr", "operator", "or", 
    "or_eq", "private", "protected", "public", "register", "reinterpret_cast", "return", "short", 
    "signed", "sizeof", "static", "static_assert", "static_cast", "struct", "switch", "template", 
    "this", "throw", "true", "try", "typedef", "typeid", "typename", "union", "unsigned", "using", 
    "virtual", "void", "volatile", "wchar_t", "while",
    
    "iostream", "vector", "string", "map", "set", "list", "queue", "stack", "algorithm", "typeinfo", 
    "ostream", "cin", "cout", "cerr", "clogs", "istream", "pair", "iterator", "null", "auto", "nullptr", 
    "endl", "begin", "end", "size", "push_back", "pop_back", "front", "back", "insert", "erase", "find", 
    "lower_bound", "upper_bound", "sort", "reverse",
    
    "main", "print", "printf", "scanf", "cin", "cout", "main()",
    
    "int", "i", "j", "x", "y", "z", "temp", "result", "count", "sum", "total", "arr", "array", "data",
    
    "if", "else", "switch", "case", "break", "continue", "return", "for", "while", "do", "try", "catch",
    
    "+", "-", "*", "/", "%", "=", "==", "!=", "<", ">", "<=", ">=", "&&", "||", "!", "++", "--", "&=", "|=", 
    "^=", "~",
    
    "int", "char", "bool", "float", "double", "long", "short", "unsigned", "void", "string", "const", "auto",
    
    "#include", "#define", "#include <iostream>", "#include <vector>", "#ifdef", "#endif", "#pragma"
};
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

std::string escape_special_chars(const std::string& word) {
    std::string escaped_word;
    for (char ch : word) {
        if (ch == '\\' || ch == '^' || ch == '$' || ch == '.' || ch == '|' || ch == '?' || ch == '*' || ch == '+' || ch == '(' || ch == ')' || ch == '[' || ch == ']' || ch == '{' || ch == '}' || ch == '#') {
            escaped_word += '\\'; // Ajouter un backslash avant le caractère spécial
        }
        escaped_word += ch;
    }
    return escaped_word;
}

std::string Document::highlight_plagiarism_in_processed_text(const std::map<std::string, int>& word_intensity) const {
    std::string highlighted_text = text; // Utiliser le texte traité pour le highlight

    // Parcourir les mots à highlight
    for (const auto& [word, intensity] : word_intensity) {
        // Échapper les caractères spéciaux dans le mot
        std::string escaped_word = escape_special_chars(word);

        // Créer une expression régulière pour rechercher le mot entier
        std::regex word_regex("\\b" + escaped_word + "\\b");

        // Remplacer toutes les occurrences du mot entier par la version highlightée
        std::string highlighted_word;
        if (intensity >= 4) {
            highlighted_word = "\033[1;31m" + word + "\033[0m"; // Rouge pour une intensité élevée (4-grams)
        } else if (intensity == 3) {
            highlighted_word = "\033[1;33m" + word + "\033[0m"; // Jaune pour une intensité moyenne (3-grams)
        } else {
            highlighted_word = "\033[1;35m" + word + "\033[0m"; // Magenta pour une intensité faible (2-grams)
        }

        // Appliquer le highlight en utilisant une expression régulière
        highlighted_text = std::regex_replace(highlighted_text, word_regex, highlighted_word);
    }

    return highlighted_text;
}