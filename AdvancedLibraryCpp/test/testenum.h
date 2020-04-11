#pragma once
#include <QObject>
#include <QtTest/QtTest>
#include <algorithm>
#include <string>

enum class dummy {
  first,
  second,
  third,
  MAX
};

namespace std {

  inline string
  to_string(const dummy& d) {
    static std::string names[static_cast<size_t>(dummy::MAX)+1ul] = {
      "first",
      "second",
      "third",
       "MAX"
      };
    return names[std::min(static_cast<size_t>(d),
                          static_cast<size_t>(dummy::MAX))];
  }
}

class TestEnum : public QObject
{
  Q_OBJECT
public:
  explicit TestEnum(QObject *parent = nullptr);

signals:

private slots:

  void test_to_string();

  void test_from_string();

  void test_from_string_custom_max_limit();

  void test_from_string_custom_min_limit();
};



