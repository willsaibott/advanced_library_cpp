#include "test_semaphore.h"
#include <mutex>
#include <QTest>

using semaphore_t = advanced::concurrency::semaphore_t;
using consumer_t  = test::concurrency::semaphore_allocator_t;

namespace test {
namespace concurrency {

std::atomic_size_t semaphore_allocator_t::allocations{ 0 };

semaphore_allocator_t::
semaphore_allocator_t(advanced::concurrency::semaphore_t* semaphore)
  : semaphore{ semaphore }, _running{ true }
{

}

void semaphore_allocator_t::
stop() {
  _running = false;
  _cv.notify_one();
}

void semaphore_allocator_t::
allocate_resource() {
  auto pred = [this]() { return is_running(); };
  std::mutex local_mtx;
  std::lock_guard guard { *semaphore };
  allocations++;
  while (is_running()) {
    std::unique_lock<std::mutex> sleep_guard( local_mtx );
    _cv.wait_for(sleep_guard, std::chrono::milliseconds{ 10 }, pred);
  }
  allocations--;
}

bool semaphore_allocator_t::
is_running() const {
  return _running;
}

int semaphore_allocator_t::
run() {
  if (semaphore) {
    allocate_resource();
  }
  return semaphore ? EXIT_SUCCESS : EXIT_FAILURE;
}

}
}


TestSemaphore::
TestSemaphore(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestSemaphore");
}

void TestSemaphore::
test_lock_method() {
  const size_t max_resources{ 3 };
  semaphore_t semaphore{ max_resources };
  semaphore_t null_semaphore{ 0 };

  // lock once
  semaphore.lock();
  semaphore.unlock();
  QCOMPARE(semaphore.count(), 0u);

  // lock twice
  semaphore.lock();
  semaphore.lock();
  semaphore.unlock();
  semaphore.unlock();
  QCOMPARE(semaphore.count(), 0u);

  // lock three times
  semaphore.lock();
  semaphore.lock();
  semaphore.lock();
  semaphore.unlock();
  semaphore.unlock();
  semaphore.unlock();
  QCOMPARE(semaphore.count(), 0u);

  // multiple locks and unlocks
  semaphore.lock();
  semaphore.lock();
  semaphore.unlock();
  semaphore.lock();
  semaphore.unlock();
  semaphore.unlock();
  QCOMPARE(semaphore.count(), 0u);

  // multiple locks and unlocks
  semaphore.lock();
  semaphore.unlock();
  semaphore.lock();
  semaphore.lock();
  semaphore.unlock();
  semaphore.unlock();
  QCOMPARE(semaphore.count(), 0u);

  QVERIFY_EXCEPTION_THROWN(null_semaphore.lock(), semaphore_t::out_of_resources_exception);
}

void TestSemaphore::
test_try_lock() {
  const size_t max_resources{ 30 };
  semaphore_t  semaphore{ max_resources };

  for (size_t ii = 0; ii < max_resources; ii++) {
    semaphore.lock();
  }
  QVERIFY2(!semaphore.try_lock(),
           "Try lock should fail imediatelly, because all available resources are allocated");

  for (size_t ii = 0; ii < max_resources; ii++) {
    semaphore.unlock();
  }
  QCOMPARE(semaphore.count(), 0u);

  for (size_t ii = 0; ii < max_resources; ii++) {
    QVERIFY2(semaphore.try_lock(),
             "It should allocate resource while there are available resources");
  }

  QVERIFY2(!semaphore.try_lock(),
           "Try lock should fail imediatelly, because all available resources are allocated");

  for (size_t ii = 0; ii < max_resources; ii++) {
    semaphore.unlock();
  }
  QCOMPARE(semaphore.count(), 0u);
}

void TestSemaphore::
test_unlock_method() {
  const size_t max_resources{ 30 };
  semaphore_t  semaphore{ max_resources };
  // lock three times
  for (size_t ii = 1; ii <= max_resources; ii++) {
    semaphore.lock();
    QCOMPARE(semaphore.count(), ii);
  }

  for (size_t ii = 1; ii <= max_resources; ii++) {
    semaphore.unlock();
    QCOMPARE(semaphore.count(), max_resources - ii);
  }
  // multiple unlock calls without any lock:
  semaphore.unlock().unlock().unlock().unlock();
  semaphore.unlock().unlock().unlock().unlock();
  QCOMPARE(semaphore.count(), 0u);
}

void TestSemaphore::
test_std_locks_integration() {
  const size_t max_resources{ 50 };
  semaphore_t  semaphore{ max_resources };
  QCOMPARE(semaphore.count(), 0u);

  {
    std::mutex dummy, dummy2, dummy3;

    std::unique_lock unique_guard{ semaphore };
    QCOMPARE(semaphore.count(), 1u);

    std::lock_guard guard{ semaphore };
    QCOMPARE(semaphore.count(), 2u);

    std::lock(semaphore, dummy);
    QCOMPARE(semaphore.count(), 3u);

    std::scoped_lock multiple_locks{ semaphore, dummy2 };
    QCOMPARE(semaphore.count(), 4u);
    std::scoped_lock multiple_read_lock{ dummy3, semaphore };
    QCOMPARE(semaphore.count(), 5u);

    // reverting std::lock(semaphore, dummy);
    semaphore.unlock();
    dummy.unlock();
  }

  QCOMPARE(semaphore.count(), 0u);
}

void TestSemaphore::
test_multiple_access_to_resource() {
  const size_t max_resources{ 3 };
  size_t       total_consumers{ 10 };
  semaphore_t  semaphore{ max_resources };
  std::vector<consumer_t> consumers(total_consumers, consumer_t{ &semaphore });

  for (auto& consumer : consumers) {
    consumer.start();
  }

  QTRY_COMPARE_WITH_TIMEOUT(consumer_t::allocations, max_resources, 200);

  for (auto& consumer : consumers) {
    consumer.stop();
  }

  for (auto& consumer : consumers) {
    consumer.join();
  }
}

void TestSemaphore::
test_increasing_max_counter_should_allow_more_multiple_access() {
  const size_t max_resources{ 3 };
  size_t       total_consumers{ 10 };
  semaphore_t  semaphore{ max_resources };
  std::vector<consumer_t> consumers(total_consumers, consumer_t{ &semaphore });

  for (auto& consumer : consumers) {
    consumer.start();
  }

  QTRY_COMPARE_WITH_TIMEOUT(consumer_t::allocations, max_resources, 200);

  for (size_t ii = max_resources; ii <= total_consumers; ii++) {
    semaphore.set_available_resources(ii);
    QTRY_COMPARE_WITH_TIMEOUT(consumer_t::allocations, ii, 200);
  }

  for (auto& consumer : consumers) {
    consumer.stop();
  }

  for (auto& consumer : consumers) {
    consumer.join();
  }
}

void TestSemaphore::
test_decreasing_max_counter() {
  size_t max_resources{ 3 };
  size_t total_consumers{ 10 };
  semaphore_t semaphore{ max_resources };
  std::vector<consumer_t> consumers(total_consumers, consumer_t{ &semaphore });

  for (auto& consumer : consumers) {
    consumer.start();
  }

  QTRY_COMPARE_WITH_TIMEOUT(consumer_t::allocations, max_resources, 200);

  for (size_t ii = max_resources; ii > 0; ii--) {
    // No consumer that already has allocated the resources will lose it:
    semaphore.set_available_resources(ii);
    QTRY_COMPARE_WITH_TIMEOUT(consumer_t::allocations, max_resources, 200);
  }

  for (auto& consumer : consumers) {
    consumer.stop();
  }
  for (auto& consumer : consumers) {
    consumer.join();
  }
}

void TestSemaphore::
test_count_should_be_consistent() {
  const size_t max_resources{ 30 };
  semaphore_t  semaphore{ max_resources };
  // lock three times
  for (size_t ii = 1; ii <= max_resources; ii++) {
    semaphore.lock();
    QCOMPARE(semaphore.count(), ii);
  }

  for (size_t ii = 1; ii <= max_resources; ii++) {
    semaphore.unlock();
    QCOMPARE(semaphore.count(), max_resources - ii);
  }
}

void TestSemaphore::
test_max_count_getter_should_be_consistent() {
  const size_t max_resources{ 30 };
  semaphore_t  semaphore{ max_resources };

  semaphore.set_available_resources(0u);
  QCOMPARE(semaphore.max_count(), 0u);

  semaphore.set_available_resources(10u);
  QCOMPARE(semaphore.max_count(), 10u);

  semaphore.set_available_resources(20u);
  QCOMPARE(semaphore.max_count(), 20u);

  semaphore.set_available_resources(static_cast<size_t>(1E10));
  QCOMPARE(semaphore.max_count(), static_cast<size_t>(1E10));
}

void TestSemaphore::
test_size_t_cast_operator_be_consistent() {
  const size_t max_resources{ 30 };
  semaphore_t  semaphore{ max_resources };
  // lock three times
  for (size_t ii = 1; ii <= max_resources; ii++) {
    semaphore.lock();
    QCOMPARE(static_cast<size_t>(semaphore), ii);
  }

  for (size_t ii = 1; ii <= max_resources; ii++) {
    semaphore.unlock();
    QCOMPARE(static_cast<size_t>(semaphore), max_resources - ii);
  }
}
