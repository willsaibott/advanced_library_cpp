#pragma once
#include <memory>
#include <list>
#include <algorithm>
#include <stdexcept>

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
};

template <class T, class node_type_t>
class base_node_t : public inode_t {
public:


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

  virtual bool
  delete_child(size_t child) = 0;

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

  virtual inline size_t
  children_count() const = 0;

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

  protected:

  /**
   * @brief set_parent    set node witch is the parent of this
   * @param parent
   */
  virtual void
  set_parent(node_type_t* parent) {
    _parent = parent;
  } // LCOV_EXCL_LINE

  node_type_t* _parent{ nullptr };
  T            _node;
};

template <class T, size_t max_node_size = 2>
class tree_node_t : public base_node_t<T, tree_node_t<T, max_node_size>> {
  friend class tree_t<T, tree_node_t<T, max_node_size>>;

public:
  using node_type_t = tree_node_t<T, max_node_size>;

  /**
   * @brief add_child add a child node
   * @param child
   * @return
   */
  virtual bool
  add_child(const T& child) {
    bool ok{ _children.size() < max_node_size };
    if (ok) {
      _children.push_back(new node_type_t{ child, this });
    }
    return ok;
  }

  /**
   * @brief delete_child it deletes a child node indexed by param child
   * @param child index of the deleted child
   * @return
   */
  virtual bool
  delete_child(size_t child) override {
    bool ok { child < _children.size() };
    if (ok) {
      auto it = _children.erase(_children.begin() + child);
      if (*it) {
        delete *it;
      }
    }
    return ok;
  }

  /**
   * @brief child   get child at position pos
   * @param pos index of child
   * @return
   */
  node_type_t&
  child(size_t pos) {
    return *_children.at(pos);
  }

  /**
   * @brief children_count  number of children nodes
   * @return
   */
  virtual inline size_t
  children_count() const override {
    return _children.size();
  }

  /**
   * Default construct if value type is also default constructible
   */
  template <typename = typename std::enable_if<
              std::is_default_constructible<T>::value >
            ::type >
  tree_node_t() { }

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
    base_node_t<T, tree_node_t<T, max_node_size> > (other._node, other._parent),
    _children(std::move(other._children))
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
    for (const auto ptr : _children) {
      if (ptr) { delete ptr; }
    }
    _children.clear();
  }

  protected:
  std::list<node_type_t*> _children;
};

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

template <class T, class node_type>
class tree_t : public base_tree_t {
public:

  virtual
  ~tree_t() {
    delete_root();
  }

  /**
   * @brief tree_t  simple default constructor
   * @param root
   */
  tree_t(const T& root) : _root{ new node_type{ root } }
  {  }

  /**
   * @brief root  it gets the root of three
   * @return
   * @throws null_node_exception_t if root is null
   */
  node_type&
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
  const node_type&
  root() const {
    if (!_root) {
      throw null_node_exception_t{};
    }
    return *_root;
  } // LCOV_EXCL_LINE

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
      _root = new node_type();
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
   * @brief add_root it adds a root object
   * @param root
   */
  tree_t&
  add_root(const T& root) {
    delete_root();
    _root = new node_type(root, nullptr);
    return *this;
  } // LCOV_EXCL_LINE

  inline virtual bool
  has_root() const override {
    return _root;
  } // LCOV_EXCL_LINE

  virtual void
  clear() override {
    delete_root();
  }

  protected:

  void
  delete_root() {
    if (_root) {
      delete _root;
      _root = nullptr;
    }
  }

  node_type* _root{ nullptr };
};

}
}
