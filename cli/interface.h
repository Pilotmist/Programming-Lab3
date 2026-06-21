#pragma once

#include "../include/Sequences/ArraySequence.h"
#include "../include/Sequences/ListSequence.h"
#include "../include/Sequences/SequenceFunctions.h"
#include "../include/Specialized/BitSequence.h"
#include "../include/Specialized/Queue.h"
#include "../include/Specialized/Stack.h"
#include "../include/Specialized/Deque.h"
#include "../include/Specialized/Vector.h"
#include "../include/Common/Option.h"
#include <iostream>
#include <string>
#include <chrono>
#include <limits>

template<typename T>
void update_ptr(T*& ptr, T* newPtr) {
    if (ptr == newPtr) return;
    delete ptr;
    ptr = newPtr;
}

template<typename F>
double benchmark(F func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

Sequence<int>* create_seq(int type, int* data, int count);
int* input_int_sequence(const std::string& prompt, int& out_length);
void stackMenu();
void dequeMenu();
void vectorMenu();
void run_interface();