cmake_minimum_required(VERSION 3.22)


project(scheme VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_library(scheme_impl 
    tokenizer.cpp
    parser.cpp
    scheme.cpp
    object.cpp
    basics.cpp
    helpers.cpp
    scope.cpp
    advanced.cpp
    heap.cpp
)

target_include_directories(scheme_impl PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(scheme repl/main.cpp repl/help.cpp)
target_link_libraries(scheme scheme_impl)

target_include_directories(scheme PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})