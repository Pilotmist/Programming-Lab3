#pragma once
#include "Sequence.h"
#include "../Base/LinkedList.h"
#include <stdexcept>

template<class T>
class ListSequence : public Sequence<T> {
private:
    class ListEnumerator : public IEnumerator<T> {
    private:
        const LinkedList<T> *list;
        const typename LinkedList<T>::Node *currentNode;
    public:
        ListEnumerator(const LinkedList<T> *l) : list(l), currentNode(nullptr) {}
        bool MoveNext() override {
            if (currentNode == nullptr) {
                currentNode = list->GetFirstNode();
            } else {
                currentNode = currentNode->next;
            }
            return currentNode != nullptr;
        }
        T GetCurrent() const override {
            if (currentNode == nullptr)
                throw std::out_of_range("Enumerator not started or finished");
            return currentNode->data;
        }
        void Reset() override {
            currentNode = nullptr;
        }
    };

protected:
    LinkedList<T> *items;

    ListSequence(LinkedList<T> *list) : items(list) {}

    virtual Sequence<T> *instance() = 0;
    virtual Sequence<T> *CreateEmpty() const = 0;

public:
    ListSequence() : items(new LinkedList<T>()) {}
    ListSequence(T *data, int count) : items(new LinkedList<T>(data, count)) {}
    ListSequence(const LinkedList<T> &list) : items(new LinkedList<T>(list)) {}

    ListSequence(const ListSequence&) = delete;
    ListSequence& operator=(const ListSequence&) = delete;

    virtual ~ListSequence() { delete items; }

    T GetFirst() const override {
        if (GetLength() == 0) throw std::out_of_range("Sequence is empty");
        return items->GetFirst();
    }

    T GetLast() const override {
        if (GetLength() == 0) throw std::out_of_range("Sequence is empty");
        return items->GetLast();
    }

    T Get(int index) const override {
        if (index < 0 || index >= GetLength()) throw std::out_of_range("IndexOutOfRange");
        return items->Get(index);
    }

    int GetLength() const override { return items->GetLength(); }
    const T operator[](int index) const override { return items->Get(index); }
    T &operator[](int index) override { return items->GetReference(index); }
    IEnumerator<T> *GetEnumerator() override { return new ListEnumerator(this->items); }

    Sequence<T> *Append(const T &item) override {
        Sequence<T> *target = this->instance();
        static_cast<ListSequence<T> *>(target)->items->Append(item);
        return target;
    }

    Sequence<T> *Prepend(const T &item) override {
        Sequence<T> *target = this->instance();
        static_cast<ListSequence<T> *>(target)->items->Prepend(item);
        return target;
    }

    Sequence<T> *InsertAt(const T &item, int index) override {
        if (index < 0 || index > GetLength()) throw std::out_of_range("IndexOutOfRange");
        Sequence<T> *target = this->instance();
        static_cast<ListSequence<T> *>(target)->items->InsertAt(item, index);
        return target;
    }

    Sequence<T> *Clone() const override {
        Sequence<T> *res = this->CreateEmpty();
        return res->Concat(const_cast<ListSequence<T> *>(this));
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= GetLength() || startIndex > endIndex)
            throw std::out_of_range("IndexOutOfRange");
        Sequence<T>* result = this->CreateEmpty();
        auto current = items->GetFirstNode();
        int i = 0;
        while (current && i <= endIndex) {
            if (i >= startIndex) {
                result->Append(current->data);
            }
            current = current->next;
            ++i;
        }
        return result;
    }

    Sequence<T> *Concat(Sequence<T> *list) override {
        Sequence<T> *target = this->instance();
        auto *enumerator = list->GetEnumerator();
        while (enumerator->MoveNext()) {
            target->Append(enumerator->GetCurrent());
        }
        delete enumerator;
        return target;
    }
};

template<class T>
class MutableListSequence : public ListSequence<T> {
public:
    using ListSequence<T>::ListSequence;
    Sequence<T> *CreateEmpty() const override { return new MutableListSequence<T>(); }
    Sequence<T>* RemoveAt(int index) override {
        if (index < 0 || index >= this->items->GetLength()) throw std::out_of_range("IndexOutOfRange");
        this->items->RemoveAt(index);
        return this;
    }
protected:
    Sequence<T> *instance() override { return this; }
};

template<typename T>
class ImmutableListSequence : public ListSequence<T> {
public:
    ImmutableListSequence() : ListSequence<T>() {}
    ImmutableListSequence(LinkedList<T> *list) : ListSequence<T>(list) {}
    ImmutableListSequence(T *data, int count) : ListSequence<T>(data, count) {}
    ~ImmutableListSequence() = default;

    Sequence<T>* Append(const T &item) override {
        LinkedList<T> *newList = new LinkedList<T>(*this->items);
        newList->Append(item);
        return new ImmutableListSequence(newList);
    }

    Sequence<T> *Prepend(const T &item) override {
        LinkedList<T> *newList = new LinkedList<T>(*this->items);
        newList->Prepend(item);
        return new ImmutableListSequence(newList);
    }

    Sequence<T> *InsertAt(const T &item, int index) override {
        if (index > this->items->GetLength()) throw std::out_of_range("IndexOutOfRange");
        LinkedList<T> *newList = new LinkedList<T>(*this->items);
        newList->InsertAt(item, index);
        return new ImmutableListSequence(newList);
    }

    Sequence<T>* GetSubsequence(int startIndex, int endIndex) const override {
        if (startIndex < 0 || endIndex >= this->GetLength() || startIndex > endIndex)
            throw std::out_of_range("IndexOutOfRange");

        LinkedList<T>* newList = new LinkedList<T>();
        typename LinkedList<T>::Node* current = this->items->GetFirstNode();
        int i = 0;
        while (current && i <= endIndex) {
            if (i >= startIndex) {
                newList->Append(current->data);
            }
            current = current->next;
            ++i;
        }
        return new ImmutableListSequence(newList);
    }

    Sequence<T>* Concat(Sequence<T>* list) override {
        LinkedList<T>* newList = new LinkedList<T>(*this->items);
        for (int i = 0; i < list->GetLength(); ++i) {
            newList->Append(list->Get(i));
        }
        return new ImmutableListSequence(newList);
    }

    Sequence<T> *CreateEmpty() const override { return new ImmutableListSequence<T>(); }

    Sequence<T>* instance() override {
        return new ImmutableListSequence(new LinkedList<T>(*this->items));
    }

    Sequence<T>* RemoveAt(int index) override {
        if (index < 0 || index >= this->items->GetLength()) throw std::out_of_range("IndexOutOfRange");
        LinkedList<T>* newList = new LinkedList<T>(*this->items);
        newList->RemoveAt(index);
        return new ImmutableListSequence(newList);
    }

    T &operator[](int index) override {
        throw std::runtime_error("Immutable sequence does not support non-const operator[]");
    }
};