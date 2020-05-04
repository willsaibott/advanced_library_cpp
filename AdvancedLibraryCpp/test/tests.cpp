#include <QVector>
#include <iostream>
#include <experimental/filesystem>
#include "test_lockable.h"
#include "test_enum.h"
#include "test_memory.h"
#include "test_stream_delimiters.h"
#include "test_random.h"
#include "test_missing_elements.h"
#include "test_subset.h"
#include "test_subarray.h"
#include "test_wrapper_thread.h"
#include "test_message_queue.h"
#include "test_lru_cache.h"
#include "test_semaphore.h"
#include "test_timer.h"
#include "test_binary_tree.h"

namespace fs = std::experimental::filesystem;

int
main(int argc, char** argv) {
  Q_UNUSED(argc);
  Q_UNUSED(argv);
  int status, final_status{ 0 }, suite{ 0 };
  QVector<QObject*> test_suits {
    new TestLockable(),
    new TestEnum(),
    new TestMemory(),
    new TestStreamDelimiters(),
    new TestRandom(),
    new TestMissingElements(),
    new TestSubset(),
    new TestSubArray(),
    new TestWrapperThread(),
    new TestMessageQueue(),
    new TestLRUCache(),
    new TestSemaphore(),
    new TestTimer(),
    new TestBinaryTree(),
  };

  fs::create_directory("results");

  for (auto& test : test_suits) {
    QString path { "results/" + test->objectName() + ".xml" };
    final_status |=
        status =
          QTest::qExec(test, QStringList() << " " << "-o" << path << "-xunitxml");
    suite++;
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
    delete test;
  }

  return final_status;
}
