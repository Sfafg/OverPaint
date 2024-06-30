#pragma once

template <typename T, unsigned int TSize = 10>
class Stack
{
    T arr[TSize];
    int top;
public:
    Stack() :top(0) {}

    bool Empty() const
    {
        return top == 0;
    }
    bool Full() const
    {
        return top == TSize;
    }
    int Size() const
    {
        return top;
    }

    T Top()
    {
        if (Empty()) return T();

        return arr[top - 1];
    }

    void Push(T element)
    {
        if (Full()) return;

        arr[top] = element;
        top++;
    }

    T Pop()
    {
        if (Empty()) { return T(); }

        T t = arr[top];
        top--;
        return t;
    }
};