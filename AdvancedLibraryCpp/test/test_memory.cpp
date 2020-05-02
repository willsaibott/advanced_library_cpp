#include "test_memory.h"
#include "../tools/memory.h"

using namespace advanced::tools;

TestMemory::
TestMemory(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestMemory");
}

void TestMemory::
test_make_unique_for_cpp_11() {
  std::unique_ptr<base_class> ptr = make_unique<derived>(10, 20);
  QCOMPARE(ptr->getA(), 10);
  QCOMPARE(ptr->getB(), 20);
}
