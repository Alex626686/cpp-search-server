#pragma once


template <typename Iterator>
class IteratorRange {

private:
    Iterator iter1_;
    Iterator iter2_;
    size_t size_;
public:
    IteratorRange(const Iterator b, const Iterator e) :
        iter1_(b),
        iter2_(e),
        size_(distance(b, e)) {}

    Iterator begin()const {
        return iter1_;
    }

    Iterator end()const {
        return iter2_;
    }

    size_t size()const {
        return size_;
    }


};

template <typename Iterator>
class Paginator {
private:
    std::vector<IteratorRange<Iterator>> pages_;

public:
    Paginator(Iterator b, Iterator e, const size_t page_size) {
        for (size_t dist = distance(b, e); dist > 0;) {
            const size_t current_page_size = std::min(dist, page_size);
            const Iterator current_page_end = next(b, current_page_size);
            pages_.push_back(IteratorRange(b, current_page_end));
            dist -= current_page_size;
            b = current_page_end;
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

