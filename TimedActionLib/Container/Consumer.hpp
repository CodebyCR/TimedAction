#include <iostream>
#include <functional>
#include <utility>

template <typename T>
class Consumer {
public:
    explicit Consumer(std::function<void(T)> func) : function(std::move(func)) {}
    ~Consumer() = default;

    void accept(T value) {
        function(value);
    }

private:
    std::function<void(T)> function;
};
