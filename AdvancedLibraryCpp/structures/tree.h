#pragma once
#include <memory>
#include <vector>
#include <queue>
#include <stack>
#include <functional>
#include <algorithm>
#include <stdexcept>
#include <climits>

namespace advanced {
namespace structures {

/** @test Todo */

class null_node_exception_t : public std::runtime_error {
public:
  null_node_exception_t() :
    std::runtime_error{ "You are trying to access a null pointer node" } { }
};

template <class T, class node_type>
class tree_t;

class inode_t {
public:
  virtual ~inode_t(){ }

  virtual bool
  delete_child(size_t child) = 0;

  virtual size_t
  children_count() const = 0;

  virtual size_t
  max_allowed_children() const = 0;

  virtual void
  clear() = 0;
};

template <class T, class node_type_t>
class base_node_t : public inode_t {
public:
  using iterator               = typename std::vector<node_type_t*>::iterator;
  using const_iterator         = typename std::vector<node_type_t*>::const_iterator;
  using reverse_iterator       = typename std::vector<node_type_t*>::reverse_iterator;
  using const_reverse_iterator = typename std::vector<node_type_t*>::const_reverse_iterator;

  template <typename = typename std::enable_if<
              std::is_default_constructible<T>::value >
            ::type >
  base_node_t() { }

  template <typename = typename std::enable_if<
              std::is_copy_constructible<T>::value >
            ::type >
  base_node_t(const T& node, node_type_t* parent = nullptr)
    : _parent{ parent }, _node{ node }
  { }

  template <typename = typename std::enable_if<
              std::is_move_constructible<T>::value >
            ::type >
  base_node_t(T&& node, node_type_t* parent = nullptr)
      : _parent{ parent }, _node{ node }
  { }

  template <typename = typename std::enable_if<
              std::is_move_constructible<T>::value >
            ::type >
  base_node_t(base_node_t&& other) :
    _parent{ other._parent }, _node{ std::move(other._node) }
  {  }

  template <typename = typename std::enable_if<
              std::is_copy_constructible<T>::value >
            ::type >
  base_node_t(const base_node_t& other) :
    _parent{ other._parent },
    _node{ other._node }
  { }

  base_node_t& operator=(const base_node_t& other) = default;
  base_node_t& operator=(base_node_t&& other)      = default;

  virtual
  ~base_node_t() { }

  /**
   * @brief set_value set_value of a node
   * @param node
   */
  void
  set_value(const T& node) {
    _node = node;
  }

  /**
   * @brief set_value set_value of a node in place
   * @param node
   */
  void
  set_value(T&& node) {
    _node = std::move(node);
  }

  /**
   * @brief swap_move  swap using move if allowed
   * @param other
   * @return
   */
  typename std::enable_if<std::is_move_constructible<T>::value, void>::type
  swap_move(node_type_t& other) {
    T tmp       = std::move(other._node);
    other._node = std::move(_node);
    _node       = std::move(tmp);
  }

  /**
   * @brief swap  swap using normal copy
   * @param other
   * @return
   */
  typename std::enable_if<std::is_copy_constructible<T>::value, void>::type
  swap(node_type_t& other) {
    T tmp       = other._node;
    other._node = _node;
    _node       = tmp;
  }

  /**
   * @brief has_parent whether the node has or not a parent
   * @return
   */
  inline bool
  has_parent() const {
    return _parent;
  }

  /**
   * @brief parent  return it's parent
   * @return it's parent
   * @throws null_node_exception_t if parent is null
   */
  node_type_t&
  parent() {
    if (!_parent) {
      throw null_node_exception_t{};
    }
    return *_parent;
  } // LCOV_EXCL_LINE

  /**
   * @brief parent  return it's parent in a const context
   * @return
   * @throws null_node_exception_t if parent is null
   */
  const node_type_t&
  parent() const {
    if (!_parent) {
      throw null_node_exception_t{};
    }
    return *_parent;
  } // LCOV_EXCL_LINE

  /**
   * @brief operator T returns the value if casted to it
   */
  inline operator T() const {
    return _node;
  }

  /**
   * @brief operator * returns the value
   */
  inline T&
  operator*() {
    return _node;
  }

  /**
   * @brief operator * returns the value in a const context
   */
  inline const T&
  operator*() const {
    return _node;
  }

  /**
   * @brief get returns a reference to value
   * @return
   */
  inline T&get() {
    return _node;
  }

  /**
   * @brief get returns a const reference to value in a const context
   * @return
   */
  inline const T&
  get() const {
    return _node;
  }

  protected:
  virtual size_t in_order(const std::function<bool(node_type_t&)>&  func) = 0;
  virtual size_t in_order(const std::function<bool(const node_type_t&)>&  func) const = 0;
  virtual size_t pre_order(const std::function<bool(node_type_t&)>& func) = 0;
  virtual size_t pre_order(const std::function<bool(const node_type_t&)>& func) const = 0;
  virtual size_t pos_order(const std::function<bool(node_type_t&)>& func) = 0;
  virtual size_t pos_order(const std::function<bool(const node_type_t&)>& func) const = 0;

  /**
   * @brief set_parent    set node witch is the parent of this
   * @param parent
   */
  virtual void
  set_parent(node_type_t* parent) {
    _parent = parent;
  } // LCOV_EXCL_LINE

  node_type_t*              _parent{ nullptr };
  T                         _node;
};

template <class  T, size_t max_node_size = 2>
class tree_node_t : public base_node_t<T, tree_node_t<T, max_node_size>> {
  friend class tree_t<T, tree_node_t<T, max_node_size>>;

public:
  using node_type_t = tree_node_t<T, max_node_size>;
  using base_type_t = base_node_t<T, tree_node_t<T, max_node_size>>;
  using iterator               = typename base_type_t::iterator;
  using const_iterator         = typename base_type_t::const_iterator;
  using reverse_iterator       = typename base_type_t::reverse_iterator;
  using const_reverse_iterator = typename base_type_t::const_reverse_iterator;

  /**
   * Default construct if value type is also default constructible
   */
  template <typename = typename std::enable_if<
              std::is_default_constructible<T>::value >
            ::type >
  tree_node_t()
  { }

  /**
   * Copy construct if value type is also copy constructible
   */
  template <typename = typename std::enable_if<
              std::is_copy_constructible<T>::value >
            ::type >
  tree_node_t(const T& node, tree_node_t* parent = nullptr)
    : base_node_t<T, tree_node_t<T, max_node_size> > (node, parent)
  { }

  /**
   * Move construct if value type is also move constructible
   */
  template <typename = typename std::enable_if<
              std::is_move_constructible<T>::value >
            ::type >
  tree_node_t(T&& node, tree_node_t* parent = nullptr)
      : base_node_t<T, tree_node_t<T, max_node_size> > (node, parent)
  { }

  template <typename = typename std::enable_if<
              std::is_move_constructible<T>::value >
            ::type >
  tree_node_t(tree_node_t&& other) :
    base_node_t<T, tree_node_t<T, max_node_size> > (other)
  {  }

  /**
   * Copy construct if value type is also copy constructible
   */
  template <typename = typename std::enable_if<
              std::is_copy_constructible<T>::value >
            ::type >
  tree_node_t(const tree_node_t& other) :
    base_node_t<T, tree_node_t<T, max_node_size> > (other._node, other._parent)
  {
    for (const auto& elem : other._children) {
      add_child(elem);
    }
  }

  tree_node_t& operator=(const tree_node_t& other) = default;
  tree_node_t& operator=(tree_node_t&& other)      = default;

  virtual
  ~tree_node_t() {
    clear();
  }

  /**
   * @brief add_child add a child node in the end of the container. It
   * calls internally std::vector<node*>::push_back()
   * @param child
   * @return whether was allowed to add a new node
   */
  virtual bool
  add_child(const T& child) {
    bool ok{ _children.size() < max_node_size };
    if (ok) {
      _children.push_back(new node_type_t{ child, this });
    }
    return ok;
  } // LCOV_EXCL_LINE

  /**
   * @brief insert_at_first_null It will ocupy the first null node, it means,
   * the first spot available
   * @param child
   * @return whether was allowed to add a new node
   */
  bool
  insert_at_first_null(const T& child) {
    bool inserted{ false };
    auto it = std::find(_children.begin(), _children.end(), nullptr);
    if ((inserted = (it != _children.end()))) {
      *it = new node_type_t{ child, this };
    }
    else {
      inserted = add_child(child);
    }
    return inserted;
  } // LCOV_EXCL_LINE

  /**
   * @brief insert_at it will try to insert a node at a given position.
   * If the position is greater than or equal to the current container's size,
   * it will resize the internal container filling with nullptr pointers,
   * and insert it in the given position.
   * If the position is lower than the curren container's size, it will check
   * if the position is already occupied, if it its, the insertion doesn't take
   * place, and the method returns false, otherwise, the node is created at the
   * given position
   *
   * @param position   position to be inserted
   * @param child
   * @return whether the insertion was successfull or not.
   */
  bool
  insert_at(size_t position, const T& child) {
    bool ok { position < max_node_size };
    if (ok) {
      if (position < _children.size()) {
        ok = _children[position] == nullptr;
        if (ok) {
          _children[position] = new node_type_t{ child, this };
        }
      }
      else {
        _children.resize(position + 1, nullptr);
        _children[position] = new node_type_t{ child, this };
      }
    }
    return ok;
  } // LCOV_EXCL_LINE

  /**
   * @brief replace_at it will replace a node at a given position.
   * If the position is greater than or equal to the current container's size,
   * it will resize the internal container filling with nullptr pointers,
   * and insert it in the given position.
   * If the position is lower than the curren container's size, it will check
   * If the position is already occupied, if it its, the replaced node will be
   * returned and a new node will be created in that position, otherwise,
   * the node is created at the given position
   * If the position is greater than the max allowed number of children,
   * nothing will change, and a nullptr will be returned
   *
   * @param position   position to be replaced
   * @param child
   * @return the replaced node if there was one, nullptr otherwise
   */
  std::unique_ptr<node_type_t>
  replace_at(size_t position, const T& child) {
    bool ok { position < max_node_size };
    std::unique_ptr<node_type_t> replaced{ nullptr };
    if (ok) {
      if (position < _children.size()) {
        if (_children[position] != nullptr) {
          replaced.reset(_children[position]);
          replaced->set_parent(nullptr);
          _children[position] = nullptr;
        }
      }
      else {
        _children.resize(position + 1, nullptr);
      }
    }
    _children[position] = new node_type_t{ child, this };
    return replaced;
  }

  /**
   * @brief delete_child it deletes a child node indexed by param child
   * and removes the entry for node inside the container
   * @param child index of the deleted child
   *
   * @result  node: { child1, child2, ... child_(n-1), child_(n+1), ... }
   * @return
   */
  virtual bool
  delete_child(size_t child) override {
    bool ok { child < _children.size() };
    if (ok) {
      auto ptr = _children[child];
      if (ptr) {
        delete ptr;
      }
      _children.erase(_children.begin() + child);
    }
    return ok;
  }

  /**
   * @brief extract_child   It removes a child node placing nullptr, it means
   * it doesn't removes the entry in the container.
   * Also, once you do that, the ownership of the object is transfered to a
   * std::unique_ptr object
   *
   * @result  node: { child1, child2, ... nullptr, child_(n+1), ... }
   *
   * @param child
   * @return std::unique_ptr object of the node extract
   */
  std::unique_ptr<node_type_t>
  extract_child(size_t child) {
    std::unique_ptr<node_type_t> node{ nullptr };
    if (child < _children.size()) {
      if (_children[child]) {
        node.reset(_children[child]);
        node->set_parent(nullptr);
        _children[child] = nullptr;
      }
    }
    return node;
  }

  /**
   * @brief has_child whether the node has or not the specified child
   * @param pos position of child
   * @return whether the node has or not the specified child
   */
  bool
  has_child(size_t pos) const noexcept {
    return pos < _children.size() && _children[pos];
  }

  /**
   * @brief clear it removes all children
   */
  virtual void
  clear() final override {
    for (const auto ptr : _children) {
      if (ptr) { delete ptr; }
    }
    _children.clear();
  }

  /**
   * @brief children_count
   * @return the number of non-null nodes
   */
  virtual size_t
  children_count() const override {
    const auto nulls { std::count(_children.begin(),
                                  _children.end(),
                                  nullptr) };
    return _children.size() - nulls;
  }

  /**
   * @brief find  it searches for a child in the list
   * @param child
   * @return a pair, containing { bool, size_t }.
   * first:  whether the element was found or not,
   * second: the position of the found element if found, otherwise,
   *         std::numeric_limits<size_t>::max()
   */
  std::pair<bool, size_t>
  find(const T& child) const {
    std::pair<bool, size_t> result;
    auto pred{
      [&child](const node_type_t* const ptr) {
        return ptr && ptr->get() == child;
      }};
    const auto it = std::find_if(_children.begin(), _children.end(), pred);

    if (it != _children.end()) {
      result = { true,  std::distance(_children.begin(), it) };
    }
    else {
      result = { false, std::numeric_limits<size_t>::max() };
    }
    return result;
  }

  /**
   * @brief child   get child at position pos
   * @param pos index of child
   * @return reference to child node
   * @throw null_node_exception_t if accessing non valid child
   * @throw std::out_of_range if index points to a node that is not inserted yet
   */
  node_type_t&
  child(size_t pos) {
    if (pos < _children.size()) {
      if (_children[pos]) {
        return *_children.at(pos);
      }
      else {
        throw null_node_exception_t{};
      }
    }
    throw std::out_of_range {
      "This node only have " + std::to_string(_children.size()) +
      " children nodes. You accessed: " + std::to_string(pos)
    };
  }

  /**
   * @brief first returns the first non-null child
   * @return
   */
  node_type_t&
  first() {
    auto pred{[](const node_type_t* const ptr) { return ptr; } };
    const auto it = std::find_if(_children.begin(), _children.end(), pred);
    if (it != _children.end()) {
      return (**it);
    }
    else {
      throw null_node_exception_t{};
    }
  } // LCOV_EXCL_LINE

  /**
   * @brief first returns the first non-null child
   * @return
   */
  const node_type_t&
  first() const {
    auto pred{[](const node_type_t* const ptr) { return ptr; } };
    const auto it = std::find_if(_children.begin(), _children.end(), pred);
    if (it != _children.end()) {
      return (**it);
    }
    else {
      throw null_node_exception_t{};
    }
  } // LCOV_EXCL_LINE

  /**
   * @brief last returns the last non-null node
   * @return
   */
  node_type_t&
  last() {
    auto pred{[](const node_type_t* const ptr) { return ptr; } };
    const auto it = std::find_if(_children.rbegin(), _children.rend(), pred);
    if (it != _children.rend()) {
      return (**it);
    }
    else {
      throw null_node_exception_t{};
    }
  } // LCOV_EXCL_LINE

  /**
   * @brief last returns the last non-null node
   * @return
   */
  const node_type_t&
  last() const {
    auto pred{[](const node_type_t* const ptr) { return ptr; } };
    const auto it = std::find_if(_children.rbegin(), _children.rend(), pred);
    if (it != _children.rend()) {
      return (**it);
    }
    else {
      throw null_node_exception_t{};
    }
  } // LCOV_EXCL_LINE

  /**
   * @brief child   get child at position pos
   * @param pos index of child
   * @return const reference to a child node
   * @throw null_node_exception_t if accessing non valid child
   * @throw std::out_of_range if index points to a node that is not inserted yet
   */
  const node_type_t&
  child(size_t pos) const {
    if (pos < _children.size()) {
      if (_children[pos]) {
        return *_children.at(pos);
      }
      else {
        throw null_node_exception_t{};
      }
    }
    throw std::out_of_range {
      "This node only have " + std::to_string(_children.size()) +
      " children nodes. You accessed: " + std::to_string(pos)
    };
  }

  /**
   * @brief begin
   * @return
   */
  const_iterator
  begin() const {
    return _children.begin();
  }

  /**
   * @brief end
   * @return
   */
  const_iterator
  end() const {
    return _children.end();
  }

  /**
   * @brief cbegin
   * @return
   */
  const_iterator
  cbegin() const {
    return _children.begin();
  }

  /**
   * @brief cend
   * @return
   */
  const_iterator
  cend() const {
    return _children.end();
  }

  /**
   * @brief rbegin
   * @return
   */
  const_reverse_iterator
  rbegin() const {
    return _children.rbegin();
  }

  /**
   * @brief rend
   * @return
   */
  const_reverse_iterator
  rend() const {
    return _children.rend();
  }

  /**
   * @brief crbegin
   * @return
   */
  const_reverse_iterator
  crbegin() {
    return _children.crbegin();
  }

  /**
   * @brief crend
   * @return
   */
  const_reverse_iterator
  crend() {
    return _children.crend();
  }

  /**
   * @brief max_allowed_children  max children nodes by node
   * @return max children nodes by node
   */
  virtual size_t
  max_allowed_children() const override {
    return max_node_size;
  }

protected:
  /**
   * @brief in_order recursive in-order trasversal
   * @param func
   * @return number of nodes
   */
  virtual size_t
  in_order(const std::function<bool (node_type_t&)> &func) override {
    static bool recursion_break{ false };
    size_t visited{ 1u };
    size_t calls{ std::max<size_t>(_children.size() - 1u, 1u) };
    recursion_break = false;

    if (_children.empty()) {
      func(*this);
    }
    else {
      for (auto it = _children.begin(); it != _children.end(); it++) {
        auto ptr { *it };
        if (ptr) {
          visited += ptr->in_order(func);
          if (recursion_break) {
            break; // LCOV_EXCL_LINE
          }
        }
        if (calls--) {
          if ((recursion_break = func(*this))) {
            break; // LCOV_EXCL_LINE
          }
        }
      }
    }

    return visited;
  }

  /**
   * @brief pre_order recursive pre-order trasversal
   * @param func
   * @return number of nodes
   */
  virtual size_t
  pre_order(const std::function<bool (node_type_t&)> &func) override {
    size_t visited{ 1u };
    if (!func(*this)) {
      for (auto it = _children.begin(); it != _children.end(); it++) {
        auto ptr { *it };
        if (ptr) {
          visited += ptr->pre_order(func);
        }
      }
    }
    return visited;
  }

  /**
   * @brief pos_order recursive pos-order trasversal
   * @param func
   * @return number of nodes
   */
  virtual size_t
  pos_order(const std::function<bool (node_type_t&)> &func) override {
    size_t visited{ 1u };
    for (auto it = _children.begin(); it != _children.end(); it++) {
      auto ptr { *it };
      if (ptr) {
        visited += ptr->pos_order(func);
      }
    }
    func(*this);
    return visited;
  }

  /**
   * @brief in_order recursive in-order trasversal
   * @param func
   * @return number of nodes
   */
  virtual size_t
  in_order(const std::function<bool(const node_type_t&)> &func) const override {
    static bool recursion_break{ false };
    size_t visited{ 1u };
    size_t calls{ std::max<size_t>(_children.size() - 1u, 1u) };
    recursion_break = false;

    if (_children.empty()) {
      func(*this);
    }
    else {
      for (auto it = _children.begin(); it != _children.end(); it++) {
        auto ptr { static_cast<const node_type_t* const>(*it) };
        if (ptr) {
          visited += ptr->in_order(func);
          if (recursion_break) {
            break; // LCOV_EXCL_LINE
          }
        }
        if (calls--) {
          if ((recursion_break = func(*this))) {
            break; // LCOV_EXCL_LINE
          }
        }
      }
    }
    return visited;
  }

  /**
   * @brief pre_order recursive pre-order trasversal
   * @param func
   * @return number of nodes
   */
  virtual size_t
  pre_order(const std::function<bool (const node_type_t&)> &func) const override {
    size_t visited{ 1u };
    if (!func(*this)) {
      for (auto it = _children.begin(); it != _children.end(); it++) {
        auto ptr { static_cast<const node_type_t* const>(*it) };
        if (ptr) {
          visited += ptr->pre_order(func);
        }
      }
    }
    return visited;
  }

  /**
   * @brief pos_order recursive pos-order trasversal
   * @param func
   * @return number of nodes
   */
  virtual size_t
  pos_order(const std::function<bool (const node_type_t&)> &func) const override {
    size_t visited{ 1u };
    for (auto it = _children.begin(); it != _children.end(); it++) {
      auto ptr { static_cast<const node_type_t* const>(*it) };
      if (ptr) {
        visited += ptr->pos_order(func);
      }
    }
    func(*this);
    return visited;
  }

  /**
   * @brief children  get children as a vector
   * @return
   */
  std::vector<node_type_t*>
  children() {
    return _children;
  }

  std::vector<node_type_t*> _children;
};

/**
 * @brief The base_tree_t class. Tree Interface class
 */
class base_tree_t {
public:
  base_tree_t()                                       = default;
  base_tree_t(const base_tree_t& other)               = default;
  base_tree_t(base_tree_t&& other) noexcept           = default;
  base_tree_t&operator=(const base_tree_t& other)     = default;
  base_tree_t&operator=(base_tree_t&& other) noexcept = default;
  virtual ~base_tree_t(){ }
  virtual void clear()          = 0;
  virtual bool has_root() const = 0;
};

/**
 * @brief Basic templated Tree class.
 * @param T: value inserted in the tree
 * @param node_type_t: node type in the tree
 *
 * @test ../tests/test_tree.cpp
 */
template <class T, class node_type_t>
class tree_t : public base_tree_t {
public:

  /**
   * @brief ~tree_t  it calls clear()
   */
  virtual
  ~tree_t() {
    clear();
  }

  /**
   * @brief tree_t  simple default constructor
   * @param root
   */
  tree_t(const T& root) : _root{ new node_type_t{ root } }
  {  }

  tree_t() = default;

  /**
   * @brief tree_t  copy constructor, it copies only the root object
   * @param other
   */
  tree_t(const tree_t& other) {
    *this = other;
  }

  /**
   * @brief tree_t  move constructor, it transfer the ownership from the
   * original object to this object created
   * @param other
   */
  tree_t(tree_t&& other) {
    *this = std::move(other);
  }

  /**
   * @brief operator = copy all elements in root
   * @param other
   * @return reference to this
   */
  tree_t&
  operator=(const tree_t& other) {
    if (other.has_root()) {
      _root = new node_type_t();
      *_root = other.root();
    }
    return *this;
  } // LCOV_EXCL_LINE

  /**
   * @brief operator = transfer the ownership of root pointer
   * @param other
   * @return reference to this
   */
  tree_t&
  operator=(tree_t&& other) {
    _root = other._root;
    other._root = nullptr;
    return *this;
  }

  /**
   * @brief root  it gets the root of three
   * @return
   * @throws null_node_exception_t if root is null
   */
  node_type_t&
  root() {
    if (!_root) {
      throw null_node_exception_t{};
    }
    return *_root;
  } // LCOV_EXCL_LINE

  /**
   * @brief root  it gets the root of three
   * @return
   * @throws null_node_exception_t if root is null
   */
  const node_type_t&
  root() const {
    if (!_root) {
      throw null_node_exception_t{};
    }
    return *_root;
  } // LCOV_EXCL_LINE

  /**
   * @brief in_order it executes an in order search
   * @param func function that if it returns true, the search will end at this
   * node
   * @return visited nodes
   */
  size_t
  in_order(const std::function<bool(node_type_t&)>& func) {
    return this->has_root() ? root().in_order(func) : 0u;
  } // LCOV_EXCL_LINE

  /**
   * @brief pre_order it executes a pre order search
   * @param func function that if it returns true, the search will end at this
   * node
   * @return visited nodes
   */
  size_t
  pre_order(const std::function<bool(node_type_t&)>& func) {
    return this->has_root() ? root().pre_order(func) : 0u;
  } // LCOV_EXCL_LINE

  /**
   * @brief pos_order it executes a pos order search
   * @param func function that if it returns true, the search will end at this
   * node
   * @return visited nodes
   */
  size_t
  pos_order(const std::function<bool(node_type_t&)>& func) {
    return this->has_root() ? root().pos_order(func) : 0u;
  } // LCOV_EXCL_LINE

  /**
   * @brief in_order it executes an in order search
   * @param func function that if it returns true, the search will end at this
   * node
   * @return visited nodes
   */
  size_t
  in_order(const std::function<bool(const node_type_t&)>& func) const {
    return this->has_root() ? root().in_order(func) : 0u;
  } // LCOV_EXCL_LINE

  /**
   * @brief pre_order it executes a pre order search
   * @param func function that if it returns true, the search will end at this
   * node
   * @return visited nodes
   */
  size_t
  pre_order(const std::function<bool(const node_type_t&)>& func) const {
    return this->has_root() ? root().pre_order(func) : 0u;
  } // LCOV_EXCL_LINE

  /**
   * @brief pos_order it executes a pos order search
   * @param func function that if it returns true, the search will end at this
   * node
   * @return visited nodes
   */
  size_t
  pos_order(const std::function<bool(const node_type_t&)>& func) const {
    return this->has_root() ? root().pos_order(func) : 0u;
  } // LCOV_EXCL_LINE

  /**
   * @brief add_root it adds a root object
   * @param root
   */
  tree_t&
  add_root(const T& root) {
    delete_root();
    _root = new node_type_t(root, nullptr);
    return *this;
  } // LCOV_EXCL_LINE

  /**
   * @brief has_root  if this root has or not a non-null root node
   * @return if this root has or not a non-null root node
   */
  inline virtual bool
  has_root() const override {
    return _root;
  } // LCOV_EXCL_LINE

  /**
   * @brief clear it will erase all nodes in the tree
   */
  virtual void
  clear() override {
    delete_root();
  }

  /**
   * @brief breadth_first_search executes a breadth first search,
   * @param pred  predicate function to be called, it must return a bool
   * that if true, the BFS will stop at this node, and run while it returns
   * false.
   * @return the number of visited nodes
   */
  size_t
  breadth_first_search(const std::function<bool(node_type_t&)>& pred) {
    size_t visited{ 0u };
    node_type_t* node{ this->_root };
    bool stop{ false };
    if (has_root()) {
      std::queue<node_type_t*> queue;
      queue.push(node);
      while (!stop && !queue.empty()) {
        node = queue.front(); queue.pop();
        stop = pred(*node);
        visited++;
        for (auto child : node->children()) {
          if (child) {
            queue.push(child);
          }
        }
      }
    }
    return visited;
  } // LCOV_EXCL_LINE

  /**
   * @brief breadth_first_search const version: executes a breadth first search.
   * @param pred  predicate function to be called, it must return a bool
   * that if true, the BFS will stop at this node, and run while it returns
   * false.
   * @return the number of visited nodes
   */
  size_t
  breadth_first_search(const std::function<bool(const node_type_t&)>& pred) const {
    size_t visited{ 0u };
    node_type_t* node{ this->_root };
    bool stop{ false };
    if (has_root()) {
      std::queue<node_type_t*> queue;
      queue.push(node);
      while (!stop && !queue.empty()) {
        node = queue.front(); queue.pop();
        stop = pred(*node);
        visited++;
        for (auto child : node->children()) {
          if (child) {
            queue.push(child);
          }
        }
      }
    }
    return visited;
  } // LCOV_EXCL_LINE

  /**
   * @brief depth_first_search executes a depth first search,
   * @param pred  predicate function to be called, it must return a bool
   * that if true, the DFS will stop at this node, and run while it returns
   * false.
   * @return the number of visited nodes
   */
  size_t
  depth_first_search(const std::function<bool(node_type_t&)>& pred) {
    size_t visited{ 0u };
    node_type_t* node{ this->_root };
    bool stop{ false };
    if (has_root()) {
      std::stack<node_type_t*> queue;
      queue.push(node);
      while (!stop && !queue.empty()) {
        node = queue.top(); queue.pop();
        stop = pred(*node);
        visited++;
        auto children { node->children() };
        for (auto it = children.rbegin(); it != children.rend(); it++) {
          auto child{ *it };
          if (child) {
            queue.push(child);
          }
        }
      }
    }
    return visited;
  } // LCOV_EXCL_LINE

  /**
   * @brief depth_first_search const version: executes a depth first search,
   * @param pred  predicate function to be called, it must return a bool
   * that if true, the DFS will stop at this node, and run while it returns
   * false.
   * @return the number of visited nodes
   */
  size_t
  depth_first_search(const std::function<bool(const node_type_t&)>& pred) const {
    size_t visited{ 0u };
    node_type_t* node{ this->_root };
    bool stop{ false };
    if (has_root()) {
      std::stack<node_type_t*> queue;
      queue.push(node);
      while (!stop && !queue.empty()) {
        node = queue.top(); queue.pop();
        stop = pred(*node);
        visited++;
        auto children { node->children() };
        for (auto it = children.rbegin(); it != children.rend(); it++) {
          auto child{ *it };
          if (child) {
            queue.push(child);
          }
        }
      }
    }
    return visited;
  } // LCOV_EXCL_LINE

  protected:

  /**
   * @brief delete_root it deletes the root alongside with allnodes in the tree
   */
  void
  delete_root() {
    if (_root) {
      delete _root;
      _root = nullptr;
    }
  }

  node_type_t* _root{ nullptr };
};

}
}
