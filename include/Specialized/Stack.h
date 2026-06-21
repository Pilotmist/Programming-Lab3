#pragma once
#include "SequenceCollection.h"
#include <stdexcept>

template<typename T>
class Stack : public SequenceCollection<T, Stack<T>> {
    using Base = SequenceCollection<T, Stack<T>>;
public:
    Stack() : Base() {}
    explicit Stack(Sequence<T>* seq) : Base(seq) {}

    void Push(const T &item) { this->data->Append(item); }

    T Pop() {
        if (this->IsEmpty()) throw std::underflow_error("Stack underflow");
        T val = this->data->GetLast();
        this->data->RemoveAt(this->data->GetLength() - 1);
        return val;
    }

    T Peek() const {
        if (this->IsEmpty()) throw std::underflow_error("Stack empty");
        return this->data->GetLast();
    }
};