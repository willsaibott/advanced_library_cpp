#pragma once
#include <unordered_map>
#include <list>
#include <cmath>


namespace advanced {
namespace structures {

/** @test TestLRUCache in test/test_lru_cache(.h|.cpp) */

/**
 * @brief Templated LRU Cache implementation
 * @see   https://www.geeksforgeeks.org/lru-cache-implementation/
 *
 * @note If you need to take some action when the LRU value is discarded,
 * override the "on_discard" method.
 */
template<typename key_t>
class cache_t {
  public:
  using list_t                    = std::list<key_t>;
  using iterator                  = typename std::list<key_t>::iterator;
  using const_iterator            = typename std::list<key_t>::const_iterator;
  using const_reverse_iterator    = typename std::list<key_t>::const_reverse_iterator;
  using map_t                     = std::unordered_map<key_t, const_iterator>;
  using map_iterator              = typename map_t::iterator;
  using value_t                   = key_t;

  cache_t()                       = default;
  cache_t(const cache_t&)         = default;
  cache_t(cache_t&&) noexcept     = default;
  inline cache_t&operator=(const cache_t&)     = default;
  inline cache_t&operator=(cache_t&&) noexcept = default;
  virtual ~cache_t() { }

  /**
   * @brief cache_t It constructs a cache object with a determinated capacity
   * @param capacity
   */
  cache_t(size_t capacity) : _max_capacity{ 0 } {
    set_capacity(capacity);
  }

  /**
   * Const search, it does not change the internal list order of elements
   * @return whether element is present or not
   */
  virtual bool
  contains(const key_t& key) const {
    return _map.count(key);
  }

  /**
   * @brief find  It searches in the LRU cache for a specified key, if it's
   *              found, the value will be moved to the front of the list, as
   *              the most recend used value.
   * @param key   key parameter to be found
   * @return      std::pair<bool, const_iterator>:
   *              { first:  whether the key was found or not
   *                second: const iterator to element found }
   */
  std::pair<bool, const_iterator>
  find(const key_t& key) {
    std::pair<bool, const_iterator> output{ false, {}};
    auto it{ _map.find(key )};
    if (it != _map.end()) {
      output.first  = true;
      output.second = it->second;
      move_to_front(it->second);
    }
    return output;
  }

  /**
   * @brief operator [] it adds a value if not found and returns a reference to
   *                    it
   * @param key      element to be added or searched
   * @return         returns a reference to the element added or found
   */
  inline const key_t&
  operator[] (const key_t& key) {
    if (!contains(key)) {
      (void)add(key);
    }
    else {
      _last_operation_result = false;
    }
    return *(find(key).second);
  }

  /**
   * @brief begin iterators for (for range)
   * @return start of list
   */
  inline const_iterator
  begin() const {
    return _list.begin();
  }

  /**
   * @brief end iterators for (for range)
   * @return end of list
   */
  inline const_iterator
  end() const {
    return _list.end();
  }

  /**
   * @brief begin iterators for (for range)
   * @return start of list
   */
  inline const_reverse_iterator
  rbegin() const {
    return _list.rbegin();
  }

  /**
   * @brief end iterators for (for range)
   * @return end of list
   */
  inline const_reverse_iterator
  rend() const {
    return _list.rend();
  }

  /**
   * @brief add It adds a key if it does not exist yet
   * @param key value to be inserted
   * @return    a reference to this
   * @note   If you need to know if the last insertion was successfull, call
   *         the last_operation method imediately after calling add.
   */
  cache_t&
  add(const key_t& key) {
    bool ok { !contains(key) };
    if (ok) {
      _list.push_front(key);
      _map[key] = _list.cbegin();
      (void) clip();
    }
    _last_operation_result = ok;
    return *this;
  }

  /**
   * @brief discards the least used recent values when the cache is full
   * @return reference to this
   * @note   use clipped method to get the ammount of clipped values
   */
  cache_t&
  clip() {
    _clipped_values = 0;
    while (_list.size() > _max_elements) {
      const auto& back { _list.back() };
      _map.erase(back);
      on_discard(back);
      _list.pop_back();
      _clipped_values++;
    }
    return *this;
  }

  /**
   * @brief clipped the number of clipped values in the last clip operation
   * @return  the number of clipped values in the last clip operation
   */
  inline size_t
  clipped() const {
    return _clipped_values;
  }

  /**
   * @brief last_add whether the last insertion was succesfull or not
   * @return  whether the last operation was succesfull or not
   */
  inline bool
  last_operation() const {
    return _last_operation_result;
  }

  /**
   * @brief set_max  it changes the max capacity of the LRU cache
   * @param capacity new capacity
   */
  void
  set_capacity(const size_t& capacity) {
    if (_max_capacity < capacity) {
      _max_capacity = capacity * (1 + std::log2(capacity));
      _map.reserve(_max_capacity);
    }

    _max_elements = capacity;
    clip();
  }

  /**
   * @brief capacity returns the current capacity of the LRU cache
   * @return         returns the current capacity of the LRU cache
   */
  size_t
  capacity() const {
    return _max_elements;
  }

  /**
   * @brief most_recent returns the most recent used value
   * @return            returns the most recent used value
   */
  const key_t&
  most_recent() const {
    return _list.front();
  }

  /**
   * @brief least_recent returns the least recent used value
   * @return             returns the least recent used value
   */
  const key_t&
  least_recent() const {
    return _list.back();
  }

  /**
   * @brief Access the list values orderd by most recent used to least recent
   *        used
   */
  const list_t&
  values() const {
    return _list;
  }

  /**
   * @brief remove  removes an element from cache
   * @param key     element to be removed from cache
   * @return    a reference to this
   * @note   If you need to know if the last removal was successfull, call
   *         the last_operation method imediately after calling remove.
   */
  cache_t&
  remove(const key_t& key) {
    auto it = _map.find(key);
    if (it != _map.end()) {
      _list.erase(it->second);
      _map.erase(key);
      _last_operation_result = true;
    }
    else {
      _last_operation_result = false;
    }
    return *this;
  }

  /**
   * @brief size returns the current number of elements in the LRU cache
   * @return     returns the current number of elements in the LRU cache
   */
  size_t
  size() const {
    return _list.size();
  }

  /**
   * @brief clear it will all entries in cache
   * @note it won't call on_discard
   */
  void
  clear() {
    _map.clear();
    _list.clear();
  }

  /**
   * @brief empty: returns
   * @return returns whether the cache is empty or not
   */
  inline bool
  empty() const {
    return size() == 0;
  }

  protected:

  /**
   * Access the map to list iterator as a const reference in extended classes
   */
  const map_t&
  map() const {
    return _map;
  }


  /**
   * Override it if you need do some action when the least recent used value is
   * discarded because the LRU cache is full
   */
  virtual void
  on_discard(const value_t& value)
  {  }

  private:

  /**
   * It moves the element to the begining of the list everytime it is referenced
   */
  void
  move_to_front(const_iterator it) {
    if (it != _list.begin()) {
      key_t key{ *it  };
      _list.erase(it);
      _list.push_front(key);
      _map[key] = _list.begin();
    }
  }

  size_t                                     _max_capacity{ 1024 };
  size_t                                     _max_elements{ 1024 };
  size_t                                     _clipped_values{ 0 };
  bool                                       _last_operation_result{ false };
  std::unordered_map<key_t, const_iterator>  _map;
  std::list<key_t>                           _list;

};
}
}
