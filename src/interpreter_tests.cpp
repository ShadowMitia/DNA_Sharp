#include "catch.hpp"
#include "interpreter.hpp"

TEST_CASE("Interpeter : '+' command", "[interpreter]") {
  DNASharp interpreter;

  interpreter.run("+");
  REQUIRE(interpreter.memory[0] == 1);
  interpreter.run("++++");
  REQUIRE(interpreter.memory[0] == 4);
}

TEST_CASE("Interpeter : '-' command", "[interpreter]") {
  DNASharp interpreter;

  interpreter.run("-");
  REQUIRE(interpreter.memory[0] == -1);
  interpreter.run("----");
  REQUIRE(interpreter.memory[0] == -4);
}

TEST_CASE("Interpeter : '<' command", "[interpreter]") {
  DNASharp interpreter;

  interpreter.run("><");
  REQUIRE(interpreter.pointer == 0);
  interpreter.run(">>>>");
  REQUIRE(interpreter.pointer == 4);
}

TEST_CASE("Interpeter : '>' command", "[interpreter]") {
  DNASharp interpreter;

  interpreter.run(">");
  REQUIRE(interpreter.pointer == 1);
  interpreter.run(">>>>");
  REQUIRE(interpreter.pointer == 4);
}

TEST_CASE("Interpeter : '[]' commands", "[interpreter]") {
  DNASharp interpreter;

  interpreter.run("++++[----]");
  REQUIRE(interpreter.memory[0] == 0);
  interpreter.run("++++[-]");
  REQUIRE(interpreter.memory[0] == 0);
}

TEST_CASE("Interpeter : '=' command", "[interpreter]") {
  DNASharp interpreter;

  interpreter.run("++++>>>>=");
  REQUIRE(interpreter.memory[0] == interpreter.memory[4]);
  interpreter.run("++++>>>>++>>>>+++>>>>=<<<<<<<<");
  REQUIRE(interpreter.memory[0] == interpreter.memory[4]);
}


