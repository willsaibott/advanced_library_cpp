#pragma once
#include <memory>
#include <vector>
#include <algorithm>

namespace advanced {

  template <class T, class node_type>
  class tree_t;

  template <class T, class node_type_t>
  class base_node_t {
  public:

    void
    set_node(const T& node) {
      _node = node;
    }

    void
    set_node(T&& node) {
      _node = std::move(node);
    }

    virtual void
    set_parent(node_type_t* parent) {
      _parent = parent;
    }

    virtual bool
    delete_child(size_t child) = 0;

    typename std::enable_if<std::is_move_constructible<T>::value, void>::type
    swap_move(node_type_t& other) {
      T tmp       = std::move(other._node);
      other._node = std::move(_node);
      _node       = std::move(tmp);
    }

    typename std::enable_if<std::is_copy_constructible<T>::value, void>::type
    swap(node_type_t& other) {
      T tmp       = other._node;
      other._node = _node;
      _node       = tmp;
    }

    node_type_t&
    parent() {
      return _parent;
    }

    const node_type_t&
    parent() const {
      return _parent;
    }

    inline operator T() const {
      return _node;
    }

    inline T&operator*() {
      return _node;
    }

    inline const T&operator*() const {
      return _node;
    }

    inline T&get() {
      return _node;
    }

    inline const T&get() const {
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

    node_type_t* _parent{ this };
    T            _node;

  };

  template <class T, size_t max_node_size = 2>
  class tree_node_t : public base_node_t<T, tree_node_t<T, max_node_size>> {
    friend class tree_t<T, tree_node_t<T, max_node_size>>;

  public:
    using node_type_t = tree_node_t<T, max_node_size>;

    virtual bool
    add_child(const T& child) {
      bool ok{ _children.size() < max_node_size };
      if (ok) {
          _children.emplace_back(new node_type_t{ child, this });
      }
      return ok;
    }

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

    virtual inline size_t
    children_count() const override {
      return _children.size();
    }

    template <typename = typename std::enable_if<
                std::is_default_constructible<T>::value >
              ::type >
    tree_node_t() { }

    template <typename = typename std::enable_if<
                std::is_copy_constructible<T>::value >
              ::type >
    tree_node_t(const T& node, tree_node_t* parent = nullptr)
      : base_node_t<T, tree_node_t<T, max_node_size> > (node, parent)
    { }

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

    std::vector<node_type_t*> _children;
  };

  template <class T, class node_type>
  class tree_t {
  public:

    virtual
    ~tree_t() {
      if (_root) {
        delete _root;
      }
    }

    template <typename ...Args>
    tree_t(Args...args) :
      _root{ new node_type(new T(std::forward<Args>(args)...)) }
    { }

    tree_t(const T& root) : _root{ new node_type{ root } }
    { }

    node_type&
    root() {
      return *_root;
    }

    protected:
    tree_t() = default;

    node_type* _root{ nullptr };
  };
}
