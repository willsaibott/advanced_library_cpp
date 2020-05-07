#include "test_timestamp.h"

TestTimestamp::
TestTimestamp(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestTimestamp");
}

void TestTimestamp::
test_difference() {
  advanced::tools::timestamp_t clock;
  QTest::qWait(100);
  auto end = clock.difference<std::chrono::milliseconds>();
  QVERIFY(end.count() >= 100);
}

void TestTimestamp::
test_elapsed() {
  advanced::tools::timestamp_t clock;
  QTest::qWait(100);
  auto ms = clock.elapsed();
  QVERIFY(ms >= 100);
}
