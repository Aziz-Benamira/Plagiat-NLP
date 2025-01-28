#include "../include/document.hpp"
#include <sstream>

// Fonction : tokenization
// Description : Tokenise le texte en supprimant les stopwords et en normalisant le texte (minuscules, suppression de la ponctuation).
// Paramètres :
//   - stopwords : un ensemble de mots vides à exclure de la tokenisation.
void Document::tokenization(set<string> stopwords) {
    // Convertir le texte en minuscules
    transform(text.begin(), text.end(), text.begin(), ::tolower);

    // Supprimer la ponctuation
    text = regex_replace(text, regex(R"([\.,!?\-;:\"'\(\)\[\]])"), " ");

    // Tokenisation
    tokens.clear();
    std::istringstream stream(text);
    std::string word;

    while (stream >> word) {
        if (stopwords.find(word) == stopwords.end()) {
            tokens.push_back(word);
        }
    }
}

// Fonction : tokenization (surcharge)
// Description : Tokenise le texte en utilisant des stopwords prédéfinis en fonction du type de document (FRANCAIS, ANGLAIS, PYTHON, CPP).
void Document::tokenization() {
    set<string> stopwords;

    // Assigner les mots vides en fonction du type de document
    switch (type) {
        case FRANCAIS:
            stopwords = {
                // Articles & contractions
                "le", "la", "les", "un", "une", "des", "du", "de", "d'", "au", "aux", "à", "l",
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
            stopwords = {
                "the", "and", "is", "in", "at", "of", "a", "an", "on", "for", "to", "with",
                "by", "that", "this", "it", "from", "not", "or", "but", "so", "because",
                "these", "those", "be", "been", "being", "have", "has", "had", "do", "does",
                "did", "I", "you", "he", "she", "we", "they", "me", "him", "her", "us", "them",
                "my", "your", "his", "their", "are", "was", "were", "will", "would"
            };
            break;

        case PYTHON:
            stopwords = {
                "import", "from", "def", "return", "if", "else", "elif", "for", "while",
                "in", "as", "with", "try", "except", "raise", "assert", "and", "or", "not",
                "class", "pass", "lambda", "yield", "break", "continue", "global", "nonlocal",
                "True", "False", "None"
            };
            break;

        case CPP:
            stopwords = {
                // Mots-clés courants en C++ qui n'indiquent généralement pas de plagiat
                "if", "else", "for", "while", "return", "break", "continue",
                "void", "int", "char", "bool", "float", "double", "string",
                "class", "struct", "public", "private", "protected",
                // Directives de préprocesseur courantes
                "#include", "#define", "main", "using", "namespace",
                // Éléments courants de la bibliothèque standard
                "std", "cout", "cin", "endl",
                // Opérateurs et symboles courants
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

    // Convertir le texte en minuscules
    transform(text.begin(), text.end(), text.begin(), ::tolower);

    // Supprimer la ponctuation et les caractères spéciaux
    text = regex_replace(text, regex("[\r\n'.()\\[\\]]"), " ");
    text = regex_replace(text, regex(R"([\,!?\-;:\"«»])"), "");

    // Normaliser les caractères spéciaux pour le français
    if (type == FRANCAIS) {
        text = regex_replace(text, regex("é|è|ê|ë|É"), "e");
        text = regex_replace(text, regex("à|â|ä"), "a");
        text = regex_replace(text, regex("ì|î|ï"), "i");
        text = regex_replace(text, regex("ò|ô|ö"), "o");
        text = regex_replace(text, regex("ù|û|ü"), "u");
        text = regex_replace(text, regex("ÿ"), "y");
        text = regex_replace(text, regex("ç"), "c");
        text = regex_replace(text, regex("[^[:print:]\n]"), " ");
    }

    // Supprimer les espaces multiples
    text = regex_replace(text, regex("\\s{2,}"), " ");

    // Tokenisation
    tokens.clear();
    std::istringstream stream(text);
    std::string word;

    while (stream >> word) {
        if (stopwords.find(word) == stopwords.end()) {
            tokens.push_back(word);
        }
    }
}

// Fonction : create_ngrams
// Description : Crée des n-grammes à partir des tokens.
// Paramètres :
//   - n : la taille des n-grammes à créer.
// Retour : un vecteur de n-grammes.
vector<string> Document::create_ngrams(int n) const {
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

// Fonction : compute_tf (surcharge)
// Description : Calcule la fréquence des termes (TF) pour les n-grammes donnés.
// Paramètres :
//   - ngrams : un vecteur de n-grammes pour lesquels calculer la fréquence.
void Document::compute_tf(vector<string>& ngrams) {
    tf.clear();
    for (const auto& ngram : ngrams) {
        tf[ngram]++;
    }
    for (const auto& ngram : tf) {
        tf[ngram.first] /= tokens.size();
    }
}

// Fonction : compute_tf (surcharge)
// Description : Calcule la fréquence des termes (TF) pour les n-grammes de taille donnée.
// Paramètres :
//   - n : la taille des n-grammes à utiliser pour le calcul de la fréquence.
void Document::compute_tf(int n, int top_ngram) {
    ngram = n;
    tf.clear();
    vector<string> ngrams = create_ngrams(n);

    for (const auto& ngram : ngrams) {
        tf[ngram]++;
    }
    for (auto& ngram : tf) {
        ngram.second /= tokens.size();
    }
    if(ngram!=-1){
        vector<pair<string, double>> sorted_tf(tf.begin(), tf.end());
    std::sort(sorted_tf.begin(), sorted_tf.end(),
              [](const pair<string, double>& a, const pair<string, double>& b) {
                  return a.second > b.second;
              });
    if (sorted_tf.size() >= top_ngram) {
        sorted_tf.resize(top_ngram);
    }
    tf.clear();
    for (const auto& pair : sorted_tf) {
        tf[pair.first] = pair.second;
    }
    }
}

// Fonction : escape_special_chars
// Description : Échappe les caractères spéciaux dans une chaîne de caractères pour une utilisation dans une expression régulière.
// Paramètres :
//   - word : la chaîne de caractères à échapper.
// Retour : la chaîne de caractères avec les caractères spéciaux échappés.
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

// Fonction : highlight_plagiarism_in_processed_text
// Description : Met en évidence les mots plagiés dans le texte traité en utilisant des balises HTML pour le highlight.
// Paramètres :
//   - word_intensity : une map contenant les mots à highlight et leur intensité.
// Retour : le texte avec les mots plagiés mis en évidence.
std::string Document::highlight_plagiarism_in_processed_text(const std::map<std::string, int>& word_intensity) const {
    std::string highlighted_text = text;

    for (const auto& [word, intensity] : word_intensity) {
        std::string escaped_word = escape_special_chars(word);
        std::regex word_regex("\\b" + escaped_word + "\\b");

        std::string highlighted_word;
        if (intensity >= 4) {
            highlighted_word = "<red>" + word + "</red>"; // Rouge pour une intensité élevée (4-grams)
        } else if (intensity == 3) {
            highlighted_word = "<yellow>" + word + "</yellow>"; // Jaune pour une intensité moyenne (3-grams)
        } else {
            highlighted_word = "<magenta>" + word + "</magenta>"; // Magenta pour une intensité faible (2-grams)
        }

        highlighted_text = std::regex_replace(highlighted_text, word_regex, highlighted_word);
    }

    return highlighted_text;
}

// Fonction : highlight_plagiarism_in_terminal
// Description : Met en évidence les mots plagiés dans le texte traité en utilisant des codes de couleur ANSI pour le terminal.
// Paramètres :
//   - word_intensity : une map contenant les mots à highlight et leur intensité.
// Retour : le texte avec les mots plagiés mis en évidence.
std::string Document::highlight_plagiarism_in_terminal(const std::map<std::string, int>& word_intensity) const {
    std::string highlighted_text = text;

    for (const auto& [word, intensity] : word_intensity) {
        std::string escaped_word = escape_special_chars(word);
        std::regex word_regex("\\b" + escaped_word + "\\b");

        std::string highlighted_word;
        if (intensity >= 4) {
            highlighted_word = "\033[31m" + word + "\033[0m"; // Rouge pour une intensité élevée (4-grams)
        } else if (intensity == 3) {
            highlighted_word = "\033[33m" + word + "\033[0m"; // Jaune pour une intensité moyenne (3-grams)
        } else {
            highlighted_word = "\033[35m" + word + "\033[0m"; // Magenta pour une intensité faible (2-grams)
        }

        highlighted_text = std::regex_replace(highlighted_text, word_regex, highlighted_word);
    }

    return highlighted_text;
}