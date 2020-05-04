#pragma once
#include <vector>
#include <unordered_map>
#include <utility>

namespace advanced {
namespace structures {

/** @test Todo */

/**
 * Implementation of union find generic data structure
 */
template <class T, bool  apply_compression = true>
class union_find_t {
  mutable std::vector<std::pair<T, size_t>> _items;
  mutable std::vector<size_t>               _ranks;
  size_t                                    _disjoint{ 0 };
  size_t                                    _acorn{ 0 };
  std::unordered_map<T, size_t>             _items_id;

public:

  /// Default not found value for find
  static const size_t not_found{ static_cast<size_t>(-1l) };

  union_find_t() = default;

  /**
   * It'll insert the elements in the initializer list
   */
  template<typename ...Args>
  union_find_t(Args&& ...args) {
    std::vector<T> items{ std::forward<Args>(args)... };
    reserve(items.size());
    for (size_t ii = 0; ii < items.size(); ++ii) {
      emplace(std::move(items[ii]), ii);
    }
  }

  void reserve(const size_t& capacity) {
    _items.reserve(capacity);
    _ranks.reserve(capacity);
    _items_id.reserve(capacity);
  }

  /**
   * It'll insert a copy of an element into the union set
   * The map cointainer defined in the template argument:
   *  map_container= std::map<T, size_t> will store all elements and save their
   * ids in the array.
   * @return "true" if the element doesn't exist in the map, "false" otherwise
   */
  bool insert(const T& elem) {
    bool already_exists { exists(elem) };
    if (!already_exists) {
      size_t id       = _items.size();
      _items_id[elem] = id;
      _ranks.push_back(1);
      _items.push_back({ elem, id });
      _disjoint++;
      _acorn++;
    }
    return !already_exists;
  }

  /**
   * It'll move an element into the union set
   * The map cointainer defined in the template argument:
   *  map_container= std::map<T, size_t> will store all elements and save their
   * ids in the array.
   * @return "true" if the element doesn't exist in the map, "false" otherwise
   */
  bool emplace(T&& elem) {
    bool already_exists { exists(elem) };
    if (!already_exists) {
      size_t id       = _items.size();
      _items_id[elem] = id;
      _items.emplace_back(std::make_pair<T, size_t>(std::move(elem), id));
      _ranks.push_back(1);
      _disjoint++;
      _acorn++;
    }
    return !already_exists;
  }

  /**
   * @param (key) key element
   * @return whether the value exists or not in the union set
   */
  bool exists(const T& key) const {
    return _items_id.count(key);
  }

  /**
   * @param (set) set id
   * @return whether the given set exists or not in the union set
   */
  bool exists_set(const size_t& set) const {
    return set < _items.size();
  }

  /**
   * @param (first)  first  element
   * @param (second) second element
   * @return whether the elements are in the same set or not
   * @note It'll call find methods internally, so it may apply
   * "path compression"
   * @see Please see find for more information
   */
  bool same_set(const T& first, const T& second) {
    return (exists(first) &&
            exists(second) &&
            _same_set(_items_id.at(first), _items_id.at(second)));
  }

  /**
   * @param (first)  first  set
   * @param (second) second set
   * @return whether the sets have the same root or not
   * @note It'll call find methods internally, so it may apply
   * "path compression"
   * @see Please see find for more information
   */
  bool same(const size_t& first, const size_t& second) {
    return exists(first) &&
           exists(second) &&
           _same_set(first, second);
  }

  /**
   * @param (elem) key element
   * @return the root set
   * @note It'll call find methods internally, so it may apply
   * "path compression"
   * @see Please see find for more information
   */
  size_t find_by_elem(const T& elem) {
    if (!exists(elem)) {
      return not_found;
    }
    return _find(_items_id.at(elem));
  }

  /**
   * @param (set) set id
   * @return the root set
   * @note It'll call find methods internally, so it may apply
   * "path compression". It means it'll make all possible children nodes to be
   * linked/joined to the root.
   */
  size_t find(const size_t& set) {
    if (!exists(set)) {
      return not_found;
    }
    return _find(set);
  }

  /**
   * @param (first)  first  set
   * @param (second) second set
   * @return whether the sets could be joined
   */
  bool join(const size_t& first, const size_t& second) {
    return exists(first)  &&
           exists(second) &&
           _join(_find(first), _find(second));
  }

  /**
   * @param (first)  first  element
   * @param (second) second element
   * @return whether the sets could be joined
   */
  bool join_sets(const T& first, const T& second) {
    return exists(first)  &&
           exists(second) &&
           _join(_find(_items_id[first]), _find(_items_id[second]));
  }

  /**
   * @return the number of disjoint sets in the union set
   */
  size_t disjoint() const {
    return _disjoint;
  }

  /**
   * @return the number of sets that has no parent
   */
  size_t acorn() const {
    return _acorn;
  }

  /**
   * @param set_id id of the set
   * @return the number of sets that are joined in this set
   */
  size_t size_of(size_t set_id) {
    auto root = find(set_id);
    return root != not_found ? _ranks[root] : 0;
  }

  /**
   * @param set_id id of the set
   * @return the number of sets that are joined in this set
   */
  size_t size_of(const T& elem) {
    auto root = find_by_elem(elem);
    return root != not_found ? _ranks[root] : 0;
  }

private:

  /**
   * Internal function
   * @param (set) set id
   * @return the root set
   * It'll make all possible children nodes to be linked/joined to the root.
   */
  size_t _find(const size_t& elem) {
    size_t& parent      { _items[elem].second };
    size_t& rank        { _ranks[elem]        };
    size_t& parent_rank { _ranks[parent]      };
    size_t  root        { not_found           };

    if (elem == parent) {
      // End of recursion
      root = elem;
    }
    else {
      if (apply_compression) {
        // Propagate it
        root = _find(parent);
        if (root != parent) {
          // Applying compression...
          if (parent_rank > 1) {
            // parent isn't compressed yet, so remove my height from it
            parent_rank -= rank;
          }
          else {
            // parent is already compressed, so my height is already in the root
            // then, zero it temporarily to not count it twice.
            rank = 0;
          }
          _join_set(elem, root);
        }
        // I'm a slave of the root now, so I have myself, and only myself.
        rank = 1;
      }
      else {
        // Propagate it
        root = _find(parent);
      }
    }
    return root;
  }

  /**
   * @param (first)  first  set
   * @param (second) second set
   * @return whether the sets have the same root or not
   * @note It'll call find methods internally, so it may apply
   * "path compression"
   * @see Please see find for more information
   */
  bool _same_set(const size_t& first, const size_t& second) {
    return first == second || _find(first) == _find(second);
  }

  /**
   * @param (first)  first  set
   * @param (second) second set
   * @return whether the sets could be joined
   */
  bool _join(const size_t& first,const size_t& second) {
    auto& first_rank { _ranks[first]             };
    auto& second_rank{ _ranks[second]            };
    bool same        { _same_set(first, second)  };
    if (!same) {
      // The chosen master will be the node which has the greater rank/height
      if (first_rank <= second_rank) {
        _join_set(first, second);
      }
      else {
        _join_set(second, first);
      }
      _disjoint--;
      _acorn -= static_cast<size_t>(!first_rank) +
                static_cast<size_t>(!second_rank);
    }
    return !same;
  }

  /**
   * @param (slave)  set to be joined
   * @param (master) set to join
   */
  void _join_set(const size_t& slave, const size_t& master) {
    auto& slave_rank { _ranks[slave]  };
    auto& master_rank{ _ranks[master] };

    _items[slave].second = master;
    master_rank += slave_rank;
  }
};
}
}
