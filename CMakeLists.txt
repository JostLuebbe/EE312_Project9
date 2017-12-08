cmake_minimum_required(VERSION 3.6)
project(Project9)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES main.cpp Input.cpp Parse.h String.h BST.h ExpTree.h ExpTree.cpp Project9.cpp Project9.h)
add_executable(Project9 ${SOURCE_FILES} main.cpp)