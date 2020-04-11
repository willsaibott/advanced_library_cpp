#include "testenum.h"
#include "../enum_factory.h"

TestEnum::
TestEnum(QObject *parent) : QObject(parent) {
}

void TestEnum::
test_to_string() {
  QCOMPARE("first",  std::to_string(dummy::first));
  QCOMPARE("second", std::to_string(dummy::second));
  QCOMPARE("third",  std::to_string(dummy::third));
}

void TestEnum::
test_from_string() {
  const dummy fst   { advanced::from_string<dummy>("first") };
  const dummy snd   { advanced::from_string<dummy>("second") };
  const dummy trd   { advanced::from_string<dummy>("third") };
  const dummy other { advanced::from_string<dummy>("other") };
  QCOMPARE(fst,   dummy::first);
  QCOMPARE(snd,   dummy::second);
  QCOMPARE(trd,   dummy::third);
  QCOMPARE(other, dummy::MAX);
}

void TestEnum::
test_from_string_custom_max_limit() {
  const dummy fst   { advanced::from_string<dummy, dummy::first, dummy::third>("first") };
  const dummy snd   { advanced::from_string<dummy, dummy::first, dummy::third>("second") };
  const dummy trd   { advanced::from_string<dummy, dummy::first, dummy::third>("third") };
  const dummy other { advanced::from_string<dummy, dummy::first, dummy::third>("other") };
  QCOMPARE(fst,   dummy::first);
  QCOMPARE(snd,   dummy::second);
  QCOMPARE(trd,   dummy::third); // third is not compared!!!
  QCOMPARE(other, dummy::third);
}

void TestEnum::
test_from_string_custom_min_limit() {
  const dummy fst   { advanced::from_string<dummy, dummy::second>("first") };
  const dummy snd   { advanced::from_string<dummy, dummy::second>("second") };
  const dummy trd   { advanced::from_string<dummy, dummy::second>("third") };
  const dummy other { advanced::from_string<dummy, dummy::second>("other") };
  QCOMPARE(fst,   dummy::MAX);    // first is not compared!!!
  QCOMPARE(snd,   dummy::second);
  QCOMPARE(trd,   dummy::third);
  QCOMPARE(other, dummy::MAX);
}

//QTEST_MAIN(TestEnum)
