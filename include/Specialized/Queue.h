#pragma once
#include "SequenceCollection.h"
#include "../Sequences/SequenceFunctions.h"
#include "../Common/Pair.h"
#include <stdexcept>

template<typename T>
class Queue : public SequenceCollection<T, Queue<T>> {
    using Base = SequenceCollection<T, Queue<T>>;
public:
    Queue() : Base() {}
    explicit Queue(Sequence<T>* seq) : Base(seq) {}

    void Enqueue(const T &item) { this->data->Append(item); }

    T Dequeue() {
        if (this->IsEmpty()) throw std::out_of_range("Queue underflow");
        T item = this->data->GetFirst();
        this->data->RemoveAt(0);
        return item;
    }

    T Peek() const {
        if (this->IsEmpty()) throw std::out_of_range("Queue is empty");
        return this->data->GetFirst();
    }

    template<typename Predicate>
    Pair<Queue<T>*, Queue<T>*> SplitBy(Predicate pred) {
        auto seqPair = SequenceFunctions::SplitBy(this->data, pred);
        return Pair<Queue<T>*, Queue<T>*>(new Queue<T>(seqPair.first),
                                          new Queue<T>(seqPair.second));
    }
};