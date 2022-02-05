#pragma once

#include <string>
#include <vector>

#include "fmt/core.h"
#include <iostream>

#include <optional>

namespace dna_sharp {
enum class Command {
    // Brainfuck
    pointer_increment,
    pointer_decrement,
    plus,
    minus,
    ascii_output,
    ascii_input,
    block_begin,
    block_end,
    // DNA Sharp
    pointer_assignement,
    assignement,
    plus_equal,
    minus_equal,
    times_equal,
    division_equal,
    integer_output,
    integer_input,
    nop
};

struct Program {
    std::string raw_program;
    std::vector<Command> commands;
};

Program parse(std::string const& input_program) {
    Program program;
    program.raw_program = input_program;
    program.commands.reserve(input_program.size());
    for (std::size_t i = 0; i < input_program.size(); i++) {
        const auto op = [&i, &input_program]() -> std::optional<Command> {
            // fmt::print("c : {}\n", input_program[i]);
            switch (input_program[i]) {
                case '>':
                    return std::optional{Command::pointer_increment};
                    break;
                case '<':
                    return std::optional{Command::pointer_decrement};
                    break;
                case '+':
                    if (input_program[i + 1] == '=') {
                        i++;
                        return std::optional{Command::plus_equal};
                    }
                    return std::optional{Command::plus};
                    break;
                case '-':
                    if (input_program[i + 1] == '=') {
                        i++;
                        return std::optional{Command::minus_equal};
                    }
                    return std::optional{Command::minus};
                    break;
                case '.':
                    return std::optional{Command::ascii_output};
                    break;
                case ',':
                    return std::optional{Command::ascii_input};
                    break;
                case '[':
                    return std::optional{Command::block_begin};
                    break;
                case ']':
                    return std::optional{Command::block_end};
                    break;
                case '*':
                    if (input_program[i + 1] == '=') {
                        i++;
                        return std::optional{Command::times_equal};
                    }
                    break;
                case '/':
                    if (input_program[i + 1] == '=') {
                        i++;
                        return std::optional{Command::division_equal};
                    }
                    break;
                case 'X':
                    return std::optional{Command::nop};
                    break;
                case ':':
                    if (input_program[i + 1] == '=') {
                        i++;
                        return std::optional{Command::assignement};
                    }
                    break;
                case '=':
                    return std::optional{Command::assignement};
                    break;
                case '~':
                    return Command::integer_input;
                    break;
                case '?':
                    return Command::integer_output;
                    break;
                default:
                    break;
            }

            return std::nullopt;
        }();
        if (op) {
            program.commands.push_back(*op);
        }
    }

    return program;
}

auto run(Program const& program) {
    std::size_t program_counter = 0;
    std::size_t pointer = 0;
    std::size_t new_pointer = 0;

    std::vector<int> memory;
    memory.resize(30'000, 0x0);

    const auto compute_new_pointer = [&]() {
        bool loop = true;
        new_pointer = pointer;
        while (loop) {
            program_counter++;
            if (program_counter >= program.commands.size()) {
                loop = false;
                continue;
            }
            switch (program.commands[program_counter]) {
                case Command::pointer_decrement:
                    new_pointer--;
                    break;
                case Command::pointer_increment:
                    new_pointer++;
                    break;
                default:
                    program_counter--;
                    loop = false;
                    break;
            }
        }
    };

    while (program_counter < program.commands.size()) {
        // fmt::print("pc {}\npointer {}\nnew_pointer {}\n", program_counter, pointer, new_pointer);
        fmt::print("Memory: ");
        for (std::size_t i = 0; i < 20; i++) {
            fmt::print("{} ", memory[i]);
        }
        fmt::print("\n");

        switch (program.commands[program_counter]) {
            case Command::plus: {
                // fmt::print("+\n");
                memory[pointer]++;
            } break;
            case Command::plus_equal: {
                // fmt::print("+=\n");
                compute_new_pointer();
                memory[pointer] += memory[new_pointer];
            } break;
            case Command::minus:
                // fmt::print("-\n");
                memory[pointer]--;
                break;
            case Command::minus_equal: {
                // fmt::print("-=\n");
                compute_new_pointer();
                memory[pointer] -= memory[new_pointer];
            } break;
            case Command::times_equal: {
                // fmt::print("*=\n");
                compute_new_pointer();
                memory[pointer] *= memory[new_pointer];
            } break;
            case Command::division_equal: {
                // fmt::print("/=\n");
                compute_new_pointer();
                memory[pointer] /= memory[new_pointer];
            } break;
            case Command::pointer_decrement:
                // fmt::print("pointer--\n");
                pointer--;
                if (pointer >= memory.size()) {
                    pointer = memory.size() - 1;
                }
                break;
            case Command::pointer_increment:
                // fmt::print("pointer++\n");
                pointer++;
                if (pointer >= memory.size()) {
                    pointer = memory.size() - 1;
                }
                break;
            case Command::integer_output:
                // fmt::print("print integer\n");
                fmt::print("{0:d}", memory[pointer]);
                break;
            case Command::ascii_output:
                // fmt::print("print ascii\n");
                // fmt::print("ctoto : {}\n", static_cast<unsigned char>(memory[pointer]));
                // fmt::print("ctoto : {:c}\n", static_cast<unsigned char>(memory[pointer]));
                fmt::print("{:c}", static_cast<unsigned char>(memory[pointer]));
                break;
            case Command::ascii_input: {
                // fmt::print("ascii input\n");
                std::string input;
                std::getline(std::cin, input);
                memory[pointer] = static_cast<unsigned char>(input[0]);
            } break;
            case Command::integer_input: {
                // fmt::print("integer input\n");
                std::string input;
                std::getline(std::cin, input);
                memory[pointer] = std::strtol(input.c_str(), nullptr, 10);
            } break;
            case Command::block_begin:
                // fmt::print("[\n");
                if (memory[pointer] == 0) {
                    int bracket_counter = 0;
                    // ////fmt::print("Finding last ]\n");
                    while (program.commands[program_counter] != Command::block_end or bracket_counter > 0) {
                        //   ////fmt::print("Finding PC {}\n", program_counter);
                        if (program.commands[program_counter] == Command::block_begin) {
                            bracket_counter++;
                        }
                        program_counter++;
                        if (program.commands[program_counter] == Command::block_end) {
                            bracket_counter--;
                        }
                    }
                }
                break;
            case Command::block_end:
                // fmt::print("]\n");
                if (memory[pointer] != 0) {
                    int bracket_counter = 0;
                    // ////fmt::print("Finding first [\n");
                    while (program.commands[program_counter] != Command::block_begin or bracket_counter > 0) {
                        //   ////fmt::print("Finding PC {}\n", program_counter);
                        if (program.commands[program_counter] == Command::block_end) {
                            bracket_counter++;
                        }
                        program_counter--;
                        if (program.commands[program_counter] == Command::block_begin) {
                            bracket_counter--;
                        }
                    }
                }
                break;

            case Command::assignement: {
                // fmt::print("=\n");
                compute_new_pointer();
                memory[pointer] = memory[new_pointer];
            } break;

            case Command::nop:
                // fmt::print("NOP\n");
                if (program.commands[program_counter + 1] == Command::nop && program.commands[program_counter + 2] == Command::nop) {
                    program_counter += 2;
                    // fmt::print("{}", program.raw_program);
                }
                break;

            default:
                // Ignore
                break;
        }
        program_counter++;
    }

    fmt::print("Memory: ");
    for (std::size_t i = 0; i < 20; i++) {
        fmt::print("{} ", memory[i]);
    }
    fmt::print("\n");

    return memory;
}
} // namespace dna_sharp
