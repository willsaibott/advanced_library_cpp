#pragma once
#include "tree.h"

namespace advanced {
namespace structures {

/** @test TestBinaryTree in tests/test_binary_tree(.h|.cpp) */

template <class T>
class binary_tree_t;

/**
 * @brief templated binary node to be inserted into the bianry tree object
 */
template <class T>
class binary_node_t : public base_node_t<T, binary_node_t<T>> {
  friend class binary_tree_t<T>;

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
  }

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
  }

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
  }

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
  }


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
   * Contructor that forward anything to base_node constructor
   */
  template<typename ...Args>
  binary_node_t(Args&&...args)
      : base_node_t<T, binary_node_t<T> > (std::forward<Args>(args)...)
  { }

  binary_node_t& operator=(const binary_node_t& other) = default;
  binary_node_t& operator=(binary_node_t&& other)      = default;

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
  }

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
      _right = new binary_node_t{ child, this };
    }
    return ok;
  }

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

    default:
      break;
    }
  }

protected:
  long long         _height{ 0 };
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

  template <typename ...Args>
  binary_tree_t(Args...args)
    :  tree_t<T, binary_node_t<T> > (std::forward<Args>(args)...)
  { }

  binary_tree_t(const T& root) : tree_t<T, binary_node_t<T> > (root)
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

  /**
   * @brief It forwars anything to binary_node custom constructor
   */
  template <typename ...Args>
  avl_tree_t(Args&&...args)
    :  tree_t<T, binary_node_t<T> > (std::forward<Args>(args)...)
  { }

  /**
   * @brief avl_tree_t  default constructor
   * @param root
   */
  avl_tree_t(const T& root) : tree_t<T, binary_node_t<T> > (root)
  { }

  /**
   * @brief insert inserts a non existent previously value in the tree
   * @param value
   * @return  whether the value was inserted or not, if the value is already in
   * tree it will return false
   */
  bool
  insert(const T& value) {
    auto it = insert(this->_root, value);
    return it != nullptr;
  }

  /**
   * @brief remove  it removes a value from the tree
   * @param value
   * @return   If the tree contained the value
   */
  bool
  remove(const T& value) {
    auto it = remove(this->_root, value);
    return it != nullptr;
  }

  protected:
  avl_tree_t() = default;

  /**
   * @brief insert  inserts the value into the tree, make necessary rotations
   * to keep tree's balance
   * @param node
   * @param value
   * @return
   */
  node_type_t*
  insert(node_type_t* node, const T& value) {
    node_type_t* result{ nullptr };
    if (node) {
      if (value < *node) {
        if (node->has_left()) {
          result = insert(node->_left, value);
        }
        else {
          node->add_left(value);
          result = node->_left;
        }

        if(height(result->_left) - height(result->_right) == 2) {
            if(value < result->_left->get()) {
              result = rotate_right(result);
            }
            else {
              result = double_rotate_right(result);
            }
        }
      }
      else if (value > *node) {
        if (node->has_right()) {
          result = insert(node->_right, value);
        }
        else {
          node->add_right(value);
          result = true;
        }

        if(height(result->_right) - height(result->_left) == 2) {
            if(value > result->_right->get()) {
              result = rotate_left(result);
            }
            else {
              result = double_rotate_left(result);
            }
        }
      }
      else {
        // result is nullptr, no equal value can be inserted in the BST
      }
    }

    if (result) {
      result->_height =
          std::max(height(result->left), height(result->right)) + 1;
    }
    return result;
  }

  /**
   * @brief remove remove an element from tree, rotate if necessary to keep the
   * tree's balance
   * @param node
   * @param value
   * @return
   */
  node_type_t*
  remove(node_type_t* node, const T& value) {
    node_type_t* temp{ nullptr };

    if (node) {
      // Searching for element
      if (value < node->get()) {
        node->_left = remove(value, node->_left);
      }
      else if (value > node->get()) {
        node->_right = remove(value, node->_right);
      }
      // Element found With 2 children
      else if (node->_left && node->_right) {
         temp = leftmost(node->_right);
         node->set_value(*temp);
         node->_right = remove(*temp, node->_right);
      }
      // With one or zero child
      else {
        temp = node;
        if(!node->_left) {
          node = node->_right;
        }
        else if(!node->_right) {
          node = node->_left;
        }
        delete temp;
      }

      if (node) {
        node->height = std::max(height(node->_left), height(node->_right)) + 1;

        // If node is unbalanced
        // If left node is deleted, right case
        if (height(node->_left) - height(node->_right) == 2) {
          // right right case
          if (height(node->_left->_left) - height(node->_left->_right) == 1) {
            node = rotate_left(node);
          }
          // right left case
          else {
            node = double_rotate_left(node);
          }
        }
        // If right node is deleted, left case
        else if (height(node->_right) - height(node->_left) == 2) {
           // left left case
           if (height(node->_right->_right) - height(node->_right->_left) == 1) {
             node = rotate_right(node);
           }
           // left right case
           else {
             node = double_rotate_right(node);
           }
        }
      }
    }

    return node;
  }

  /**
   * @brief height   height of a node
   * @param node
   * @return height of a node
   */
  inline static long long
  height(node_type_t* node) {
    return node ? node->_height : 0;
  }

  /**
   * @brief rotate_right rotate a node to right
   * @param other
   * @return
   */
  inline static node_type_t*
  rotate_right(node_type_t* & other) {
    auto tmp{ other->_left };
    other->_left   = tmp->_right;
    tmp->_right    = other;
    other->_height = std::max(height(other->_left), height(other->_right)) + 1;
    tmp->_height   = std::max(height(tmp->_left),   other->_height)        + 1;
    return tmp;
  }

  /**
   * @brief rotate_left rotate a node to left
   * @param other
   * @return
   */
  inline static node_type_t*
  rotate_left(node_type_t* & other) {
    auto tmp{ other->_right };
    other->_right  = tmp->left;
    tmp->_left     = other;
    other->_height = std::max(height(other->_left),  height(other->_right)) + 1;
    tmp->_height   = std::max(height(other->_right), other->_height)        + 1;
  }

  /**
   * @brief double_rotate_left rotates a node to left after rotates it's right
   * child to right
   * @param node
   * @return
   */
  inline static node_type_t*
  double_rotate_left(node_type_t* &node) {
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
  double_rotate_right(node_type_t* &node) {
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
    return node->has_right() ? leftmost(node->_right) : node;
  }
};

}
}
