#pragma once
#include <vector>
#include <utility>

namespace advanced {

/**
 * Implementation of union find generic data structure
 */
class union_set_t {
  std::vector<std::pair<size_t, size_t>> _items;
  std::vector<size_t>                    _ranks;
  size_t                                 _disjoint{ 0 };
  size_t                                 _acorn{ 0 };
  bool                                   _apply_compression{ true };

public:

  /// Default not found value for find
  static const size_t not_found{ static_cast<size_t>(-1l) };

  /**
   * Default constructor { capacity: 32, apply_compression: true }
   */
  union_set_t() : union_set_t{ 32 } {}

  /**
   * @param capacity          initial union set size
   * @param apply_compression whether apply or not compression
   */
  union_set_t(const size_t& capacity, bool apply_compression = true) :
    _apply_compression{ apply_compression }
  {
    resize(capacity);
  }

  /**
   * Resizes the union set if the size is greater than the current size
   */
  bool resize(const size_t& size) {
    size_t old_size{ _items.size()   };
    bool resizable { size > old_size };
    if (resizable) {
      _items.resize(size);
      _ranks.resize(size);
      for (size_t ii = old_size; ii < size; ii++) {
        insert(ii);
      }
    }
    return resizable;
  }

  /**
   * It'll insert an element into the union set
   * @return "true" if the element doesn't exist in the map, "false" otherwise
   */
  bool insert(const size_t& elem) {
    bool can_be_inserted { exists(elem) };
    if (can_be_inserted) {
      _ranks[elem] = 0;
      _items[elem] = { elem, elem };
      _disjoint++;
      _acorn++;
    }
    return !can_be_inserted;
  }

  /**
   * @param (set) set id
   * @return whether the given set exists or not in the union set
   */
  bool exists(const size_t& set) const {
    return set < _items.size();
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
    return exists(first) && exists(second) && _same_set(first, second);
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

private:

  /**
   * Internal function
   * @param (set) set id
   * @return the root set
   * It'll make all possible children nodes to be linked/joined to the root.
   */
  size_t _find(const size_t& elem) {
    size_t& parent { _items[elem].second };
    size_t& rank   { _ranks[elem]        };
    size_t  root   { not_found           };

    if (elem == parent) {
      // End of recursion
      root = elem;
    }
    else {
      if (_apply_compression) {
        // Propagate it
        root = _find(parent);
        if (root != parent) {
          // I'm father of no one now, because I'm joining to the root
          rank = 0;
          _join_set(elem, root);
        }
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
  bool _join(const size_t& first, const size_t& second) {
    auto first_rank  { _ranks[first]            };
    auto second_rank { _ranks[second]           };
    bool same        { _same_set(first, second) };
    if (!same) {
      // The chosen master will be the node which has the greater rank/height
      if (first_rank <= second_rank) {
        _join_set(first, second);
      }
      else {
        _join_set(second, first);
      }
      _disjoint--;
      _acorn -= (size_t)(!first_rank) + (size_t)(!second_rank);
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
    master_rank += slave_rank + 1;
  }
};

}
