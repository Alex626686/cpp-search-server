#include "request_queue.h"




RequestQueue::RequestQueue(const SearchServer& search_server)
    : search_server_(search_server) {}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    std::vector<Document> res = search_server_.FindTopDocuments(raw_query, status);
    if (res.empty()) {
        requests_.push_back({ true });
    }
    else {
        requests_.push_back({ false });
    }
    if (requests_.size() > min_in_day_) {
        requests_.pop_front();
    }
    return res;
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    std::vector<Document> res = search_server_.FindTopDocuments(raw_query);
    if (res.empty()) {
        requests_.push_back({ true });
    }
    else {
        requests_.push_back({ false });
    }
    if (requests_.size() > min_in_day_) {
        requests_.pop_front();
    }
    return res;
}

int RequestQueue::GetNoResultRequests() const {
    int res = 0;
    for (const auto& qr : requests_) {
        if (qr.empty_request)
            ++res;
    }
    return res;
}