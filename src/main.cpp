#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fmt/core.h>

#include <filesystem>
#include <fstream>
#include <optional>
#include <string>
#include <unordered_map>
#include <vector>

#include <iostream>
#include <string_view>

#include <algorithm>

#include "conversions.hpp"
#include "interpreter.hpp"

using namespace std::literals::string_view_literals;

std::string read_file(std::filesystem::path const& path) {
    std::ifstream file(path, std::fstream::ate);
    auto size = file.tellg();
    file.seekg(0);
    std::string output;
    output.resize(static_cast<std::size_t>(size));
    if (!file.read(output.data(), size)) {
        return {};
    }

    return output;
}

std::string read_stdin() {
    std::string stdin_line;
    std::string result;
    for (std::string line; std::getline(std::cin, stdin_line);) {
        result += stdin_line;
        result += '\n';
    }
    return result;
}

void print_usage(std::string_view app_name) {
    fmt::print("DNA# interpreter V0.1\n\n");
    fmt::print("Usage: {} [OPTIONS] file\nfile can be '-' to read from stdin. \n\n", app_name);
    fmt::print("OPTIONS:\n");
    fmt::print("--help       Shows this help\n");
    fmt::print("--to_symbol  Prints the source file converted to symbol form\n");
    fmt::print("--to_helix   Prints the source file converted to helix form\n");
    fmt::print("--to_line    Prints the source file converted to line form\n");
}

void conversion() {}

struct ProgramArguments {
    bool is_conversion = false;
    Conversion convert_to = Conversion::HELIX;
    bool invalid = false;
};

ProgramArguments parse_arguments(std::size_t argc, std::vector<const char*> const& argv) {
    auto result = ProgramArguments{};

    for (std::size_t i = 1; i < argc; i++) {
        const auto arg = std::string_view(argv[i]);
        if (arg == "--help") {
            result.invalid = true;
        } else if (arg == "--to_symbol") {
            result.convert_to = Conversion::SYMBOL;
            result.is_conversion = true;
        } else if (arg == "--to_helix") {
            result.convert_to = Conversion::HELIX;
            result.is_conversion = true;
        } else if (arg == "--to_line") {
            result.convert_to = Conversion::LINE;
            result.is_conversion = true;
        } else if (arg.starts_with("--")) {
            fmt::print("Unknown argument: {}\n", arg);
            result.invalid = true;
        }
    }
    return result;
}

int main(int argc, char* argv[]) {

    std::vector<const char*> args(argv, argv + argc);

    auto [is_conversion, convert_to, invalid] = parse_arguments(argc, args);

    const std::string_view program_name{args[0]};
    if (argc < 2 || invalid) {
        print_usage(program_name);
        return EXIT_FAILURE;
    }

    std::filesystem::path input_file(std::string_view{args[argc - 1]});

    if (((input_file != "-") and ((not std::filesystem::exists(input_file)) or not std::filesystem::is_regular_file(input_file)))) {
        fmt::print("File invalid or not found: {}\n", input_file.string());
        return EXIT_FAILURE;
    }

    std::string program = [&]() {
        std::string result;
        if (input_file != "-") {
            result = read_file(input_file);
        } else {
            result = read_stdin();
        }
        return result;
    }();

    if (program.empty()) {
        return EXIT_SUCCESS;
    }

    program.erase(std::remove_if(std::begin(program), std::end(program), ::isspace), std::end(program));

    bool is_helix = false;
    bool is_alpha = false;
    int alpha_counter = 0;

    for (std::size_t i = 0; i < program.size(); i++) {
        if ((program[i] == 'A' or program[i] == 'C' or program[i] == 'T' or program[i] == 'G')) {
            alpha_counter++;
        } else {
            if (program[i] == '-' and alpha_counter >= 3) {
                is_helix = true;
                break;
            }
        }

        if (alpha_counter >= 5) {
            is_alpha = true;
            break;
        }

        if (program[i] == '/' and program[i + 1] == '/') {
            while (program[i] != '\n') {
                i++;
            }
            continue;
        }
        if (program[i] == '/' and program[i + 1] == '*') {
            // TODO: Maybe deal with multiline comments?
            while (program[i] != '*' and program[i + 1] != '/') {
                i++;
            }
            continue;
        }
    }

    if (is_helix) {
        program.erase(std::remove(std::begin(program), std::end(program), '-'), std::end(program));
    }

    if (is_conversion) {

        if ((is_alpha and not is_helix and convert_to == Conversion::LINE) or ((not is_helix and not is_alpha) and convert_to == Conversion::SYMBOL)) {
            fmt::print("{}\n", read_file(input_file));

            return EXIT_SUCCESS;
        }

        switch (convert_to) {
            case Conversion::HELIX: {
                if (not is_alpha) {
                    const std::string res = convert_symbol_to_helix(program);
                    fmt::print("{}\n", res);
                } else if (not is_helix) {
                    const std::string res = convert_line_to_helix(program);
                    fmt::print("{}\n", res);
                } else {
                    fmt::print("{}\n", read_file(input_file));
                }

            } break;
            case Conversion::LINE: {
                if (is_helix) {
                    fmt::print("{}\n", program);
                    return EXIT_SUCCESS;
                }
                const std::string res = convert_symbol_to_line(program);
                fmt::print("{}\n", res);
            }

            break;
            case Conversion::SYMBOL:
                const std::string res = convert_line_to_symbol(program);
                fmt::print("{}\n", res);
                break;
        }

        return EXIT_SUCCESS;
    }

    if (is_alpha or is_helix) {
        std::string temp;
        for (std::size_t i = 0; i < program.size(); i += 4) {
            const auto el = std::string() + program[i] + program[i + 1] + program[i + 2] + program[i + 3];
            if (nucleotide_to_symbol.contains(el)) {
                temp += nucleotide_to_symbol.at(el);
            } else {
                temp += el;
            }
        }
        program = temp;
    }

    // fmt::print("{}\n", program);

    // DNASharp interpreter;
    // interpreter.run(program);

    const auto dna_program = dna_sharp::parse(program);
    dna_sharp::run(dna_program);

    fmt::print("\n");
    return EXIT_SUCCESS;
}