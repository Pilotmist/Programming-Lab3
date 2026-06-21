#include <gtest/gtest.h>
#include "../include/Specialized/Deque.h"

TEST(DequeTest, BasicOperations) {
    Deque<int> d;
    d.PushBack(10);
    d.PushFront(5);
    d.PushBack(20);
    EXPECT_EQ(d.GetLength(), 3);
    EXPECT_EQ(d.PeekFront(), 5);
    EXPECT_EQ(d.PeekBack(), 20);
    EXPECT_EQ(d.PopFront(), 5);
    EXPECT_EQ(d.PopBack(), 20);
    EXPECT_EQ(d.PopFront(), 10);
    EXPECT_TRUE(d.IsEmpty());
}

TEST(DequeTest, Underflow) {
    Deque<int> d;
    EXPECT_THROW(d.PopFront(), std::underflow_error);
    EXPECT_THROW(d.PopBack(), std::underflow_error);
}

TEST(DequeTest, MapWhereReduce) {
    Deque<int> d;
    d.PushBack(1); d.PushBack(2); d.PushBack(3);
    Deque<int>* mapped = d.Map([](const int& x) -> int { return x * 2; });
    ASSERT_EQ(mapped->GetLength(), 3);
    EXPECT_EQ(mapped->PopFront(), 2);
    EXPECT_EQ(mapped->PopFront(), 4);
    EXPECT_EQ(mapped->PopFront(), 6);
    delete mapped;

    Deque<int>* filtered = d.Where([](const int& x) -> bool { return x % 2 == 0; });
    ASSERT_EQ(filtered->GetLength(), 1);
    EXPECT_EQ(filtered->PopFront(), 2);
    delete filtered;

    int sum = d.Reduce(0, [](const int& a, const int& b) -> int { return a + b; });
    EXPECT_EQ(sum, 6);
}


TEST(DequeTest, ConcatAndSubsequence) {
    Deque<int> d1, d2;
    d1.PushBack(1); d1.PushBack(2);
    d2.PushBack(3); d2.PushBack(4);
    Deque<int>* concat = d1.Concat(&d2);
    ASSERT_EQ(concat->GetLength(), 4);
    EXPECT_EQ(concat->PopFront(), 1);
    EXPECT_EQ(concat->PopFront(), 2);
    EXPECT_EQ(concat->PopFront(), 3);
    EXPECT_EQ(concat->PopFront(), 4);
    delete concat;

    Deque<int>* sub = d1.GetSubsequence(0, 0);
    ASSERT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->PopFront(), 1);
    delete sub;
}

TEST(DequeTest, ContainsSubsequence) {
    Deque<int> d;
    d.PushBack(1); d.PushBack(2); d.PushBack(3);
    Deque<int> sub;
    sub.PushBack(2);
    EXPECT_TRUE(d.ContainsSubsequence(&sub));
    Deque<int> sub2;
    sub2.PushBack(1); sub2.PushBack(3);
    EXPECT_FALSE(d.ContainsSubsequence(&sub2));
}

TEST(DequeTest, SortEmptyDeque) {
    Deque<int> deque;
    deque.Sort();
    EXPECT_TRUE(deque.IsEmpty());
}

TEST(DequeTest, SortSingleElement) {
    Deque<int> deque;
    deque.PushBack(5);
    deque.Sort();
    EXPECT_EQ(deque.GetLength(), 1);
    EXPECT_EQ(deque.PeekFront(), 5);
}

TEST(DequeTest, SortReverseOrder) {
    Deque<int> deque;
    deque.PushBack(5);
    deque.PushBack(4);
    deque.PushBack(3);
    deque.PushBack(2);
    deque.PushBack(1);

    deque.Sort();

    EXPECT_EQ(deque.GetLength(), 5);
    EXPECT_EQ(deque.PopFront(), 1);
    EXPECT_EQ(deque.PopFront(), 2);
    EXPECT_EQ(deque.PopFront(), 3);
    EXPECT_EQ(deque.PopFront(), 4);
    EXPECT_EQ(deque.PopFront(), 5);
}

TEST(DequeTest, SortRandomOrder) {
    Deque<int> deque;
    deque.PushBack(3);
    deque.PushBack(1);
    deque.PushBack(4);
    deque.PushBack(1);
    deque.PushBack(5);
    deque.PushBack(9);
    deque.PushBack(2);
    deque.PushBack(6);

    deque.Sort();

    EXPECT_EQ(deque.GetLength(), 8);
    EXPECT_EQ(deque.PopFront(), 1);
    EXPECT_EQ(deque.PopFront(), 1);
    EXPECT_EQ(deque.PopFront(), 2);
    EXPECT_EQ(deque.PopFront(), 3);
    EXPECT_EQ(deque.PopFront(), 4);
    EXPECT_EQ(deque.PopFront(), 5);
    EXPECT_EQ(deque.PopFront(), 6);
    EXPECT_EQ(deque.PopFront(), 9);
}

TEST(DequeTest, MergeEmptyDeques) {
    Deque<int> left;
    Deque<int> right;

    Deque<int> result = left.MergeDeques(left, right);
    EXPECT_TRUE(result.IsEmpty());
}

TEST(DequeTest, MergeWithEmptyLeft) {
    Deque<int> right;
    right.PushBack(1);
    right.PushBack(2);
    right.PushBack(3);

    Deque<int> left;
    Deque<int> result = left.MergeDeques(left, right);

    EXPECT_EQ(result.GetLength(), 3);
    EXPECT_EQ(result.PopFront(), 1);
    EXPECT_EQ(result.PopFront(), 2);
    EXPECT_EQ(result.PopFront(), 3);
}

TEST(DequeTest, MergeWithEmptyRight) {
    Deque<int> left;
    left.PushBack(1);
    left.PushBack(2);
    left.PushBack(3);

    Deque<int> right;
    Deque<int> result = left.MergeDeques(left, right);

    EXPECT_EQ(result.GetLength(), 3);
    EXPECT_EQ(result.PopFront(), 1);
    EXPECT_EQ(result.PopFront(), 2);
    EXPECT_EQ(result.PopFront(), 3);
}

TEST(DequeTest, MergeTwoSortedDeques) {
    Deque<int> left;
    left.PushBack(1);
    left.PushBack(3);
    left.PushBack(5);
    left.PushBack(7);

    Deque<int> right;
    right.PushBack(2);
    right.PushBack(4);
    right.PushBack(6);
    right.PushBack(8);

    Deque<int> result = left.MergeDeques(left, right);

    EXPECT_EQ(result.GetLength(), 8);
    EXPECT_EQ(result.PopFront(), 1);
    EXPECT_EQ(result.PopFront(), 2);
    EXPECT_EQ(result.PopFront(), 3);
    EXPECT_EQ(result.PopFront(), 4);
    EXPECT_EQ(result.PopFront(), 5);
    EXPECT_EQ(result.PopFront(), 6);
    EXPECT_EQ(result.PopFront(), 7);
    EXPECT_EQ(result.PopFront(), 8);
}

TEST(DequeTest, MergeWithDuplicates) {
    Deque<int> left;
    left.PushBack(1);
    left.PushBack(2);
    left.PushBack(4);
    left.PushBack(6);

    Deque<int> right;
    right.PushBack(2);
    right.PushBack(3);
    right.PushBack(4);
    right.PushBack(5);

    Deque<int> result = left.MergeDeques(left, right);

    EXPECT_EQ(result.GetLength(), 8);
    EXPECT_EQ(result.PopFront(), 1);
    EXPECT_EQ(result.PopFront(), 2);
    EXPECT_EQ(result.PopFront(), 2);
    EXPECT_EQ(result.PopFront(), 3);
    EXPECT_EQ(result.PopFront(), 4);
    EXPECT_EQ(result.PopFront(), 4);
    EXPECT_EQ(result.PopFront(), 5);
    EXPECT_EQ(result.PopFront(), 6);
}

TEST(DequeTest, MergeUnevenSizes) {
    Deque<int> left;
    left.PushBack(1);
    left.PushBack(3);
    left.PushBack(5);

    Deque<int> right;
    right.PushBack(2);
    right.PushBack(4);
    right.PushBack(6);
    right.PushBack(8);
    right.PushBack(10);

    Deque<int> result = left.MergeDeques(left, right);

    EXPECT_EQ(result.GetLength(), 8);
    EXPECT_EQ(result.PopFront(), 1);
    EXPECT_EQ(result.PopFront(), 2);
    EXPECT_EQ(result.PopFront(), 3);
    EXPECT_EQ(result.PopFront(), 4);
    EXPECT_EQ(result.PopFront(), 5);
    EXPECT_EQ(result.PopFront(), 6);
    EXPECT_EQ(result.PopFront(), 8);
    EXPECT_EQ(result.PopFront(), 10);
}

TEST(DequeTest, MergeWithNegativeNumbers) {
    Deque<int> left;
    left.PushBack(-5);
    left.PushBack(-3);
    left.PushBack(0);

    Deque<int> right;
    right.PushBack(-4);
    right.PushBack(-2);
    right.PushBack(1);
    right.PushBack(3);

    Deque<int> result = left.MergeDeques(left, right);

    EXPECT_EQ(result.GetLength(), 7);
    EXPECT_EQ(result.PopFront(), -5);
    EXPECT_EQ(result.PopFront(), -4);
    EXPECT_EQ(result.PopFront(), -3);
    EXPECT_EQ(result.PopFront(), -2);
    EXPECT_EQ(result.PopFront(), 0);
    EXPECT_EQ(result.PopFront(), 1);
    EXPECT_EQ(result.PopFront(), 3);
}

TEST(DequeTest, MergeStrings) {
    Deque<std::string> left;
    left.PushBack("apple");
    left.PushBack("cherry");
    left.PushBack("elderberry");

    Deque<std::string> right;
    right.PushBack("banana");
    right.PushBack("date");
    right.PushBack("fig");

    Deque<std::string> result = left.MergeDeques(left, right);

    EXPECT_EQ(result.GetLength(), 6);
    EXPECT_EQ(result.PopFront(), "apple");
    EXPECT_EQ(result.PopFront(), "banana");
    EXPECT_EQ(result.PopFront(), "cherry");
    EXPECT_EQ(result.PopFront(), "date");
    EXPECT_EQ(result.PopFront(), "elderberry");
    EXPECT_EQ(result.PopFront(), "fig");
}

TEST(DequeTest, LargeDequeSort) {
    Deque<int> deque;
    const int SIZE = 1000;

    for (int i = SIZE; i > 0; --i) {
        deque.PushBack(i);
    }

    deque.Sort();

    EXPECT_EQ(deque.GetLength(), SIZE);
    for (int i = 1; i <= SIZE; ++i) {
        EXPECT_EQ(deque.PopFront(), i);
    }
}

TEST(DequeTest, LargeDequeMerge) {
    Deque<int> left;
    Deque<int> right;
    const int SIZE = 500;

    for (int i = 1; i <= SIZE; ++i) {
        if (i % 2 == 1) {
            left.PushBack(i);
        } else {
            right.PushBack(i);
        }
    }

    Deque<int> result = left.MergeDeques(left, right);

    EXPECT_EQ(result.GetLength(), SIZE);
    for (int i = 1; i <= SIZE; ++i) {
        EXPECT_EQ(result.PopFront(), i);
    }
}

TEST(DequeTest, MergeDoesNotModifyOriginals) {
    Deque<int> left;
    left.PushBack(1);
    left.PushBack(3);
    left.PushBack(5);

    Deque<int> right;
    right.PushBack(2);
    right.PushBack(4);
    right.PushBack(6);

    Deque<int> result = left.MergeDeques(left, right);

    EXPECT_EQ(left.GetLength(), 3);
    EXPECT_EQ(left.PopFront(), 1);
    EXPECT_EQ(left.PopFront(), 3);
    EXPECT_EQ(left.PopFront(), 5);

    EXPECT_EQ(right.GetLength(), 3);
    EXPECT_EQ(right.PopFront(), 2);
    EXPECT_EQ(right.PopFront(), 4);
    EXPECT_EQ(right.PopFront(), 6);
}
