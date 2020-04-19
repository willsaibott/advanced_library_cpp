#pragma once
#include <QObject>
#include <QtTest/QtTest>
#include <algorithm>
#include <string>



class TestEnum : public QObject
{

  Q_OBJECT
public:
  explicit TestEnum(QObject *parent = nullptr);

  enum class dummy {
    first,
    second,
    third,
    MAX
  };

signals:

private slots:

  void test_to_string();

  void test_from_string();

  void test_from_string_custom_max_limit();

  void test_from_string_custom_min_limit();
};


namespace std {

  inline string
  to_string(const TestEnum::dummy& d) {
    static std::string names[static_cast<size_t>(TestEnum::dummy::MAX)+1ul] = {
      "first",
      "second",
      "third",
       "MAX"
      };
    return names[std::min(static_cast<size_t>(d),
                          static_cast<size_t>(TestEnum::dummy::MAX))];
  }
}


