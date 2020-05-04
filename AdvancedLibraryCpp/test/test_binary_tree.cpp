#include "test_binary_tree.h"

using namespace advanced::structures;

TestBinaryTree::
TestBinaryTree(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestBinaryTree");
}

void TestBinaryTree::
test_add_left() {
  binary_tree_t<std::string> tree;
  auto& root{ tree.add_root("grandpa").root() };
  QCOMPARE(root.children_count(), 0u);
  QVERIFY(!root.has_left());
  QVERIFY(root.add_left("father"));
  QVERIFY(root.has_left());
  QCOMPARE(root.children_count(), 1u);
  QVERIFY(!root.add_left("step-father"));

  auto& father{ root.left() };
  QCOMPARE(father.children_count(), 0u);
  QVERIFY(!father.has_left());
  QVERIFY(father.add_left("brother"));
  QCOMPARE(root.children_count(), 1u);
  QVERIFY(father.has_left());
  QVERIFY(!father.add_left("brother-in-law"));
}

void TestBinaryTree::
test_add_right() {
  binary_tree_t<std::string> tree;
  auto& root{ tree.add_root("grandma").root() };
  QCOMPARE(root.children_count(), 0u);
  QVERIFY(!root.has_right());
  QVERIFY(root.add_right("mother"));
  QVERIFY(root.has_right());
  QCOMPARE(root.children_count(), 1u);
  QVERIFY(!root.add_right("step-mother"));

  auto& mother{ root.right() };
  QCOMPARE(mother.children_count(), 0u);
  QVERIFY(!mother.has_right());
  QVERIFY(mother.add_right("sister"));
  QCOMPARE(root.children_count(), 1u);
  QVERIFY(mother.has_right());
  QVERIFY(!mother.add_right("sister-in-law"));
}

void TestBinaryTree::
test_left_getter() {
  binary_tree_t<std::string> tree;
  auto& root{ tree.add_root("grandpa").root() };
  QVERIFY(root.add_left("father"));

  auto& father{ root.left() };
  QCOMPARE(father.get(), "father");

  QVERIFY(father.add_left("brother"));

  const auto& brother{ father.left() };
  QCOMPARE(brother.get(), "brother");
}

void TestBinaryTree::
test_right_getter() {
  binary_tree_t<std::string> tree;
  auto& root{ tree.add_root("grandma").root() };
  QVERIFY(root.add_right("mother"));

  auto& mother{ root.right() };
  QCOMPARE(mother.get(), "mother");
  QVERIFY(mother.add_right("sister"));

  const auto& sister{ mother.right() };
  QCOMPARE(sister.get(), "sister");
}

void TestBinaryTree::
test_has_left() {
  binary_tree_t<std::string> tree;
  auto& root{ tree.add_root("grandpa").root() };
  QVERIFY(!root.has_left());
  QVERIFY(root.add_left("father"));
  QVERIFY(root.has_left());

  auto& father{ root.left() };
  QVERIFY(!father.has_left());
  QVERIFY(father.add_left("brother"));
  QVERIFY(father.has_left());

  const auto& brother{ father.left() };
  QVERIFY(!brother.has_left());
}

void TestBinaryTree::
test_has_right() {
  binary_tree_t<std::string> tree;
  auto& root{ tree.add_root("grandma").root() };
  QVERIFY(!root.has_right());
  QVERIFY(root.add_right("mother"));
  QVERIFY(root.has_right());

  auto& mother{ root.right() };
  QVERIFY(!mother.has_right());
  QVERIFY(mother.add_right("sister"));
  QVERIFY(mother.has_right());

  const auto& sister{ mother.right() };
  QVERIFY(!sister.has_right());
}

void TestBinaryTree::
test_children_count() {
  binary_tree_t<int> tree;
  auto& root{ tree.add_root(0).root() };
  QCOMPARE(root.children_count(), 0u);

  root.add_left(1);
  QCOMPARE(root.children_count(), 1u);
  root.add_right(2);
  QCOMPARE(root.children_count(), 2u);

  root.add_left(3);
  QCOMPARE(root.children_count(), 2u);
  root.add_right(4);
  QCOMPARE(root.children_count(), 2u);

  // adding children nodes to child does not affect the current node number of
  // child nodes
  auto & left { root.left() };
  auto & right { root.right() };
  left.add_left(3);
  QCOMPARE(root.children_count(),  2u);
  QCOMPARE(left.children_count(),  1u);
  QCOMPARE(right.children_count(), 0u);
  left.add_right(4);
  QCOMPARE(root.children_count(),  2u);
  QCOMPARE(left.children_count(),  2u);
  QCOMPARE(right.children_count(), 0u);

  right.add_left(5);
  QCOMPARE(root.children_count(),  2u);
  QCOMPARE(left.children_count(),  2u);
  QCOMPARE(right.children_count(), 1u);
  right.add_right(6);
  QCOMPARE(root.children_count(),  2u);
  QCOMPARE(left.children_count(),  2u);
  QCOMPARE(right.children_count(), 2u);
}

void TestBinaryTree::
test_custom_node_constructor() {
  binary_tree_t<std::string> str_tree("king");
  QCOMPARE(*str_tree.root(), "king");

  binary_tree_t<int> int_tree(42);
  QCOMPARE(*int_tree.root(), 42);

  {
    binary_tree_t<test::structures::mock_node_value>
        special_tree(test::structures::mock_node_value{42, 20.0, "friend"});
    auto& root { *special_tree.root() };
    QCOMPARE(root.int_v,    42);
    QCOMPARE(root.float_v,  20.0);
    QCOMPARE(root.name,     "friend");
  }
}

void TestBinaryTree::
test_delete_child() {
  binary_tree_t<test::structures::mock_node_value> kingdom(
        test::structures::mock_node_value{ 42, 100.0, "king" });
  auto& real_family{ kingdom.root() };
  real_family.add_left({ 29, 50, "prince" });
  real_family.add_right({ 20, 50, "princess" });
  QCOMPARE(real_family.children_count(), 2u);
  QVERIFY(real_family.has_left());
  QVERIFY(real_family.has_right());

  // prince has died:
  real_family.delete_child(
        binary_node_t<test::structures::mock_node_value>::direction::left);
  QCOMPARE(real_family.children_count(), 1u);
  QVERIFY(!real_family.has_left());
  QVERIFY(real_family.has_right());

  // princess has died:
  real_family.delete_child(
        binary_node_t<test::structures::mock_node_value>::direction::right);
  QCOMPARE(real_family.children_count(), 0u);
  QVERIFY(!real_family.has_left());
  QVERIFY(!real_family.has_right());

  // new children was born:
  real_family.add_left({ 29, 50, "prince 2" });
  real_family.add_right({ 20, 50, "princess 2" });
  QCOMPARE(real_family.children_count(), 2u);
  QVERIFY(real_family.has_left());
  QVERIFY(real_family.has_right());
  QCOMPARE(real_family.left().get().name,  "prince 2");
  QCOMPARE(real_family.right().get().name, "princess 2");

  // new prince has died:
  real_family.delete_child(0u);
  QCOMPARE(real_family.children_count(), 1u);
  QVERIFY(!real_family.has_left());
  QVERIFY(real_family.has_right());

  // new princess has died:
  real_family.delete_child(1u);
  QCOMPARE(real_family.children_count(), 0u);
  QVERIFY(!real_family.has_left());
  QVERIFY(!real_family.has_right());
}

void TestBinaryTree::
test_swap() {
  binary_tree_t<test::structures::mock_node_value> tribe(
        test::structures::mock_node_value{ 42, 100.0, "Tsunade" });
  auto& hokkage{ tribe.root() };
  hokkage.add_left({ 21, 50.0, "Kakashi" });
  hokkage.add_right({ 12, 5000.0, "Naruto" });
  QCOMPARE(hokkage.children_count(), 2u);

  hokkage.swap(binary_node_t<test::structures::mock_node_value>::direction::left);
  QCOMPARE(hokkage.get().name,    "Kakashi");
  QCOMPARE(hokkage.get().int_v,   21);
  QCOMPARE(hokkage.get().float_v, 50.0);

  {
    const auto& tsunade{ hokkage.left() };
    QCOMPARE(tsunade.get().name,    "Tsunade");
    QCOMPARE(tsunade.get().int_v,   42);
    QCOMPARE(tsunade.get().float_v, 100.0);

    const auto& naruto{ hokkage.right() };
    QCOMPARE(naruto.get().name,    "Naruto");
    QCOMPARE(naruto.get().int_v,   12);
    QCOMPARE(naruto.get().float_v, 5000.0);
  }

  hokkage.swap(binary_node_t<test::structures::mock_node_value>::direction::right);
  QCOMPARE(hokkage.get().name,    "Naruto");
  QCOMPARE(hokkage.get().int_v,   12);
  QCOMPARE(hokkage.get().float_v, 5000.0);
  {
    const auto& tsunade{ hokkage.left() };
    QCOMPARE(tsunade.get().name,    "Tsunade");
    QCOMPARE(tsunade.get().int_v,   42);
    QCOMPARE(tsunade.get().float_v, 100.0);

    const auto& kakashi{ hokkage.right() };
    QCOMPARE(kakashi.get().name,    "Kakashi");
    QCOMPARE(kakashi.get().int_v,   21);
    QCOMPARE(kakashi.get().float_v, 50.0);
  }
}

void TestBinaryTree::
test_get_root() {
  binary_tree_t<test::structures::mock_node_value> tribe(
        test::structures::mock_node_value{ 42, 100.0, "Tsunade" });
  const auto& hokkage{ tribe.root() };
  QCOMPARE(hokkage.get().name,    "Tsunade");
  QCOMPARE(hokkage.get().int_v,   42);
  QCOMPARE(hokkage.get().float_v, 100.0);
}

void TestBinaryTree::
test_initializing_root_on_constructor() {
  binary_tree_t<test::structures::mock_node_value> tribe(
        test::structures::mock_node_value{ 42, 100.0, "Tsunade" });
  const auto& hokkage{ tribe.root() };
  QCOMPARE(hokkage.get().name,    "Tsunade");
  QCOMPARE(hokkage.get().int_v,   42);
  QCOMPARE(hokkage.get().float_v, 100.0);
}

void TestBinaryTree::
test_children_nodes_should_throw_null_node_exception_when_they_are_null() {
  binary_tree_t<test::structures::mock_node_value> tribe(
        test::structures::mock_node_value{ 42, 100.0, "Tsunade" });
  const auto& hokkage{ tribe.root() };
  QVERIFY_EXCEPTION_THROWN(hokkage.left(),   null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(hokkage.right(),  null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(hokkage.parent(), null_node_exception_t);
}
