#pragma once
#include "SequenceCollection.h"
#include "../Sequences/SequenceFunctions.h"
#include <stdexcept>

template<typename T>
class Deque : public SequenceCollection<T, Deque<T> > {
    using Base = SequenceCollection<T, Deque<T> >;

public:
    Deque() : Base() {
    }

    explicit Deque(Sequence<T> *seq) : Base(seq) {
    }

    void PushFront(const T &item) { this->data->Prepend(item); }
    void PushBack(const T &item) { this->data->Append(item); }

    T PopFront() {
        if (this->IsEmpty()) throw std::underflow_error("Deque underflow");
        T val = this->data->GetFirst();
        this->data->RemoveAt(0);
        return val;
    }

    T PopBack() {
        if (this->IsEmpty()) throw std::underflow_error("Deque underflow");
        T val = this->data->GetLast();
        this->data->RemoveAt(this->data->GetLength() - 1);
        return val;
    }

    T PeekFront() const {
        if (this->IsEmpty()) throw std::underflow_error("Deque empty");
        return this->data->GetFirst();
    }

    T PeekBack() const {
        if (this->IsEmpty()) throw std::underflow_error("Deque empty");
        return this->data->GetLast();
    }

    Deque<T> static MergeDeques(Deque<T> &left, Deque<T> &right){
        Deque<T> result;

        IEnumerator<T> *leftEnum = left.data->GetEnumerator();
        IEnumerator<T> *rightEnum = right.data->GetEnumerator();

        bool hasLeft = leftEnum->MoveNext();
        bool hasRight = rightEnum->MoveNext();

        while (hasLeft && hasRight) {
            T leftVal = leftEnum->GetCurrent();
            T rightVal = rightEnum->GetCurrent();

            if (leftVal <= rightVal) {
                result.PushBack(leftVal);
                hasLeft = leftEnum->MoveNext();
            } else {
                result.PushBack(rightVal);
                hasRight = rightEnum->MoveNext();
            }
        }

        while (hasLeft) {
            result.PushBack(leftEnum->GetCurrent());
            hasLeft = leftEnum->MoveNext();
        }

        while (hasRight) {
            result.PushBack(rightEnum->GetCurrent());
            hasRight = rightEnum->MoveNext();
        }

        delete leftEnum;
        delete rightEnum;

        return result;
    }

    void Sort() {
        int len = this->data->GetLength();
        if (len <= 1) return;
        T* arr = new T[len];
        auto* it = this->data->GetEnumerator();
        int i = 0;
        while (it->MoveNext()) {
            arr[i++] = it->GetCurrent();
        }
        delete it;
        SequenceFunctions::SortArray(arr, len);
        Sequence<T>* newSeq = new MutableArraySequence<T>();
        for (int j = 0; j < len; ++j) {
            newSeq->Append(arr[j]);
        }
        delete[] arr;
        delete this->data;
        this->data = newSeq;
    }
};