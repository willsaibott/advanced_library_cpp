#pragma once

#include <QObject>
#include <QTest>
#include <QVector>

#include <queue>
#include <memory>
#include "simple_worker_moc.h"

class TestMessageQueue : public QObject
{
  Q_OBJECT
public:
  explicit TestMessageQueue(QObject *parent = nullptr);

  template<class message_queue>
  void compare(const message_queue&                        msg_queue,
               const std::vector<test::protocol::msg_ptr>& expected_msgs) {
    QCOMPARE(msg_queue.messages_processed(), expected_msgs);
  }

private slots:

  void test_message_queue_should_start_in_another_thread_when_start_method_is_called();
  void test_message_queue_should_stop_when_stop_method_is_called();
  void test_message_queue_size_methods();
  void test_message_queue_should_discard_queued_messages_when_discard_method_is_called();
  void test_reentrance();
  void test_on_start_method_should_called_after_another_context_call_the_start_method();
  void test_on_stop_method_should_called_after_another_context_call_the_stop_method();
  void test_simple_queue_should_process_messages_one_by_one_as_a_fifo();
  void test_priority_queue_should_process_messages_ordered_by_greatest_priority();
};
