#pragma once
#include "../Sequences/Sequence.h"
#include "../Sequences/SequenceFunctions.h"
#include "../Sequences/ListSequence.h"

template <typename T, typename Derived> // T - тип элементов в коллекции,
// Derived (полученный) - переданная коллекция (Стек, очередь, дека)
class SequenceCollection {
protected:
    Sequence<T>* data;

    explicit SequenceCollection(Sequence<T>* seq) : data(seq) {}

    Sequence<T>* CreateSequence() const {
        return new MutableListSequence<T>();
    }

public:
    SequenceCollection() : data(CreateSequence()) {}

    SequenceCollection(const SequenceCollection&) = delete;
    SequenceCollection& operator=(const SequenceCollection&) = delete;

    SequenceCollection(SequenceCollection&& other) noexcept : data(other.data) {
        other.data = nullptr;
    }

    virtual ~SequenceCollection() { delete data; }

    template <typename Func>
    Derived* Map(Func func) const {
        Sequence<T>* newSeq = SequenceFunctions::Map(data, func);
        return new Derived(newSeq);
    }

    template <typename Predicate>
    Derived* Where(Predicate pred) const {
        Sequence<T>* newSeq = SequenceFunctions::Where(data, pred);
        return new Derived(newSeq);
    }

    template <typename U, typename Func>
    U Reduce(U initial, Func func) const {
        return SequenceFunctions::Reduce(data, initial, func);
    }

    Derived* Concat(const Derived* other) const {
        Sequence<T>* cloned = data->Clone();
        Sequence<T>* resultSeq = cloned->Concat(other->data);
        if (resultSeq != cloned) {
            delete cloned;
        }
        return new Derived(resultSeq);
    }

    Derived* GetSubsequence(int start, int end) const {
        Sequence<T>* sub = data->GetSubsequence(start, end);
        return new Derived(sub);
    }

    bool ContainsSubsequence(const Derived* sub) const {
        return SequenceFunctions::ContainsSubsequence(data, sub->data);
    }

    bool IsEmpty() const { return data->GetLength() == 0; }
    int GetLength() const { return data->GetLength(); }
    const Sequence<T>* GetSequence() const { return data; }
};