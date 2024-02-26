#pragma once

#include <Arduino.h>


template<typename T>
class RingBuffer {
private:
    static constexpr size_t BUFFER_SIZE = 30;
    T *buffer;
    size_t head = 0;
    size_t tail = 0;

public:
    RingBuffer() : buffer(new T[BUFFER_SIZE]) {}

    explicit RingBuffer(size_t size) : buffer(new T[size]) {}

    ~RingBuffer() {
        delete[] buffer;
    }

    bool isFull() const {
        return (head + 1) % BUFFER_SIZE == tail;
    }

    bool isEmpty() const {
        return head == tail;
    }

    bool enqueue(T &&value) {
        if (isFull()) {
            return false;
        }

        buffer[head] = std::move(value);
        head = (head + 1) % BUFFER_SIZE;
        return true;
    }

    T dequeue() {
        T value = std::move(buffer[tail]);
        tail = (tail + 1) % BUFFER_SIZE;
        return std::move(value);
    }
};