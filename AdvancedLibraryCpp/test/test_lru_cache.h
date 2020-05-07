#pragma once

#include <QObject>
#include <QTest>
#include <QVector>

#include <string>
#include "../structures/cache.h"

namespace test {
namespace structures {

class extended_cache : public advanced::structures::cache_t<std::string> {
public:

  const std::vector<std::string>&
  discarded_values () const {
    return _discarded_values;
  }

  size_t
  items_in_map() const {
    return map().size();
  }

  size_t
  items_in_list() const {
    return values().size();
  }

  // cache_t interface
protected:
  virtual void
  on_discard(const value_t& value) override;

  std::vector<std::string> _discarded_values;
};

class complex_data_type {
public:
  std::string  str;
  float        float_value{ -13 };
  int          int_value{ -13 };
  long         id;

  complex_data_type() : id{ instances++ } { }
  complex_data_type(long id) : id{ id } { }

  /**
   * @brief operator ==  defining comparators for hashing...
   * @param other
   * @return whether they have the same id
   */
  inline bool
  operator==(const complex_data_type& other) const {
    return id == other.id;
  }

  /**
   * @brief operator ==  defining comparators for hashing...
   * @param other
   * @return whether they have different ids
   */
  inline bool
  operator!=(const complex_data_type& other) const {
    return id != other.id;
  }

  inline static long instances{ 0 };
};

}
}


namespace std {

/**
 * Defining a hash definition for our mocked complex_data_type
 */
template <>
struct hash<test::structures::complex_data_type> {
 size_t
 operator()(const test::structures::complex_data_type & x) const {
   return hash<long>()(x.id);
 }
};

}


class TestLRUCache : public QObject
{
  Q_OBJECT
public:
  explicit TestLRUCache(QObject *parent = nullptr);

  template <class Iterator, class cache_t> void
  populate(Iterator first, Iterator last, cache_t& cache) {
    for (auto it = first; it != last; it++) {
      cache.add(*it);
    }
  }

private slots:

  void test_add_method_should_insert_values_into_the_lru_cache();
  void test_last_insertion_getter();
  void test_find_method_should_return_whether_a_value_exists_in_the_lru_cache();
  void test_find_method_should_reorder_the_lru_cache_by_most_recent_used();
  void test_operator_subscript_should_add_a_value_if_not_exists();
  void test_operator_subscript_should_search_for_a_value_if_exists();
  void test_most_recent_value();
  void test_least_recent_value();
  void test_clipped_getter();
  void test_clip_method_should_keep_the_capacity_consistent();
  void test_countains_method_should_return_whether_a_value_is_present_or_not();
  void test_set_and_get_capacity();
  void test_non_default_constructor();
  void test_on_discarded_method();
  void test_lru_cache_with_complex_datatypes();
  void test_for_range_loop();
  void test_remove_element_from_cache();
  void test_move_copy();
  void test_cache_as_pointers();
  void test_clear();
};

