#include <gtest/gtest.h>
#include "../include/Specialized/Queue.h"
#include <string>

TEST(QueueTest, ConstructorAndEmptyState) {
    Queue<int> q;
    EXPECT_TRUE(q.IsEmpty());
    EXPECT_EQ(q.GetLength(), 0);
}

TEST(QueueTest, EnqueueDequeue) {
    Queue<int> q;
    q.Enqueue(10);
    q.Enqueue(20);
    q.Enqueue(30);

    EXPECT_EQ(q.GetLength(), 3);
    EXPECT_FALSE(q.IsEmpty());

    EXPECT_EQ(q.Peek(), 10);
    EXPECT_EQ(q.Dequeue(), 10);
    EXPECT_EQ(q.Dequeue(), 20);
    EXPECT_EQ(q.GetLength(), 1);
    EXPECT_EQ(q.Dequeue(), 30);
    EXPECT_TRUE(q.IsEmpty());
}

TEST(QueueTest, Exceptions) {
    Queue<int> q;
    EXPECT_THROW(q.Dequeue(), std::out_of_range);
    EXPECT_THROW(q.Peek(), std::out_of_range);
}

TEST(QueueTest, MapFunction) {
    Queue<int> q;
    q.Enqueue(1);
    q.Enqueue(2);
    q.Enqueue(3);

    auto* mappedQueue = q.Map([](int x) { return x * 2; });

    EXPECT_EQ(mappedQueue->GetLength(), 3);
    EXPECT_EQ(mappedQueue->Dequeue(), 2);
    EXPECT_EQ(mappedQueue->Dequeue(), 4);
    EXPECT_EQ(mappedQueue->Dequeue(), 6);

    delete mappedQueue;
}

TEST(QueueTest, WhereFunction) {
    Queue<int> q;
    q.Enqueue(1);
    q.Enqueue(10);
    q.Enqueue(3);
    q.Enqueue(20);

    auto* filteredQueue = q.Where([](int x) { return x > 5; });

    EXPECT_EQ(filteredQueue->GetLength(), 2);
    EXPECT_EQ(filteredQueue->Dequeue(), 10);
    EXPECT_EQ(filteredQueue->Dequeue(), 20);

    delete filteredQueue;
}

TEST(QueueTest, ReduceFunction) {
    Queue<int> q;
    q.Enqueue(1);
    q.Enqueue(2);
    q.Enqueue(3);
    q.Enqueue(4);

    int sum = q.Reduce(0, [](int acc, int x) { return acc + x; });
    EXPECT_EQ(sum, 10);
}

TEST(QueueTest, ConcatQueues) {
    Queue<int> q1;
    q1.Enqueue(1);
    Queue<int> q2;
    q2.Enqueue(2);

    auto* result = q1.Concat(&q2);

    EXPECT_EQ(result->GetLength(), 2);
    EXPECT_EQ(result->Dequeue(), 1);
    EXPECT_EQ(result->Dequeue(), 2);

    delete result;
}

TEST(QueueTest, SubsequenceOperations) {
    Queue<int> q;
    for(int i = 0; i < 5; ++i) q.Enqueue(i);

    auto* sub = q.GetSubsequence(1, 3);
    EXPECT_EQ(sub->GetLength(), 3);
    EXPECT_EQ(sub->Dequeue(), 1);

    EXPECT_TRUE(q.ContainsSubsequence(sub));

    delete sub;
}

TEST(QueueTest, SplitBy) {
    Queue<int> q;
    q.Enqueue(1);
    q.Enqueue(2);
    q.Enqueue(3);
    q.Enqueue(4);

    auto parts = q.SplitBy([](const int& x) { return x % 2 == 0; });
    EXPECT_EQ(parts.first->GetLength(), 2);
    EXPECT_EQ(parts.second->GetLength(), 2);
    EXPECT_EQ(parts.first->Dequeue(), 2);
    EXPECT_EQ(parts.second->Dequeue(), 1);
    delete parts.first;
    delete parts.second;
}