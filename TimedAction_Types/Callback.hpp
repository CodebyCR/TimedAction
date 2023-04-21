//
// Created by Christoph Rohde on 22.04.23.
//

#pragma once

#include <iostream>
#include <functional>

/// @brief this class should be used to separated the callback functions from the TimedAction class.
///
template<typename A, typename I, typename E>
class Callback { // ? interface implementation um Typenangabe zu vermeiden?

public:
    std::function<void(A &value)> onAction;
    std::function<void(I &value)> onInterval;
    std::function<void(E &value)> onEnd;

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

