#pragma once

#include <string>
#include <unordered_map>

enum class Conversion { SYMBOL, LINE, HELIX };

const auto nucleotide_to_symbol = [] {
  std::unordered_map<std::string_view, std::string_view> nucleotide_to_symbol;
  nucleotide_to_symbol["ATAT"] = ">";
  nucleotide_to_symbol["ATGC"] = "<";
  nucleotide_to_symbol["ATTA"] = "+";
  nucleotide_to_symbol["ATCG"] = "-";
  nucleotide_to_symbol["GCAT"] = ".";
  nucleotide_to_symbol["GCGC"] = ",";
  nucleotide_to_symbol["GCTA"] = "[";
  nucleotide_to_symbol["GCCG"] = "]";
  nucleotide_to_symbol["TAAT"] = "=";
  nucleotide_to_symbol["TAGC"] = "+=";
  nucleotide_to_symbol["TATA"] = "-=";
  nucleotide_to_symbol["TACG"] = "*=";
  nucleotide_to_symbol["CGAT"] = "/=";
  nucleotide_to_symbol["CGGC"] = "~";
  nucleotide_to_symbol["CGTA"] = "?";
  nucleotide_to_symbol["CGCG"] = "X";
  return nucleotide_to_symbol;
}();

const auto symbol_to_nucleotide = [] {
  std::unordered_map<std::string_view, std::string_view> symbol_to_nucleotide;
  symbol_to_nucleotide[">"] = "ATAT";
  symbol_to_nucleotide["<"] = "ATGC";
  symbol_to_nucleotide["+"] = "ATTA";
  symbol_to_nucleotide["-"] = "ATCG";
  symbol_to_nucleotide["."] = "GCAT";
  symbol_to_nucleotide[","] = "GCGC";
  symbol_to_nucleotide["["] = "GCTA";
  symbol_to_nucleotide["]"] = "GCCG";
  symbol_to_nucleotide["="] = "TAAT";
  symbol_to_nucleotide["+="] = "TAGC";
  symbol_to_nucleotide["-="] = "TATA";
  symbol_to_nucleotide["*="] = "TACG";
  symbol_to_nucleotide["/="] = "CGAT";
  symbol_to_nucleotide["~"] = "CGGC";
  symbol_to_nucleotide["?"] = "CGTA";
  symbol_to_nucleotide["X"] = "CGCG";
  return symbol_to_nucleotide;
}();

std::string convert_line_to_helix(std::string const &program) {
  std::string res;
  for (std::size_t i = 0; i < program.size(); i += 2) {
    switch ((i / 2) % 8) {
    case (static_cast<unsigned long>(-1)):
    case 0:
    case 7:
      res += "  ";
      break;
    case 1:
    case 6:
      res += " ";
      break;
      break;
    }
    res += program[i];
    switch ((i / 2) % 8) {
    case 1:
    case 6:
      res += "--";
      break;
    case 2:
    case 5:
      res += "----";
      break;
    case 3:
    case 4:
      res += "-----";
      break;
    }
    res += program[i + 1];
    res += '\n';
  }

  return res;
}

std::string convert_symbol_to_helix(std::string const &program) {
  std::string temp;
  temp.reserve(program.size() * 4);
  for (std::size_t i = 0; i < program.size(); i++) {
    const auto el = std::string() + program[i] + program[i + 1];
    if (symbol_to_nucleotide.contains(el)) {
      temp += symbol_to_nucleotide.at(el);
      i++;
    } else if (symbol_to_nucleotide.contains(std::string() + program[i])) {
      temp += symbol_to_nucleotide.at(std::string() + program[i]);
    }
  }
  return convert_line_to_helix(temp);
}

std::string convert_line_to_symbol(std::string const &program) {
  std::string res;
  res.reserve(program.size() / 4);
  for (std::size_t i = 0; i < program.size(); i += 4) {
    const auto el = std::string() + program[i] + program[i + 1] +
                    program[i + 2] + program[i + 3];
    if (nucleotide_to_symbol.contains(el)) {
      res += nucleotide_to_symbol.at(el);
    }
  }
  return res;
}

std::string convert_symbol_to_line(std::string const &program) {
  std::string res;
  res.reserve(program.size() * 4);
  for (std::size_t i = 0; i < program.size(); i++) {
    const auto el = std::string() + program[i] + program[i + 1];
    if (symbol_to_nucleotide.contains(el)) {
      res += symbol_to_nucleotide.at(el);
      i++;
    } else if (symbol_to_nucleotide.contains(std::string() + program[i])) {
      res += symbol_to_nucleotide.at(std::string() + program[i]);
    }
  }
  return res;
}