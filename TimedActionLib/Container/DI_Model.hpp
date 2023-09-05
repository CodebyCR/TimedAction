#pragma once

#include <iostream>
#include <memory>

[[maybe_unused]]
template <typename T>
class DI_Model {
public:

    [[NoExplicit]]
    DI_Model() : ptr_(std::make_unique<T>()) {}

    [[NoExplicit]]
    DI_Model(const DI_Model& value) : ptr_(std::make_unique<T>(*value.ptr_)) {}


    explicit DI_Model(const T& value) : ptr_(std::make_unique<T>(value)) {}

    explicit DI_Model( const T&& value) : ptr_(std::make_unique<T>(std::move(value))) {}

    ~DI_Model() = default;


    explicit operator T&() {
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

// example usage:
// DI_Model<I_ExecutionTimer> di = Cron("0 */1 * * 7 * 2023");
// di->execute();
// di = Cron("0 */1 * * 7 * 2024");
// di->execute();
