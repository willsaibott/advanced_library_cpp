#pragma once

#include <QObject>
#include <QTest>

class TestMissingElements : public QObject
{
  Q_OBJECT
public:
  explicit TestMissingElements(QObject *parent = nullptr);

signals:


private slots:
  void test_find_missing_element_of_sequence_1_N();
  void test_find_missing_element_of_sequence_not_starting_in_1();
  void test_find_missing_element_of_arithmetic_progression();
  void test_find_missing_element_of_int_geometric_progression();
  void test_find_missing_element_of_double_geometric_progression();
  void test_find_missing_element_of_unsorted_values();
  void test_find_not_common_element();
  void test_find_not_repeated_element();
  void test_non_valid_sequences();
  void test_find_missing_on_first();
  void test_find_missing_on_invalid_sequences();
};

