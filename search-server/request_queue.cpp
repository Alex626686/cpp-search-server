#include "request_queue.h"




RequestQueue::RequestQueue(const SearchServer& search_server)
    : search_server_(search_server) {}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query, DocumentStatus status) {
    std::vector<Document> res = search_server_.FindTopDocuments(raw_query, status);
    requests_.push_back({ res });
    if (requests_.size() > min_in_day_) {
        requests_.pop_front();
    }
    return res;
}

std::vector<Document> RequestQueue::AddFindRequest(const std::string& raw_query) {
    std::vector<Document> res = search_server_.FindTopDocuments(raw_query);
    requests_.push_back({ res });
    if (requests_.size() > min_in_day_) {
        requests_.pop_front();
    }
    return res;
}

int RequestQueue::GetNoResultRequests() const {
    int res = 0;
    for (const auto& qr : requests_) {
        if (qr.res_request.empty())
            ++res;
    }
    return res;
}