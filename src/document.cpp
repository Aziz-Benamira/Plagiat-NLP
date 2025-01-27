#include "../include/document.hpp"
#include <sstream>

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
    //         stopwords = {
    // "et", "ou", "mais", "donc", "car", "en", "de", "la", "le", "les", "un", "une", 
    // "à", "au", "aux", "du", "des", "par", "pour", "sur", "avec", "sans", "ne", "pas", 
    // "ce", "cette", "ces", "dans", "tout", "tous", "il", "elle", "ils", "elles", 
    // "lui", "leur", "nous", "vous", "je", "tu", "moi", "toi", "que", "qui", "quoi"};
        stopwords={
    // Articles & contractions
    "le", "la", "les", "un", "une", "des", "du", "de", "d'", "au", "aux", "à","l",
    
    // Pronouns
    "je", "tu", "il", "elle", "nous", "vous", "ils", "elles", "me", "te", "se",
    "lui", "leur", "y", "en", "ce", "cet", "cette", "ces", "celui", "celle",
    
    // Prepositions
    "dans", "par", "pour", "sur", "vers", "avec", "sans", "sous", "chez", "entre",
    "depuis", "pendant", "durant", "hors", "contre", "malgré",
    
    // Conjunctions
    "mais", "ou", "et", "donc", "or", "ni", "car", "que", "quand", "comme", "si",
    "lorsque", "puisque", "quoique",
    
    // Common verbs (conjugated forms)
    "est", "sont", "a", "ont", "étais", "était", "fut", "sera", "ai", "as", "avons",
    "avez", "ayant", "eu",
    
    // Negations
    "ne", "pas", "plus", "jamais", "rien", "aucun", "aucune",
    
    // Adverbs
    "très", "trop", "peu", "ici", "là", "ainsi", "alors", "après", "avant",
    "aujourd'hui", "demain", "hier", "toujours", "soudain",
    
    // Quantifiers
    "tout", "tous", "toute", "toutes", "chaque", "plusieurs", "certains", "quelques"
};
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
        // Most common C++ keywords that typically don't indicate plagiarism
        "if", "else", "for", "while", "return", "break", "continue",
        "void", "int", "char", "bool", "float", "double", "string",
        "class", "struct", "public", "private", "protected",
        
        // Common includes and preprocessor directives
        "#include", "#define", "main", "using", "namespace",
        
        // Common standard library elements
        "std", "cout", "cin", "endl",
        
        // Common operators and symbols
        "nullptr", "true", "false"
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
                text = regex_replace(text, regex("[\r\n'.()\\[\\]]"), " ");
                text = regex_replace(text, regex(R"([\,!?\-;:\"«»])"), "");
                
                
                std::regex caracteres_speciaux(R"([^[:print:]\n])");
                if(type==FRANCAIS){
                    text = regex_replace(text, regex("é|è|ê|ë|É"), "e");   
                    text = regex_replace(text, regex("à|â|ä"), "a");
                    text = regex_replace(text, regex("ì|î|ï"), "i");
                    text = regex_replace(text, regex("ò|ô|ö"), "o");
                    text = regex_replace(text, regex("ù|û|ü"), "u");
                    text = regex_replace(text, regex("ÿ"), "y");
                    text = regex_replace(text, regex("ç"), "c");
                    text = std::regex_replace(text, caracteres_speciaux, " ");

                    // text= re.sub(r'[\x80-\xFF]', '', text);

                }
                text = regex_replace(text, regex("\\s{2,}"), " ");
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
        ngram=n;
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
            highlighted_word = "<red>" + word + "</red>"; // Rouge pour une intensité élevée (4-grams)
        } else if (intensity == 3) {
            highlighted_word = "<yellow>" + word + "</yellow>"; // Jaune pour une intensité moyenne (3-grams)
        } else {
            highlighted_word = "<magenta>" + word + "</magenta>"; // Magenta pour une intensité faible (2-grams)
        }

        // Appliquer le highlight en utilisant une expression régulière
        highlighted_text = std::regex_replace(highlighted_text, word_regex, highlighted_word);
    }

    return highlighted_text;
}

#include "../include/document.hpp"

// ... (autres fonctions existantes)

std::string Document::highlight_plagiarism_in_terminal(const std::map<std::string, int>& word_intensity) const {
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
            highlighted_word = "\033[31m" + word + "\033[0m"; // Rouge pour une intensité élevée (4-grams)
        } else if (intensity == 3) {
            highlighted_word = "\033[33m" + word + "\033[0m"; // Jaune pour une intensité moyenne (3-grams)
        } else {
            highlighted_word = "\033[35m" + word + "\033[0m"; // Magenta pour une intensité faible (2-grams)
        }

        // Appliquer le highlight en utilisant une expression régulière
        highlighted_text = std::regex_replace(highlighted_text, word_regex, highlighted_word);
    }

    return highlighted_text;
}