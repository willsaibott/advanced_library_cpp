#pragma once

#include <QObject>
#include <QTest>

#include "../structures/tree.h"

class TestTree : public QObject
{
  Q_OBJECT
public:
  explicit TestTree(QObject *parent = nullptr);

  advanced::structures::tree_t<std::string,
                               advanced::structures::tree_node_t<std::string, 4>>
  get_test_tree();

private slots:

  void test_add_child();
  void test_delete_child();
  void test_get_child();
  void test_children_count();
  void test_has_child();
  void test_trasversal_in_order();
  void test_trasversal_pre_order();
  void test_trasversal_pos_order();
  void test_bfs();
  void test_dfs();
  void test_search_value();
  void test_max_allowed_children();
  void test_insert_at_first_null();
  void test_insert_at();
  void test_replace_at();
  void test_extract_child();
  void test_clear();
  void test_find();
  void test_for_range();
  void test_first();
  void test_last();
};
