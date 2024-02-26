#pragma once

template<typename T>
class Optional {
private:
    T value;
    bool isPresent;

public:
    Optional(): isPresent(false) {}

    explicit Optional(T value): value(value), isPresent(true) {}

    static Optional<T> empty() {
        return Optional<T>();
    }
    static Optional<T> of(T t) {
        return Optional<T>(t);
    }

    bool hasValue() const {
        return isPresent;
    }

    T getValue() const {
        return value;
    }

    void set(T val) {
        isPresent = true;
        value = val;
    }
};
