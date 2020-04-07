#pragma once
#include <memory>
#include <vector>

namespace advanced {

  template <class T, size_t max_node_size>
  class tree_t;

  template <class T, size_t max_node_size = 2>
  class tree_node_t {
    friend class tree_t<T, max_node_size>;

    T*                                         parent;
    std::unique_ptr<T>                         node;
    std::vector<tree_node_t<T, max_node_size>> children;

    tree_node_t(T* node = nullptr, T* parent = nullptr)
        : children(max_node_size),
          parent{ parent },
          node{ node } { }

    void
    set_node(T* node) {
      this->node = std::move(node);
    }

    void
    set_node(std::unique_ptr<T>&& node) {
      this->node = std::move(node);
    }

  public:

    void
    set_child(size_t child_index, std::unique_ptr<T>&& child) {
      if (child_index < max_node_size) {
        children[child_index].set_node(std::move(child));
      }
    }

    void
    set_child(size_t child_index, T* child) {
      if (child_index < max_node_size) {
        children[child_index].set_node(child);
      }
    }

    void set_parent(T* parent) {
      this->parent = parent;
    }

    void
    delete_child(size_t child) {
      if (child < max_node_size) {
        children[child] = nullptr;
      }
    }

    T&
    child(size_t child) const {
      return *children.at(child);
    }

    operator T() {
      return *node;
    }

    T&operator*() {
      return *node;
    }

    const T&operator*() const {
      return *node;
    }

    T&get() {
      return *node;
    }

    const T&get() const {
      return *node;
    }
  };


  template <class T, size_t max_node_size = 2>
  class tree_t {
    std::unique_ptr<tree_node_t<T, max_node_size>> _root{ nullptr };

  public:

    using node_type = tree_node_t<T, max_node_size>;

    template <typename ...Args>
    tree_t(Args...args) :
      _root{ new tree_node_t<T, max_node_size>>(
                new T(std::forward<Args>(args)...)) }
    {
      _root->set_parent(_root.get());
    }

    tree_node_t<T, max_node_size>& root() {
      return *_root;
    }
  };
}
