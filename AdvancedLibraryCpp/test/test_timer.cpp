#include "test_timer.h"
#include <QTest>
#include <cmath>

using namespace advanced::concurrency;
using namespace test::concurrency;

namespace test {
namespace concurrency {

const std::vector<double> &moc_timer::
intervals() {
  return _intervals;
}

int moc_timer::
on_interval() {
  auto elapsed = _clock.elapsed();
  _intervals.push_back(elapsed);
  _clock.reset();
  return EXIT_SUCCESS;
}

void moc_timer::
on_stop() {
  on_stop_called = true;
}

void moc_timer::
on_start() {
  on_start_called = true;
}

}
}

TestTimer::
TestTimer(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestTimer");
}

/**
 * @brief TestTimer::test_simple_timer_should_execute_some_function_with_a_given_period
 * @note This test is highly machine dependent, so, you can comment it, if
 * they do not pass in your machine.
 *
 * They exists to comprove the concept and to give an example of usage
 */
void TestTimer::
test_simple_timer_should_execute_some_function_with_a_given_period() {
  const std::vector<double> input {
    45.1, 5.0, 89.9, -39.3, 67.8,
  };
  std::vector<double>::const_iterator it { input.begin() };
  std::vector<double> values;
  auto pred { [&values, &it, &input]() {
      if (it != input.end()) {
        values.push_back(*it++);
      }
      return it != input.end();
  }};
  const size_t interval_ms{ 100 };
  advanced::concurrency::timer_t timer{ interval_ms, pred };
  timer.start();

  for (size_t ii = 0; ii < input.size(); ii++) {
    QTRY_COMPARE_WITH_TIMEOUT(values.size(), ii+1, interval_ms);
  }

  QCOMPARE(values, input);
}

/**
 * @brief TestTimer::test_simple_timer_should_work_with_1ms_time_resolution_and_error_under_5_percent
 * @note This test is highly machine dependent, so, you can comment it, if
 * they do not pass in your machine.
 *
 * They exists to comprove the concept and to give an example of usage
 */
void TestTimer::
test_simple_timer_should_work_with_1ms_time_resolution_and_error_under_5_percent() {
  const size_t interval_ms{ 1 };
  test::concurrency::moc_timer timer{ interval_ms };
  timer.start();
  QTest::qWait(1000 * interval_ms);
  timer.stop();
  timer.join();

  const auto& intervals{ timer.intervals() };
  double sum_of_intervals{ 0 };
  for (size_t ii = 1; ii < intervals.size() - 1; ii++) {
    sum_of_intervals += intervals[ii];
  }

  const double upper_bound { ((intervals.size() - 2) * interval_ms) * 1.05 };
  const double lower_bound { ((intervals.size() - 2) * interval_ms) * 0.95 };

  QVERIFY2(sum_of_intervals <= upper_bound && sum_of_intervals >= lower_bound,
           "Time resolution was not enough");
}

void TestTimer::
test_timer_should_have_on_stop_method_called_after_stop() {
  const size_t interval_ms{ 10 };
  test::concurrency::moc_timer timer{ interval_ms };
  timer.start();

  QTest::qWait(static_cast<int>(interval_ms * 10u ));
  QVERIFY(!timer.on_stop_called);

  timer.stop();
  QTRY_VERIFY_WITH_TIMEOUT(timer.on_stop_called, interval_ms);
  timer.join();
}

void TestTimer::
test_timer_should_have_on_start_method_called_after_start() {
  const size_t interval_ms{ 10 };
  test::concurrency::moc_timer timer{ interval_ms };

  QTest::qWait(static_cast<int>(interval_ms * 10u ));
  QVERIFY(!timer.on_start_called);
  timer.start();
  QTRY_VERIFY_WITH_TIMEOUT(timer.on_start_called, interval_ms);

  timer.stop();
  timer.join();
}

/**
 * @brief TestTimer::test_set_interval
 * @note This test is highly machine dependent, so, you can comment it, if
 * they do not pass in your machine.
 *
 * They exists to comprove the concept and to give an example of usage
 */
void TestTimer::
test_set_interval() {
  const size_t interval_ms{ 10 };
  test::concurrency::moc_timer timer{ interval_ms };
  timer.start();

  QTest::qWait(interval_ms * 51);
  timer.set_interval(interval_ms * 2);
  QTest::qWait((interval_ms * 2) * 26);
  timer.stop();
  timer.join();

  const auto& intervals{ timer.intervals() };
  {
    double sum_of_intervals { 0 };
    for (size_t ii = 0; ii < 50; ii++) {
      sum_of_intervals += intervals[ii];
    }
    const double upper_bound { ((50) * interval_ms) * 1.05 };
    const double lower_bound { ((50) * interval_ms) * 0.95 };

    QVERIFY2(sum_of_intervals <= upper_bound && sum_of_intervals >= lower_bound,
             "Time resolution was not enough");
  }

  {
    double sum_of_intervals { 0 };
    for (size_t ii = 52; ii < intervals.size(); ii++) {
      sum_of_intervals += intervals[ii];
    }
    const double upper_bound { ((intervals.size() - 52) * 2 * interval_ms) * 1.05 };
    const double lower_bound { ((intervals.size() - 52) * 2 * interval_ms) * 0.95 };

    QVERIFY2(sum_of_intervals <= upper_bound && sum_of_intervals >= lower_bound,
             "Time resolution was not enough");
  }
}

