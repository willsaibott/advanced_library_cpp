#pragma once

#include <QObject>
#include <QTest>
#include "../concurrency/thread.h"

namespace test {
  class start_only : public advanced::concurrency::thread_t {
    std::atomic_bool _running{ true };
    std::atomic_int  _code{ EXIT_SUCCESS };

  protected:

    // thread_t interface
    virtual int
    run() override {
      while (_running) {
        QTest::qWait(100);
      }
      return _code;
    }

  public:

    virtual
    ~start_only () override {
      _running = false;
    }

    void clear() {
      _running = true;
    }

    void mock_exit_code(int exit_code) {
      _code = exit_code;
    }

    void stop() {
      _running = false;
    }

  };
}

class TestWrapperThread : public QObject
{
  Q_OBJECT
public:
  explicit TestWrapperThread(QObject *parent = nullptr);

private slots:

  void test_start_method_should_trigger_run_method();
  void test_join();
  void test_joinable();
  void test_exist_code();
  void test_is_running();
  void test_thread_ids_should_be_different();
};

