#include "test_memory.h"
#include "../tools/memory.h"

TestMemory::
TestMemory(QObject *parent) : QObject(parent) {

}

void TestMemory::
test_make_unique_for_cpp_11() {
  std::unique_ptr<base_class> ptr = advanced::make_unique<derived>(10, 20);
  QCOMPARE(ptr->getA(), 10);
  QCOMPARE(ptr->getB(), 20);
}
