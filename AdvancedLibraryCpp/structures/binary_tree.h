#pragma once
#include "tree.h"

namespace advanced {
namespace structures {

/** @test TestBinaryTree in tests/test_binary_tree(.h|.cpp) */

template <class T>
class binary_tree_t;

template <class T>
class avl_tree_t;

/**
 * @brief templated binary node to be inserted into the bianry tree object
 */
template <class T>
class binary_node_t : public base_node_t<T, binary_node_t<T>> {
  friend class binary_tree_t<T>;
  friend class avl_tree_t<T>;

public:
  using node_type_t = binary_node_t<T>;
  using base_type_t = base_node_t<T, binary_node_t<T>>;

  enum class direction {
    left  = 0,
    right = 1
  };

  /**
   * @brief left returns the left node
   * @return     returns the left node
   * @throws null_node_exception_t if left is null
   */
  node_type_t&
  left() {
    if (!_left) {
      throw null_node_exception_t{};
    }
    return *_left;
  } // LCOV_EXCL_LINE

  /**
   * @brief left returns the left node in a const context
   * @return     returns the left node
   * @throws null_node_exception_t if left is null
   */
  const node_type_t&
  left() const {
    if (!_left) {
      throw null_node_exception_t{};
    }
    return *_left;
  } // LCOV_EXCL_LINE

  /**
   * @brief right returns the right node
   * @return      returns the right node
   * @throws null_node_exception_t if right is null
   */
  node_type_t&
  right() {
    if (!_right) {
      throw null_node_exception_t{};
    }
    return *_right;
  } // LCOV_EXCL_LINE

  /**
   * @brief right returns the right node in a const context
   * @return      returns the right node
   * @throws null_node_exception_t if right is null
   */
  const node_type_t&
  right() const {
    if (!_right) {
      throw null_node_exception_t{};
    }
    return *_right;
  } // LCOV_EXCL_LINE

  /**
   * @brief has_left returns whether the node has or not left child
   * @return whether the node has or not left child
   */
  inline bool
  has_left() const {
    return _left;
  }

  /**
   * @brief has_right returns whether the node has or not right child
   * @return whether the node has or not right child
   */
  inline bool
  has_right() const {
    return _right;
  }

  /**
   * @brief children_count  number of children nodes
   * @return number of children nodes
   */
  virtual size_t
  children_count() const override {
    return has_left() + has_right();
  }

  binary_node_t() = default;

  /**
   * @brief binary_node_t it completely copy the entire node and content
   * @param other
   */
  binary_node_t(const binary_node_t& other) {
    *this = other;
  }

  /**
   * @brief binary_node_t  Move constructor, transfer the ownership of left and
   * right child to the new object
   * @param other
   */
  binary_node_t(binary_node_t&& other) {
    *this = std::move(other);
  }


  /**
   * Contructor that forward anything to base_node constructor
   */
  template<typename ...Args>
  binary_node_t(Args&&...args)
      : base_node_t<T, binary_node_t<T> > (std::forward<Args>(args)...)
  { }

  /**
   * @brief operator = copy assingment operator creates a new empty node and copy
   * the contents from the other node to it, also creates and copy recursively
   * the left and right children if they exist in the other
   * It performs a full copy, so expensive, but can be used as prototype
   * from Prototype Design pattern.
   * @param other node to be copied
   * @return reference to this
   */
  binary_node_t&
  operator=(const binary_node_t& other) {
    delete_child(direction::right);
    delete_child(direction::left);

    if (other.has_left()) {
      _left   = new binary_node_t();
      *_left  = other.left();
      _left->set_parent(this);
    }
    if (other.has_right()) {
      _right  = new binary_node_t();
      *_right = other.right();
      _right->set_parent(this);
    }

    base_type_t::_node = other._node;
    return *this;
  } // LCOV_EXCL_LINE

  /**
   * @brief operator = move operator is extremely fast, it just transfer the
   * ownership of the pointer from other to this
   * @note the other will lost reference to any child or parent.
   *
   * @param other object to be moved
   * @return reference to this
   */
  binary_node_t&
  operator=(binary_node_t&& other) {
    delete_child(direction::right);
    delete_child(direction::left);
    _left  = other._left;
    _right = other._right;
    other._left = other._right = nullptr;
    other.set_parent(nullptr);

    if (has_left()) {
      _left->set_parent(this);
    }
    if (has_right()) {
      _right->set_parent(this);
    }

    base_type_t::_node = std::move(other._node);
    return *this;
  } // LCOV_EXCL_LINE

  /**
   * @brief ~binary_node_t deletes all children if they exist
   */
  virtual
  ~binary_node_t() {
    delete_child(direction::left);
    delete_child(direction::right);
  }

  /**
   * @brief add_left  add left child
   * @param child
   * @return whether it could be added or not, it means, if there was no
   * previous left child
   */
  virtual bool
  add_left(const T& child) {
    bool ok{ !_left };
    if (ok) {
      _left = new binary_node_t{ child, this };
    }
    return ok;
  } // LCOV_EXCL_LINE
// LCOV_EXCL_LINE

  /**
   * @brief add_right add right child
   * @param child
   * @return whether it could be added or not, it means, if there was no
   * previous left child
   */
  virtual bool
  add_right(const T& child) {
    bool ok{ !_right };
    if (ok) {
      _right  = new binary_node_t{ child, this };
    }
    return ok;
  } // LCOV_EXCL_LINE
// LCOV_EXCL_LINE

  /**
   * @brief delete_child deletes a child
   * @param index
   * @return
   */
  virtual bool
  delete_child(size_t index) override {
    direction node{ static_cast<direction>(index) };
    return delete_child(node);
  }

  /**
   * @brief delete_child deletes a child
   * @param node
   * @return
   */
  virtual bool
  delete_child(direction node) {
    bool ok{ false };
    if (node == direction::left && _left) {
      delete _left;
      _left = nullptr;
      ok = true;
    }
    else if (node == direction::right && _right) {
      delete _right;
      _right = nullptr;
      ok = true;
    }
    return ok;
  }

  /**
   * @brief swap  It swaps the current node with one of it's children, left or
   * right
   * @param node
   */
  void
  swap(direction node) {
    switch (node) {
    case direction::left:
      if (has_left()) {
        base_type_t::swap(left());
      }
      break;
    case direction::right:
      if (has_right()) {
        base_type_t::swap(right());
      }
      break;

    default: break; // LCOV_EXCL_LINE
    } // LCOV_EXCL_LINE
  }

protected:
  long long         _height{ 1 };
  binary_node_t<T>* _left{ nullptr };
  binary_node_t<T>* _right{ nullptr };

  using base_type_t::swap;

};

/**
 * @brief templated simple binary tree
 */
template <class T>
class binary_tree_t : public tree_t<T, binary_node_t<T>>{
public:
  using node_type_t = binary_node_t<T>;
  using base_type_t = tree_t<T, node_type_t>;

  binary_tree_t(const T& root) : tree_t<T, node_type_t> (root)
  { }

  binary_tree_t() = default;

protected:
};

/**
 * @brief templated avl tree
 */
template <class T>
class avl_tree_t : public tree_t<T, binary_node_t<T>>{
public:
  using node_type_t = binary_node_t<T>;
  using base_type_t = tree_t<T, node_type_t>;

  /**
   * @brief avl_tree_t  default constructor
   * @param root
   */
  avl_tree_t(const T& root) {
    insert(root);
  }

  avl_tree_t() = default;

  /**
   * @brief insert inserts a non existent previously value in the tree
   * @param value
   * @return  whether the value was inserted or not, if the value is already in
   * tree it will return false
   */
  bool
  insert(const T& value) {
    bool inserted{ true };
    insert(this->_root, value, inserted);
    return inserted;
  }

  /**
   * @brief remove  it removes one value from the tree
   * @param value
   * @return   If the tree contained the value
   */
  bool
  remove(const T& value) {
    bool removed{ false };
    remove(this->_root, value, removed);
    return removed;
  }

  /**
   * @brief remove_all it removes all elemnts that matches the value
   * @param value
   * @return the count of removed elemnts
   * @note   complexity is O(repeated_values * log(n))
   */
  size_t
  remove_all(const T& value) {
    size_t removed_elements{ 0 };
    while (remove(value)) {
      removed_elements++;
    }
    return removed_elements;
  }

  /**
   * @brief height gets the height of the three
   * @return
   */
  inline size_t
  height() const {
    return height(this->_root);
  }

  /**
   * @brief left_most
   * @return the left most node
   */
  const node_type_t&
  left_most() const {
    return *avl_tree_t::leftmost(this->_root);
  }

  /**
   * @brief right_most
   * @return the right most node
   */
  const node_type_t&
  right_most() const {
    return *avl_tree_t::rightmost(this->_root);
  }

  /**
   * @brief contains   it searches the tree recursively for a value
   * @param value      key value to be found
   * @return           whether the value is in the tree or not
   */
  bool
  contains(const T& value) const {
    return this->has_root() ? contains(this->root(), value) : false;
  }

  /**
   * @brief root tree root
   * @return tree root reference
   */
  const node_type_t&
  root() const {
    return base_type_t::root();
  }

  /**
   * @brief root tree root
   * @return tree root reference
   */
  const node_type_t&
  root() {
    return base_type_t::root();
  }

  /**
   * @brief enable_duplicates It makes the AVL tree allow multiple equal entries
   * @default not allowed
   */
  void
  enable_duplicates() {
    _allow_duplicates = true;
  }

  /**
   * @brief disable_duplicates It makes the AVL tree disallow multiple equal
   * entries
   * @default not allowed
   */
  void
  disable_duplicates() {
    _allow_duplicates = false;
  }

  /**
   * @brief size number of elements in tree
   * @return number of elements in tree
   */
  size_t
  size() const {
    return _number_of_elements;
  }

  /**
   * @brief is_multiple_keys_allowed whether this class allowes or not
   * multiple keys
   * @return whether this class allowes or not multiple keys
   */
  inline bool
  is_multiple_keys_allowed() const {
    return _allow_duplicates;
  }

  /**
   * @brief clear  it removes all elements from the tree
   */
  virtual void
  clear() override {
    base_type_t::clear();
    _number_of_elements = 0;
  }

  protected:

  using base_type_t::add_root;

  /**
   * @brief contains  searches the tree recursively for a value
   * @param node      current node
   * @param value     value
   * @return          whether the value is in the tree or not
   */
  bool
  contains(const node_type_t& node, const T&value) const {
    bool found{ false };
    if (value < *node) {
      if (node.has_left()) {
        found = contains(node.left(), value);
      }
    }
    else if (value > *node) {
      if (node.has_right()) {
        found = contains(node.right(), value);
      }
    }
    else {
      found = value == *node;
    }
    return found;
  }

  /**
   * @brief insert_left  inserts in the left side of the node
   * @param node
   * @param value
   * @param inserted
   */
  void
  insert_left(node_type_t*& node, const T& value, bool& inserted) {
    if (node->has_left()) {
      insert(node->_left, value, inserted);
    }
    else {
      node->add_left(value);
      _number_of_elements++;
    }
  }

  /**
   * @brief insert_right  it inserts in the right side of the node
   * @param node
   * @param value
   * @param inserted
   */
  void
  insert_right(node_type_t*& node, const T& value, bool& inserted) {
    if (node->has_right()) {
      insert(node->_right, value, inserted);
    }
    else {
      node->add_right(value);
      _number_of_elements++;
    }
  }

  /**
   * @brief rotate_if_needed   Rotate the tree if the children nodes are not
   * balanced
   * @param father[in|out] node that will be the parent after the rotation
   * @param value          value inserted
   */
  void
  rotate_if_needed(node_type_t*& father, const T& value) {
    father->_height =
      std::max(height(father->_left), height(father->_right)) + 1;

    if (height(father->_left) - height(father->_right) == 2) {
      if (value < father->_left->get()) {
        father = rotate_right(father);
      }
      else {
        father = double_rotate_right(father);
      }
    }
    else if (height(father->_right) - height(father->_left) == 2) {
      if (value > father->_right->get()) {
        father = rotate_left(father);
      }
      else {
        father = double_rotate_left(father);
      }
    }
  }

  /**
   * @brief rotate_on_remove  Rotates the AVL tree when an element was removed
   * from the tree
   * @param father the node will be the father after rotation
   * @param value
   */
  void
  rotate_on_remove_if_needed(node_type_t*& father) {
    father->_height = std::max(height(father->_left), height(father->_right)) + 1;

    // If node is unbalanced
    // If left node is deleted, right case
    if (height(father->_left) - height(father->_right) == 2) {
      // right right case
      if (height(father->_left->_left) - height(father->_left->_right) == 1) {
        father = rotate_right(father);
      }
      // right left case
      else {
        father = double_rotate_right(father);
      }
    }
    // If right node is deleted, left case
    else if (height(father->_right) - height(father->_left) == 2) {
      // left left case
      if (height(father->_right->_right) - height(father->_right->_left) == 1) {
        father = rotate_left(father);
      }
      // left right case
      else {
        father = double_rotate_left(father);
      }
    }
  }

  /**
   * @brief insert  inserts the value into the tree, make necessary rotations
   * to keep tree's balance
   * @param node
   * @param value
   * @return
   */
  void
  insert(node_type_t*& node, const T& value, bool& inserted) {
    node_type_t* father{ node };
    if (node) {
      if (value < node->get()) {
        insert_left(node, value, inserted);
      }
      else if (value > node->get()) {
        insert_right(node, value, inserted);
      }
      else if (_allow_duplicates) {
        insert_right(node, value, inserted);
      }
      else {
        inserted = false;
      }
    }
    // I'm root:
    else {
      add_root(value);
      _number_of_elements++;
      father = this->_root;
    }

    if (inserted) {
      rotate_if_needed(father, value);
    }

    node = father;
  }

  /**
   * @brief remove remove an element from tree, rotate if necessary to keep the
   * tree's balance
   * @param node
   * @param value
   * @return
   */
  void
  remove(node_type_t*& node, const T& value, bool& removed) {
    node_type_t* temp{ node };

    if (node) {
      // Searching for element
      if (value < node->get()) {
        remove(node->_left, value, removed);
      }
      else if (value > node->get()) {
        remove(node->_right, value, removed);
      }
      // Element found With 2 children
      else if (node->children_count() == 2u) {
        temp = leftmost(node->_right);
        node->set_value(*temp);
        remove(node->_right, *temp, removed);
      }
      // With one or zero child
      else {
        temp = node;
        node = node->has_left() ? node->_left : node->_right;
        if (node) {
          temp->_left = temp->_right = nullptr;
          node->set_parent(temp->_parent);
        }
        delete temp;
        removed = true;
        _number_of_elements--;
      }

      if (node) {
        rotate_on_remove_if_needed(node);
      }
    }
  }

  /**
   * @brief height   height of a node
   * @param node
   * @return height of a node
   */
  inline static long long
  height(const node_type_t* node) {
    return node ? node->_height : 0;
  }

  /**
   * @brief rotate_right rotate a node to right
   * @param other
   * @return
   */
  inline static node_type_t*
  rotate_right(node_type_t* other) {
    node_type_t* head{ other };
    if (other && other->has_left()) {
      head = other->_left;
      other->_left   = head->_right;
      head->_right   = other;
      head->set_parent(other->_parent);
      other->set_parent(head);
      other->_height = std::max(height(other->_left), height(other->_right)) + 1;
      head->_height  = std::max(height(head->_left),  other->_height)        + 1;
    }
    return head;
  }

  /**
   * @brief rotate_left rotate a node to left
   * @param other
   * @return
   */
  inline static node_type_t*
  rotate_left(node_type_t* other) {
    node_type_t* head{ other };
    if (other && other->has_right()) {
      head = other->_right;
      other->_right  = head->_left;
      head->_left     = other;
      head->set_parent(other->_parent);
      other->set_parent(head);
      other->_height = std::max(height(other->_left), height(other->_right)) + 1;
      head->_height  = std::max(height(head->_right), other->_height)        + 1;
    }
    return head;
  }

  /**
   * @brief double_rotate_left rotates a node to left after rotates it's right
   * child to right
   * @param node
   * @return
   */
  inline static node_type_t*
  double_rotate_left(node_type_t* node) {
    node->_right = rotate_right(node->_right);
    return rotate_left(node);
  }

  /**
   * @brief double_rotate_right rotates a node to right after rotates it's left
   * child to left
   * @param node
   * @return
   */
  inline static node_type_t*
  double_rotate_right(node_type_t* node) {
    node->_left = rotate_left(node->_left);
    return rotate_right(node);
  }

  /**
   * @brief leftmost   get the leftmost decendent of a node
   * @param node
   * @return
   */
  inline static node_type_t*
  leftmost(node_type_t* node) {
    return node->has_left() ? leftmost(node->_left) : node;
  }

  /**
   * @brief rightmost  get the rightmost decendent of a node
   * @param node
   * @return
   */
  inline static node_type_t*
  rightmost(node_type_t* node) {
    return node->has_right() ? rightmost(node->_right) : node;
  }

private:

  bool    _allow_duplicates{ false };
  size_t  _number_of_elements{ 0 };
};

}
}
