#pragma once

#include <string>
#include <vector>

#include "fmt/core.h"
#include <iostream>

struct DNASharp {

    std::vector<long> memory;
    std::size_t pointer = 0;
    std::size_t new_pointer = 0;
    std::size_t program_counter = 0;

    DNASharp() { memory.resize(30'000, 0); }

    bool run(std::string const& program) {
        memory.clear();
        memory.resize(30'000, 0);
        pointer = 0;
        new_pointer = 0;
        program_counter = 0;

        //   fmt::print("Program size {}\n", program.size());
        while (program_counter < program.size()) {
            // fmt::print("PC {} Pointer {}\n", program_counter, pointer);
            // fmt::print("new_pointer {} pointer {} ||", new_pointer, pointer);

            // for (std::size_t i = 0; i < 30; i++) {
            //   fmt::print("{} ", memory[i]);
            // }
            // fmt::print("\n");

            // std::string toto;
            // std::getline(std::cin, toto);

            switch (program[program_counter]) {
                case '+': {
                    if (program[program_counter + 1] == '=') {
                        bool loop = true;
                        new_pointer = pointer;
                        program_counter++;
                        while (loop) {
                            program_counter++;
                            switch (program[program_counter]) {
                                case '<':
                                    new_pointer--;
                                    break;
                                case '>':
                                    new_pointer++;
                                    break;
                                default:
                                    program_counter--;
                                    loop = false;
                                    break;
                            }
                        }
                        memory[pointer] += memory[new_pointer];
                    } else {
                        memory[pointer]++;
                    }
                } break;
                case '-':
                    if (program[program_counter + 1] == '=') {
                        bool loop = true;
                        new_pointer = pointer;
                        program_counter++;
                        while (loop) {
                            program_counter++;
                            switch (program[program_counter]) {
                                case '<':
                                    new_pointer--;
                                    break;
                                case '>':
                                    new_pointer++;
                                    break;
                                default:
                                    program_counter--;
                                    loop = false;
                                    break;
                            }
                        }
                        memory[pointer] -= memory[new_pointer];
                    } else {
                        memory[pointer]--;
                    }
                    break;
                case '*':
                    if (program[program_counter + 1] == '=') {
                        bool loop = true;
                        new_pointer = pointer;
                        program_counter++;
                        while (loop) {
                            program_counter++;
                            switch (program[program_counter]) {
                                case '<':
                                    new_pointer--;
                                    break;
                                case '>':
                                    new_pointer++;
                                    break;
                                default:
                                    program_counter--;
                                    loop = false;
                                    break;
                            }
                        }
                        memory[pointer] *= memory[new_pointer];
                    } else {
                        return EXIT_FAILURE;
                    }
                    break;
                case '/':
                    if (program[program_counter + 1] == '=') {
                        bool loop = true;
                        new_pointer = pointer;
                        program_counter++;
                        while (loop) {
                            program_counter++;
                            switch (program[program_counter]) {
                                case '<':
                                    new_pointer--;
                                    break;
                                case '>':
                                    new_pointer++;
                                    break;
                                default:
                                    program_counter--;
                                    loop = false;
                                    break;
                            }
                        }
                        memory[pointer] /= memory[new_pointer];
                    } else {
                        return EXIT_FAILURE;
                    }
                    break;
                case '<':
                    pointer--;
                    if (pointer >= memory.size()) {
                        pointer = memory.size() - 1;
                    }
                    break;
                case '>':
                    pointer++;
                    if (pointer >= memory.size()) {
                        pointer = memory.size() - 1;
                    }
                    break;
                case '~':
                    fmt::print("{:d}", memory[pointer]);
                    break;
                case '.':
                    fmt::print("{:c}", memory[pointer]);
                    break;
                case ',': {
                    std::string input;
                    std::getline(std::cin, input);
                    memory[pointer] = input[0];
                } break;
                case '?': {
                    std::string input;
                    std::getline(std::cin, input);
                    memory[pointer] = std::atol(input.c_str());
                } break;
                case '[':
                    if (memory[pointer] == 0) {
                        int bracket_counter = 0;
                        // fmt::print("Finding last ]\n");
                        while (program[program_counter] != ']' or bracket_counter > 0) {
                            //   fmt::print("Finding PC {}\n", program_counter);
                            if (program[program_counter] == '[') {
                                bracket_counter++;
                            }
                            program_counter++;
                            if (program[program_counter] == ']') {
                                bracket_counter--;
                            }
                        }
                    }
                    break;
                case ']':
                    if (memory[pointer] != 0) {
                        int bracket_counter = 0;
                        // fmt::print("Finding first [\n");
                        while (program[program_counter] != '[' or bracket_counter > 0) {
                            //   fmt::print("Finding PC {}\n", program_counter);
                            if (program[program_counter] == ']') {
                                bracket_counter++;
                            }
                            program_counter--;
                            if (program[program_counter] == '[') {
                                bracket_counter--;
                            }
                        }
                    }
                    break;

                case ':':
                    if (program[program_counter + 1] != '=') {
                        break;
                    } else {
                        return EXIT_FAILURE;
                    }
                    program_counter++;
                    [[fallthrough]];
                case '=': {
                    bool loop = true;
                    new_pointer = pointer;
                    while (loop) {
                        program_counter++;
                        switch (program[program_counter]) {
                            case '<':
                                new_pointer--;
                                break;
                            case '>':
                                new_pointer++;
                                break;
                            default:
                                program_counter--;
                                loop = false;
                                break;
                        }
                    }
                    memory[pointer] = memory[new_pointer];
                    break;
                }

                case 'X':
                    if (program[program_counter + 1] == 'X' and program[program_counter + 2] == 'X') {
                        program_counter += 2;
                        fmt::print("{}", program);
                    }
                    // NOP
                    break;

                default:
                    // Ignore
                    break;
            }

            program_counter++;
        }

        return true;
    }
};