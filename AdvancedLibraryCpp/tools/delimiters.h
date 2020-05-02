#include <iostream>
#include <algorithm>
#include <locale>

namespace advanced {
namespace tools {

  struct alpha_filter_t {
    inline bool operator()(char c) const {
      return std::isalpha(c);
    }
  };

  struct alpha_num_filter_t {
    inline bool operator()(char c) const {
      return std::isalnum(c);
    }
  };

  struct lower_filter_t {
    inline bool operator()(char c) const {
      return std::islower(c);
    }
  };

  struct upper_filter_t {
    inline bool operator()(char c) const {
      return std::isupper(c);
    }
  };

  struct digit_filter_t {
    inline bool operator()(char c) const {
      return std::isdigit(c);
    }
  };

  template <class Filter>
  struct custom_delimiter_t : std::ctype<char> {

    custom_delimiter_t() : std::ctype<char>(get_table()) {}

    static mask const* get_table() {
      static Filter filter;
      static mask rc[table_size];
      for (size_t ii = 0; ii < table_size; ii++) {
        if (!filter(static_cast<char>(ii))) {
          rc[ii] = std::ctype_base::space;
        }
      }
      return &rc[0];
    }
  };
}
}
