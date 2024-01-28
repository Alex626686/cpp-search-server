#pragma once


template <typename Iterator>
class IteratorRange {

private:
    Iterator begin_;
    Iterator end_;
public:
    IteratorRange(const Iterator first, const Iterator last) :
        begin_(first),
        end_(last)
    {}

    Iterator begin()const {
        return begin_;
    }

    Iterator end()const {
        return end_;
    }

    size_t size()const {
        return distance(begin_, end_);
    }


};

template <typename Iterator>
class Paginator {
private:
    std::vector<IteratorRange<Iterator>> pages_;

public:
    Paginator(Iterator begin, Iterator end, const size_t page_size) {
        for (size_t dist = distance(begin, end); dist > 0;) {
            const size_t current_page_size = std::min(dist, page_size);
            const Iterator current_page_end = next(begin, current_page_size);
            pages_.push_back(IteratorRange(begin, current_page_end));
            dist -= current_page_size;
            begin = current_page_end;
        }
    }
    auto begin() const {
        return pages_.begin();
    }
    auto end()const {
        return pages_.end();
    }
    auto size()const {
        return pages_.size();
    }
};


template <typename Iterator>
std::ostream& operator<< (std::ostream& out, const IteratorRange<Iterator> range) {
    for (auto i : range) {
        out << i;
    }
    return out;
}


template <typename Container>
auto Paginate(const Container& c, size_t page_size) {
    return Paginator(begin(c), end(c), page_size);
}
