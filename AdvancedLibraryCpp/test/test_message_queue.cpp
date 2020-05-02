#include "test_message_queue.h"

using namespace test;

TestMessageQueue::
TestMessageQueue(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestMessageQueue");
}

void TestMessageQueue::
test_message_queue_should_start_in_another_thread_when_start_method_is_called() {
  concurrency::simple_msg_worker_t simple_worker;

  simple_worker.start();
  QTRY_VERIFY2_WITH_TIMEOUT(simple_worker.is_running(),
                            "Thread should be started by now",
                            100);
  auto my_id    { std::this_thread::get_id() };
  auto worker_id{ simple_worker.instance().get_id() };
  QVERIFY2(my_id != worker_id, "They should run in different threads");

  // Message queue should stop on destructor...
}

void TestMessageQueue::
test_message_queue_should_stop_when_stop_method_is_called() {
  concurrency::simple_msg_worker_t simple_worker;

  simple_worker.start();
  QTRY_VERIFY2_WITH_TIMEOUT(simple_worker.is_running(),
                            "Thread should be started by now",
                            100);
  simple_worker.stop();
  QTRY_VERIFY2_WITH_TIMEOUT(!simple_worker.is_running(),
                            "Thread should be stopped by now",
                            100);

  // Message queue should be joined automatically on thread_t destructor...
}

void TestMessageQueue::
test_message_queue_size_methods() {
  const QVector<protocol::msg_ptr> msgs {
    std::make_shared<protocol::msg_readA>(),
    std::make_shared<protocol::msg_readB>(),
    std::make_shared<protocol::msg_writeA>(),
    std::make_shared<protocol::msg_writeB>(),
    std::make_shared<protocol::msg_readB>(),
    std::make_shared<protocol::msg_writeB>(),
    std::make_shared<protocol::msg_writeA>(),
    std::make_shared<protocol::msg_readA>(),
  };
  concurrency::simple_msg_worker_t simple_worker;
  size_t expected_size = 0;

  QCOMPARE(simple_worker.size(), expected_size);

  for (const auto& msg_ptr : msgs) {
    simple_worker.receive_message(msg_ptr);
    QCOMPARE(simple_worker.size(), (++expected_size));
  }
}

void TestMessageQueue::
test_message_queue_should_discard_queued_messages_when_discard_method_is_called() {
  const QVector<protocol::msg_ptr> msgs {
    std::make_shared<protocol::msg_readA>(),
    std::make_shared<protocol::msg_readB>(),
    std::make_shared<protocol::msg_writeA>(),
    std::make_shared<protocol::msg_writeB>(),
    std::make_shared<protocol::msg_readB>(),
    std::make_shared<protocol::msg_writeB>(),
    std::make_shared<protocol::msg_writeA>(),
    std::make_shared<protocol::msg_readA>(),
  };
  concurrency::simple_msg_worker_t simple_worker;

  for (const auto& msg_ptr : msgs) {
    simple_worker.receive_message(msg_ptr);
  }
  QCOMPARE(simple_worker.size(), static_cast<size_t>(msgs.size()));

  simple_worker.discard_messages();

  // The discard will occurr only when the thread is running
  QCOMPARE(simple_worker.size(), static_cast<size_t>(msgs.size()));
  simple_worker.start();
  QTRY_VERIFY_WITH_TIMEOUT(simple_worker.empty(), 100);
  compare(simple_worker, {});
}

void TestMessageQueue::
test_reentrance() {
  concurrency::reentrant_msg_worker_t worker;
  worker.start();
  worker.receive_message(std::make_shared<protocol::msg_readA>());
  QTest::qWait(300);
  worker.stop();
  worker.join();
}

void TestMessageQueue::
test_on_start_method_should_called_after_another_context_call_the_start_method() {
  concurrency::simple_msg_worker_t simple_worker;

  QVERIFY2(!simple_worker.already_started(),
           "on start method should be called only after start has been called");

  simple_worker.start();

  QTRY_VERIFY2_WITH_TIMEOUT(simple_worker.already_started(),
                            "on start method should be called after start has been called",
                            100);

  // Message queue should stop on destructor...
}

void TestMessageQueue::
test_on_stop_method_should_called_after_another_context_call_the_stop_method() {
  concurrency::simple_msg_worker_t simple_worker;

  simple_worker.start();
  QTest::qWait(100);

  QVERIFY2(!simple_worker.already_stopped(),
           "on stop method should be called only after stop");

  simple_worker.stop();
  QTRY_VERIFY2_WITH_TIMEOUT(simple_worker.already_stopped(),
                            "on stop method should be called after stop",
                            100);

  // Message queue should be joined automatically on thread_t destructor...
}

void TestMessageQueue::
test_simple_queue_should_process_messages_one_by_one_as_a_fifo() {
  const std::vector<protocol::msg_ptr> msgs {
    std::make_shared<protocol::msg_readA>(),
    std::make_shared<protocol::msg_readB>(),
    std::make_shared<protocol::msg_writeA>(),
    std::make_shared<protocol::msg_writeB>(),
    std::make_shared<protocol::msg_readB>(),
    std::make_shared<protocol::msg_writeB>(),
    std::make_shared<protocol::msg_writeA>(),
    std::make_shared<protocol::msg_readA>(),
  };
  concurrency::simple_msg_worker_t simple_worker;

  for (const auto& msg_ptr : msgs) {
    simple_worker.receive_message(msg_ptr);
  }

  QTest::qWait(100);
  // No message should be processed while message queue is not started:
  compare(simple_worker, {});

  simple_worker.start();
  simple_worker.stop();
  simple_worker.join();

  // All Messages should be processed one by one in FIFO order:
  compare(simple_worker, msgs);
}

void TestMessageQueue::
test_priority_queue_should_process_messages_ordered_by_greatest_priority() {
  std::vector<protocol::msg_ptr> msgs {
    std::make_shared<protocol::msg_readA>(protocol::priority_type::LOW),
    std::make_shared<protocol::msg_readB>(protocol::priority_type::NORMAL),
    std::make_shared<protocol::msg_writeA>(protocol::priority_type::HIGH),
    std::make_shared<protocol::msg_writeB>(protocol::priority_type::HIGH),
    std::make_shared<protocol::msg_readB>(protocol::priority_type::NORMAL),
    std::make_shared<protocol::msg_writeB>(protocol::priority_type::LOW),

    // default normal:
    std::make_shared<protocol::msg_writeA>(),
    std::make_shared<protocol::msg_readA>()
  };

  concurrency::priority_msg_worker_t worker_by_priority;

  for (const auto& msg_ptr : msgs) {
    worker_by_priority.receive_message(msg_ptr);
  }

  QTest::qWait(100);
  // No message should be processed while message queue is not started:
  compare(worker_by_priority, {});

  worker_by_priority.start();
  worker_by_priority.stop();
  worker_by_priority.join();

  // Sorting messages by priority...
  std::sort(msgs.begin(),
            msgs.end(),
            advanced::concurrency::pointer_comparison<
                protocol::message_base_t,
                std::greater<protocol::message_base_t>>{});

  // Sorting may preserve
  compare(worker_by_priority, msgs);
}
