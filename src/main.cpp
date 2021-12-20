#include <cctype>
#include <cstddef>
#include <cstdlib>
#include <exception>
#include <fmt/core.h>

#include <filesystem>
#include <fstream>
#include <string>
#include <unordered_map>
#include <vector>

#include <iostream>
#include <string_view>

#include <algorithm>

#include "conversions.hpp"
#include "interpreter.hpp"

using namespace std::literals::string_view_literals;

std::string read_file(std::filesystem::path const &path) {
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
  fmt::print("A DNA# language interpreter, with \n\n");
  fmt::print(
      "Usage: {} [OPTIONS] file\nfile can be '-' to read from stdin. \n\n",
      app_name);
  fmt::print("OPTIONS:\n");
  fmt::print("--help       Shows this help\n");
  fmt::print("--to_symbol  Prints the source file converted to symbol form\n");
  fmt::print("--to_helix   Prints the source file converted to helix form\n");
  fmt::print("--to_line    Prints the source file converted to line form\n");
}

int main(int argc, char *argv[]) {

  if (argc < 2) {
    print_usage(argv[0]);
    return EXIT_FAILURE;
  }

  bool is_conversion = false;
  Conversion convert_to = Conversion::HELIX;

  for (std::size_t i = 1; i < static_cast<std::size_t>(argc); i++) {
    const auto arg = std::string_view(argv[i]);
    if (arg == "--help") {
      print_usage(argv[0]);
      return EXIT_SUCCESS;
    }
    if (arg == "--to_symbol") {
      is_conversion = true;
      convert_to = Conversion::SYMBOL;
    } else if (arg == "--to_helix") {
      is_conversion = true;
      convert_to = Conversion::HELIX;
    } else if (arg == "--to_line") {
      is_conversion = true;
      convert_to = Conversion::LINE;
    } else if (arg.starts_with("--")) {
      fmt::print("Unknown argument: {}\n", arg);
      return EXIT_FAILURE;
    }
  }
  std::filesystem::path input_file(std::string_view{argv[argc - 1]});

  if (((input_file != "-") and
       ((not std::filesystem::exists(input_file)) or
        not std::filesystem::is_regular_file(input_file)))) {
    fmt::print("File not found: {}\n", input_file.string());
    return EXIT_FAILURE;
  }

  std::string program = [&]() {
    if (input_file != "-") {
      return read_file(input_file);
    } else {
      return read_stdin();
    }
  }();

  if (program.empty()) {
    return EXIT_SUCCESS;
  }

  program.erase(
      std::remove_if(std::begin(program), std::end(program), ::isspace),
      std::end(program));

  bool is_helix = false;
  bool is_alpha = false;
  int alpha_counter = 0;

  for (std::size_t i = 0; i < program.size(); i++) {
    if ((program[i] == 'A' or program[i] == 'C' or program[i] == 'T' or
         program[i] == 'G')) {
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
    program.erase(std::remove(std::begin(program), std::end(program), '-'),
                  std::end(program));
  }

  if (is_conversion) {

    if ((is_alpha and not is_helix and convert_to == Conversion::LINE) or
        ((not is_helix and not is_alpha) and
         convert_to == Conversion::SYMBOL)) {
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
      const auto el = std::string() + program[i] + program[i + 1] +
                      program[i + 2] + program[i + 3];
      if (nucleotide_to_symbol.contains(el)) {
        temp += nucleotide_to_symbol.at(el);
      } else {
        temp += el;
      }
    }
    program = temp;
  }

  // fmt::print("{}\n", program);

  DNASharp interpreter;
  interpreter.run(program);

  fmt::print("\n");
  return EXIT_SUCCESS;
}