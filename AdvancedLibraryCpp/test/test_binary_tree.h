#pragma once

#include <QObject>
#include <QTest>

#include "../structures/binary_tree.h"


namespace test {
namespace structures {

class mock_node_value {
public:

  mock_node_value() = default;
  mock_node_value(int v, float v2, const std::string& n) :
    int_v{ v }, float_v{ v2 }, name{ n } { }
  int         int_v{ 0 };
  float       float_v{ 23};
  std::string name;
};

}
}

class TestBinaryTree : public QObject
{
  Q_OBJECT
public:
  explicit TestBinaryTree(QObject *parent = nullptr);

private slots:

  void test_add_left();
  void test_add_right();
  void test_left_getter();
  void test_right_getter();
  void test_has_left();
  void test_has_right();
  void test_children_count();
  void test_custom_node_constructor();
  void test_delete_child();
  void test_swap();
  void test_get_root();
  void test_initializing_root_on_constructor();
  void test_children_nodes_should_throw_null_node_exception_when_they_are_null();
};

