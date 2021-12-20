cmake_minimum_required(VERSION 3.4)

project(dna_sharp
    VERSION 0.0.1
    LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 20)


include(${PROJECT_SOURCE_DIR}/cmake/flags.cmake) # will containe most default flags
include(${PROJECT_SOURCE_DIR}/cmake/asan.cmake) 
include(${PROJECT_SOURCE_DIR}/cmake/cppcheck.cmake)

if(NOT EXISTS "${CMAKE_BINARY_DIR}/conan.cmake")
        message(STATUS "Downloading conan.cmake from https://github.com/conan-io/cmake-conan")
        file(DOWNLOAD "https://raw.githubusercontent.com/conan-io/cmake-conan/master/conan.cmake"
                      "${CMAKE_BINARY_DIR}/conan.cmake")
endif()
include(${CMAKE_BINARY_DIR}/conan.cmake)
conan_cmake_run(CONANFILE 
REQUIRES
fmt/8.0.1
                BASIC_SETUP
                BUILD missing
                CONFIGURATION_TYPES "Release")
set(CMAKE_MODULE_PATH "${CMAKE_BINARY_DIR}" ${CMAKE_MODULE_PATH})

# customizable flags for each build and compiler.
#set_flag(COMPILER clang gnu intel TARGET release relwithdebinfo FLAGS -O3 -march=native)
#set_flag(COMPILER clang gnu intel FLAGS -fexceptions -fopenmp)
#set_flag(COMPILER msvc FLAGS /openmp)
#set_flag(COMPILER clang FLAGS -stdlib=libc++)


include_directories("${PROJECT_SOURCE_DIR}/src")
#add_subdirectory("${PROJECT_SOURCE_DIR}/src")

add_executable(dna_sharp src/main.cpp)

target_link_libraries(dna_sharp PUBLIC ${CONAN_LIBS})
#target_include_directories(main PUBLIC "${PROJECT_SOURCE_DIR}/external/utils")
#target_include_directories(main SYSTEM PUBLIC "${PROJECT_SOURCE_DIR}/external/[NAME]"
#                                       PUBLIC "${PROJECT_SOURCE_DIR}/external/[NAME]")
