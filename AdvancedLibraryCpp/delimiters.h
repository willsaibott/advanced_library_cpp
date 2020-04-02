#include <iostream>
#include <algorithm>
#include <locale>
#include <functional>

namespace advanced {

  struct custom_delimiter_t : std::ctype<char> {
    custom_delimiter_t( std::function<bool(size_t)> func ) : std::ctype<char>(get_table(func)) {}

    static mask const* get_table( std::function<bool(size_t)> func ) {
      static mask rc[table_size];
      for (size_t ii = 0; ii < table_size; ii++) {
        if (!func(ii)) {
          rc[ii] = std::ctype_base::space;
        }
      }
      return &rc[0];
    }
  };

}