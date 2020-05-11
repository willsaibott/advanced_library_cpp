#include "test_binary_tree.h"

using namespace advanced::structures;

TestBinaryTree::
TestBinaryTree(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestBinaryTree");
}

advanced::structures::binary_tree_t<int> TestBinaryTree::
get_test_tree() {
  binary_tree_t<int> tree(7);
  tree.root().add_left(4);
  tree.root().left().add_left(2);
  tree.root().left().add_right(5);
  tree.root().left().left().add_left(1);
  tree.root().left().left().add_right(3);
  tree.root().left().right().add_right(6);
  tree.root().add_right(10);
  tree.root().right().add_left(8);
  tree.root().right().left().add_right(9);
  tree.root().right().add_right(12);
  tree.root().right().right().add_left(11);
  return tree;
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

  const auto& const_father{ father };
  const auto& brother{ const_father.left() };
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

  const auto& const_mother{ mother };
  const auto& sister{ const_mother.right() };
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

  const binary_tree_t<int> int_tree(42);
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
test_swap_move() {
  binary_tree_t<test::structures::mock_node_value> tribe(
        test::structures::mock_node_value{ 42, 100.0, "Tsunade" });
  auto& hokkage{ tribe.root() };
  hokkage.add_left({ 21, 50.0, "Kakashi" });
  hokkage.add_right({ 12, 5000.0, "Naruto" });
  QCOMPARE(hokkage.children_count(), 2u);

  hokkage.swap_move(hokkage.left());
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

  hokkage.swap_move(hokkage.right());
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
  QVERIFY_EXCEPTION_THROWN(tribe.root().left(),   null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(tribe.root().right(),  null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(tribe.root().parent(), null_node_exception_t);
}

void TestBinaryTree::
test_set_value() {
  binary_tree_t<test::structures::mock_node_value> tribe(
        test::structures::mock_node_value{ 42, 100.0, "Tsunade" });
  auto& hokkage{ tribe.root() };
  hokkage.add_left({ 21, 50.0, "Kakashi" });
  hokkage.add_right({ 12, 5000.0, "Naruto" });
  QCOMPARE(hokkage.children_count(), 2u);

  auto old_hokkage = hokkage.get();
  hokkage.set_value(hokkage.left());
  hokkage.left().set_value(old_hokkage);
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
}

void TestBinaryTree::
test_has_parent() {
  binary_tree_t<std::string> tree;
  auto& root{ tree.add_root("grandpa").root() };

  QVERIFY(!root.has_parent());
  QVERIFY(root.add_left("father"));
  QVERIFY(root.add_right("mother"));

  auto& father{ root.left() };
  auto& mother{ root.right() };
  QVERIFY(father.has_parent());
  QVERIFY(mother.has_parent());
}

void TestBinaryTree::
test_parent_getter() {
  binary_tree_t<std::string> tree;
  auto& root{ tree.add_root("grandpa").root() };
  QVERIFY(root.add_left("father"));
  QVERIFY(root.add_right("mother"));

  auto& father{ root.left() };
  auto& mother{ root.right() };
  QCOMPARE(father.parent().get(), "grandpa");
  QCOMPARE(mother.parent().get(), "grandpa");

  const auto& const_father{ father };
  const auto& const_mother{ mother };
  QCOMPARE(const_father.parent().get(), "grandpa");
  QCOMPARE(const_mother.parent().get(), "grandpa");
}

void TestBinaryTree::
test_null_root_should_throw_exception() {
  binary_tree_t<test::structures::mock_node_value> tribe;
  const auto& const_ref_tree{ tribe };
  QVERIFY_EXCEPTION_THROWN(tribe.root(), null_node_exception_t);
  QVERIFY_EXCEPTION_THROWN(const_ref_tree.root(), null_node_exception_t);
}

void TestBinaryTree::
test_move_copy() {
  binary_tree_t<std::string> tree;
  auto& root{ tree.add_root("grandpa").root() };
  QVERIFY(root.add_left("father"));
  QVERIFY(root.add_right("mother"));

  binary_tree_t<std::string> moved_tree = std::move(tree);
  auto& moved_root{ moved_tree.root() };
  auto& father2{ moved_root.left() };
  auto& mother2{ moved_root.right() };
  QCOMPARE(father2.get(), "father");
  QCOMPARE(mother2.get(), "mother");

  QVERIFY_EXCEPTION_THROWN(tree.root(), null_node_exception_t);

  tree = std::move(moved_tree);
  QCOMPARE(tree.root().left().get(), "father");
  QCOMPARE(tree.root().right().get(), "mother");

  QVERIFY_EXCEPTION_THROWN(moved_tree.root(), null_node_exception_t);

  binary_tree_t<test::structures::mock_node_value> tribe(
        test::structures::mock_node_value{ 42, 100.0, "Tsunade" });
  auto& hokkage1{ tribe.root() };
  hokkage1.add_left({ 21, 50.0, "Kakashi" });
  hokkage1.add_right({ 12, 5000.0, "Naruto" });

  binary_tree_t<test::structures::mock_node_value> copied_tree = tribe;
  auto& hokkage2{ copied_tree.root() };

  QCOMPARE(hokkage1.get().name,    hokkage2.get().name);
  QCOMPARE(hokkage1.get().int_v,   hokkage2.get().int_v);
  QCOMPARE(hokkage1.get().float_v, hokkage2.get().float_v);
  QCOMPARE(hokkage1.left().get().name,    hokkage2.left().get().name);
  QCOMPARE(hokkage1.left().get().int_v,   hokkage2.left().get().int_v);
  QCOMPARE(hokkage1.left().get().float_v, hokkage2.left().get().float_v);
  QCOMPARE(hokkage1.right().get().name,    hokkage2.right().get().name);
  QCOMPARE(hokkage1.right().get().int_v,   hokkage2.right().get().int_v);
  QCOMPARE(hokkage1.right().get().float_v, hokkage2.right().get().float_v);

  binary_node_t<test::structures::mock_node_value>
      node{ test::structures::mock_node_value{ 0, 0.0, "null" }, nullptr };
  node.add_left({1, 2.0, "bomb"});
  node.add_right({2, 2.0, "bomb2"});
  hokkage2 = std::move(node);
  QCOMPARE(hokkage2.get().int_v,    0);
  QCOMPARE(hokkage2.get().float_v,  0.0);
  QCOMPARE(hokkage2.get().name,     "null");
  QCOMPARE(node.get().int_v,    0);
  QCOMPARE(node.get().float_v,  0.0);
  QCOMPARE(node.get().name,     "");
  QVERIFY(!node.has_left());
  QVERIFY(!node.has_right());
  QVERIFY(hokkage2.has_left());
  QVERIFY(hokkage2.has_right());
  QVERIFY(hokkage2.left().has_parent());
  QVERIFY(hokkage2.right().has_parent());
  QCOMPARE(hokkage2.left().parent().get().name,  "null");
  QCOMPARE(hokkage2.right().parent().get().name, "null");
}

void TestBinaryTree::
test_clear() {
  binary_tree_t<test::structures::mock_node_value> tribe(
        test::structures::mock_node_value{ 42, 100.0, "Tsunade" });
  auto& hokkage{ tribe.root() };
  hokkage.add_left({ 21, 50.0, "Kakashi" });
  hokkage.add_right({ 12, 5000.0, "Naruto" });

  tribe.clear();
  QVERIFY_EXCEPTION_THROWN(tribe.root(), null_node_exception_t);
}

void TestBinaryTree::
test_as_pointer() {
  std::unique_ptr<base_tree_t> pointer{
    new binary_tree_t<test::structures::mock_node_value>(
          test::structures::mock_node_value( 42, 100.0, "Tsunade" )) };
  binary_tree_t<test::structures::mock_node_value>& tribe(
    dynamic_cast<binary_tree_t<test::structures::mock_node_value>&>(*pointer));
  auto& hokkage{ tribe.root() };
  hokkage.add_left({ 21, 50.0, "Kakashi" });
  hokkage.add_right({ 12, 5000.0, "Naruto" });
}

void TestBinaryTree::
test_trasversal_in_order() {
  const std::vector<int> expected { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12 };
  std::vector<int> visited;
  binary_tree_t<int> tree = get_test_tree();

  size_t total =
    tree.in_order(
      [&visited](binary_node_t<int>& node) {
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

void TestBinaryTree::
test_trasversal_pre_order() {
  const std::vector<int> expected { 7, 4, 2, 1, 3, 5, 6, 10, 8, 9, 12, 11 };
  binary_tree_t<int> tree = get_test_tree();
  std::vector<int> visited;

  size_t total =
    tree.pre_order(
      [&visited](binary_node_t<int>& node) {
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

void TestBinaryTree::
test_trasversal_pos_order() {
  const std::vector<int> expected { 1, 3, 2, 6, 5, 4, 9, 8, 11, 12, 10, 7 };
  binary_tree_t<int> tree = get_test_tree();
  std::vector<int> visited;

  size_t total =
    tree.pos_order(
      [&visited](binary_node_t<int>& node) {
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

void TestBinaryTree::
test_max_allowed_children() {
  binary_tree_t<int>         tree1(0);
  binary_tree_t<std::string> tree2("Dump");
  binary_tree_t<test::structures::mock_node_value> tree3({0, 0.0, "bob"});

  QCOMPARE(tree1.root().max_allowed_children(), 2u);
  QCOMPARE(tree1.root().max_allowed_children(), 2u);
  QCOMPARE(tree1.root().max_allowed_children(), 2u);

  binary_tree_t<int> tree4 = get_test_tree();
  size_t total =
    tree4.pre_order([](const binary_node_t<int>& node) {
      return node.max_allowed_children() != 2u;
    });
  QCOMPARE(total, 12u);
}

void TestBinaryTree::
test_bfs() {
  const std::vector<int> expected { 7, 4, 10, 2, 5, 8, 12, 1, 3, 6, 9, 11 };
  binary_tree_t<int> tree = get_test_tree();
  std::vector<int> visited;

  size_t total =
    tree.breadth_first_search(
      [&visited](binary_node_t<int>& node) {
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

void TestBinaryTree::
test_dfs() {
  const std::vector<int> expected { 7, 4, 2, 1, 3, 5, 6, 10, 8, 9, 12, 11 };
  binary_tree_t<int> tree = get_test_tree();
  std::vector<int> visited;

  size_t total =
    tree.depth_first_search(
      [&visited](binary_node_t<int>& node) {
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

void TestBinaryTree::
test_search_value() {
  binary_tree_t<int> tree = get_test_tree();
  size_t steps{ 0u };

  steps = tree.in_order([](auto& node) { return *node == 7; });
  QCOMPARE(steps, 7u);
  steps = tree.pre_order([](auto& node) { return *node == 7; });
  QCOMPARE(steps, 1u);
  steps = tree.pos_order([](auto& node) { return *node == 10; });
  QCOMPARE(steps, 12u);
  steps = tree.breadth_first_search([](auto& node) { return *node == 8; });
  QCOMPARE(steps, 6u);
  steps = tree.depth_first_search([](auto& node) { return *node == 12; });
  QCOMPARE(steps, 11u);
}
