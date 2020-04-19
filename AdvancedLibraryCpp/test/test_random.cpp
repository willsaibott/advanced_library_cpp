#include "test_random.h"
#include <limits>
#include "../tools/random.h"

using namespace advanced;

TestRandom::
TestRandom(QObject *parent) : QObject(parent) { }

void TestRandom::
test_int8_random() {

  for (uint8_t ii = 0; ii < std::numeric_limits<int8_t>::max(); ii++) {
    int8_t number = static_cast<int8_t>(
        next_random<int16_t>(0ul, 2 * ii) -
        static_cast<int16_t>(std::numeric_limits<int8_t>::min())
    );

    QVERIFY(number >= std::numeric_limits<int8_t>::min() && number <= ii);
  }
}

void TestRandom::
test_int16_random() {
  const uint16_t limit = 10000;
  for (int16_t ii = 0; ii < limit; ii++) {
    int16_t number = static_cast<int16_t>(
        next_random<int32_t>(0ul, 2ul * static_cast<uint16_t>(ii)) - ii
    );

    QVERIFY(number >= -ii && number <= ii);
  }
}

void TestRandom::
test_int32_random() {
  const int32_t limit = 10000;
  for (int32_t ii = 0; ii < limit; ii++) {
    int32_t number = static_cast<int32_t>(
        next_random<int64_t>(0ul, 2ul * static_cast<uint32_t>(ii)) - ii
    );

    QVERIFY(number >= -ii && number <= ii);
  }
}

void TestRandom::
test_int64_random() {
  const int64_t limit = 10000;
  for (int64_t ii = 0; ii < limit; ii++) {
    int64_t number = static_cast<int64_t>(
        next_random<int64_t>(0ul, 2ul * static_cast<uint64_t>(ii)) - ii
    );

    QVERIFY(number >= -ii && number <= ii);
  }
}

void TestRandom::
test_double_random() {
  const int64_t limit = 10000;
  for (int64_t ii = 0; ii < limit; ii++) {
    double number = next_random_double(ii - 2.0, ii + 2.0);
    QVERIFY(number >= ii - 2.0 && number <= ii + 2.0);
  }
}

void TestRandom::
test_char_random() {
  const auto limit = 10000;
  for (auto ii = 0; ii < limit; ii++) {
    char c = next_random_char("ae;0\n");
    QVERIFY(c == 'a' || c == 'e' || c == ';' || c == '0' || c == '\n');
  }

  char empty = next_random_char("");
  QVERIFY(empty == '\0');

  char unique = next_random_char("a");
  QVERIFY(unique == 'a');
}

void TestRandom::
test_string_random() {
  const auto limit = 10000;
  for (auto ii = 0; ii < limit; ii++) {
    std::string random { next_random_string(10, "ae;0\n") };
    for (const auto& c : random) {
      QVERIFY(c == 'a' || c == 'e' || c == ';' || c == '0' || c == '\n');
    }
  }

  std::string empty{ next_random_string(20, "") };
  QVERIFY(empty == "");

  std::string unique = next_random_string(20, "a");
  QVERIFY(std::all_of(unique.begin(),
                      unique.end(),
                      [](char c) { return c=='a'; }));
}
