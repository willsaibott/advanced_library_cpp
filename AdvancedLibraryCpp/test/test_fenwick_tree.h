#pragma once

#include <QObject>
#include <QTest>

#include "../structures/fenwick_tree.h"
#include "../tools/math_operation.h"

namespace test {
namespace structures {

class moc_fenwick_data {
public:

  moc_fenwick_data() = default;
  moc_fenwick_data(int v1, float v2) : value_int{ v1 }, value_float{ v2 }
  { }

  int   value_int{ 0 };
  float value_float{ 0 };

  inline operator
  bool () const {
    return value_int && value_float != 0.0f;
  }

  inline bool
  operator==(const moc_fenwick_data& other) const {
    return value_int == other.value_int && value_float == other.value_float;
  }
};

struct split_transform_t {
  inline moc_fenwick_data
  operator()(const moc_fenwick_data& d1, const moc_fenwick_data& d2) const {
    return { d1.value_int - d2.value_int, d1.value_float - d2.value_float };
  }
};

struct fenwick_join_transform_t {
  using reverse_t = split_transform_t;
  inline moc_fenwick_data
  operator()(const moc_fenwick_data& d1, const moc_fenwick_data& d2) const {
    return { d1.value_int + d2.value_int, d1.value_float + d2.value_float };
  }

  moc_fenwick_data
  null_value() const {
    static const moc_fenwick_data null_instance{ 0, 0.0f };
    return null_instance;
  }
};

}
}

class TestFenwickTree : public QObject
{
  Q_OBJECT
public:
  explicit TestFenwickTree(QObject *parent = nullptr);

  advanced::structures::fenwick_tree_t<int, advanced::tools::sum_t<int>>
  get_sum_tree();

  advanced::structures::fenwick_tree_t<double, advanced::tools::multiplication_t<double>>
  get_multiplication_tree();

  advanced::structures::fenwick_tree_t<test::structures::moc_fenwick_data,
                                       test::structures::fenwick_join_transform_t>
  get_custom_transform_tree();

private slots:

  void test_default_constructor();
  void test_move();
  void test_copy();
  void test_custom_constructor();
  void test_rebuild();
  void test_update();
  void test_subscript_operator();
  void test_at_getter();
  void test_size_getter();
  void test_resize();
  void test_reserve();
  void test_is_built_getter();
  void test_query();
  void test_set_value();
  void test_pointer();
};

