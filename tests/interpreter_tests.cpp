#include "../src/interpreter.hpp"

#define CATCH_CONFIG_MAIN
#include "catch2/catch.hpp"

TEST_CASE("Interpeter : '+' command", "[interpreter]") {

    const auto dna_program = dna_sharp::parse("+");
    const auto test1 = dna_sharp::run(dna_program);

    REQUIRE(test1[0] == 1);

    const auto dna_program2 = dna_sharp::parse("++++");
    const auto test2 = dna_sharp::run(dna_program2);

    REQUIRE(test2[0] == 4);
}

TEST_CASE("Interpeter : '-' command", "[interpreter]") {
    const auto dna_program = dna_sharp::parse("-");
    const auto test1 = dna_sharp::run(dna_program);

    REQUIRE(test1[0] == -1);

    const auto dna_program2 = dna_sharp::parse("----");
    const auto test2 = dna_sharp::run(dna_program2);

    REQUIRE(test2[0] == -4);
}

TEST_CASE("Interpeter : '<' command", "[interpreter]") {
    const auto dna_program = dna_sharp::parse(">+<");
    const auto test1 = dna_sharp::run(dna_program);

    REQUIRE(test1[0] == 0);
    REQUIRE(test1[1] == 1);

    const auto dna_program2 = dna_sharp::parse(">>>>+<<<<");
    const auto test2 = dna_sharp::run(dna_program2);

    REQUIRE(test2[0] == 0);
    REQUIRE(test2[4] == 1);
}

TEST_CASE("Interpeter : '>' command", "[interpreter]") {
    const auto dna_program = dna_sharp::parse(">+");
    const auto test1 = dna_sharp::run(dna_program);

    REQUIRE(test1[0] == 0);
    REQUIRE(test1[1] == 1);

    const auto dna_program2 = dna_sharp::parse(">>>>+");
    const auto test2 = dna_sharp::run(dna_program2);

    REQUIRE(test2[0] == 0);
    REQUIRE(test2[4] == 1);
}

TEST_CASE("Interpeter : '[]' commands", "[interpreter]") {

    const auto dna_program = dna_sharp::parse("++++[----]");
    const auto test1 = dna_sharp::run(dna_program);

    REQUIRE(test1[0] == 0);

    const auto dna_program2 = dna_sharp::parse("++++[-]");
    const auto test2 = dna_sharp::run(dna_program2);

    REQUIRE(test2[0] == 0);
}

TEST_CASE("Interpeter : '=' command", "[interpreter]") {

    const auto dna_program = dna_sharp::parse("++++>>>>=");
    const auto test1 = dna_sharp::run(dna_program);

    REQUIRE(test1[0] == test1[4]);

    const auto dna_program2 = dna_sharp::parse("++++>>>>++>>>>+++>>>>=<<<<<<<<");
    const auto test2 = dna_sharp::run(dna_program2);

    REQUIRE(test2[12] == test2[4]);
}
