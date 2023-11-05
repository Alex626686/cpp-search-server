#include <algorithm> 
#include <cmath> 
#include <iostream> 
#include <set> 
#include <string> 
#include <utility> 
#include <vector> 
#include <map> 

using namespace std;

const int MAX_RESULT_DOCUMENT_COUNT = 5;

string ReadLine() {
    string s;
    getline(cin, s);
    return s;
}

int ReadLineWithNumber() {
    int result = 0;
    cin >> result;
    ReadLine();
    return result;
}

vector<string> SplitIntoWords(const string& text) {
    vector<string> words;
    string word;
    for (const char c : text) {
        if (c == ' ') {
            if (!word.empty()) {
                words.push_back(word);
                word.clear();
            }
        }
        else {
            word += c;
        }
    }
    if (!word.empty()) {
        words.push_back(word);
    }

    return words;
}

struct Document {
    int id;
    double relevance;
};

class SearchServer {
public:
    void SetStopWords(const string& text) {
        for (const string& word : SplitIntoWords(text)) {
            stop_words_.insert(word);
        }
    }

    void AddDocument(int document_id, const string& document) {
        const vector<string> words = SplitIntoWordsNoStop(document);
        ++document_count_;
        double one_word = (1.0 / words.size());

        for (string word : words) {
            word_to_document_freqs_[word].insert({ document_id, +one_word });
        }
    }
    vector<Document> FindTopDocuments(const string& raw_query) const {
        const Query splited_query_words = ParseQuery(raw_query);
        auto matched_documents = FindAllDocuments(splited_query_words);

        sort(matched_documents.begin(), matched_documents.end(),
            [](const Document& lhs, const Document& rhs) {
                return lhs.relevance > rhs.relevance;
            });
        if (matched_documents.size() > MAX_RESULT_DOCUMENT_COUNT) {
            matched_documents.resize(MAX_RESULT_DOCUMENT_COUNT);
        }
        return matched_documents;
    }

private:
    struct Query {
        set <string> p_words;
        set <string> m_words;
    };

    map<string, map<int, double>> word_to_document_freqs_;
    set<string> stop_words_;
    int document_count_ = 0;

    bool IsStopWord(const string& word) const {
        return stop_words_.count(word) > 0;
    }

    vector<string> SplitIntoWordsNoStop(const string& text) const {
        vector<string> words;
        for (const string& word : SplitIntoWords(text)) {
            if (!IsStopWord(word)) {
                words.push_back(word);
            }
        }
        return words;
    }

    Query ParseQuery(const string& text) const {
        Query split_query;
        for (const string& word : SplitIntoWordsNoStop(text)) {
            if (word[0] != '-') {
                split_query.p_words.insert(word);
            }
            else {
                string new_m_word = word.substr(1);
                if (!stop_words_.count(new_m_word)) {
                    split_query.m_words.insert(new_m_word);
                }
            }
        }
        return split_query;
    }

    double CalculateIdf(const string& word) const {
        double word_count = word_to_document_freqs_.at(word).size();
        return log(document_count_ / word_count);
    }

    vector<Document> FindAllDocuments(const Query& splited_query_words) const {
        map<int, double> document_to_relevance;
        vector<Document> matched_documents;

        for (string s : splited_query_words.p_words) {
            if (word_to_document_freqs_.count(s)) {
                double idf = CalculateIdf(s);
                map<int, double> id_tf = word_to_document_freqs_.at(s);

                for (const auto& [id, tf] : id_tf) {
                    document_to_relevance[id] += tf * idf;
                }
            }
        }

        for (string s : splited_query_words.m_words) {
            if (word_to_document_freqs_.count(s)) {
                map<int, double> id_tf = word_to_document_freqs_.at(s);
                for (const auto& [id, tf] : id_tf) {
                    document_to_relevance.erase(id);
                }
            }
        }
        for (const auto& [id, relev] : document_to_relevance) {
            matched_documents.push_back({ id, relev });
        }
        return matched_documents;
    }

};

SearchServer CreateSearchServer() {
    SearchServer search_server;
    search_server.SetStopWords(ReadLine());

    const int document_count = ReadLineWithNumber();
    for (int document_id = 0; document_id < document_count; ++document_id) {
        search_server.AddDocument(document_id, ReadLine());
    }

    return search_server;
}

int main() {
    const SearchServer search_server = CreateSearchServer();

    const string query = ReadLine();
    for (const auto& [document_id, relevance] : search_server.FindTopDocuments(query)) {
        cout << "{ document_id = "s << document_id << ", " << "relevance = "s << relevance << " }"s << endl;
    }
}