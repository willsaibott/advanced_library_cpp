#include "test_tree.h"

using namespace advanced::structures;

TestTree::
TestTree(QObject *parent) : QObject{ parent } {
  QObject::setObjectName("TestTree");
}

/**
 * @brief TestTree::get_test_tree
 * format:
 *                                    7
 *  null                   4              null            10
 *          1        2         5   6              8    9       11    13
 *            nll nll  3 nll                                       12
 * @return
 */
tree_t<std::string, advanced::structures::tree_node_t<std::string, 4>>
  TestTree::
get_test_tree() {
  tree_t<std::string, advanced::structures::tree_node_t<std::string, 4>> tree("7");
  tree.root().insert_at(1, "4");
  tree.root().insert_at(3, "10");
  tree.root().child(1).add_child("1");
  tree.root().child(1).add_child("2");
  tree.root().child(1).add_child("5");
  tree.root().child(1).add_child("6");
  tree.root().child(1).child(1).insert_at(2, "3");
  tree.root().child(3).add_child("8");
  tree.root().child(3).add_child("9");
  tree.root().child(3).add_child("11");
  tree.root().child(3).add_child("13");
  tree.root().child(3).child(3).add_child("12");
  return tree;
}

void TestTree::
test_add_child() {
  tree_t<int, tree_node_t<int, 4>> quad_tree;
  QVERIFY(!quad_tree.has_root());
  quad_tree.add_root(10);
  QVERIFY(quad_tree.has_root());
  auto& root { quad_tree.root() };
  QVERIFY(root.add_child(8));
  QVERIFY(root.add_child(7));
  QVERIFY(root.add_child(6));
  QVERIFY(root.add_child(5));
  QVERIFY(!root.add_child(4));
}

void TestTree::
test_delete_child() {
  tree_t<int, tree_node_t<int, 4>> quad_tree;
  QVERIFY(!quad_tree.has_root());
  quad_tree.add_root(10);
  QVERIFY(quad_tree.has_root());
  auto& root { quad_tree.root() };
  QVERIFY(root.add_child(8));
  QVERIFY(root.add_child(7));
  QVERIFY(root.add_child(6));
  QVERIFY(root.add_child(5));
  QCOMPARE(root.children_count(), 4u);

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 3u);
  QCOMPARE(root.child(0).get(), 7);

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 2u);
  QCOMPARE(root.child(0).get(), 6);

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 1u);
  QCOMPARE(root.child(0).get(), 5);

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 0u);
  QVERIFY(!root.has_child(0));
}

void TestTree::
test_get_child() {
  tree_t<int, tree_node_t<int, 4>> quad_tree(10);
  auto& root { quad_tree.root() };
  QVERIFY(root.add_child(8));
  QCOMPARE(root.child(0).get(), 8);
  QVERIFY(root.add_child(7));
  QCOMPARE(root.child(1).get(), 7);
  QVERIFY(root.add_child(6));
  QCOMPARE(root.child(2).get(), 6);
  QVERIFY(root.add_child(5));
  QCOMPARE(root.child(3).get(), 5);

  const auto& ref{ root };
  QCOMPARE(ref.child(0).get(), 8);
  QCOMPARE(ref.child(1).get(), 7);
  QCOMPARE(ref.child(2).get(), 6);
  QCOMPARE(ref.child(3).get(), 5);

  auto quad_str_tree = get_test_tree();
  QVERIFY_EXCEPTION_THROWN(quad_str_tree.root().child(0), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(quad_str_tree.root().child(2), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(quad_str_tree.root().child(5), std::out_of_range);
  QVERIFY_EXCEPTION_THROWN(quad_str_tree.root().child(6), std::out_of_range);
  QVERIFY_EXCEPTION_THROWN(quad_str_tree.root().child(static_cast<size_t>(-1)),
                           std::out_of_range);

  const auto& quad_str_ref{ quad_str_tree.root() };
  QVERIFY_EXCEPTION_THROWN(quad_str_ref.child(0), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(quad_str_ref.child(2), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(quad_str_ref.child(5), std::out_of_range);
  QVERIFY_EXCEPTION_THROWN(quad_str_ref.child(6), std::out_of_range);
  QVERIFY_EXCEPTION_THROWN(quad_str_ref.child(static_cast<size_t>(-1)),
                           std::out_of_range);
}

void TestTree::
test_children_count() {
  tree_t<int, tree_node_t<int, 4>> quad_tree(10);
  auto& root { quad_tree.root() };
  QVERIFY(root.add_child(8));
  QVERIFY(root.add_child(7));
  QVERIFY(root.add_child(6));
  QVERIFY(root.add_child(5));
  QCOMPARE(root.children_count(), 4u);

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 3u);
  QCOMPARE(root.child(0).get(), 7);

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 2u);
  QCOMPARE(root.child(0).get(), 6);

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 1u);
  QCOMPARE(root.child(0).get(), 5);

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 0u);
  QVERIFY(!root.has_child(0));
}

void TestTree::
test_has_child() {
  tree_t<int, tree_node_t<int, 4>> quad_tree(10);
  auto& root { quad_tree.root() };
  QVERIFY(root.add_child(8));
  QVERIFY(root.add_child(7));
  QVERIFY(root.add_child(6));
  QVERIFY(root.add_child(5));
  QCOMPARE(root.children_count(), 4u);
  QVERIFY(root.has_child(0));
  QVERIFY(root.has_child(1));
  QVERIFY(root.has_child(2));
  QVERIFY(root.has_child(3));
  QVERIFY(!root.has_child(4));
  QVERIFY(!root.has_child(5));

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 3u);
  QCOMPARE(root.child(0).get(), 7);
  QVERIFY(root.has_child(0));
  QVERIFY(root.has_child(1));
  QVERIFY(root.has_child(2));
  QVERIFY(!root.has_child(3));
  QVERIFY(!root.has_child(4));
  QVERIFY(!root.has_child(5));

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 2u);
  QCOMPARE(root.child(0).get(), 6);
  QVERIFY(root.has_child(0));
  QVERIFY(root.has_child(1));
  QVERIFY(!root.has_child(2));
  QVERIFY(!root.has_child(3));
  QVERIFY(!root.has_child(4));
  QVERIFY(!root.has_child(5));

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 1u);
  QCOMPARE(root.child(0).get(), 5);
  QVERIFY(root.has_child(0));
  QVERIFY(!root.has_child(1));
  QVERIFY(!root.has_child(2));
  QVERIFY(!root.has_child(3));
  QVERIFY(!root.has_child(4));
  QVERIFY(!root.has_child(5));

  QVERIFY(root.delete_child(0));
  QCOMPARE(root.children_count(), 0u);
  QVERIFY(!root.has_child(0));
  QVERIFY(!root.has_child(1));
  QVERIFY(!root.has_child(2));
  QVERIFY(!root.has_child(3));
  QVERIFY(!root.has_child(4));
  QVERIFY(!root.has_child(5));
}

void TestTree::
test_trasversal_in_order() {
  auto quad_tree = get_test_tree();
  const std::vector<std::string> expected {
    "7", "1", "4", "2", "2", "3", "4", "5", "4", "6", "7", // left
    "7", "8", "10", "9", "10", "11", "10", "12", "13" // right
  };
  std::vector<std::string> visited;

  size_t total = quad_tree.in_order(
                  [&visited](auto& node) {
                    visited.push_back(node.get());
                    return false;
                  });

  QCOMPARE(total,   13u);
  QCOMPARE(visited, expected);

  const auto& tree_ref{ quad_tree };
  visited.clear();
  total = tree_ref.in_order(
            [&visited](const auto& node) {
              visited.push_back(node.get());
              return false;
            });
  QCOMPARE(total,   13u);
  QCOMPARE(visited, expected);
}

void TestTree::
test_trasversal_pre_order() {
  auto quad_tree = get_test_tree();
  const std::vector<std::string> expected {
    "7", "4", "1", "2", "3", "5", "6", "10", "8", "9", "11", "13", "12"
  };
  std::vector<std::string> visited;

  size_t total = quad_tree.pre_order(
                  [&visited](auto& node) {
                    visited.push_back(node.get());
                    return false;
                  });

  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);

  const auto& tree_ref{ quad_tree };
  visited.clear();
  total = tree_ref.pre_order(
            [&visited](const auto& node) {
              visited.push_back(node.get());
              return false;
            });
  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);
}

void TestTree::
test_trasversal_pos_order() {
  auto quad_tree = get_test_tree();
  const std::vector<std::string> expected {
    "1", "3", "2", "5", "6", "4", "8", "9", "11", "12", "13", "10", "7"
  };
  std::vector<std::string> visited;

  size_t total = quad_tree.pos_order(
                  [&visited](auto& node) {
                    visited.push_back(node.get());
                    return false;
                  });

  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);

  const auto& tree_ref{ quad_tree };
  visited.clear();
  total = tree_ref.pos_order(
            [&visited](const auto& node) {
              visited.push_back(node.get());
              return false;
            });
  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);
}

void TestTree::
test_bfs() {
  auto quad_tree = get_test_tree();
  const std::vector<std::string> expected {
    "7",
    "4", "10",
    "1", "2", "5", "6",   "8", "9", "11", "13",
           "3",                        "12"
  };
  std::vector<std::string> visited;

  size_t total = quad_tree.breadth_first_search(
                  [&visited](auto& node) {
                    visited.push_back(node.get());
                    return false;
                  });

  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);

  const auto& tree_ref{ quad_tree };
  visited.clear();
  total = tree_ref.breadth_first_search(
            [&visited](const auto& node) {
              visited.push_back(node.get());
              return false;
            });
  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);
}

void TestTree::
test_dfs() {
  auto quad_tree = get_test_tree();
  const std::vector<std::string> expected {
    "7", "4", "1", "2", "3", "5", "6", "10", "8", "9", "11", "13", "12"
  };
  std::vector<std::string> visited;

  size_t total = quad_tree.depth_first_search(
                  [&visited](auto& node) {
                    visited.push_back(node.get());
                    return false;
                  });

  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);

  const auto& tree_ref{ quad_tree };
  visited.clear();
  total = tree_ref.depth_first_search(
            [&visited](const auto& node) {
              visited.push_back(node.get());
              return false;
            });
  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);
}

void TestTree::
test_search_value() {
  auto tree = get_test_tree();
  size_t steps{ 0u };

  steps = tree.in_order([](const auto& node) { return *node == "2"; });
  QCOMPARE(steps, 4u);
  steps = tree.pre_order([](const auto& node) { return *node == "13"; });
  QCOMPARE(steps, 12u);
  steps = tree.pos_order([](const auto& node) { return *node == "10"; });
  QCOMPARE(steps, 13u);
  steps = tree.breadth_first_search([](const auto& node) { return *node == "8"; });
  QCOMPARE(steps, 8u);
  steps = tree.depth_first_search([](const auto& node) { return *node == "8"; });
  QCOMPARE(steps, 9u);
}

void TestTree::
test_max_allowed_children() {
  tree_t<std::string, tree_node_t<std::string, 4u>> quad_tree = get_test_tree();
  size_t total = quad_tree.pre_order(
                  [](auto& node) {
                    return node.max_allowed_children() != 4u;
                  });
  QCOMPARE(total, 13u); // all nodes has the same max allowed children
}

void TestTree::
test_insert_at_first_null() {
  auto quad_tree = get_test_tree();
  auto& root { quad_tree.root() };
  QCOMPARE(root.children_count(), 2u);

  QVERIFY(root.insert_at_first_null("77"));
  QVERIFY(root.has_child(0));
  QCOMPARE(root.child(0).get(), "77");
  QCOMPARE(root.children_count(), 3u);
  QCOMPARE(root.child(0).parent().get(), "7");

  QVERIFY(root.insert_at_first_null("777"));
  QVERIFY(root.has_child(2));
  QCOMPARE(root.children_count(), 4u);
  QCOMPARE(root.child(2).get(), "777");
  QCOMPARE(root.child(2).parent().get(), "7");

  QVERIFY(!root.insert_at_first_null("777"));

  auto& first{ root.child(0) };
  QVERIFY(first.insert_at_first_null("42"));
  QCOMPARE(first.children_count(), 1u);
  QCOMPARE(first.child(0).get(), "42");
  QVERIFY(first.child(0).has_parent());
  QCOMPARE(first.child(0).parent().get(), "77");
}

void TestTree::
test_insert_at() {
  auto quad_tree = get_test_tree();
  auto& root { quad_tree.root() };
  QCOMPARE(root.children_count(), 2u);
  QVERIFY(!root.insert_at(1, "777"));
  QVERIFY(!root.insert_at(3, "777"));

  QVERIFY(root.insert_at(0, "77"));
  QVERIFY(root.has_child(0));
  QCOMPARE(root.child(0).get(), "77");
  QCOMPARE(root.children_count(), 3u);
  QCOMPARE(root.child(0).parent().get(), "7");
  QVERIFY(!root.insert_at(0, "777"));
  QVERIFY(!root.insert_at(1, "777"));
  QVERIFY(!root.insert_at(3, "777"));

  QVERIFY(root.insert_at(2, "777"));
  QVERIFY(root.has_child(2));
  QCOMPARE(root.children_count(), 4u);
  QCOMPARE(root.child(2).get(), "777");
  QCOMPARE(root.child(2).parent().get(), "7");
  QVERIFY(!root.insert_at(0, "777"));
  QVERIFY(!root.insert_at(1, "777"));
  QVERIFY(!root.insert_at(2, "777"));
  QVERIFY(!root.insert_at(3, "777"));

  auto& first{ root.child(0) };
  QVERIFY(first.insert_at(0, "42"));
  QCOMPARE(first.children_count(), 1u);
  QCOMPARE(first.child(0).get(), "42");
  QVERIFY(first.child(0).has_parent());
  QCOMPARE(first.child(0).parent().get(), "77");
}

void TestTree::
test_replace_at() {
  auto quad_tree = get_test_tree();
  auto& root { quad_tree.root() };
  QCOMPARE(root.children_count(), 2u);

  std::unique_ptr<tree_node_t<std::string, 4u>> ptr{
    root.replace_at(3, ">5000")
  };

  QVERIFY(ptr != nullptr);
  QCOMPARE(ptr->get(), "10");
  QCOMPARE(ptr->first().get(), "8");
  QCOMPARE(ptr->last().get(),  "13");
  QCOMPARE(ptr->last().first().get(),  "12");
  QCOMPARE(ptr->child(1).get(), "9");
  QCOMPARE(ptr->child(2).get(), "11");
  QVERIFY(root.has_child(3));
  QVERIFY(root.has_child(1));
  QVERIFY(!root.has_child(0));
  QVERIFY(!root.has_child(2));
  QCOMPARE(root.children_count(), 2u);
  QCOMPARE(root.child(3).get(), ">5000");
  QCOMPARE(root.child(3).children_count(), 0u);
  QVERIFY(!root.child(3).has_child(0));
  QVERIFY(!root.child(3).has_child(1));
  QVERIFY(!root.child(3).has_child(2));
  QVERIFY(!root.child(3).has_child(3));

  auto ptr2 = root.replace_at(0, "77");
  QCOMPARE(ptr2, nullptr);
  QVERIFY(root.has_child(3));
  QVERIFY(root.has_child(1));
  QVERIFY(root.has_child(0));
  QVERIFY(!root.has_child(2));
  QCOMPARE(root.children_count(), 3u);
  QCOMPARE(root.first().get(), "77");
  QCOMPARE(root.first().children_count(), 0u);
  QVERIFY(!root.first().has_child(0));
  QVERIFY(!root.first().has_child(1));
  QVERIFY(!root.first().has_child(2));
  QVERIFY(!root.first().has_child(3));

  auto ptr3 = root.first().replace_at(3, "777");
  QCOMPARE(ptr3, nullptr);
  QVERIFY(root.has_child(3));
  QVERIFY(root.has_child(1));
  QVERIFY(root.has_child(0));
  QVERIFY(!root.has_child(2));
  QCOMPARE(root.first().children_count(), 1u);
  QCOMPARE(root.first().first().get(),   "777");
  QCOMPARE(root.first().last().get(),    "777");
  QCOMPARE(root.first().child(3).get(),  "777");
  QCOMPARE(root.first().first().children_count(), 0u);
  QVERIFY(!root.first().first().has_child(0));
  QVERIFY(!root.first().first().has_child(1));
  QVERIFY(!root.first().first().has_child(2));
  QVERIFY(!root.first().first().has_child(3));
  QVERIFY_EXCEPTION_THROWN(root.first().child(0), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(root.first().child(1), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(root.first().child(2), null_node_exception_t);
}

void TestTree::
test_extract_child() {
  auto quad_tree = get_test_tree();
  auto& root { quad_tree.root() };
  QCOMPARE(root.children_count(), 2u);

  std::unique_ptr<tree_node_t<std::string, 4u>> ptr{ root.extract_child(3) };

  QVERIFY(ptr != nullptr);
  QCOMPARE(ptr->get(), "10");
  QCOMPARE(ptr->first().get(), "8");
  QCOMPARE(ptr->last().get(),  "13");
  QCOMPARE(ptr->last().first().get(),  "12");
  QCOMPARE(ptr->child(1).get(), "9");
  QCOMPARE(ptr->child(2).get(), "11");
  QVERIFY(!root.has_child(3));
  QVERIFY(root.has_child(1));
  QVERIFY(!root.has_child(0));
  QVERIFY(!root.has_child(2));
  QCOMPARE(root.children_count(), 1u);

  auto ptr2 = root.extract_child(0);
  QCOMPARE(ptr2, nullptr);
  QVERIFY(!root.has_child(3));
  QVERIFY(root.has_child(1));
  QVERIFY(!root.has_child(0));
  QVERIFY(!root.has_child(2));
  QCOMPARE(root.children_count(), 1u);

  auto ptr3 = root.extract_child(2);
  QCOMPARE(ptr3, nullptr);
  QVERIFY(!root.has_child(3));
  QVERIFY(root.has_child(1));
  QVERIFY(!root.has_child(0));
  QVERIFY(!root.has_child(2));
  QCOMPARE(root.children_count(), 1u);
  QVERIFY_EXCEPTION_THROWN(root.child(0), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(root.child(2), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(root.child(3), null_node_exception_t);

  auto ptr4 = root.extract_child(1);
  QVERIFY(ptr4 != nullptr);
  QCOMPARE(ptr4->get(), "4");
  QCOMPARE((*ptr4).first().get(),          "1");
  QCOMPARE((*ptr4).last().get(),           "6");
  QCOMPARE((*ptr4).child(1).first().get(), "3");
  QCOMPARE((*ptr4).child(1).get(),         "2");
  QCOMPARE((*ptr4).child(2).get(),         "5");
  QCOMPARE(root.children_count(), 0u);
  QVERIFY(!root.has_child(3));
  QVERIFY(!root.has_child(1));
  QVERIFY(!root.has_child(0));
  QVERIFY(!root.has_child(2));
  QVERIFY_EXCEPTION_THROWN(root.child(0), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(root.child(1), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(root.child(2), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(root.child(3), null_node_exception_t);
}

void TestTree::
test_clear() {
  auto quad_tree = get_test_tree();
  QVERIFY(quad_tree.has_root());
  quad_tree.clear();
  QVERIFY(!quad_tree.has_root());
}

void TestTree::
test_find() {
  const auto quad_tree = get_test_tree();
  const auto&root { quad_tree.root() };

  {
    auto res_not_found{ root.find("2") };
    QCOMPARE(res_not_found.first,  false);
    QCOMPARE(res_not_found.second, std::numeric_limits<size_t>::max());
  }

  {
    auto res_found{ root.find("4") };
    QCOMPARE(res_found.first,  true);
    QCOMPARE(res_found.second, 1u);
  }

  {
    auto res_found{ root.find("10") };
    QCOMPARE(res_found.first,  true);
    QCOMPARE(res_found.second, 3u);
  }
}

void TestTree::
test_for_range() {
  const std::vector<std::string> expected{ "4", "10" };
  std::vector<std::string> visited;
  auto quad_tree = get_test_tree();
  auto&root { quad_tree.root() };

  for (auto& node : root) {
    if (node) {
      visited.push_back(node->get());
    }
  }

  QCOMPARE(visited, expected);
  visited.clear();
  for (auto it = root.rbegin(); it != root.rend(); it++) {
    auto ptr { *it };
    if (ptr) {
      visited.push_back((ptr)->get());
    }
  }
  std::reverse(visited.begin(), visited.end());
  QCOMPARE(visited, expected);
  visited.clear();
  for (auto it = root.crbegin(); it != root.crend(); it++) {
    auto ptr { *it };
    if (ptr) {
      visited.push_back((ptr)->get());
    }
  }
  std::reverse(visited.begin(), visited.end());
  QCOMPARE(visited, expected);

  visited.clear();
  for (auto it = root.cbegin(); it != root.cend(); it++) {
    auto ptr { *it };
    if (ptr) {
      visited.push_back((ptr)->get());
    }
  }
  QCOMPARE(visited, expected);
  visited.clear();

  const auto& ref{ root };
  for (auto& node : ref) {
    if (node) {
      visited.push_back(node->get());
    }
  }

  QCOMPARE(visited, expected);
}

void TestTree::
test_first() {
  auto quad_tree = get_test_tree();
  auto&root { quad_tree.root() };
  QCOMPARE(root.first().get(), "4");
  QCOMPARE(root.first().first().get(), "1");
  QVERIFY_EXCEPTION_THROWN(root.first().first().first(), null_node_exception_t);

  const auto& ref{ root };
  QCOMPARE(ref.first().get(), "4");
  QCOMPARE(ref.first().first().get(), "1");
  QVERIFY_EXCEPTION_THROWN(ref.first().first().first(), null_node_exception_t);
}

void TestTree::
test_last() {
  auto quad_tree = get_test_tree();
  auto&root { quad_tree.root() };
  QCOMPARE(root.last().get(), "10");
  QCOMPARE(root.last().last().get(), "13");
  QCOMPARE(root.last().last().last().get(), "12");
  QVERIFY_EXCEPTION_THROWN(root.last().last().last().last(),
                           null_node_exception_t);

  const auto& ref{ root };
  QCOMPARE(ref.last().get(), "10");
  QCOMPARE(ref.last().last().get(), "13");
  QCOMPARE(ref.last().last().last().get(), "12");
  QVERIFY_EXCEPTION_THROWN(ref.last().last().last().last(),
                           null_node_exception_t);
}
