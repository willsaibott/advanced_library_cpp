#include "test_enum.h"
#include "../tools/enum_factory.h"

using namespace advanced::tools;

TestEnum::
TestEnum(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestEnum");
}

void TestEnum::
test_to_string() {
  QCOMPARE("first",  std::to_string(dummy::first));
  QCOMPARE("second", std::to_string(dummy::second));
  QCOMPARE("third",  std::to_string(dummy::third));
}

void TestEnum::
test_from_string() {
  const dummy fst   { from_string<dummy>("first") };
  const dummy snd   { from_string<dummy>("second") };
  const dummy trd   { from_string<dummy>("third") };
  const dummy other { from_string<dummy>("other") };
  QCOMPARE(fst,   dummy::first);
  QCOMPARE(snd,   dummy::second);
  QCOMPARE(trd,   dummy::third);
  QCOMPARE(other, dummy::MAX);
}

void TestEnum::
test_from_string_custom_max_limit() {
  const dummy fst   { from_string<dummy, dummy::first, dummy::third>("first") };
  const dummy snd   { from_string<dummy, dummy::first, dummy::third>("second") };
  const dummy trd   { from_string<dummy, dummy::first, dummy::third>("third") };
  const dummy other { from_string<dummy, dummy::first, dummy::third>("other") };
  QCOMPARE(fst,   dummy::first);
  QCOMPARE(snd,   dummy::second);
  QCOMPARE(trd,   dummy::third); // third is not compared!!!
  QCOMPARE(other, dummy::third);
}

void TestEnum::
test_from_string_custom_min_limit() {
  const dummy fst   { from_string<dummy, dummy::second>("first") };
  const dummy snd   { from_string<dummy, dummy::second>("second") };
  const dummy trd   { from_string<dummy, dummy::second>("third") };
  const dummy other { from_string<dummy, dummy::second>("other") };
  QCOMPARE(fst,   dummy::MAX);    // first is not compared!!!
  QCOMPARE(snd,   dummy::second);
  QCOMPARE(trd,   dummy::third);
  QCOMPARE(other, dummy::MAX);
}

//QTEST_MAIN(TestEnum)
