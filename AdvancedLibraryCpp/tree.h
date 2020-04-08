#pragma once
#include <memory>
#include <vector>

namespace advanced {

  template <class T, size_t max_node_size>
  class tree_t;

  template <class T, size_t max_node_size = 2>
  class tree_node_t {
    friend class tree_t<T, max_node_size>;

  public:
    using node_type_t = tree_node_t<T, max_node_size>;

    void
    add_child(std::unique_ptr<T>&& child) {
      if (_children.size() < max_node_size) {
          _children.emplace_back(
            std::unique_ptr<node_type_t>(new node_type_t{ std::move(child), this })
          );
      }
    }

    void
    add_child(T* child) {
      if (_children.size() < max_node_size) {
          _children.emplace_back(
            std::unique_ptr<node_type_t>(new node_type_t{ child, this })
          );
      }
    }

    void set_parent(node_type_t* parent) {
      _parent = parent;
    }

    void
    delete_child(size_t child) {
      if (child < _children.size()) {
        _children.erase(_children.begin() + child);
      }
    }

    node_type_t&
    parent() {
      return _parent;
    }

    const node_type_t&
    parent() const {
      return _parent;
    }

    const node_type_t&
    child(size_t child) const {
      return *(_children.at(child));
    }

    node_type_t&
    child(size_t child) {
      return *(_children.at(child));
    }

    inline operator T() const {
      return *_node;
    }

    inline T&operator*() {
      return *_node;
    }

    inline const T&operator*() const {
      return *_node;
    }

    inline T&get() {
      return *_node;
    }

    inline const T&get() const {
      return *_node;
    }

    tree_node_t() = default;

    tree_node_t(T* node, tree_node_t* parent = nullptr)
        : _parent{ parent },
          _node{ node }
    { }

    tree_node_t(tree_node_t&& other) :
      _parent{ other._parent },
      _node{ std::move(other._node) },
      _children(std::move(other._children))
    {  }

    tree_node_t(const tree_node_t& other) = delete;


    tree_node_t& operator=(const tree_node_t& other) = delete;
    tree_node_t& operator=(tree_node_t&& other)      = default;

    private:

    node_type_t*                              _parent{ this };
    std::unique_ptr<T>                        _node{ nullptr };
    std::vector<std::unique_ptr<node_type_t>> _children;


    void
    set_node(T* node) {
      _node = std::unique_ptr<T>(node);
    }

    void
    set_node(std::unique_ptr<T>&& node) {
      _node = std::move(node);
    }
  };


  template <class T, size_t max_node_size = 2>
  class tree_t {

  public:

    using node_type = tree_node_t<T, max_node_size>;

    template <typename ...Args>
    tree_t(Args...args) :
      _root{ new node_type(new T(std::forward<Args>(args)...)) }
    { }

    node_type& root() {
      return *_root;
    }

    private:
    std::unique_ptr<node_type> _root{ nullptr };
  };
}
