#pragma once
#include <deque>

namespace advanced {
namespace structures {

/** @test Todo */

template<typename command>
class command_manager_t {
  public:

  ~command_manager_t() {
    dispose(_first);
    dispose(_second);
  }

  command*
  undo() {
    command* ptr{ nullptr };
    if (_first.size()) {
      ptr = _second.back(); _second.pop_back();
      _second.push_back(ptr);
      clip();
    }
    return ptr;
  }

  command*
  redo() {
    command* ptr{ nullptr };
    if (_second.size()) {
      ptr = _first.back(); _first.pop_back();
      _first.push_back(ptr);
      clip();
    }
    return ptr;
  }

  command*
  store(command* new_command) {
    if (_second.size()) {
      dispose();
    }
    _first.push_back(new_command);
    clip();
  }

  size_t
  dispose() {
    return dispose(_second);
  }

  private:

  size_t
  dispose(std::deque<command*>& list) {
    size_t items{ 0 };
    while (list.size()) {
      auto ptr = list.back(); list.pop_back();
      delete ptr;
      items++;
    }
    return items;
  }

  void
  clip() {
    while (_first.size() > _max_elements) {
      auto ptr = _first.front(); _first.pop_front();
      delete ptr;
    }
    while (_second.size() > _max_elements) {
      auto ptr = _second.front(); _second.pop_front();
      delete ptr;
    }
  }

  size_t               _max_elements{ 1024 };
  std::deque<command*> _first;
  std::deque<command*> _second;
};
}
}
