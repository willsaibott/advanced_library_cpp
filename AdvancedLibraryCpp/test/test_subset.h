#pragma once

#include <QObject>
#include <QTest>

class TestSubset : public QObject
{
  Q_OBJECT
public:
  explicit TestSubset(QObject *parent = nullptr);

signals:

private slots:

  void test_subset_of_vector_with_sum_zero();
  void test_subset_of_vector_with_sum_non_zero();
  void test_subset_of_array_with_sum_zero();
  void test_subset_of_array_with_sum_non_zero();

};

