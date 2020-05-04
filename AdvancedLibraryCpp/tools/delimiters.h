#include <iostream>
#include <algorithm>
#include <locale>

namespace advanced {
namespace tools {

  /**
   * @brief The alpha_filter_t struct filter chars that are non-alphabetic,
   * it means, non-printable chars, numbers and other chars like (~^!"$%...)
   */
  struct alpha_filter_t {
    inline bool operator()(char c) const {
      return std::isalpha(c);
    }
  };

  /**
   * @brief The alpha_num_filter_t struct filter chars that are neither
   * alphabetic, nor numbers, it means, non-printable chars, and other chars
   * like (~^!"$%...)
   */
  struct alpha_num_filter_t {
    inline bool operator()(char c) const {
      return std::isalnum(c);
    }
  };

  /**
   * @brief The lower_filter_t struct filter chars that are non-lower case,
   * it means, upper case chars, non-printable chars, and other chars like
   * (~^!"$%...)
   */
  struct lower_filter_t {
    inline bool operator()(char c) const {
      return std::islower(c);
    }
  };

  /**
   * @brief The upper_filter_t struct filter chars that are non-upper case,
   * it means, lower case chars, non-printable chars, and other chars like
   * (~^!"$%...)
   */
  struct upper_filter_t {
    inline bool operator()(char c) const {
      return std::isupper(c);
    }
  };

  /**
   * @brief The upper_filter_t struct filter chars that are non-digit,
   * it means, alphabetic chars, non-printable chars, and other chars like
   * (~^!"$%...)
   */
  struct digit_filter_t {
    inline bool operator()(char c) const {
      return std::isdigit(c);
    }
  };

  /**
   * @brief The custom_delimiter_t struct can be used to filter parse input
   * from std::basic_input_streams like std::ifstream, std::istringstream,
   * std::cin and so on...
   *    You can also create your own filter using the examples of this file.
   *    everything you need to do, is define a struct with the operator"()"
   *    (function call) defined inside of it
   *
   * @example
   *  How to use? use the imbue method present in the std::basic_ios,
   *  look at this:
   *
   *  1) Get only alphabetic chars from string:
   *  std::string_stream ss;
   *  ss.imbue(std::locale{ ss.getloc(),
   *                        new custom_delimiter_t<alpha_filter_t>{} });
   *
   *  2) Get only the lower case chars from stdin:
   *  std::cin.imbue(std::locale{ std::cin.getloc(),
   *                              new custom_delimiter_t<lower_filter_t>{} });
   *
   *  3) Get only digits from file:
   *  std::ifstream if;
   *  if.inbue(std::locale{ if.getloc(), new custom_delimiter_t<digit_filter_t>{} })
   *
   * @note The functions std::isaplpha, std::islower and std::isupper used in
   * the structures declared in this file are locale dependent, please
   * @see https://en.cppreference.com/w/cpp/string/byte/isalpha
   *
   * also, @see https://en.cppreference.com/w/cpp/io/basic_ios/imbue
   */
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
