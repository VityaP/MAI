#ifndef TITERATOR_H
#define TITERATOR_H

#include <memory>
#include <iostream>

template <class N, class T>
class TIterator
{
public:
    TIterator(std::shared_ptr<N> n) {
        cur = n;
    }

    std::shared_ptr<T> operator* () {
        return cur->GetFigure();
    }

    std::shared_ptr<T> operator-> () {
        return cur->GetFigure();
    }

    void operator++() {
        cur = cur->GetNext();
    }

    TIterator operator++ (int) {
        TIterator cur(*this);
        ++(*this);
        return cur;
    }

    bool operator== (const TIterator &i) {
        return (cur->GetFigure()->Side() == i.cur->GetFigure()->Side());
    }

    bool operator!= (const TIterator &i) {
        return (cur->GetFigure()->Side() != i.cur->GetFigure()->Side());
    }

    bool operator<= (const TIterator &i) {
        return (cur->GetFigure()->Side() <= i.cur->GetFigure()->Side());
    }

private:
    std::shared_ptr<N> cur;
};

#endif