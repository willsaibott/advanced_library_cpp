#include <mutex>
#include <shared_mutex>
#include <thread>
#include <functional>
#include <QMutex>
#include <QMutexLocker>
#include <QReadWriteLock>
#include "test_lockable.h"
#include "../concurrency/safe.h"

using namespace advanced;
using namespace advanced::concurrency;

TestLockable::
TestLockable(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestLockable");
}

void TestLockable::
test_copy_constructible() {
  static const std::vector<std::string> expected_values = {
    "abc", "cba", "def", "dhtyj", "wergrw`@", "\\]/c@.,[@,", "",
    "togs231", "*/*45", "coiwbecb",
    "eojponrv", "3876798", "  ", ")~", "cecvr",
    ".rwevrvbeowibvoi3wbhoi4bhgiou34hofi43hifgho3l4hiogho34"
  };

  lockable_t<std::vector<std::string>> strings{ expected_values };
  for (size_t ii = 0; ii < expected_values.size(); ii++) {
    QCOMPARE(strings[ii], expected_values[ii]);
  }
}

void TestLockable::
test_move_constructible() {
  static const std::vector<std::string> expected_values = {
    "abc", "cba", "def", "dhtyj", "wergrw`@", "\\]/c@.,[@,", "",
    "togs231", "*/*45", "coiwbecb",
    "eojponrv", "3876798", "  ", ")~", "cecvr",
    ".rwevrvbeowibvoi3wbhoi4bhgiou34hofi43hifgho3l4hiogho34"
  };

  lockable_t<std::vector<std::string>> strings_old{ expected_values };
  lockable_t<std::vector<std::string>> strings{ std::move(strings_old) };
  for (size_t ii = 0; ii < expected_values.size(); ii++) {
    QCOMPARE(strings[ii], expected_values[ii]);
  }
  QVERIFY(strings_old.empty());
}

void TestLockable::
test_copy() {
  static const std::vector<std::string> expected_values = {
    "abc", "cba", "def", "dhtyj", "wergrw`@", "\\]/c@.,[@,", "",
    "togs231", "*/*45", "coiwbecb",
    "eojponrv", "3876798", "  ", ")~", "cecvr",
    ".rwevrvbeowibvoi3wbhoi4bhgiou34hofi43hifgho3l4hiogho34"
  };

  lockable_t<std::vector<std::string>> strings;
  strings = expected_values;
  for (size_t ii = 0; ii < expected_values.size(); ii++) {
    QCOMPARE(strings[ii], expected_values[ii]);
  }
}

void TestLockable::
test_move() {
  static const std::vector<std::string> expected_values = {
    "abc", "cba", "def", "dhtyj", "wergrw`@", "\\]/c@.,[@,", "",
    "togs231", "*/*45", "coiwbecb",
    "eojponrv", "3876798", "  ", ")~", "cecvr",
    ".rwevrvbeowibvoi3wbhoi4bhgiou34hofi43hifgho3l4hiogho34"
  };

  lockable_t<std::vector<std::string>> strings_old{ expected_values };
  lockable_t<std::vector<std::string>> strings = std::move(strings_old);
  for (size_t ii = 0; ii < expected_values.size(); ii++) {
    QCOMPARE(strings[ii], expected_values[ii]);
  }
  QVERIFY(strings_old.empty());
}

void TestLockable::
concurrency_using_std_locks() {
  static const std::vector<std::string> expected_values = {
    "abc", "cba", "def", "dhtyj", "wergrw`@", "\\]/c@.,[@,", "",
    "togs231", "*/*45", "coiwbecb",
    "eojponrv", "3876798", "  ", ")~", "cecvr",
    ".rwevrvbeowibvoi3wbhoi4bhgiou34hofi43hifgho3l4hiogho34"
  };

  lockable_t<std::vector<std::string>> strings{ expected_values };

  auto pred = [&strings]() {
    for (auto& str : strings) {
      for(size_t ii = 0; ii < 100; ii++) {
        std::unique_lock<std::mutex> lock(strings);
        // reverse the string and rowback:
        std::reverse(str.begin(), str.end());
        std::reverse(str.begin(), str.end());

        lock.unlock();
        lock.lock();

        // change a char and rollback:
        auto tmp = str[0];
        str[0] = 'z';
        str[0] = tmp;
      }
    }
  };
  std::vector<std::thread> threads(std::thread::hardware_concurrency());

  for (auto& t : threads) {
    t = std::thread(pred);
  }

  for (auto& t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  for (size_t ii = 0; ii < expected_values.size(); ii++) {
    QCOMPARE(strings[ii], expected_values[ii]);
  }
}

void TestLockable::
concurrency_using_std_shared_locks() {
  const std::vector<std::string> expected_values = {
    "abc", "cba", "def", "dhtyj", "wergrw`@", "\\]/c@.,[@,", "",
    "togs231", "*/*45", "coiwbecb",
    "eojponrv", "3876798", "  ", ")~", "cecvr",
    ".rwevrvbeowibvoi3wbhoi4bhgiou34hofi43hifgho3l4hiogho34"
  };

  lockable_t<std::vector<std::string>, std::shared_timed_mutex>
      strings{ expected_values };

  auto pred = [&strings, &expected_values]() {
    for (size_t ii = 0; ii < expected_values.size(); ii++) {
      auto& str{ strings[ii] };
      for(size_t jj = 0; jj < 100; jj++) {
        // write scope
        {
          std::lock_guard<std::shared_timed_mutex> lock(strings);
          // reverse the string and rowback:
          std::reverse(str.begin(), str.end());
          std::reverse(str.begin(), str.end());
        }

        // read scope
        std::shared_lock<std::shared_timed_mutex> lock(strings);
        QCOMPARE(str, expected_values[ii]);
      }
    }
  };
  std::vector<std::thread> threads(std::thread::hardware_concurrency());

  for (auto& t : threads) {
    t = std::thread(pred);
  }

  for (auto& t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}

void TestLockable::
concurrency_using_std_scoped_locks() {
  const std::vector<std::string> expected_values = {
    "abc", "cba", "def", "dhtyj", "wergrw`@", "\\]/c@.,[@,", "",
    "togs231", "*/*45", "coiwbecb",
    "eojponrv", "3876798", "  ", ")~", "cecvr",
    ".rwevrvbeowibvoi3wbhoi4bhgiou34hofi43hifgho3l4hiogho34"
  };

  lockable_t<std::vector<std::string>>
      strings(expected_values.begin(), expected_values.begin() + 4);
  lockable_t<std::vector<std::string>>
      strings2(expected_values.begin() + 4, expected_values.end());

  std::vector<std::thread> threads(std::thread::hardware_concurrency());
  auto pred = [&strings, &strings2]() {
    for(size_t jj = 0; jj < 100; jj++) {
      // locking two resources simultaneously:
      std::lock(strings, strings2);

      // Adding dummy trash string to comprove the concepts:
      strings2.push_back("######");
      strings.insert(strings.end(), strings2.begin(), strings2.end());
      // removing dummy trash string:
      strings.pop_back();
      strings2.clear();

      strings2.unlock();
      strings.unlock();
    }
  };

  for (auto& t : threads) {
    t = std::thread(pred);
  }

  for (auto& t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  for (size_t ii = 0; ii < expected_values.size(); ii++) {
    QCOMPARE(strings[ii], expected_values[ii]);
  }
  QVERIFY(strings2.empty());
}

void TestLockable::
concurrency_using_qt_locks() {
  static const std::vector<std::string> expected_values = {
    "abc", "cba", "def", "dhtyj", "wergrw`@", "\\]/c@.,[@,", "",
    "togs231", "*/*45", "coiwbecb",
    "eojponrv", "3876798", "  ", ")~", "cecvr",
    ".rwevrvbeowibvoi3wbhoi4bhgiou34hofi43hifgho3l4hiogho34"
  };

  lockable_t<std::vector<std::string>, QMutex>
      strings{ expected_values };

  auto pred = [&strings]() {
    for (auto& str : strings) {
      for(size_t ii = 0; ii < 100; ii++) {
        QMutexLocker lock{ &strings };
        // reverse the string and rowback:
        std::reverse(str.begin(), str.end());
        std::reverse(str.begin(), str.end());

        lock.unlock();
        lock.relock();

        // change a char and rollback:
        auto tmp = str[0];
        str[0] = 'z';
        str[0] = tmp;
      }
    }
  };
  std::vector<std::thread> threads(std::thread::hardware_concurrency());

  for (auto& t : threads) {
    t = std::thread(pred);
  }

  for (auto& t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }

  for (size_t ii = 0; ii < expected_values.size(); ii++) {
    QCOMPARE(strings[ii], expected_values[ii]);
  }
}

void TestLockable::
concurrency_using_qt_shared_locks() {
  const std::vector<std::string> expected_values = {
    "abc", "cba", "def", "dhtyj", "wergrw`@", "\\]/c@.,[@,", "",
    "togs231", "*/*45", "coiwbecb",
    "eojponrv", "3876798", "  ", ")~", "cecvr",
    ".rwevrvbeowibvoi3wbhoi4bhgiou34hofi43hifgho3l4hiogho34"
  };

  lockable_t<std::vector<std::string>, QReadWriteLock>
      strings{ expected_values };

  auto pred = [&strings, &expected_values]() {
    for (size_t ii = 0; ii < expected_values.size(); ii++) {
      auto& str{ strings[ii] };
      for(size_t jj = 0; jj < 100; jj++) {
        // write scope:
        strings.lockForWrite();
        // reverse the string and rowback:
        std::reverse(str.begin(), str.end());
        std::reverse(str.begin(), str.end());
        strings.unlock();

        // read scope:
        strings.lockForRead();
        QCOMPARE(str, expected_values[ii]);
        strings.unlock();
      }
    }
  };
  std::vector<std::thread> threads(std::thread::hardware_concurrency());

  for (auto& t : threads) {
    t = std::thread(pred);
  }

  for (auto& t : threads) {
    if (t.joinable()) {
      t.join();
    }
  }
}

//QTEST_MAIN(TestLockable)
