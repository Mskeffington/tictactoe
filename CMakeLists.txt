cmake_minimum_required(VERSION 2.6)
project(tictactoe)

add_executable(tictactoe main.cpp)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Werror -std=gnu++11")
target_link_libraries(tictactoe PUBLIC ncurses)

install(TARGETS tictactoe RUNTIME DESTINATION bin)
