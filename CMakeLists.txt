cmake_minimum_required(VERSION 3.20)
project(IRC_git)

set(CMAKE_CXX_STANDARD 98)

add_executable(IRC_git main.cpp Client.cpp Server.cpp Client.hpp Server.hpp)
