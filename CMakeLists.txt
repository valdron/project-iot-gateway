


cmake_minimum_required(VERSION 3.8)

# shortcut
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)


INCLUDE_DIRECTORIES(${SOURCE_DIR})


#  makes tests
add_subdirectory(test)

#makes main program
add_subdirectory(src)


