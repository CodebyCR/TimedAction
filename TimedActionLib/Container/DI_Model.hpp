#pragma once

#include <iostream>
#include <memory>

template <typename T>
class DI_Model {
public:

    [[NoExplicit]]
    DI_Model() : ptr_(std::make_unique<T>()) {}

    [[NoExplicit]]
    DI_Model(const DI_Model& value) : ptr_(std::make_unique<T>(*value.ptr_)) {}

    [[NoExplicit]]
    DI_Model(const T& value) : ptr_(std::make_unique<T>(value)) {}

    operator T&() {
        return *ptr_;
    }

    DI_Model& operator=(const T& value) {
        *ptr_ = value;
        return *this;
    }

    DI_Model& operator=(const DI_Model& value) {
        *ptr_ = value;
        return *this;
    }

    T* operator->() {
        return ptr_.get();
    }

private:
    std::unique_ptr<T> ptr_;
};
