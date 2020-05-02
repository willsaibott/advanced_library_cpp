#pragma once

#include <QVector>
#include <QTest>
#include <queue>
#include "../concurrency/message_queue.h"
#include "simple_protocol_moc.h"

namespace test {
namespace concurrency {

template <class queue>
class base_worker_t :
    public advanced::concurrency::message_queue_t<queue> {
public:

  using base_class = advanced::concurrency::message_queue_t<queue>;
  using message_t  = typename base_class::message_t;

  virtual
  ~base_worker_t() override {
    base_class::stop();
    base_class::join();
  }

  const std::vector<protocol::msg_ptr>&
  messages_processed() const {
    return _messages_processed;
  }

  inline bool
  already_started() const {
    return _has_started;
  }

  inline bool
  already_stopped() const {
    return _has_stopped;
  }

  void
  clear() {
    _has_started = false;
    _has_stopped = false;
  }

  virtual void
  on_start() override {
    _has_started = true;
  }

  virtual void
  on_stop() override {
    _has_stopped = true;
  }

  virtual void
  on_new_message(const message_t &msg) override {
    switch (msg->type()) {
    case protocol::message_type::READ_A:
    case protocol::message_type::READ_B:
    case protocol::message_type::WRITE_A:
    case protocol::message_type::WRITE_B:

      _messages_processed.push_back(msg);
    break;
    default:
      break;
    }
  }

protected:

  std::vector<protocol::msg_ptr>  _messages_processed;
  std::atomic_bool                _has_started{ false };
  std::atomic_bool                _has_stopped{ false };

};

class priority_msg_worker_t :
    public base_worker_t<std::priority_queue<
              protocol::msg_ptr,
              std::vector<protocol::msg_ptr>,
              advanced::concurrency::pointer_comparison<protocol::message_base_t>>>
{
public:
  using base_class = base_worker_t<std::priority_queue<protocol::msg_ptr>>;
  using message_t  = base_class::message_t;
};

class simple_msg_worker_t : public base_worker_t<std::queue<protocol::msg_ptr>>
{
public:
  using base_class = base_worker_t<std::queue<protocol::msg_ptr>>;
  using message_t  = base_class::message_t;
};

class reentrant_msg_worker_t :
    public advanced::concurrency::message_queue_t<std::queue<protocol::msg_ptr>> {
public:


  using base_class = base_worker_t<std::queue<protocol::msg_ptr>>;
  using message_t  = base_class::message_t;

  // message_queue_t interface
protected:

  size_t _counts{ 0 };

  virtual void
  on_new_message(const message_t &msg) override {
    (void)size();
    (void)empty();
    (void)is_running();
    // send the same message to myself:
    send(*this, msg);
    if (_counts++ >= 10) {
      discard_messages(); // end the loop here
    }

    QVERIFY_EXCEPTION_THROWN(run(), std::runtime_error);
  }
};

}
}
