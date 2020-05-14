#include "test_fenwick_tree.h"

#include <cmath>

using namespace advanced::structures;
using namespace test::structures;

TestFenwickTree::
TestFenwickTree(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestFenwickTree");
}

fenwick_tree_t<int, advanced::tools::sum_t<int>>
TestFenwickTree::
get_sum_tree() {
  fenwick_tree_t<int, advanced::tools::sum_t<int>> tree;
  const std::vector<int> input{
    2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 100, 10, 5, 20, 101,
    97, 36, 11, 75, 56, 47, 32, 8, 49, 53, 94, 98, 32, 4, 53,
    4, 81, 22, 8, 46, 73, 11, 52, 14, 33, 61, 36, 11, 86, 90
  };
  tree.rebuild(input);
  return tree;
}

fenwick_tree_t<double, advanced::tools::multiplication_t<double>>
TestFenwickTree::
get_multiplication_tree() {
  fenwick_tree_t<double, advanced::tools::multiplication_t<double>> tree;
  const std::vector<double> input{
    2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 100, 10, 5, 20, 101,
    97, 36, 11, 75, 56, 47, 32, 8, 49, 53, 94, 98, 32, 4, 53,
    4, 81, 22, 8, 46, 73, 11, 52, 14, 33, 61, 36, 11, 86, 90
  };
  tree.rebuild(input);
  return tree;
}

fenwick_tree_t<moc_fenwick_data, fenwick_join_transform_t>
TestFenwickTree::get_custom_transform_tree() {
  fenwick_tree_t<moc_fenwick_data, fenwick_join_transform_t> tree;
  const std::vector<moc_fenwick_data> input{
    { 2, 2.0 }, { 1, 1.0 }, { 1, 1.0 }, { 3, 3.0 },
    { 2, 2.0 }, { 3, 3.0 }, { 4, 4.0 }, { 5, 5.0 },
    { 6, 6.0 }, { 7, 7.0 }, { 8, 8.0 }, { 9, 9.0 },
    { 100, 100.0 }, { 10, 10.0 }, { 5, 5.0 }, { 20, 20.0 }, { 101, 101.0 },
    { 97, 97.0 }, { 36, 36.0 }, { 11, 11.0 }, { 75, 75.0 }, { 56, 56.0 },
    { 47, 47.0 }, { 32, 32.0 }, { 8, 8.0 }, { 49, 49.0 }, { 53, 53.0 },
    { 94, 94.0 }, { 98, 98.0 }, { 32, 32.0 }, { 4, 4.0 }, { 53, 53.0 },
    { 4, 4.0 }, { 81, 81.0 }, { 22, 22.0 }, { 8, 8.0 }, { 46, 46.0 },
    { 73, 73.0 }, { 11, 11.0 }, { 52, 52.0 }, { 14, 14.0 }, { 33, 33.0 },
    { 61, 61.0 }, { 36, 36.0 }, { 11, 11.0 }, { 86, 86.0 }, { 90, 90.0 }
  };
  tree.rebuild(input);
  return tree;
}

void TestFenwickTree::
test_default_constructor() {
  fenwick_tree_t<int, advanced::tools::sum_t<int>> tree;
  fenwick_tree_t<double, advanced::tools::multiplication_t<double>> mult;
  fenwick_tree_t<moc_fenwick_data, fenwick_join_transform_t> custom;
  QVERIFY(!tree.is_built());
  QVERIFY_EXCEPTION_THROWN(tree.query(0, 17), fenwick_not_built_exception_t);
  QVERIFY_EXCEPTION_THROWN(tree.update(0, 17), fenwick_not_built_exception_t);
  QVERIFY(!mult.is_built());
  QVERIFY_EXCEPTION_THROWN(mult.query(0, 17), fenwick_not_built_exception_t);
  QVERIFY_EXCEPTION_THROWN(mult.update(0, 17), fenwick_not_built_exception_t);
  QVERIFY(!custom.is_built());
  QVERIFY_EXCEPTION_THROWN(custom.query(0, 17), fenwick_not_built_exception_t);
  QVERIFY_EXCEPTION_THROWN(custom.update(0, { 17, 17.0 }),
                           fenwick_not_built_exception_t);
}

void TestFenwickTree::
test_move() {
  auto tree1{ get_sum_tree() };
  auto tree2{ get_multiplication_tree() };
  auto tree3{ get_custom_transform_tree() };
  auto sum_tree{ std::move(tree1) };
  auto mult_tree{ std::move(tree2) };
  auto custom_tree{ std::move(tree3) };
  QCOMPARE(sum_tree.query(0, 6), 12);
  QCOMPARE(mult_tree.query(0, 6), 36.0);

  auto obj{ custom_tree.query(0, 6) };
  QCOMPARE(obj.value_int,   12);
  QCOMPARE(obj.value_float, 12.0f);

  fenwick_tree_t<int, advanced::tools::sum_t<int>> other_sum_tree;
  other_sum_tree = std::move(sum_tree);
  QCOMPARE(sum_tree.size(), 0u);
  QVERIFY(!sum_tree.is_built());

  fenwick_tree_t<double, advanced::tools::multiplication_t<double>> other_mult_tree;
  other_mult_tree = std::move(mult_tree);
  QCOMPARE(mult_tree.size(), 0u);
  QVERIFY(!mult_tree.is_built());

  fenwick_tree_t<moc_fenwick_data, fenwick_join_transform_t> other_custom_tree;
  other_custom_tree = std::move(custom_tree);
  QCOMPARE(custom_tree.size(), 0u);
  QVERIFY(!custom_tree.is_built());

  QCOMPARE(other_sum_tree.query(0, 6), 12);
  QCOMPARE(other_mult_tree.query(0, 6), 36.0);

  auto other_obj{ other_custom_tree.query(0, 6) };
  QCOMPARE(other_obj.value_int,   12);
  QCOMPARE(other_obj.value_float, 12.0f);
}

void TestFenwickTree::
test_copy() {
  auto tree1{ get_sum_tree() };
  auto tree2{ get_multiplication_tree() };
  auto tree3{ get_custom_transform_tree() };
  auto sum_tree{ tree1 };
  auto mult_tree{ tree2 };
  auto custom_tree{ tree3 };
  QCOMPARE(sum_tree.query(0, 6), 12);
  QCOMPARE(mult_tree.query(0, 6), 36.0);

  auto obj{ custom_tree.query(0, 6) };
  QCOMPARE(obj.value_int,   12);
  QCOMPARE(obj.value_float, 12.0f);

  fenwick_tree_t<int, advanced::tools::sum_t<int>> other_sum_tree;
  other_sum_tree = sum_tree;
  QVERIFY(sum_tree.is_built());

  fenwick_tree_t<double, advanced::tools::multiplication_t<double>> other_mult_tree;
  other_mult_tree = mult_tree;
  QVERIFY(mult_tree.is_built());

  fenwick_tree_t<moc_fenwick_data, fenwick_join_transform_t> other_custom_tree;
  other_custom_tree = custom_tree;
  QVERIFY(custom_tree.is_built());

  QCOMPARE(other_sum_tree.query(0, 6), 12);
  QCOMPARE(other_mult_tree.query(0, 6), 36.0);

  auto other_obj{ other_custom_tree.query(0, 6) };
  QCOMPARE(other_obj.value_int,   12);
  QCOMPARE(other_obj.value_float, 12.0f);
}

void TestFenwickTree::
test_custom_constructor() {
  const size_t number_of_elements = 13u;
  fenwick_tree_t<int, advanced::tools::sum_t<int>> sum_tree {
    { 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 100, 10, 5, 20, 101,
      97, 36, 11, 75, 56, 47, 32, 8, 49, 53, 94, 98, 32, 4, 53,
      4, 81, 22, 8, 46, 73, 11, 52, 14, 33, 61, 36, 11, 86, 90 },
    number_of_elements
  };

  fenwick_tree_t<int, advanced::tools::multiplication_t<double>> mult_tree {
    { 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 100, 10, 5, 20, 101,
      97, 36, 11, 75, 56, 47, 32, 8, 49, 53, 94, 98, 32, 4, 53,
      4, 81, 22, 8, 46, 73, 11, 52, 14, 33, 61, 36, 11, 86, 90 },
    number_of_elements
  };

  fenwick_tree_t<moc_fenwick_data, fenwick_join_transform_t> custom_tree{
    {
      { 2, 2.0 }, { 1, 1.0 }, { 1, 1.0 }, { 3, 3.0 },
      { 2, 2.0 }, { 3, 3.0 }, { 4, 4.0 }, { 5, 5.0 },
      { 6, 6.0 }, { 7, 7.0 }, { 8, 8.0 }, { 9, 9.0 },
      { 100, 100.0 }, { 10, 10.0 }, { 5, 5.0 }, { 20, 20.0 }, { 101, 101.0 },
      { 97, 97.0 }, { 36, 36.0 }, { 11, 11.0 }, { 75, 75.0 }, { 56, 56.0 },
      { 47, 47.0 }, { 32, 32.0 }, { 8, 8.0 }, { 49, 49.0 }, { 53, 53.0 },
      { 94, 94.0 }, { 98, 98.0 }, { 32, 32.0 }, { 4, 4.0 }, { 53, 53.0 },
      { 4, 4.0 }, { 81, 81.0 }, { 22, 22.0 }, { 8, 8.0 }, { 46, 46.0 },
      { 73, 73.0 }, { 11, 11.0 }, { 52, 52.0 }, { 14, 14.0 }, { 33, 33.0 },
      { 61, 61.0 }, { 36, 36.0 }, { 11, 11.0 }, { 86, 86.0 }, { 90, 90.0 }
    },
    number_of_elements
  };

  QVERIFY(sum_tree.is_built());
  QCOMPARE(sum_tree.size(),    number_of_elements + 1);
  QVERIFY(mult_tree.is_built());
  QCOMPARE(mult_tree.size(),   number_of_elements + 1);
  QVERIFY(custom_tree.is_built());
  QCOMPARE(custom_tree.size(), number_of_elements + 1);
  QCOMPARE(sum_tree.query(0, 6), 12);
  QCOMPARE(mult_tree.query(0, 6), 36.0);

  auto obj{ custom_tree.query(0, 6) };
  QCOMPARE(obj.value_int,   12);
  QCOMPARE(obj.value_float, 12.0f);
}

void TestFenwickTree::
test_rebuild() {
  const size_t number_of_elements = 13u;
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto mult_tree{ get_multiplication_tree() };

  sum_tree.rebuild({ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 100, 10, 5, 20, 101,
                     97, 36, 11, 75, 56, 47, 32, 8, 49, 53, 94, 98, 32, 4, 53,
                     4, 81, 22, 8, 46, 73, 11, 52, 14, 33, 61, 36, 11, 86, 90 },
                   number_of_elements);
  mult_tree.rebuild({ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 100, 10, 5, 20, 101,
                      97, 36, 11, 75, 56, 47, 32, 8, 49, 53, 94, 98, 32, 4, 53,
                      4, 81, 22, 8, 46, 73, 11, 52, 14, 33, 61, 36, 11, 86, 90 },
                    number_of_elements);
  custom_tree.rebuild(
    {
      { 2, 2.0 }, { 1, 1.0 }, { 1, 1.0 }, { 3, 3.0 },
      { 2, 2.0 }, { 3, 3.0 }, { 4, 4.0 }, { 5, 5.0 },
      { 6, 6.0 }, { 7, 7.0 }, { 8, 8.0 }, { 9, 9.0 },
      { 100, 100.0 }, { 10, 10.0 }, { 5, 5.0 }, { 20, 20.0 }, { 101, 101.0 },
      { 97, 97.0 }, { 36, 36.0 }, { 11, 11.0 }, { 75, 75.0 }, { 56, 56.0 },
      { 47, 47.0 }, { 32, 32.0 }, { 8, 8.0 }, { 49, 49.0 }, { 53, 53.0 },
      { 94, 94.0 }, { 98, 98.0 }, { 32, 32.0 }, { 4, 4.0 }, { 53, 53.0 },
      { 4, 4.0 }, { 81, 81.0 }, { 22, 22.0 }, { 8, 8.0 }, { 46, 46.0 },
      { 73, 73.0 }, { 11, 11.0 }, { 52, 52.0 }, { 14, 14.0 }, { 33, 33.0 },
      { 61, 61.0 }, { 36, 36.0 }, { 11, 11.0 }, { 86, 86.0 }, { 90, 90.0 }
    },
    number_of_elements);

  QVERIFY(sum_tree.is_built());
  QCOMPARE(sum_tree.size(),     number_of_elements + 1);
  QVERIFY(mult_tree.is_built());
  QCOMPARE(mult_tree.size(),    number_of_elements + 1);
  QVERIFY(custom_tree.is_built());
  QCOMPARE(custom_tree.size(),  number_of_elements + 1);
  QCOMPARE(sum_tree.query(0, 6), 12);
  QCOMPARE(mult_tree.query(0, 6), 36.0);
  QCOMPARE(sum_tree.query(0,  number_of_elements),   sum_tree.query(0, 48u));
  QCOMPARE(mult_tree.query(0, number_of_elements),   mult_tree.query(0, 48u));
  QCOMPARE(custom_tree.query(0, number_of_elements), custom_tree.query(0, 48u));

  auto obj{ custom_tree.query(0, 6) };
  QCOMPARE(obj.value_int,   12);
  QCOMPARE(obj.value_float, 12.0f);
}

void TestFenwickTree::
test_update() {
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto mult_tree{ get_multiplication_tree() };
  int sum{ 0 };
  float sum2{ 0.0 };
  double mult{ 1 };

  QCOMPARE(sum_tree.query(0, 17), 287);
  QCOMPARE(mult_tree.query(0, 8), 720.0);

  auto obj{ custom_tree.query(0, 17) };
  QCOMPARE(obj.value_int,   287);
  QCOMPARE(obj.value_float, 287.0f);

  for (size_t ii = 1; ii <= 17u; ii++) {
    sum  += static_cast<int>(ii);
    sum2 += static_cast<float>(ii);
    sum_tree.update(ii, static_cast<int>(ii));
    custom_tree.update(ii, { static_cast<int>(ii), static_cast<float>(ii) });
  }

  for (size_t ii = 1; ii <= 8u; ii++) {
    mult *= 2.0;
    mult_tree.update(ii, 2);
  }

  obj = custom_tree.query(0, 17);
  QCOMPARE(sum_tree.query(0, 17), 287 + sum);
  QCOMPARE(obj.value_int,         287 + sum);
  QCOMPARE(obj.value_float,       287.0f + sum2);
  QCOMPARE(mult_tree.query(0, 8), 720.0 * mult);

  {
    auto size = custom_tree.size();
    QVERIFY(!custom_tree.update(size, { static_cast<int>(size),
                                           static_cast<float>(size)}));
    size = std::numeric_limits<size_t>::max();
    QVERIFY(!custom_tree.update(size, { static_cast<int>(size),
                                        static_cast<float>(size)}));
  }
  {
    auto size = sum_tree.size();
    QVERIFY(!sum_tree.update(size, static_cast<int>(size)));
    size = std::numeric_limits<size_t>::max();
    QVERIFY(!sum_tree.update(size, static_cast<int>(size)));
  }
  {
    auto size = mult_tree.size();
    QVERIFY(!mult_tree.update(size, static_cast<double>(size)));
    size = std::numeric_limits<size_t>::max();
    QVERIFY(!mult_tree.update(size, static_cast<double>(size)));
  }
}

void TestFenwickTree::
test_subscript_operator() {
  static const advanced::tools::multiplication_t<double> mult_transform;
  static const advanced::tools::sum_t<int>               sum_transform;
  static const fenwick_join_transform_t                  custom_transform;
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto mult_tree{ get_multiplication_tree() };

  {
    auto size = sum_tree.size();
    QCOMPARE(sum_tree[0], sum_transform.null_value());

    for (size_t ii = 1; ii < size; ii++) {
      QVERIFY(sum_tree[ii] != 0);
    }

    QVERIFY_EXCEPTION_THROWN(sum_tree[size], std::out_of_range);
    QVERIFY_EXCEPTION_THROWN(sum_tree[std::numeric_limits<size_t>::max()],
                             std::out_of_range);
  }
  {
    auto size = mult_tree.size();
    QCOMPARE(mult_tree[0], mult_transform.null_value());

    for (size_t ii = 1; ii < size; ii++) {
      QVERIFY(mult_tree[ii] != 0.0);
    }

    QVERIFY_EXCEPTION_THROWN(mult_tree[size], std::out_of_range);
    QVERIFY_EXCEPTION_THROWN(mult_tree[std::numeric_limits<size_t>::max()],
                             std::out_of_range);
  }
  {
    auto size = custom_tree.size();
    QCOMPARE(custom_tree[0].value_int,
             custom_transform.null_value().value_int);
    QCOMPARE(custom_tree[0].value_float,
             custom_transform.null_value().value_float);

    for (size_t ii = 1; ii < size; ii++) {
      QVERIFY(custom_tree[ii].value_int   != 0);
      QVERIFY(custom_tree[ii].value_float != 0.0f);
    }

    QVERIFY_EXCEPTION_THROWN(custom_tree[size], std::out_of_range);
    QVERIFY_EXCEPTION_THROWN(custom_tree[std::numeric_limits<size_t>::max()],
                             std::out_of_range);
  }
}

void TestFenwickTree::
test_at_getter() {
  static const advanced::tools::multiplication_t<double> mult_transform;
  static const advanced::tools::sum_t<int>               sum_transform;
  static const fenwick_join_transform_t                  custom_transform;
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto mult_tree{ get_multiplication_tree() };

  {
    auto size = sum_tree.size();
    QCOMPARE(sum_tree.at(0), sum_transform.null_value());

    for (size_t ii = 1; ii < size; ii++) {
      QVERIFY(sum_tree.at(ii) != 0);
    }

    QVERIFY_EXCEPTION_THROWN(sum_tree.at(size), std::out_of_range);
    QVERIFY_EXCEPTION_THROWN(sum_tree.at(std::numeric_limits<size_t>::max()),
                             std::out_of_range);
  }
  {
    auto size = mult_tree.size();
    QCOMPARE(mult_tree.at(0), mult_transform.null_value());

    for (size_t ii = 1; ii < size; ii++) {
      QVERIFY(mult_tree.at(ii) != 0.0);
    }

    QVERIFY_EXCEPTION_THROWN(mult_tree.at(size), std::out_of_range);
    QVERIFY_EXCEPTION_THROWN(mult_tree.at(std::numeric_limits<size_t>::max()),
                             std::out_of_range);
  }
  {
    auto size = custom_tree.size();
    QCOMPARE(custom_tree.at(0).value_int,
             custom_transform.null_value().value_int);
    QCOMPARE(custom_tree.at(0).value_float,
             custom_transform.null_value().value_float);

    for (size_t ii = 1; ii < size; ii++) {
      QVERIFY(custom_tree.at(ii).value_int   != 0);
      QVERIFY(custom_tree.at(ii).value_float != 0.0f);
    }

    QVERIFY_EXCEPTION_THROWN(custom_tree.at(size), std::out_of_range);
    QVERIFY_EXCEPTION_THROWN(custom_tree.at(std::numeric_limits<size_t>::max()),
                             std::out_of_range);
  }
}

void TestFenwickTree::
test_size_getter() {
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto mult_tree{ get_multiplication_tree() };

  QCOMPARE(custom_tree.size(), 48u);
  QCOMPARE(sum_tree.size(),    48u);
  QCOMPARE(mult_tree.size(),   48u);
}

void TestFenwickTree::
test_resize() {
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto mult_tree{ get_multiplication_tree() };

  custom_tree.resize(10u);
  QCOMPARE(custom_tree.size(), 10u);
  custom_tree.resize(100u);
  QCOMPARE(custom_tree.size(), 10u);

  sum_tree.resize(10u);
  QCOMPARE(sum_tree.size(), 10u);
  sum_tree.resize(100u);
  QCOMPARE(sum_tree.size(), 10u);

  mult_tree.resize(10u);
  QCOMPARE(mult_tree.size(), 10u);
  mult_tree.resize(100u);
  QCOMPARE(mult_tree.size(), 10u);
}

void TestFenwickTree::
test_reserve() {
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto mult_tree{ get_multiplication_tree() };

  custom_tree.reserve(100u);
  sum_tree.reserve(100u);
  mult_tree.reserve(100u);
  QCOMPARE(custom_tree.size(), 48u);
  QCOMPARE(sum_tree.size(),    48u);
  QCOMPARE(mult_tree.size(),   48u);
}

void TestFenwickTree::
test_is_built_getter() {
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto mult_tree{ get_multiplication_tree() };

  QCOMPARE(custom_tree.is_built(),  true);
  QCOMPARE(sum_tree.is_built(),     true);
  QCOMPARE(mult_tree.is_built(),    true);
}

void TestFenwickTree::
test_query() {
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto obj = custom_tree.query(7, 14);
  QCOMPARE(sum_tree.query(7, 14), 149);
  QCOMPARE(obj.value_int,   149);
  QCOMPARE(obj.value_float, 149.0f);

  obj = custom_tree.query(14, 7);
  QCOMPARE(sum_tree.query(14, 7), 149);
  QCOMPARE(obj.value_int,   149);
  QCOMPARE(obj.value_float, 149.0f);

  QCOMPARE(sum_tree.query(7, 27), 739);
  obj = custom_tree.query(7, 27);
  QCOMPARE(obj.value_int,   739);
  QCOMPARE(obj.value_float, 739.0f);

  QCOMPARE(sum_tree.query(27, 7), 739);
  obj = custom_tree.query(27, 7);
  QCOMPARE(obj.value_int,   739);
  QCOMPARE(obj.value_float, 739.0f);

  QCOMPARE(sum_tree.query(1, 47), 1660);
  obj = custom_tree.query(1, 47);
  QCOMPARE(obj.value_int,   1660);
  QCOMPARE(obj.value_float, 1660.0f);

  QCOMPARE(sum_tree.query(47, 1), 1660);
  obj = custom_tree.query(47, 1);
  QCOMPARE(obj.value_int,   1660);
  QCOMPARE(obj.value_float, 1660.0f);

  QCOMPARE(sum_tree.query(0, std::numeric_limits<size_t>::max()), 1660);
  obj = custom_tree.query(0, std::numeric_limits<size_t>::max());
  QCOMPARE(obj.value_int,   1660);
  QCOMPARE(obj.value_float, 1660.0f);

  QCOMPARE(sum_tree.query(std::numeric_limits<size_t>::max(), 0), 1660);
  obj = custom_tree.query(std::numeric_limits<size_t>::max(), 0);
  QCOMPARE(obj.value_int,   1660);
  QCOMPARE(obj.value_float, 1660.0f);
}

void TestFenwickTree::
test_set_value() {
  auto custom_tree{ get_custom_transform_tree() };
  auto sum_tree{ get_sum_tree() };
  auto mult_tree{ get_multiplication_tree() };
  int sum{ 0 };
  float sum2{ 0.0 };
  double mult{ 1 };

  QCOMPARE(sum_tree.query(11, 27),  717);
  QCOMPARE(mult_tree.query(11, 16), 7200000);

  auto obj{ custom_tree.query(11, 27) };
  QCOMPARE(obj.value_int,   717);
  QCOMPARE(obj.value_float, 717.0f);

  for (size_t ii = 11; ii <= 27u; ii++) {
    sum  += static_cast<int>(ii);
    sum2 += static_cast<float>(ii);
    QVERIFY(sum_tree.set_value(ii, static_cast<int>(ii)));
    QVERIFY(custom_tree.set_value(ii, { static_cast<int>(ii), static_cast<float>(ii) }));
  }

  for (size_t ii = 11; ii <= 16u; ii++) {
    mult *= 2;
    QVERIFY(mult_tree.set_value(ii, 2));
  }

  obj = custom_tree.query(11, 27);
  QCOMPARE(sum_tree.query(11, 27),  sum);
  QCOMPARE(obj.value_int,           sum);
  QCOMPARE(obj.value_float,         sum2);
  QCOMPARE(mult_tree.query(11, 16), mult);

  {
    auto size = custom_tree.size();
    QVERIFY(!custom_tree.set_value(size, { static_cast<int>(size),
                                           static_cast<float>(size)}));
    size = std::numeric_limits<size_t>::max();
    QVERIFY(!custom_tree.set_value(size, { static_cast<int>(size),
                                           static_cast<float>(size)}));
  }
  {
    auto size = sum_tree.size();
    QVERIFY(!sum_tree.set_value(size, static_cast<int>(size)));
    size = std::numeric_limits<size_t>::max();
    QVERIFY(!sum_tree.set_value(size, static_cast<int>(size)));
  }
  {
    auto size = mult_tree.size();
    QVERIFY(!mult_tree.set_value(size, static_cast<double>(size)));
    size = std::numeric_limits<size_t>::max();
    QVERIFY(!mult_tree.set_value(size, static_cast<double>(size)));
  }
}

void TestFenwickTree::
test_pointer() {
  const size_t number_of_elements = 13u;

  std::unique_ptr<fenwick_tree_t<int, advanced::tools::sum_t<int>>> sum_tree =
    std::make_unique<fenwick_tree_t<int, advanced::tools::sum_t<int>>>(
    std::vector<int>{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 100, 10, 5, 20, 101,
      97, 36, 11, 75, 56, 47, 32, 8, 49, 53, 94, 98, 32, 4, 53,
      4, 81, 22, 8, 46, 73, 11, 52, 14, 33, 61, 36, 11, 86, 90 },
    number_of_elements
  );

  std::unique_ptr mult_tree {
    std::make_unique<fenwick_tree_t<double, advanced::tools::multiplication_t<double>>> (
      std::vector<double>{ 2, 1, 1, 3, 2, 3, 4, 5, 6, 7, 8, 9, 100, 10, 5, 20, 101,
        97, 36, 11, 75, 56, 47, 32, 8, 49, 53, 94, 98, 32, 4, 53,
        4, 81, 22, 8, 46, 73, 11, 52, 14, 33, 61, 36, 11, 86, 90 },
      number_of_elements
    )
  };

  std::unique_ptr custom_tree {
    std::make_unique<fenwick_tree_t<moc_fenwick_data, fenwick_join_transform_t>>(
      std::vector<moc_fenwick_data>{
        { 2, 2.0 }, { 1, 1.0 }, { 1, 1.0 }, { 3, 3.0 },
        { 2, 2.0 }, { 3, 3.0 }, { 4, 4.0 }, { 5, 5.0 },
        { 6, 6.0 }, { 7, 7.0 }, { 8, 8.0 }, { 9, 9.0 },
        { 100, 100.0 }, { 10, 10.0 }, { 5, 5.0 }, { 20, 20.0 }, { 101, 101.0 },
        { 97, 97.0 }, { 36, 36.0 }, { 11, 11.0 }, { 75, 75.0 }, { 56, 56.0 },
        { 47, 47.0 }, { 32, 32.0 }, { 8, 8.0 }, { 49, 49.0 }, { 53, 53.0 },
        { 94, 94.0 }, { 98, 98.0 }, { 32, 32.0 }, { 4, 4.0 }, { 53, 53.0 },
        { 4, 4.0 }, { 81, 81.0 }, { 22, 22.0 }, { 8, 8.0 }, { 46, 46.0 },
        { 73, 73.0 }, { 11, 11.0 }, { 52, 52.0 }, { 14, 14.0 }, { 33, 33.0 },
        { 61, 61.0 }, { 36, 36.0 }, { 11, 11.0 }, { 86, 86.0 }, { 90, 90.0 }
      },
      number_of_elements
    )
  };
}
