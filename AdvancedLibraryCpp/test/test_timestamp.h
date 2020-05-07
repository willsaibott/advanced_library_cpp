#pragma once

#include <QObject>
#include <QTest>
#include "../tools/timestamp.h"

class TestTimestamp : public QObject
{
  Q_OBJECT
public:
  explicit TestTimestamp(QObject *parent = nullptr);

private slots:

  void test_difference();
  void test_elapsed();
};

