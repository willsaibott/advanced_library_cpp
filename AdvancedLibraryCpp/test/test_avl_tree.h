#pragma once

#include <QObject>
#include <QTest>
#include "structures/binary_tree.h"

namespace test {
namespace structures {

class moc_avl_node {
public:
  moc_avl_node() = default;
  moc_avl_node(const std::string& n, int v) : id{ v }, name{ n } { }
  int         id{ 0 };
  std::string name;

  inline bool
  operator<(const moc_avl_node& other) const {
    bool ok{ name != other.name };
    return ok ? name < other.name : id < other.id;
  }

  inline bool
  operator<=(const moc_avl_node& other) const {
    return name <= other.name;
  }

  inline bool
  operator>(const moc_avl_node& other) const {
    bool ok{ name != other.name };
    return ok ? name > other.name : id > other.id;
  }

  inline bool
  operator>=(const moc_avl_node& other) const {
    return name >= other.name;
  }

  inline bool
  operator==(const moc_avl_node& other) const {
    return name == other.name && id == other.id;
  }
};

}
}

class TestAVLTree : public QObject
{
  Q_OBJECT
public:
  explicit TestAVLTree(QObject *parent = nullptr);

  template<class T>
  size_t
  test_node(const advanced::structures::binary_node_t<T>& node) {
    size_t counter{ 1 };
    if (node.has_left()) {
      if (node.left().get() > node.get()) {
        return 1000000;
      }
      counter += test_node<T>(node.left());
    }
    if (node.has_right()) {
      if (node.right().get() < node.get()) {
        return 1000000;
      }
      counter += test_node<T>(node.right());
    }
    return counter;
  }

  advanced::structures::avl_tree_t<int> get_test_tree();

private slots:

  void test_has_left();
  void test_has_right();
  void test_children_count();
  void test_initializing_root_on_constructor();
  void test_children_nodes_should_throw_null_node_exception_when_they_are_null();
  void test_has_parent();
  void test_parent_getter();
  void test_null_root_should_throw_exception();
  void test_move_copy();
  void test_rotation();
  void test_insert();
  void test_height();
  void test_left_and_right_most();
  void test_remove();
  void test_search_tree();
  void test_allow_multiple_equal_entires();
  void test_remove_one_element();
  void test_remove_all_elements();
  void test_clear();
  void test_custom_structure();
  void test_as_pointer();
  void test_trasversal_in_order();
  void test_trasversal_pre_order();
  void test_trasversal_pos_order();
  void test_max_allowed_children();
  void test_bfs();
  void test_dfs();
  void test_search_value();
};

