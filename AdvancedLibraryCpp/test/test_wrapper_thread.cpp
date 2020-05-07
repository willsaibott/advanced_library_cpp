#include "test_wrapper_thread.h"

#include <QTest>

TestWrapperThread::
TestWrapperThread(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestWrapperThread");
}

void TestWrapperThread::
test_start_method_should_trigger_run_method() {
  test::start_only thread;
  thread.start();
  QTest::qWait(200);
  QVERIFY(thread.is_running());
}

void TestWrapperThread::
test_join() {
  test::start_only thread;
  thread.start();
  QTest::qWait(200);
  QVERIFY(thread.is_running());

  thread.stop();
  thread.join();
  QVERIFY(!thread.is_running());
}

void TestWrapperThread::
test_joinable() {
  test::start_only thread;
  thread.start();
  QTest::qWait(200);
  QVERIFY(thread.is_running());
  QVERIFY(thread.joinable());

  thread.stop();
  thread.join();
  QVERIFY(!thread.is_running());
  QVERIFY(!thread.joinable());
}

void TestWrapperThread::
test_exist_code() {
  test::start_only thread;
  thread.start();
  QTest::qWait(200);

  thread.stop();
  thread.join();
  QCOMPARE(thread.exit_code(), EXIT_SUCCESS);

  thread.clear();
  thread.start();
  QTest::qWait(200);
  thread.mock_exit_code(EXIT_FAILURE);
  QTest::qWait(200);
  thread.stop();
  thread.join();
  QCOMPARE(thread.exit_code(), EXIT_FAILURE);
}

void TestWrapperThread::
test_is_running() {
  test::start_only thread;
  thread.start();
  QTest::qWait(200);
  QVERIFY(thread.is_running());

  thread.stop();
  thread.join();
  QVERIFY(!thread.is_running());
}

void TestWrapperThread::
test_thread_ids_should_be_different() {
  test::start_only thread;
  thread.start();
  QTest::qWait(200);
  QVERIFY(thread.is_running());

  auto my_id    { std::this_thread::get_id() };
  auto other_id { thread.instance().get_id() };
  Q_ASSERT(my_id != other_id);

  thread.stop();
  thread.join();
  QVERIFY(!thread.is_running());
}

void TestWrapperThread::
test_as_pointer() {
  std::unique_ptr<test::start_only> thread_ptr { std::make_unique<test::start_only>() };
  test::start_only& thread{ *thread_ptr };
  thread.start();
  QTest::qWait(200);
  QVERIFY(thread.is_running());

  auto my_id    { std::this_thread::get_id() };
  auto other_id { thread.instance().get_id() };
  Q_ASSERT(my_id != other_id);

  thread.stop();
  thread.join();
  QVERIFY(!thread.is_running());
}
