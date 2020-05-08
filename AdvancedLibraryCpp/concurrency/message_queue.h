#pragma once
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <chrono>
#include <memory>
#include "safe.h"
#include "thread.h"

namespace advanced {
namespace concurrency {

/** @test TestMessageQueue in test/test_message_queue(.h|.cpp) */

/**
 * Basic simple Thread that reacts after receives a message.
 * 1. You just need to override the pure virtual method "on_new_message"
 * to implement how your message queue will process new messages.
 * 2. If you need to do some kind of pre-initializations, override the "on_start"
 * method.
 * 3. If you need to do some kind of cleanup after stopping the message_queue,
 * override the "on_stop" method.
 *
 * All methods are reentrant
 *
 * Note:
 * If you override on_start and on_stop functions, do not forget to call
 * the "safe_delete" method on destructor
 * Also, the on_start and on_stop methods won't be executed if the destructor
 * object of message_queue_t is called before the message_queue_t has been
 * stopped
 */
template<class queue>
class message_queue_t : public thread_t {
  public:
  using message_t = typename queue::value_type;

  /**
   * Stops the queue, the thread_t destructor will join to the child
   * std::thread instance
   */
  virtual
  ~message_queue_t() override {
    safe_delete();
    clear();
  }

  /**
   * Call it in your destructor if you override the on_start and/or on_stop
   * methods
   */
  void
  safe_delete() {
    stop();
    join();
  }

  /**
   * It sends a message to a message queue
   * @param destination    message_queue to be sent
   * @param msg            message object
   * Note: It'll lock the internal queue of the destination object.
   */
  inline static void
  send(message_queue_t& destination, const message_t& msg) {
      destination.receive_message(msg);
  }

  /**
   * It stores the message in a queue of messages
   * @param msg            message object
   * Note: It'll lock the internal queue object.
   */
  void
  receive_message(const message_t& msg) {
    std::unique_lock<std::mutex> lock{ _queue };
    _queue.push(msg);
    lock.unlock();
    _cv.notify_one();
  }

  /**
   * It will flag the message queue that it should stop when all messages are
   * processed
   */
  virtual void
  stop() final {
    _stopped = true;
    _cv.notify_one();
  }

  /**
   * It flags the queue to discard all the current queued messages
   */
  void
  discard_messages() {
    _discard = true;
    _cv.notify_one();
  }

  /**
   * It returns true if the thread already started, and it's ready to process
   * any received messages
   */
  virtual bool
  is_running() const noexcept {
    return _running || thread_t::is_running();
  }

  /**
   * It returns the current queue size
   */
  inline size_t
  size() const {
    std::unique_lock<std::mutex> guard(_queue);
    return _queue.size();
  }

  /**
   * It returns true if the queue is empty
   */
  inline bool
  empty() const {
    std::unique_lock<std::mutex> guard(_queue);
    return _queue.empty();
  }

  protected:

  virtual int
  run() final override {
    auto pred { [this]() { return !_queue.empty(); } };
    std::mutex mtx_to_sleep;

    if (_running) {
      throw std::runtime_error{ "trying to run this thread twice" };
    }

    on_start();
    _running = true;
    while (_running) {
      std::unique_lock<std::mutex> lock{ mtx_to_sleep };
      _cv.wait_for(lock, std::chrono::milliseconds{ 100 }, pred);

      if (_discard) {
        _discard = false;

        // discard messages inside the loop
        std::unique_lock<std::mutex> guard(_queue);
        clear();
      }
      else {
        while (pred() && !_discard) {
          const auto message{ pop() };
          on_new_message(message);
        }
      }
      _running = !_stopped;
    }
    on_stop();
    return EXIT_SUCCESS;
  } // LCOV_EXCL_LINE

  /**
   * Override it if you need to do some initialization
   * Called once before the object starts listening to new messages
   *
   * Note:
   * If you override on_start and/or on_stop functions, do not forget to call
   * the "safe_delete" method on destructor
   */
  virtual void
  on_start() { }

  /**
   * Override it if you need to do some cleanup
   * Called once after the object stops processing messages.
   *
   * Note:
   * If you override on_start and/or on_stop functions, do not forget to call
   * the "safe_delete" method on destructor
   */
  virtual void
  on_stop() { }

  /**
   * Override it to define the behaviour and the processing of new messages
   */
  virtual void
  on_new_message(const message_t& msg) = 0;

  template <typename, typename = void>
  struct has_front : std::false_type {};

  template <typename T>
  struct has_front<T, std::void_t<decltype(&T::front)>> : std::true_type
  {};

  template <typename, typename = void>
  struct has_top : std::false_type {};

  template <typename T>
  struct has_top<T, std::void_t<decltype(&T::top)>> : std::true_type
  {};

  message_t
  pop() {
    std::unique_lock<std::mutex> guard(_queue);
    if constexpr(has_top<queue>::value) {
      auto top = _queue.top(); _queue.pop();
      return std::move(top);
    }
    else {
      auto front = _queue.front(); _queue.pop();
      return std::move(front);
    }
  }

  private:

  /**
   * @brief clear it will clear the entire queue
   */
  void
  clear() {
    while (!_queue.empty()) { _queue.pop(); }
  }

  std::condition_variable       _cv;
  mutable lockable_t<queue>     _queue;
  bool                          _running{ false };
  std::atomic_bool              _stopped{ false };
  std::atomic_bool              _discard{ false };
};

/**
 * Wrapper comparison struct used for std smart pointers to be used
 * when the message is built using smart pointers and the queue used is
 * the heap type std::priority_queue<> that remains the messages ordered by
 * some priority.
 */
template <class T, class transform_t = std::less<T>>
struct pointer_comparison {

  inline bool
  operator()(const std::shared_ptr<T>& p1, const std::shared_ptr<T>& p2) const {
    static const transform_t transform;
    return transform(*p1, *p2);
  }

  inline bool
  operator()(std::shared_ptr<T>* p1, std::shared_ptr<T>* p2) const {
    static const transform_t transform;
    return transform(**p1, **p2);
  }

  inline bool
  operator()(const std::unique_ptr<T>& p1, const std::unique_ptr<T>& p2) const {
    static const transform_t transform;
    return transform(*p1, *p2);
  }
};

}
}

