//
// Created by Christoph Rohde on 22.04.23.
//

#pragma once

#include <functional>
#include <iostream>

/// @brief this class should be used to separated the callback functions from the TimedAction class.
///
template<typename A, typename I, typename E>
class Callback {    // ? interface implementation um Typenangabe zu vermeiden?

public:
    std::function<void(A& value)> onAction;
    std::function<void(I& value)> onInterval;
    std::function<void(E& value)> onEnd;
};

/// Implementation example:
// auto callback = Callback<int, int, int> {
//     .onAction = [](int &value) {
//         std::cout << "onAction: " << value << std::endl;
//     },
//     .onInterval = [](int &value) {
//         std::cout << "onInterval: " << value << std::endl;
//     },
//     .onEnd = [](int &value) {
//         std::cout << "onEnd: " << value << std::endl;
//     }
// };

//
//#include <iostream>
//
//class BaseBar
//{
//public:
//    virtual ~BaseBar() {}
//};
//
//template<typename A, typename I, typename E>
//class Bar : public BaseBar
//{
//public:
//    Bar(A a, I i, E e) : a(a), i(i), e(e) {}
//    A a;
//    I i;
//    E e;
//};
//
//class Foo
//{
//public:
//    Foo(BaseBar* bar) : bar(bar) {}
//    BaseBar* bar;
//};
//
//int main()
//{
//    Bar<int, double, char> bar(42, 3.14, 'a');
//    Foo foo(&bar);
//    auto* derivedBar = dynamic_cast<Bar<int, double, char>*>(foo.bar);
//    if (derivedBar != nullptr) {
//        std::cout << derivedBar->a << " " << derivedBar->i << " " << derivedBar->e << std::endl;
//    }
//    return 0;
//}
