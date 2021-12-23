cmake_minimum_required(VERSION 3.20)
project(IRC_git)

set(CMAKE_CXX_STANDARD 98)

add_executable(IRC_git main.cpp Server.cpp User.cpp Server.hpp User.hpp Defines.hpp Channel.cpp Channel.hpp)
