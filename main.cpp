#include "include/document.hpp"
#include "include/corpus.hpp"
#include "include/similarity_analyzer.hpp"
#include "include/file_reader.hpp"
#include "include/plagiarism_detector.hpp"
#include <memory>
#include <fstream>
using namespace std;


// Convertit une valeur de type 'types' en chaîne de caractères.
string toString(types t) {
    switch (t) {
        case ANGLAIS: return "anglais";
        case FRANCAIS: return "fr";
        case CPP: return "c++";
        case PYTHON: return "python";
        default: return "unknown";
    }
}

int main(int argc, char* argv[]) {
    // precision de 3 chiffres après la virgule
    cout.precision(3);
    // Au cas où l'utilisateur ne fournit pas les bons arguments
    if(argc<4) cout << "Usage: " << argv[0] << " <test_file> <type> <corpus_path> (<output_path>/terminal par defaut)" << endl;

    // Récupérer les arguments
    string test_file = argv[1];
    string type = argv[2];
    cout<<"type : "<<type<<endl;
    string corpus_path = argv[3];
    string output_path;
    // Convertir le type en enum
    types t;
    if (type == "anglais") {
        t = ANGLAIS;
    } else if (type == "fr") {
        t = FRANCAIS;
    } else if (type == "c++") {
        t = CPP;
    } else if (type == "py") {
        t = PYTHON;
    } else {
        t = ANGLAIS;
    }

    // initialiser les objets
    FileReader fileReader;
    Corpus corpus;
    shared_ptr<Document> doc;

    // Lire les fichiers
    try {
        doc = fileReader.readDocument(test_file, t);
        corpus = fileReader.readCorpus(corpus_path,1);
    } catch (const exception& e) {
        cerr << "Erreur : " << e.what() << endl;
        return 1;
    }
    cout<<"Corpus lu contenant "<<corpus.size()<<" documents"<<endl;

    // Ouvrir le fichier de sortie si nécessaire
    ofstream output_file;
    if (argc==5) {
        output_path= argv[4];
        output_file.open(output_path, ios::out | ios::binary);
        if (!output_file.is_open()) {
            cerr << "Erreur : Impossible d'ouvrir le fichier output.txt" << endl;
            return 1;
        }
        output_file << "\xEF\xBB\xBF"; // UTF-8 BOM
        cout.rdbuf(output_file.rdbuf());
    }

    // Afficher les informations
    cout << endl;
    cout << "# Détecteur De Plagiat" << endl;
    cout << "### Langue du document à tester : " << toString(t) << endl;
    cout << "### Titre du document à tester : " << doc->title << endl;

    int ngram = 3;
    // Créer un analyseur de similarité et un détecteur de plagiat
    SimilarityAnalyzer analyzer(corpus);
    PlagiarismDetector detector(analyzer, ngram);

    // Resultat pour tout les documents
    auto result = detector.check_plagiarism(*doc);

    // Trie descendant
    vector<pair<shared_ptr<Document>, double>> sortedResult(result.begin(), result.end());
sort(sortedResult.begin(), sortedResult.end(),
          [](const auto& a, const auto& b) {
              return a.second > b.second;
          });

    cout << "### Résultats de plagiat" << endl;
    for (const auto& couple : sortedResult) {
        cout << "- Document : " << couple.first->title 
                  << " (" << couple.second * 100 << "% de similitude)" << endl;
    }

    // Limiter à 5 documents
    vector<shared_ptr<Document>> top_documents;
    for (const auto& [doc, score] : sortedResult) {
        top_documents.push_back(doc);
        if (top_documents.size() >= 5) break; 
    }
    map<string, int> gram_intensity;
    // resultat final apartir des 5 top documents
    double final_score = detector.get_final_score(*doc, top_documents,gram_intensity);
    // Obtenir les mots plagiés avec leur intensité
    auto word_intensity = detector.get_plagiarized_words_with_intensity(gram_intensity);
    string highlighted_text;
    // Surligner les mots plagiés (depend de la sortie)
    if (argc==5) {
        highlighted_text = doc->highlight_plagiarism_in_processed_text(word_intensity);
    } else {
        highlighted_text = doc->highlight_plagiarism_in_terminal(word_intensity);
    }

    cout << endl;
    cout << "### Ceci est le schéma des couleurs utilisé pour surligner les mots plagiés :" << endl;
    if (argc==5) {
        cout << "<red>rouge</red>, pour une intensité élevée" << endl;
        cout << "<yellow>doré</yellow>, pour une intensité moyenne" << endl;
        cout << "<magenta>magenta</magenta>, pour une intensité faible." << endl;
    } else {
        cout << "\033[31mrouge\033[0m, pour une intensite elevee" << endl;
        cout << "\033[33mdore\033[0m, pour une intensite moyenne" << endl;
        cout << "\033[35mmagenta\033[0m, pour une intensite faible." << endl;
    }

    cout << "\n### Texte surligné" << endl;
    cout << highlighted_text << endl;
    cout <<"Score final de plagiat : "<< final_score*100<<"%"<<endl;
    // Fermer le fichier de sortie si nécessaire
    if (argc==5) {
        output_file.close();
    }

    return 0;
}

