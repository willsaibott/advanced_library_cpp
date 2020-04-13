#include "test_stream_delimiters.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include "../delimiters.h"

TestStreamDelimiters::
TestStreamDelimiters(QObject *parent) : QObject(parent) {

}

void TestStreamDelimiters::
test_string_stream_words_splitter() {
  const std::vector<std::string> expected_values {
    "abcd",
    "Adjacency",
    "rh",
    "english",
    "peter",
    "ss",
    "P",
    "song",
    "hour",
    "bob"
  };
  std::stringstream ss {
    "abcd//:.Adjacency;.192rh00english peter@ss<P\nsong39hour0~?><;:,\"\'()bob"
  };
  std::vector<std::string> values;
  std::string word;

  ss.imbue(std::locale{
             ss.getloc(),
             new advanced::custom_delimiter_t<advanced::alpha_filter_t>{} });

  while (ss >> word) {
    values.push_back(word);
  }

  QCOMPARE(values, expected_values);
}

void TestStreamDelimiters::
test_string_stream_lowercase_only() {
  const std::vector<std::string> expected_values {
    "abcd",
    "djacency",
    "rh",
    "english",
    "peter",
    "ss",
    "song",
    "hour",
    "bob"
  };
  std::stringstream ss {
    "abcd//:.Adjacency;.192rh00english peter@ss<P\nsong39hour0~?><;:,\"\'()bob"
  };
  std::vector<std::string> values;
  std::string word;

  ss.imbue(std::locale{
             ss.getloc(),
             new advanced::custom_delimiter_t<advanced::lower_filter_t>{} } );

  while (ss >> word) {
    values.push_back(word);
  }

  QCOMPARE(values, expected_values);
}

void TestStreamDelimiters::
test_string_stream_uppercase_only() {
  const std::vector<std::string> expected_values {
    "A",
    "PETER",
    "P",
  };
  std::stringstream ss {
    "abcd//:.Adjacency;.192rh00english PETER@ss<P\nsong39hour0~?><;:,\"\'()bob"
  };
  std::vector<std::string> values;
  std::string word;

  ss.imbue(std::locale{
             ss.getloc(),
             new advanced::custom_delimiter_t<advanced::upper_filter_t>{} } );

  while (ss >> word) {
    values.push_back(word);
  }

  QCOMPARE(values, expected_values);
}

void TestStreamDelimiters::
test_string_stream_digit_only_only() {
  const std::vector<std::string> expected_values {
    "192",
    "00",
    "39",
    "0"
  };
  std::stringstream ss {
    "abcd//:.Adjacency;.192rh00english PETER@ss<P\nsong39hour0~?><;:,\"\'()bob"
  };
  std::vector<std::string> values;
  std::string word;

  ss.imbue(std::locale{
             ss.getloc(),
             new advanced::custom_delimiter_t<advanced::digit_filter_t>{} } );

  while (ss >> word) {
    values.push_back(word);
  }

  QCOMPARE(values, expected_values);
}
