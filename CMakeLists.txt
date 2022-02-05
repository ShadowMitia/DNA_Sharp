cmake_minimum_required(VERSION 3.4)

project(dna_sharp
    VERSION 0.0.1
    LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)


include(${PROJECT_SOURCE_DIR}/cmake/yacht.cmake) # will containe most default flags
add_compile_commands_json()

conan_get_package(PACKAGE fmt/8.0.1 catch2/2.13.7)

find_package(Catch2 REQUIRED)

add_executable(dna_sharp src/main.cpp)
target_link_libraries(dna_sharp PUBLIC ${CONAN_LIBS})

catch2_add_test(NAME interpreter_tests SOURCES tests/interpreter_tests.cpp)
target_link_libraries(interpreter_tests PRIVATE ${CONAN_LIBS} Catch2::Catch2)