#include <iostream>
#include <vector>
#include <map>
#include <cmath>
#include <string>
#include <cstdlib>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <set>
#include <regex>
#include <iterator>

// TODO - Implement the weighted similarity calculation

// TODO - make Corpus Class

class Document {
public:
    std::string text;
    // TODO : change tf to frequency divide by length of n-grams
    std::map<std::string, int> tf;
    std::map<std::string, double> tf_idf;
    // TODO : add name attribute
    // TODO : constructor pdf , text reader
    explicit Document(const std::string& t) : text(t) {}

    void tokenization() {
        // Convert to lowercase
        std::transform(text.begin(), text.end(), text.begin(), ::tolower);

        // Remove punctuation
        text = std::regex_replace(text, std::regex(R"([\.,!?\-;:\"'\(\)\[\]])"), " ");

        // Tokenize and remove stopwords
        std::set<std::string> stopwords = {"the", "is", "in", "and", "of", "to", "a", "it", "on", "for", "as", "at", "with", "by", "an", "be", "this", "that"};
        std::istringstream stream(text);
        std::string word;
        std::string clean_text;

        while (stream >> word) {
            if (stopwords.find(word) == stopwords.end()) {
                clean_text += word + " ";
            }
        }
        text = clean_text;
    }

    // Function to create n-grams (bigram or trigram)
    std::vector<std::string> create_ngrams(int n) const {
    std::vector<std::string> ngrams;
    std::istringstream stream(text);
    std::vector<std::string> words;
    std::string word;

    // Tokenize the text into words
    while (stream >> word) {
        words.push_back(word);
    }

    // Create n-grams
    for (size_t i = 0; i + n <= words.size(); ++i) {
        std::string ngram = words[i];
        for (int j = 1; j < n; ++j) {
            ngram += " " + words[i + j];
        }
        ngrams.push_back(ngram);
    }

    return ngrams;
}

    void compute_tf(int n) {
        std::map<std::string, int> ngram_count;
        std::vector<std::string> ngrams = create_ngrams(n);

        for (const auto& ngram : ngrams) {
            ngram_count[ngram]++;
        }
        tf = ngram_count;
    }

    void compute_tf_idf(const std::vector<Document>& docs, int n) {
        int doc_count = docs.size();
        std::map<std::string, int> df; // Document frequency

        // Compute DF
        for (const auto& doc : docs) {
            std::set<std::string> unique_ngrams;
            std::vector<std::string> ngrams = doc.create_ngrams(n);
            for (const auto& ngram : ngrams) {
                unique_ngrams.insert(ngram);
            }
            for (const auto& ngram : unique_ngrams) {
                df[ngram]++;
            }
        }

        // Compute TF-IDF
        for (const auto& ngram : tf) {
            int doc_freq = df[ngram.first];
            double idf = log((double)doc_count / (doc_freq + 1));
            tf_idf[ngram.first] = ngram.second * idf;
        }
    }
};

class SimilarityAnalyzer {
public:
    // TODO : tf-idf calculator
    // TODO : euclidien calculator
    // TODO : Manhattan calculator
    enum Method {
        COSINE,
        JACCARD
    };

    static double compute_similarity(const Document& doc1, const Document& doc2, Method method) {
        switch (method) {
        case COSINE:
            return compute_cosine_similarity(doc1, doc2);
        case JACCARD:
            return compute_jaccard_similarity(doc1, doc2);
        default:
            return 0.0;
        }
    }

private:
    static double compute_cosine_similarity(const Document& doc1, const Document& doc2) { // APPROVED
        double dot_product = 0.0, magnitude_doc1 = 0.0, magnitude_doc2 = 0.0;

        for (const auto& ngram : doc1.tf_idf) {
            if (doc2.tf_idf.find(ngram.first) != doc2.tf_idf.end()) { // if the ngram is common
                dot_product += ngram.second * doc2.tf_idf.at(ngram.first);
            }
            magnitude_doc1 += ngram.second * ngram.second;
        }

        for (const auto& ngram : doc2.tf_idf) {
            magnitude_doc2 += ngram.second * ngram.second;
        }

        if (magnitude_doc1 == 0 || magnitude_doc2 == 0) {
            return 0.0;
        }

        return dot_product / (sqrt(magnitude_doc1) * sqrt(magnitude_doc2));
    }

    static double compute_jaccard_similarity(const Document& doc1, const Document& doc2) { // APPROVED
        std::set<std::string> ngrams1, ngrams2;

        for (const auto& ngram : doc1.tf) {
            ngrams1.insert(ngram.first);
        }
        for (const auto& ngram : doc2.tf) {
            ngrams2.insert(ngram.first);
        }

        std::set<std::string> intersection, union_set;
        std::set_intersection(ngrams1.begin(), ngrams1.end(), ngrams2.begin(), ngrams2.end(),
                              std::inserter(intersection, intersection.begin()));
        std::set_union(ngrams1.begin(), ngrams1.end(), ngrams2.begin(), ngrams2.end(),
                       std::inserter(union_set, union_set.begin()));

        return (double)intersection.size() / union_set.size();
    }
};
class PlagiarismDetector {
public:
    std::vector<Document> documents;

    // Method to check plagiarism using a similarity method
    std::map<std::pair<int, int>, double> check_plagiarism(SimilarityAnalyzer::Method method) {
        std::map<std::pair<int, int>, double> similarity_scores;

        for (size_t i = 0; i < documents.size(); ++i) {
            for (size_t j = i + 1; j < documents.size(); ++j) {
                double score = SimilarityAnalyzer::compute_similarity(documents[i], documents[j], method);
                similarity_scores[{i, j}] = score;
            }
        }

        return similarity_scores;
    }
};

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <max n-gram size>" << std::endl;
        return 1;
    }

    int max_n = std::atoi(argv[1]);
    if (max_n <= 0) {
        std::cerr << "Invalid max n-gram size. It must be a positive integer." << std::endl;
        return 1;
    }

    // Documents
    Document doc1("Machine learning and artificial intelligence are revolutionizing the way we analyze data. These technologies enable predictive modeling, improve decision-making processes, and create opportunities for innovation. AI applications include natural language processing, computer vision, and robotics.");
    Document doc2("Machine learning and artificial intelligence are transforming data analysis. These technologies enable predictive modeling, improve decision-making processes, and drive innovation. Applications of AI include natural language processing, computer vision, and robotics.");
    Document doc3("Machine learning and artificial intelligence have revolutionized data analysis. These technologies allow predictive modeling, improve decision-making, and foster innovation. AI applications include natural language processing, computer vision, and robotics.");

    std::vector<Document> documents = {doc1, doc2, doc3};

    // TODO : maybe add it to the PlagiarismDetector class

    // Define weights for each n-gram size
    std::map<int, double> weights;
    double total_weight = 0.0;
    for (int n = 1; n <= max_n; ++n) {
        weights[n] = 1.0 / n; // Example: Inverse weighting
        total_weight += weights[n];
    }
    
    // Normalize weights to sum to 1
    for (auto& [n, weight] : weights) {
        weight /= total_weight;
    }

    // Store scores for weighted calculation
    std::map<std::pair<int, int>, double> weighted_scores;

    // Iterate through n-gram sizes
    for (int n = 1; n <= max_n; ++n) {
        for (auto& doc : documents) {
            doc.tokenization();
            doc.compute_tf(n);
        }

        for (auto& doc : documents) {
            doc.compute_tf_idf(documents, n);
        }

        SimilarityAnalyzer::Method method = SimilarityAnalyzer::COSINE;
        PlagiarismDetector detector;
        detector.documents = documents;

        auto scores = detector.check_plagiarism(method);

        // Accumulate weighted scores
        for (const auto& [pair, score] : scores) {
            weighted_scores[pair] += weights[n] * score;
        }
    }
    
    // Output the final weighted similarity scores
    // TODO : CHange output 
    for (const auto& [pair, score] : weighted_scores) {
        std::cout << "Weighted similarity between document " << pair.first + 1
                  << " and document " << pair.second + 1
                  << ": " << score << std::endl;
    }

    return 0;
}