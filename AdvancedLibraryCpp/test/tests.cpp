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
#include "test_avl_tree.h"
#include "test_timestamp.h"
#include "test_math.h"
#include "test_tree.h"

namespace fs = std::experimental::filesystem;

/*
 *
  #define DECLARE_TEST(TestClass) \
  {\
      TESTLIB_SELFCOVERAGE_START(#TestClass) \
      QCoreApplication app(argc, argv); \
      app.setAttribute(Qt::AA_Use96Dpi, true); \
      TestClass test; \
      QTEST_SET_MAIN_SOURCE_PATH \
      QString path { "results/" + test.objectName() + ".xml" }; \
      auto status = QTest::qExec(&test, argc, argv); \
      if (status) { \
        std::cerr << "Test " << suite << " returned: " << status << std::endl; \
      } \
  }\
*/

//#define TESTGUI
#ifdef TESTGUI

int main(int argc, char** argv) {
  fs::create_directory("results");
  size_t suite{ 0 };

  // if (true) to become collapsable

  if (true)
  {
      TESTLIB_SELFCOVERAGE_START(TestLockable)
      QCoreApplication app(argc, argv);
      app.setAttribute(Qt::AA_Use96Dpi, true);
      TestLockable test;
      QTEST_SET_MAIN_SOURCE_PATH \
      QString path { "results/" + test.objectName() + ".xml" };
      auto status = QTest::qExec(&test, argc, argv);
      if (status) {
        std::cerr << "Test " << suite << " returned: " << status << std::endl;
      }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestEnum)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestEnum test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestMemory)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestMemory test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestStreamDelimiters)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestStreamDelimiters test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestRandom)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestRandom test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestMissingElements)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestMissingElements test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestSubset)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestSubset test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestSubArray)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestSubArray test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestWrapperThread)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestWrapperThread test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  {
    TESTLIB_SELFCOVERAGE_START(TestMessageQueue)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestMessageQueue test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestLRUCache)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestLRUCache test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestSemaphore)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestSemaphore test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestTimer)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestTimer test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestBinaryTree)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestBinaryTree test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestAVLTree)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestAVLTree test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestTree)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestTree test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestTimestamp)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestTimestamp test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }
  if (true)
  {
    TESTLIB_SELFCOVERAGE_START(TestMath)
        QCoreApplication app(argc, argv);
    app.setAttribute(Qt::AA_Use96Dpi, true);
    TestMath test;
    QTEST_SET_MAIN_SOURCE_PATH \
        QString path { "results/" + test.objectName() + ".xml" };
    auto status = QTest::qExec(&test, argc, argv);
    if (status) {
      std::cerr << "Test " << suite << " returned: " << status << std::endl;
    }
  }

// Qt does not recognize under macros:  :(
//  DECLARE_TEST(TestRandom)
//  DECLARE_TEST(TestMissingElements)
//  DECLARE_TEST(TestSubset)
//  DECLARE_TEST(TestSubArray)
//  DECLARE_TEST(TestWrapperThread)
//  DECLARE_TEST(TestMessageQueue)
//  DECLARE_TEST(TestLRUCache)
//  DECLARE_TEST(TestSemaphore)
//  DECLARE_TEST(TestTimer)
//  DECLARE_TEST(TestBinaryTree)
}

#else

int
main(int argc, char** argv) {
  int status, final_status{ 0 };
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
    new TestAVLTree(),
    new TestTree(),
    new TestTimestamp(),
    new TestMath()
  };

  fs::create_directory("results");
  system("rm -rf *.gcda");

  for (auto& test : test_suits) {
    QString path { "results/" + test->objectName() + ".xml" };
    auto args{ QStringList() << " " << "-o" << path << "-xunitxml" };
    final_status += status = QTest::qExec(test, args);
    if (status) {
      std::cerr << test->objectName().toStdString() << " returned: "
                << status << "\n";
    }
    delete test;
  }

  Q_UNUSED(argc);
  Q_UNUSED(argv);
  return final_status;
}

#endif
