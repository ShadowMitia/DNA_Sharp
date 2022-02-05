#include "conversions.hpp"
#include "catch.hpp"


constexpr auto line_quine{R"s(CGCGCGCGCGCG)s"};
constexpr auto symbol_quine{R"s(XXX)s"};
constexpr auto helix_quine{R"s(  CG
 C--G
C----G
C-----G
C-----G
C----G
)s"};

constexpr auto helix_helloworld{R"s(  AT
 T--A
A----T
T-----A
T-----A
G----C
 T--A
  GC
  CG
 C--G
A----T
A-----T
T-----A
A----T
 A--T
  GC
  AT
 C--G
T----A
C-----G
T-----A
G----C
 C--G
  CG
  AT
 A--T
T----A
A-----T
A-----T
G----C
 A--T
  GC
  TA
 G--C
T----A
G-----C
C-----G
C----G
 A--T
  GC
  TA
 G--C
A----T
G-----C
A-----T
C----G
 A--T
  CG
  GC
 A--T
A----T
C-----G
A-----T
C----G
 A--T
  CG
  GC
 A--T
T----A
G-----C
A-----T
G----C
 A--T
  CG
  GC
 A--T
G----C
A-----T
A-----T
T----A
 A--T
  TA
  AT
 T--A
G----C
A-----T
A-----T
A----T
 G--C
  AT
  AT
 G--C
T----A
G-----C
A-----T
G----C
 G--C
  AT
  TA
 T--A
A----T
G-----C
G-----C
A----T
 A--T
  TA
  AT
 T--A
A----T
T-----A
G-----C
A----T
 T--A
  TA
  AT
 G--C
A----T
T-----A
A-----T
T----A
 G--C
  AT
  TA
 T--A
A----T
G-----C
G-----C
A----T
 A--T
  AT
  AT
 T--A
G----C
A-----T
)s"};

constexpr auto line_helloworld{
    R"s(ATTAATTATAGCTAGCCGCGATATTAATATGCATCGTACGTAGCCGCGATATTAATATGCATGCTAGCTAGCCGCGATGCTAGCATGCATCGATCGGCATATCGATCGATCGGCATTAGCATGCATCGGCATGCATATTAATTAATTAGCATATATGCATATGCTAGCATGCGCATTATAATGCGCATATTAATTAATTAGCATTATAATGCATTAATTAGCATTATAATGCGCATATATATTAGCAT)s"};

constexpr auto symbol_helloworld{
    R"s(+++=+=X>=<-*=+=X>=<<+=+=X<+=<--.---.+=<-..+++.>.<+=<.-=<.+++.-=<++.-=<.>+.)s"};


TEST_CASE("LINE -> HELIX", "[]") {
  REQUIRE(convert_line_to_helix(line_quine) == helix_quine);
  REQUIRE(convert_line_to_helix(line_helloworld) == helix_helloworld);
}

TEST_CASE("SYMBOL -> HELIX", "[]") {
  REQUIRE(convert_symbol_to_helix(symbol_quine) == helix_quine);
  REQUIRE(convert_symbol_to_helix(symbol_helloworld) == helix_helloworld);
}

TEST_CASE("LINE -> SYMBOL", "[]") {
  REQUIRE(convert_line_to_symbol(line_quine) == symbol_quine);
  REQUIRE(convert_line_to_symbol(line_helloworld) == symbol_helloworld);
}
