#pragma once

#include <QObject>
#include "../concurrency/thread.h"
#include "../concurrency/semaphore.h"


namespace test {
namespace concurrency {

class semaphore_allocator_t : public advanced::concurrency::thread_t {

public:

  ~semaphore_allocator_t()
  override {
    stop();
    join();
  }

  semaphore_allocator_t() = default;
  semaphore_allocator_t(const semaphore_allocator_t& other) : semaphore{ other.semaphore } {}
  semaphore_allocator_t(semaphore_allocator_t&& )     = default;
  inline semaphore_allocator_t& operator=(const semaphore_allocator_t&) = default;
  inline semaphore_allocator_t& operator=(semaphore_allocator_t&& )     = default;
  semaphore_allocator_t(advanced::concurrency::semaphore_t* semaphore);

  void
  stop();

  void
  allocate_resource();

  bool
  is_running() const;

  static std::atomic_size_t allocations;

  // thread_t interface
protected:

  virtual int run() override;
  advanced::concurrency::semaphore_t* semaphore{ nullptr };
  std::condition_variable    _cv;
  std::atomic_bool           _running{ true };
};

}
}

class TestSemaphore : public QObject
{
  Q_OBJECT
public:
  explicit TestSemaphore(QObject *parent = nullptr);

private slots:

  void test_lock_method();
  void test_try_lock();
  void test_unlock_method();
  void test_std_locks_integration();
  void test_multiple_access_to_resource();
  void test_increasing_max_counter_should_allow_more_multiple_access();
  void test_decreasing_max_counter();
  void test_count_should_be_consistent();
  void test_max_count_getter_should_be_consistent();
  void test_size_t_cast_operator_be_consistent();
};

