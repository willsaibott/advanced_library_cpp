#pragma once
#include <memory>

namespace test {
namespace protocol {

enum class
message_type {
  READ_A = 0,
  READ_B,
  WRITE_A,
  WRITE_B,
  INVALID
};

enum class
priority_type {
  LOW = 0,
  NORMAL,
  HIGH
};

struct message_base_t {
  message_type  _type;
  priority_type _priority;

  message_base_t()
    : message_base_t{ message_type::INVALID, priority_type::LOW }
  {}

  message_base_t(message_type _type, priority_type priority)
    : _type{ _type }, _priority{ priority }
  { }

  virtual ~message_base_t() {}
  virtual message_type
  type() const {
    return _type;
  }

  virtual priority_type
  priority() const {
    return _priority;
  }

  /**
   * Used by std::less ...
   */
  inline bool
  operator<(const message_base_t& other) const {
    auto diff  = static_cast<int>(priority()) - static_cast<int>(other.priority());
    auto diff2 = static_cast<int>(type()) - static_cast<int>(other.type());
    return diff ? diff < 0 :  diff2 < 0;
  }

  inline bool
  operator>(const message_base_t& other) const {
    auto diff  = static_cast<int>(priority()) - static_cast<int>(other.priority());
    auto diff2 = static_cast<int>(type()) - static_cast<int>(other.type());
    return diff ? diff > 0 :  diff2 > 0;
  }

  inline bool
  operator<=(const message_base_t& other) const {
    auto diff  = static_cast<int>(priority()) - static_cast<int>(other.priority());
    auto diff2 = static_cast<int>(type()) - static_cast<int>(other.type());
    return diff ? diff < 0 :  diff2 <= 0;
  }

  inline bool
  operator>=(const message_base_t& other) const {
    auto diff  = static_cast<int>(priority()) - static_cast<int>(other.priority());
    auto diff2 = static_cast<int>(type()) - static_cast<int>(other.type());
    return diff ? diff > 0 :  diff2 >= 0;
  }
};

struct msg_readA : message_base_t {
  msg_readA(priority_type priority = priority_type::NORMAL)
    : message_base_t (message_type::READ_A, priority) {
  }
};

struct msg_readB : message_base_t {
  msg_readB(priority_type priority = priority_type::NORMAL)
    : message_base_t (message_type::READ_B, priority) {
  }
};

struct msg_writeA : message_base_t {
  msg_writeA(priority_type priority = priority_type::NORMAL)
    : message_base_t (message_type::WRITE_A, priority) {
  }
};

struct msg_writeB : message_base_t {
  msg_writeB(priority_type priority = priority_type::NORMAL)
    : message_base_t (message_type::WRITE_B, priority) {
  }
};

using msg_ptr = std::shared_ptr<protocol::message_base_t>;

}
}
