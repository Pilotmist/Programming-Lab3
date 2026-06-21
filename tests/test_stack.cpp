#include <gtest/gtest.h>
#include "../include/Specialized/Stack.h"

TEST(StackTest, BasicPushPop) {
    Stack<int> s;
    EXPECT_TRUE(s.IsEmpty());
    s.Push(10);
    EXPECT_EQ(s.Peek(), 10);
    s.Push(20);
    EXPECT_EQ(s.GetLength(), 2);
    EXPECT_EQ(s.Pop(), 20);
    EXPECT_EQ(s.Pop(), 10);
    EXPECT_TRUE(s.IsEmpty());
}

TEST(StackTest, Underflow) {
    Stack<int> s;
    EXPECT_THROW(s.Pop(), std::underflow_error);
    EXPECT_THROW(s.Peek(), std::underflow_error);
}

TEST(StackTest, MapWhereReduce) {
    Stack<int> s;
    s.Push(1); s.Push(2); s.Push(3);
    Stack<int>* mapped = s.Map([](const int& x) -> int { return x * 2; });
    ASSERT_EQ(mapped->GetLength(), 3);
    EXPECT_EQ(mapped->Pop(), 6);
    EXPECT_EQ(mapped->Pop(), 4);
    EXPECT_EQ(mapped->Pop(), 2);
    delete mapped;

    Stack<int>* filtered = s.Where([](const int& x) -> bool { return x > 1; });
    ASSERT_EQ(filtered->GetLength(), 2);
    EXPECT_EQ(filtered->Pop(), 3);
    EXPECT_EQ(filtered->Pop(), 2);
    delete filtered;

    int sum = s.Reduce(0, [](const int& a, const int& b) -> int { return a + b; });
    EXPECT_EQ(sum, 6);
}

TEST(StackTest, ConcatAndSubsequence) {
    Stack<int> s1, s2;
    s1.Push(1); s1.Push(2);
    s2.Push(3); s2.Push(4);
    Stack<int>* concat = s1.Concat(&s2);
    ASSERT_EQ(concat->GetLength(), 4);
    EXPECT_EQ(concat->Pop(), 4);
    EXPECT_EQ(concat->Pop(), 3);
    EXPECT_EQ(concat->Pop(), 2);
    EXPECT_EQ(concat->Pop(), 1);
    delete concat;

    Stack<int>* sub = s1.GetSubsequence(0, 0);
    ASSERT_EQ(sub->GetLength(), 1);
    EXPECT_EQ(sub->Peek(), 1);
    delete sub;
}

TEST(StackTest, ContainsSubsequence) {
    Stack<int> s1;
    s1.Push(1); s1.Push(2); s1.Push(3);
    Stack<int> s2;
    s2.Push(1); s2.Push(2);
    EXPECT_TRUE(s1.ContainsSubsequence(&s2));
    Stack<int> s3;
    s3.Push(2); s3.Push(3);
    EXPECT_TRUE(s1.ContainsSubsequence(&s3));
    Stack<int> s4;
    s4.Push(1); s4.Push(3);
    EXPECT_FALSE(s1.ContainsSubsequence(&s4));
}