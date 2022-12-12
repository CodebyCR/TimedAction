cmake_minimum_required(VERSION 3.24)
project("TimedAction")

set(CMAKE_CXX_STANDARD 20)

add_executable(TimedAction main.cpp TimedAction.hpp Cron/Cron.hpp)
