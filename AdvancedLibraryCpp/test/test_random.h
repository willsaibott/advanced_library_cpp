#pragma once

#include <QObject>
#include <QTest>

class TestRandom : public QObject
{
  Q_OBJECT
public:
  explicit TestRandom(QObject *parent = nullptr);

signals:

private slots:
  void test_int8_random();
  void test_int16_random();
  void test_int32_random();
  void test_int64_random();
  void test_double_random();
  void test_char_random();
  void test_string_random();
};

