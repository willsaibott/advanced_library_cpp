#pragma once
#include "tree.h"

namespace advanced {

  template <class T>
  class binary_tree_t;

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

    T&
    left() {
      return *(*_left);
    }

    T&
    right() {
      return *(*_right);
    }

    inline bool
    has_left() const {
      return _left;
    }

    inline bool
    has_right() const {
      return _right;
    }

    inline size_t
    children_count() const {
      return has_left() + has_right();
    }

    binary_node_t() = default;

    template<typename ...Args>
    binary_node_t(Args...args)
        : base_node_t<T, binary_node_t<T> > (std::forward<Args>(args)...)
    { }

    binary_node_t& operator=(const binary_node_t& other) = default;
    binary_node_t& operator=(binary_node_t&& other)      = default;

    virtual
    ~binary_node_t() {
      delete_child(direction::left);
      delete_child(direction::right);
    }

    virtual bool
    add_left(const T& child) override {
      bool ok{ !_left };
      if (ok) {
        _left = new binary_node_t{ child, this };
      }
      return ok;
    }

    virtual bool
    add_right(const T& child) override {
      bool ok{ !_right };
      if (ok) {
        _right = new binary_node_t{ child, this };
      }
      return ok;
    }

    virtual bool
    delete_child(direction node) override {
      bool ok{ false };
      if (node == direction::left && _left) {
        delete _left;
        _left = nullptr;
        ok = true;
      }
      else if (node == direction::right && _left) {
        delete _right;
        _right = nullptr;
        ok = true;
      }
      return ok;
    }

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

  template <class T>
  class binary_tree_t : public tree_t<T, binary_node_t<T>>{
  public:

    template <typename ...Args>
    binary_tree_t(Args...args)
      :  tree_t<T, binary_node_t<T> > (std::forward<Args>(args)...)
    { }

    binary_tree_t(T* root) : tree_t<T, binary_node_t<T> > (root)
    { }

  protected:
    binary_tree_t() = default;
  };

  template <class T>
  class avl_tree_t : public tree_t<T, binary_node_t<T>>{
  public:
    using node_type_t = binary_node_t<T>;

    template <typename ...Args>
    avl_tree_t(Args...args)
      :  tree_t<T, binary_node_t<T> > (std::forward<Args>(args)...)
    { }

    avl_tree_t(const T& root) : tree_t<T, binary_node_t<T> > (root)
    { }

    bool
    insert(const T& value) {
      auto it = insert(this->_root, value);
      return it != nullptr;
    }

    bool
    remove(const T& value) {
      auto it = remove(this->_root, value);
      return it != nullptr;
    }

    protected:
    avl_tree_t() = default;

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
           node->set_node(*temp);
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

    inline static long long
    height(node_type_t* node) {
      return node ? node->_height : 0;
    }

    inline static node_type_t*
    rotate_right(node_type_t* & other) {
      auto tmp{ other->_left };
      other->_left   = tmp->_right;
      tmp->_right    = other;
      other->_height = std::max(height(other->_left), height(other->_right)) + 1;
      tmp->_height   = std::max(height(tmp->_left),   other->_height)        + 1;
      return tmp;
    }

    inline static node_type_t*
    rotate_left(node_type_t* & other) {
      auto tmp{ other->_right };
      other->_right  = tmp->left;
      tmp->_left     = other;
      other->_height = std::max(height(other->_left),  height(other->_right)) + 1;
      tmp->_height   = std::max(height(other->_right), other->_height)        + 1;
    }

    inline static node_type_t*
    double_rotate_left(node_type_t* &node) {
      node->_right = rotate_right(node->_right);
      return rotate_left(node);
    }

    inline static node_type_t*
    double_rotate_right(node_type_t* &node) {
      node->_left = rotate_left(node->_left);
      return rotate_right(node);
    }

    inline static node_type_t*
    leftmost(node_type_t* node) {
      return node->has_left() ? leftmost(node->_left) : node;
    }

    inline static node_type_t*
    rightmost(node_type_t* node) {
      return node->has_right() ? leftmost(node->_right) : node;
    }
  };

}
