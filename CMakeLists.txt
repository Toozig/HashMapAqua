cmake_minimum_required(VERSION 3.12)
project(HashMap)

set(CMAKE_CXX_STANDARD 14)

add_executable(HashMap main.cpp HashMap.hpp HashMapTester1.3.cpp "SpamDetector.cpp")