#include <QVector>
#include <iostream>
#include "test_lockable.h"
#include "test_enum.h"
#include "test_memory.h"
#include "test_stream_delimiters.h"
#include "test_random.h"
#include "test_missing_elements.h"

int
main(int argc, char** argv) {
  int status, final_status{ 0 }, suite{ 0 };
  QVector<QObject*> test_suits {
    new TestLockable(),
    new TestEnum(),
    new TestMemory(),
    new TestStreamDelimiters(),
    new TestRandom(),
    new TestMissingElements()
  };

  for (auto& test : test_suits) {
    final_status |= status = QTest::qExec(test, argc, argv);
    suite++;
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  return final_status;
}
