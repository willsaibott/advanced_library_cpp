#pragma once

#include <QObject>
#include "../concurrency/timer.h"
#include "../tools/timestamp.h"

namespace test {
namespace concurrency {

class moc_timer : public advanced::concurrency::base_timer_t {

  // base_timer_t interface

public:

  ~moc_timer() override {
    safe_delete();
  }

  moc_timer() {}
  moc_timer(size_t timeout_ms)
    : advanced::concurrency::base_timer_t{ timeout_ms } { }

  std::atomic_bool  on_start_called{ false };
  std::atomic_bool  on_stop_called{ false };

  const std::vector<double>& intervals();

  using advanced::concurrency::base_timer_t::notify;

protected:

  virtual int on_interval() override;
  virtual void on_stop() override;
  virtual void on_start() override;

  advanced::tools::timestamp_t  _clock;
  std::vector<double>           _intervals;
};

}
}

class TestTimer : public QObject
{
  Q_OBJECT
public:
  explicit TestTimer(QObject *parent = nullptr);

private slots:

  void test_simple_timer_should_execute_some_function_with_a_given_period();
  void test_simple_timer_should_work_with_1ms_time_resolution_and_error_under_5_percent();
  void test_timer_should_have_on_stop_method_called_after_stop();
  void test_timer_should_have_on_start_method_called_after_start();
  void test_set_interval();
  void test_is_running();
  void test_notify();
};

