#include "test_avl_tree.h"
#include <set>
#include <cmath>

using namespace advanced::structures;

TestAVLTree::
TestAVLTree(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestAVLTree");
}

advanced::structures::avl_tree_t<int> TestAVLTree::
get_test_tree() {
  avl_tree_t<int> tree;
  tree.insert(7);
  tree.insert(4);
  tree.insert(10);
  tree.insert(2);
  tree.insert(5);
  tree.insert(8);
  tree.insert(12);
  tree.insert(1);
  tree.insert(3);
  tree.insert(6);
  tree.insert(9);
  tree.insert(11);
  return tree;
}

void TestAVLTree::
test_has_left() {
  avl_tree_t<long> tree;
  tree.insert(6);
  tree.insert(0);

  const auto& root { tree.root() };
  QVERIFY(root.has_left());
  QCOMPARE(root.get(), 6);
  QVERIFY(!root.left().has_left());
  QCOMPARE(root.left().get(), 0);
}

void TestAVLTree::
test_has_right() {
  avl_tree_t<long> tree;
  tree.insert(-6);
  tree.insert(0);

  const auto& root { tree.root() };
  QVERIFY(root.has_right());
  QCOMPARE(root.get(), -6);
  QVERIFY(!root.right().has_right());
  QCOMPARE(root.right().get(), 0);
}

void TestAVLTree::
test_children_count() {
  avl_tree_t<long> tree;
  tree.insert(0);

  const auto& root  { tree.root() };
  QCOMPARE(root.children_count(), 0u);

  tree.insert(-6);
  QCOMPARE(root.children_count(), 1u);
  tree.insert(6);
  QCOMPARE(root.children_count(), 2u);

  const auto& left  { root.left() };
  const auto& right { root.right() };
  QCOMPARE(left.children_count(),  0u);
  QCOMPARE(right.children_count(), 0u);
}

void TestAVLTree::
test_initializing_root_on_constructor() {
  avl_tree_t<std::string> tree{ "Bob" };
  QVERIFY(tree.has_root());
  QCOMPARE(tree.root().get(), "Bob");
}

void TestAVLTree::
test_children_nodes_should_throw_null_node_exception_when_they_are_null() {
  avl_tree_t<std::string> tree{ "cyan" };
  QVERIFY(tree.has_root());

  const auto& cyan{ tree.root() };
  QVERIFY_EXCEPTION_THROWN(cyan.left(),  null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(cyan.right(), null_node_exception_t);
}

void TestAVLTree::
test_has_parent() {
  avl_tree_t<std::string> tree{ "cyan" };

  tree.insert("blue");
  tree.insert("grey");

  const auto& cyan{ tree.root()  };
  const auto& blue{ cyan.left()  };
  const auto& grey{ cyan.right() };
  QVERIFY(!cyan.has_parent());
  QVERIFY(blue.has_parent());
  QVERIFY(grey.has_parent());
}

void TestAVLTree::
test_parent_getter() {
  avl_tree_t<std::string> tree{ "cyan" };

  tree.insert("blue");
  tree.insert("grey");

  const auto& cyan{ tree.root()  };
  const auto& blue{ cyan.left()  };
  const auto& grey{ cyan.right() };
  QCOMPARE(*blue.parent(), *cyan);
  QCOMPARE(*grey.parent(), *cyan);
}

void TestAVLTree::
test_null_root_should_throw_exception() {
  avl_tree_t<std::string> tree;
  QVERIFY(!tree.has_root());
  QVERIFY_EXCEPTION_THROWN(tree.root(), null_node_exception_t);
}

void TestAVLTree::
test_move_copy() {
  {
    avl_tree_t<std::string> tree{ "magenta" };
    tree.insert("brown");   // left
    tree.insert("silver");  // right
    tree.insert("blue");    // left-left
    tree.insert("orange"); // right-left
    tree.insert("cyan");    // left-right
    tree.insert("yellow");  // right-right

    avl_tree_t<std::string> copied_tree{ tree };
    avl_tree_t<std::string> moved_tree{ std::move(tree) };

    QCOMPARE(copied_tree.root().get(),                  "magenta");
    QCOMPARE(copied_tree.root().left().get(),           "brown");
    QCOMPARE(copied_tree.root().right().get(),          "silver");
    QCOMPARE(copied_tree.root().left().left().get(),    "blue");
    QCOMPARE(copied_tree.root().left().right().get(),   "cyan");
    QCOMPARE(copied_tree.root().right().left().get(),   "orange");
    QCOMPARE(copied_tree.root().right().right().get(),  "yellow");
    QCOMPARE(moved_tree.root().get(),                   "magenta");
    QCOMPARE(moved_tree.root().left().get(),            "brown");
    QCOMPARE(moved_tree.root().right().get(),           "silver");
    QCOMPARE(moved_tree.root().left().left().get(),     "blue");
    QCOMPARE(moved_tree.root().left().right().get(),    "cyan");
    QCOMPARE(moved_tree.root().right().left().get(),    "orange");
    QCOMPARE(moved_tree.root().right().right().get(),   "yellow");
    QVERIFY(!tree.has_root());
  }
  {
    avl_tree_t<std::string> tree{ "magenta" }, copied_tree, moved_tree;
    tree.insert("brown");   // left
    tree.insert("silver");  // right
    tree.insert("blue");    // left-left
    tree.insert("orange");  // right-left
    tree.insert("cyan");    // left-right
    tree.insert("yellow");  // right-right

    copied_tree = tree;
    moved_tree  = std::move(tree);

    QCOMPARE(copied_tree.root().get(),                  "magenta");
    QCOMPARE(copied_tree.root().left().get(),           "brown");
    QCOMPARE(copied_tree.root().right().get(),          "silver");
    QCOMPARE(copied_tree.root().left().left().get(),    "blue");
    QCOMPARE(copied_tree.root().left().right().get(),   "cyan");
    QCOMPARE(copied_tree.root().right().left().get(),   "orange");
    QCOMPARE(copied_tree.root().right().right().get(),  "yellow");
    QCOMPARE(moved_tree.root().get(),                   "magenta");
    QCOMPARE(moved_tree.root().left().get(),            "brown");
    QCOMPARE(moved_tree.root().right().get(),           "silver");
    QCOMPARE(moved_tree.root().left().left().get(),     "blue");
    QCOMPARE(moved_tree.root().left().right().get(),    "cyan");
    QCOMPARE(moved_tree.root().right().left().get(),    "orange");
    QCOMPARE(moved_tree.root().right().right().get(),   "yellow");
    QVERIFY(!tree.has_root());
  }
}

void TestAVLTree::
test_rotation() {
  {
    avl_tree_t<std::string> tree{ "blue" };
    tree.insert("brown");
    tree.insert("cyan");
    tree.insert("magenta");
    tree.insert("orange");
    tree.insert("silver");
    tree.insert("yellow");
    QCOMPARE(tree.root().get(),                  "magenta");
    QCOMPARE(tree.root().left().get(),           "brown");
    QCOMPARE(tree.root().right().get(),          "silver");
    QCOMPARE(tree.root().left().left().get(),    "blue");
    QCOMPARE(tree.root().left().right().get(),   "cyan");
    QCOMPARE(tree.root().right().left().get(),   "orange");
    QCOMPARE(tree.root().right().right().get(),  "yellow");
  }
  {
    avl_tree_t<std::string> tree{ "yellow" };
    tree.insert("silver");
    tree.insert("orange");
    tree.insert("magenta");
    tree.insert("cyan");
    tree.insert("brown");
    tree.insert("blue");
    QCOMPARE(tree.root().get(),                  "magenta");
    QCOMPARE(tree.root().left().get(),           "brown");
    QCOMPARE(tree.root().right().get(),          "silver");
    QCOMPARE(tree.root().left().left().get(),    "blue");
    QCOMPARE(tree.root().left().right().get(),   "cyan");
    QCOMPARE(tree.root().right().left().get(),   "orange");
    QCOMPARE(tree.root().right().right().get(),  "yellow");
  }
  {
    avl_tree_t<std::string> tree;
    tree.insert("yellow");
    tree.insert("blue");
    tree.insert("silver");
    tree.insert("brown");
    tree.insert("magenta");
    tree.insert("orange");
    tree.insert("cyan");
    QCOMPARE(tree.root().get(),                  "magenta");
    QCOMPARE(tree.root().left().get(),           "brown");
    QCOMPARE(tree.root().right().get(),          "silver");
    QCOMPARE(tree.root().left().left().get(),    "blue");
    QCOMPARE(tree.root().left().right().get(),   "cyan");
    QCOMPARE(tree.root().right().left().get(),   "orange");
    QCOMPARE(tree.root().right().right().get(),  "yellow");
  }
  {
    avl_tree_t<std::string> tree;
    tree.insert("blue");
    tree.insert("yellow");
    tree.insert("brown");
    tree.insert("silver");
    tree.insert("orange");
    tree.insert("magenta");
    tree.insert("cyan");
    QCOMPARE(tree.root().get(),                       "orange");
    QCOMPARE(tree.root().left().get(),                "brown");
    QCOMPARE(tree.root().right().get(),               "silver");
    QCOMPARE(tree.root().left().left().get(),         "blue");
    QCOMPARE(tree.root().left().right().get(),        "magenta");
    QCOMPARE(tree.root().left().right().left().get(), "cyan");
    QCOMPARE(tree.root().right().right().get(),       "yellow");
  }
}

void TestAVLTree::
test_insert() {
  const std::vector<int> input {
    53, 35, 90, 25, 57, 27, 76, 9, 59, 93,
    71, 94, 38, 78, 41, 67, 3, 68, 27, 85,
    96, 27, 44, 37, 92, 28, 51, 93, 74, 76,
    38, 42, 1, 67, 37, 44, 8, 56, 98, 80,
    39, 91, 82, 21, 52, 11, 97, 74, 79, 67,
    80, 21, 43, 84, 89, 79, 28, 44, 16, 75,
    68, 69, 61, 60, 28, 20, 36, 86, 29, 79,
    24, 80, 61, 28, 39, 36, 55, 24, 24, 26,
    15, 87, 33, 86, 11, 49, 27, 30, 41, 99,
    43, 92, 2, 46, 7, 40, 49, 27, 36, 32,
  };
  std::set<int> inserted;
  avl_tree_t<int> tree;

  for (const auto& elem : input) {
    bool insertion_ok { inserted.count(elem) == 0u };
    QCOMPARE(tree.insert(elem), insertion_ok);
    inserted.insert(elem);
    QCOMPARE(tree.size(), inserted.size());
  }

  QCOMPARE(test_node<int>(tree.root()), tree.size());
}

void TestAVLTree::
test_height() {
  const std::vector<int> input {
    53, 35, 90, 25, 57, 27, 76, 9, 59, 93,
    71, 94, 38, 78, 41, 67, 3, 68, 27, 85,
    96, 27, 44, 37, 92, 28, 51, 93, 74, 76,
    38, 42, 1, 67, 37, 44, 8, 56, 98, 80,
    39, 91, 82, 21, 52, 11, 97, 74, 79, 67,
    80, 21, 43, 84, 89, 79, 28, 44, 16, 75,
    68, 69, 61, 60, 28, 20, 36, 86, 29, 79,
    24, 80, 61, 28, 39, 36, 55, 24, 24, 26,
    15, 87, 33, 86, 11, 49, 27, 30, 41, 99,
    43, 92, 2, 46, 7, 40, 49, 27, 36, 32,
  };
  std::set<int> inserted;
  avl_tree_t<int> tree;

  for (const auto& elem : input) {
    tree.insert(elem);
    inserted.insert(elem);
    QVERIFY(tree.height() > 0);
    QVERIFY(tree.height() <= std::ceil(1.45 * (std::log2(inserted.size()) + 1)));
  }
}

void TestAVLTree::
test_left_and_right_most() {
  const std::vector<int> input {
    53, 35, 90, 25, 57, 27, 76, 9, 59, 93,
    71, 94, 38, 78, 41, 67, 3, 68, 27, 85,
    96, 27, 44, 37, 92, 28, 51, 93, 74, 76,
    38, 42, 1, 67, 37, 44, 8, 56, 98, 80,
    39, 91, 82, 21, 52, 11, 97, 74, 79, 67,
    80, 21, 43, 84, 89, 79, 28, 44, 16, 75,
    68, 69, 61, 60, 28, 20, 36, 86, 29, 79,
    24, 80, 61, 28, 39, 36, 55, 24, 24, 26,
    15, 87, 33, 86, 11, 49, 27, 30, 41, 99,
    43, 92, 2, 46, 7, 40, 49, 27, 36, 32,
  };
  std::set<int> inserted;
  avl_tree_t<int> tree;

  for (const auto& elem : input) {
    tree.insert(elem);
    inserted.insert(elem);
    QCOMPARE(tree.left_most().get(),  *inserted.begin());
    QCOMPARE(tree.right_most().get(), *inserted.rbegin());
  }
}

void TestAVLTree::
test_remove() {
  const std::vector<int> input {
    53, 35, 90, 25, 57, 27, 76, 9, 59, 93,
    71, 94, 38, 78, 41, 67, 3, 68, 27, 85,
    96, 27, 44, 37, 92, 28, 51, 93, 74, 76,
    38, 42, 1, 67, 37, 44, 8, 56, 98, 80,
    39, 91, 82, 21, 52, 11, 97, 74, 79, 67,
    80, 21, 43, 84, 89, 79, 28, 44, 16, 75,
    68, 69, 61, 60, 28, 20, 36, 86, 29, 79,
    24, 80, 61, 28, 39, 36, 55, 24, 24, 26,
    15, 87, 33, 86, 11, 49, 27, 30, 41, 99,
    43, 92, 2, 46, 7, 40, 49, 27, 36, 32,
  };
  std::set<int> inserted;
  avl_tree_t<int> tree;

  for (const auto& elem : input) {
    tree.insert(elem);
    inserted.insert(elem);
  }

  for (const auto& elem : input) {
    bool contains { inserted.count(elem) == 1u };
    QCOMPARE(tree.remove(elem), contains);
    inserted.erase(elem);
    QCOMPARE(tree.size(), inserted.size());

    if (inserted.size()) {
      QCOMPARE(test_node(tree.root()), tree.size());
      QVERIFY(tree.height() > 0);
      QVERIFY(tree.height() <= std::ceil(1.45 * (std::log2(inserted.size()) + 1)));
      QCOMPARE(tree.left_most().get(),  *inserted.begin());
      QCOMPARE(tree.right_most().get(), *inserted.rbegin());
    }
    else {
      QCOMPARE(tree.height(), 0u);
      QVERIFY_EXCEPTION_THROWN(tree.root(), null_node_exception_t);
    }
  }
}

void TestAVLTree::
test_search_tree() {
  const std::vector<int> input {
    53, 35, 90, 25, 57, 27, 76, 9, 59, 93,
    71, 94, 38, 78, 41, 67, 3, 68, 27, 85,
    96, 27, 44, 37, 92, 28, 51, 93, 74, 76,
    38, 42, 1, 67, 37, 44, 8, 56, 98, 80,
    39, 91, 82, 21, 52, 11, 97, 74, 79, 67,
    80, 21, 43, 84, 89, 79, 28, 44, 16, 75,
    68, 69, 61, 60, 28, 20, 36, 86, 29, 79,
    24, 80, 61, 28, 39, 36, 55, 24, 24, 26,
    15, 87, 33, 86, 11, 49, 27, 30, 41, 99,
    43, 92, 2, 46, 7, 40, 49, 27, 36, 32,
  };
  std::set<int> inserted;
  avl_tree_t<int> tree;

  for (const auto& elem : input) {
    bool contains { inserted.count(elem) == 1u };
    QCOMPARE(tree.contains(elem), contains);
    tree.insert(elem);
    inserted.insert(elem);
    QCOMPARE(tree.contains(elem), true);
  }

  for (const auto& elem : input) {
    bool contains { inserted.count(elem) == 1u };
    QCOMPARE(tree.contains(elem), contains);
    tree.remove(elem);
    inserted.erase(elem);
    QCOMPARE(tree.contains(elem), false);
  }
}

void TestAVLTree::
test_allow_multiple_equal_entires() {
  const std::vector<int> input {
    53, 35, 90, 25, 57, 27, 76, 9, 59, 93,
    71, 94, 38, 78, 41, 67, 3, 68, 27, 85,
    96, 27, 44, 37, 92, 28, 51, 93, 74, 76,
    38, 42, 1, 67, 37, 44, 8, 56, 98, 80,
    39, 91, 82, 21, 52, 11, 97, 74, 79, 67,
    80, 21, 43, 84, 89, 79, 28, 44, 16, 75,
    68, 69, 61, 60, 28, 20, 36, 86, 29, 79,
    24, 80, 61, 28, 39, 36, 55, 24, 24, 26,
    15, 87, 33, 86, 11, 49, 27, 30, 41, 99,
    43, 92, 2, 46, 7, 40, 49, 27, 36, 32,
  };
  std::multiset<int> inserted;
  avl_tree_t<int> tree;

  QVERIFY(tree.insert(42));
  // multiple equal values disallowed:
  QVERIFY(!tree.insert(42));

  QVERIFY(!tree.is_multiple_keys_allowed());
  tree.enable_duplicates();
  QVERIFY(tree.is_multiple_keys_allowed());
  // multiple equal values allowed:
  QVERIFY(tree.insert(42));
  QVERIFY(tree.insert(42));
  QVERIFY(tree.insert(42));

  tree.disable_duplicates();
  QVERIFY(!tree.is_multiple_keys_allowed());
  // multiple equal values disallowed:
  QVERIFY(!tree.insert(42));

  // Still have to remove all inserted values while multiple keys was allowed:
  QVERIFY(tree.remove(42));
  QVERIFY(tree.remove(42));
  QVERIFY(tree.remove(42));
  QVERIFY(tree.remove(42));
  QVERIFY(!tree.remove(42));
}

void TestAVLTree::
test_remove_one_element() {
  const std::vector<int> input {
    53, 35, 90, 25, 57, 27, 76, 9, 59, 93,
    71, 94, 38, 78, 41, 67, 3, 68, 27, 85,
    96, 27, 44, 37, 92, 28, 51, 93, 74, 76,
    38, 42, 1, 67, 37, 44, 8, 56, 98, 80,
    39, 91, 82, 21, 52, 11, 97, 74, 79, 67,
    80, 21, 43, 84, 89, 79, 28, 44, 16, 75,
    68, 69, 61, 60, 28, 20, 36, 86, 29, 79,
    24, 80, 61, 28, 39, 36, 55, 24, 24, 26,
    15, 87, 33, 86, 11, 49, 27, 30, 41, 99,
    43, 92, 2, 46, 7, 40, 49, 27, 36, 32,
  };
  std::multiset<int> inserted;
  avl_tree_t<int> tree;
  tree.enable_duplicates();

  // Testing removing one keys:
  for (const auto& elem : input) {
    bool contains { inserted.count(elem) > 0u };
    QCOMPARE(tree.contains(elem), contains);
    QVERIFY(tree.insert(elem));
    inserted.insert(elem);
    QCOMPARE(tree.contains(elem), true);
    QCOMPARE(tree.size(), inserted.size());
  }

  for (const auto& elem : input) {
    QCOMPARE(tree.contains(elem), true);
    QVERIFY(tree.remove(elem));

    auto it = inserted.find(elem);
    if (it != inserted.end()) {
      inserted.erase(it);
    }

    if (inserted.size()) {
      QCOMPARE(test_node(tree.root()), tree.size());
    }

    QCOMPARE(tree.contains(elem), inserted.count(elem) > 0u);
    QCOMPARE(tree.size(), inserted.size());
  }
}

void TestAVLTree::
test_remove_all_elements() {
  const std::vector<int> input {
    53, 35, 90, 25, 57, 27, 76, 9, 59, 93,
    71, 94, 38, 78, 41, 67, 3, 68, 27, 85,
    96, 27, 44, 37, 92, 28, 51, 93, 74, 76,
    38, 42, 1, 67, 37, 44, 8, 56, 98, 80,
    39, 91, 82, 21, 52, 11, 97, 74, 79, 67,
    80, 21, 43, 84, 89, 79, 28, 44, 16, 75,
    68, 69, 61, 60, 28, 20, 36, 86, 29, 79,
    24, 80, 61, 28, 39, 36, 55, 24, 24, 26,
    15, 87, 33, 86, 11, 49, 27, 30, 41, 99,
    43, 92, 2, 46, 7, 40, 49, 27, 36, 32,
  };
  std::multiset<int> inserted;
  avl_tree_t<int> tree;
  tree.enable_duplicates();

  // Testing removing all keys:
  for (const auto& elem : input) {
    bool contains { inserted.count(elem) > 0u };
    QCOMPARE(tree.contains(elem), contains);
    QVERIFY(tree.insert(elem));
    inserted.insert(elem);
    QCOMPARE(tree.contains(elem), true);
    QCOMPARE(tree.size(), inserted.size());
  }

  for (const auto& elem : input) {
    QCOMPARE(tree.contains(elem),   inserted.count(elem) > 0u);
    QCOMPARE(tree.remove_all(elem), inserted.count(elem));
    inserted.erase(elem);
    QCOMPARE(tree.contains(elem), false);
    QCOMPARE(tree.size(), inserted.size());
    if (inserted.size()) {
      QCOMPARE(test_node(tree.root()), tree.size());
    }
  }
}

void TestAVLTree::
test_clear() {
  using test::structures::moc_avl_node;
  const std::vector<moc_avl_node> input {
    moc_avl_node{ "53", 35 }, moc_avl_node{ "90", 25 }, moc_avl_node{ "57", 27 },
    moc_avl_node{ "76", 9 }, moc_avl_node{ "59", 93 }, moc_avl_node{ "71", 94 },
    moc_avl_node{ "38", 78 }, moc_avl_node{ "41", 67 },  moc_avl_node{ "3", 68 },
    moc_avl_node{ "27", 85 }, moc_avl_node{ "96", 27 }, moc_avl_node{ "44", 37 },
    moc_avl_node{ "92", 28 }, moc_avl_node{ "51", 93 }, moc_avl_node{ "74", 76 },
    moc_avl_node{ "38", 42 }, moc_avl_node{ "1", 67 }, moc_avl_node{ "37", 44 },
    moc_avl_node{ "8", 56 }, moc_avl_node{ "98", 80 }, moc_avl_node{ "39", 91 },
    moc_avl_node{ "82", 21 }, moc_avl_node{ "52", 11 }, moc_avl_node{ "97", 74 },
    moc_avl_node{ "79", 67 }, moc_avl_node{ "80", 21 }, moc_avl_node{ "43", 84 },
    moc_avl_node{ "89", 79 }
  };
  std::set<moc_avl_node> inserted;
  avl_tree_t<moc_avl_node> tree;

  QCOMPARE(tree.size(), 0u);

  for (const auto& elem : input) {
    bool contains { inserted.count(elem) == 1u };
    QCOMPARE(tree.contains(elem), contains);
    tree.insert(elem);
    inserted.insert(elem);
    QCOMPARE(tree.contains(elem), true);
  }

  tree.clear();
  QCOMPARE(tree.size(), 0u);
  QVERIFY_EXCEPTION_THROWN(tree.root(), null_node_exception_t);
}

void TestAVLTree::
test_custom_structure() {
  using test::structures::moc_avl_node;
  const std::vector<moc_avl_node> input {
    moc_avl_node{ "53", 35 }, moc_avl_node{ "90", 25 }, moc_avl_node{ "57", 27 },
    moc_avl_node{ "76", 9 }, moc_avl_node{ "59", 93 }, moc_avl_node{ "71", 94 },
    moc_avl_node{ "38", 78 }, moc_avl_node{ "41", 67 },  moc_avl_node{ "3", 68 },
    moc_avl_node{ "27", 85 }, moc_avl_node{ "96", 27 }, moc_avl_node{ "44", 37 },
    moc_avl_node{ "92", 28 }, moc_avl_node{ "51", 93 }, moc_avl_node{ "74", 76 },
    moc_avl_node{ "38", 42 }, moc_avl_node{ "1", 67 }, moc_avl_node{ "37", 44 },
    moc_avl_node{ "8", 56 }, moc_avl_node{ "98", 80 }, moc_avl_node{ "39", 91 },
    moc_avl_node{ "82", 21 }, moc_avl_node{ "52", 11 }, moc_avl_node{ "97", 74 },
    moc_avl_node{ "79", 67 }, moc_avl_node{ "80", 21 }, moc_avl_node{ "43", 84 },
    moc_avl_node{ "89", 79 }
  };
  std::set<moc_avl_node> inserted;
  avl_tree_t<moc_avl_node> tree;

  for (const auto& elem : input) {
    bool contains { inserted.count(elem) == 1u };
    QCOMPARE(tree.contains(elem), contains);
    tree.insert(elem);
    inserted.insert(elem);
    QCOMPARE(tree.contains(elem), true);
  }

  for (const auto& elem : input) {
    bool contains { inserted.count(elem) == 1u };
    QCOMPARE(tree.contains(elem), contains);
    tree.remove(elem);
    inserted.erase(elem);
    QCOMPARE(tree.contains(elem), false);

    if (inserted.size()) {
      QCOMPARE(test_node(tree.root()), tree.size());
      QVERIFY(tree.height() > 0);
      QVERIFY(tree.height() <= std::ceil(1.45 * (std::log2(inserted.size()) + 1)));
      QCOMPARE(tree.left_most().get(),  *inserted.begin());
      QCOMPARE(tree.right_most().get(), *inserted.rbegin());
    }
    else {
      QCOMPARE(tree.height(), 0u);
      QVERIFY_EXCEPTION_THROWN(tree.root(), null_node_exception_t);
    }
  }
}

void TestAVLTree::
test_as_pointer() {
  const std::vector<int> input {
    53, 35, 90, 25, 57, 27, 76, 9, 59, 93,
    71, 94, 38, 78, 41, 67, 3, 68, 27, 85,
    96, 27, 44, 37, 92, 28, 51, 93, 74, 76,
    38, 42, 1, 67, 37, 44, 8, 56, 98, 80,
    39, 91, 82, 21, 52, 11, 97, 74, 79, 67,
    80, 21, 43, 84, 89, 79, 28, 44, 16, 75,
    68, 69, 61, 60, 28, 20, 36, 86, 29, 79,
    24, 80, 61, 28, 39, 36, 55, 24, 24, 26,
    15, 87, 33, 86, 11, 49, 27, 30, 41, 99,
    43, 92, 2, 46, 7, 40, 49, 27, 36, 32,
  };
  std::set<int> inserted;
  std::unique_ptr<base_tree_t> tree_ptr( new avl_tree_t<int>());
  avl_tree_t<int>& tree{ dynamic_cast<avl_tree_t<int>&>(*tree_ptr) };

  for (const auto& elem : input) {
    bool contains { inserted.count(elem) == 1u };
    QCOMPARE(tree.contains(elem), contains);
    tree.insert(elem);
    inserted.insert(elem);
    QCOMPARE(tree.contains(elem), true);
  }

  for (const auto& elem : input) {
    bool contains { inserted.count(elem) == 1u };
    QCOMPARE(tree.contains(elem), contains);
    tree.remove(elem);
    inserted.erase(elem);
    QCOMPARE(tree.contains(elem), false);
  }
}

void TestAVLTree::
test_trasversal_in_order() {
  const std::vector<int> expected { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  const avl_tree_t<int> tree = get_test_tree();
  std::vector<int> visited;

  size_t total =
    tree.in_order(
      [&visited](const binary_node_t<int>& node) {
        visited.push_back(node.get());
        return false;
      });

  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);

  const auto& tree_ref{ tree };
  visited.clear();
  total =
      tree_ref.in_order(
        [&visited](const binary_node_t<int>& node) {
          visited.push_back(node.get());
          return false;
        });
  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);
}

void TestAVLTree::
test_trasversal_pre_order() {
  const std::vector<int> expected { 7, 4, 2, 1, 3, 5, 6, 10, 8, 9, 12, 11 };
  avl_tree_t<int> tree = get_test_tree();
  std::vector<int> visited;

  size_t total =
    tree.pre_order(
      [&visited](const binary_node_t<int>& node) {
        visited.push_back(node.get());
        return false;
      });

  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);

  const auto& tree_ref{ tree };
  visited.clear();
  total =
      tree_ref.pre_order(
        [&visited](const binary_node_t<int>& node) {
          visited.push_back(node.get());
          return false;
        });
  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);
}

void TestAVLTree::
test_trasversal_pos_order() {
  const std::vector<int> expected { 1, 3, 2, 6, 5, 4, 9, 8, 11, 12, 10, 7 };
  avl_tree_t<int> tree = get_test_tree();
  std::vector<int> visited;

  size_t total =
    tree.pos_order(
      [&visited](const binary_node_t<int>& node) {
        visited.push_back(node.get());
        return false;
      });

  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);

  const auto& tree_ref{ tree };
  visited.clear();
  total =
      tree_ref.pos_order(
        [&visited](const binary_node_t<int>& node) {
          visited.push_back(node.get());
          return false;
        });
  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);
}

void TestAVLTree::
test_max_allowed_children() {
  avl_tree_t<int> tree = get_test_tree();
  size_t total =
    tree.pre_order([](const binary_node_t<int>& node) {
      return node.max_allowed_children() != 2u;
    });
  QCOMPARE(total, 12u);
}

void TestAVLTree::
test_bfs() {
  const std::vector<int> expected { 7, 4, 10, 2, 5, 8, 12, 1, 3, 6, 9, 11 };
  avl_tree_t<int> tree = get_test_tree();
  std::vector<int> visited;

  size_t total =
    tree.breadth_first_search(
      [&visited](const binary_node_t<int>& node) {
        visited.push_back(node.get());
        return false;
      });

  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);

  const auto& tree_ref{ tree };
  visited.clear();
  total =
      tree_ref.breadth_first_search(
        [&visited](const binary_node_t<int>& node) {
          visited.push_back(node.get());
          return false;
        });
  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);
}

void TestAVLTree::
test_dfs() {
  const std::vector<int> expected { 7, 4, 2, 1, 3, 5, 6, 10, 8, 9, 12, 11 };
  avl_tree_t<int> tree = get_test_tree();
  std::vector<int> visited;

  size_t total =
    tree.depth_first_search(
      [&visited](const binary_node_t<int>& node) {
        visited.push_back(node.get());
        return false;
      });

  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);

  const auto& tree_ref{ tree };
  visited.clear();
  total =
      tree_ref.depth_first_search(
        [&visited](const binary_node_t<int>& node) {
          visited.push_back(node.get());
          return false;
        });
  QCOMPARE(total,   expected.size());
  QCOMPARE(visited, expected);
}

void TestAVLTree::
test_search_value() {
  avl_tree_t<int> tree = get_test_tree();
  size_t steps{ 0u };

  steps = tree.in_order([](const auto& node) { return *node == 7; });
  QCOMPARE(steps, 7u);
  steps = tree.pre_order([](const auto& node) { return *node == 7; });
  QCOMPARE(steps, 1u);
  steps = tree.pos_order([](const auto& node) { return *node == 10; });
  QCOMPARE(steps, 12u);
  steps = tree.breadth_first_search([](const auto& node) { return *node == 8; });
  QCOMPARE(steps, 6u);
  steps = tree.depth_first_search([](const auto& node) { return *node == 12; });
  QCOMPARE(steps, 11u);
}
