#include "test_lru_cache.h"

using advanced::structures::cache_t;

TestLRUCache::
TestLRUCache(QObject *parent) : QObject(parent) {
  QObject::setObjectName("TestLRUCache");
}

void TestLRUCache::
test_add_method_should_insert_values_into_the_lru_cache() {
  const std::list<long> input {
    34l, 3l, 5l, 67l, -345l, 9230797509l, -28394834l
  };

  cache_t<long> cache;
  // insert in reverse order:
  populate(input.rbegin(), input.rend(), cache);
  QCOMPARE(cache.values(), input);
}

void TestLRUCache::
test_last_insertion_getter() {
  const std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  cache_t<int> cache;
  QVERIFY2(!cache.last_operation(), "No insertion was done yet");

  // insert in reverse order:
  for (auto it = input.rbegin(); it != input.rend(); it++) {
    cache.add(*it);
    QVERIFY2(cache.last_operation(), "Insertion should be fine");
  }

  QCOMPARE(cache.values(), input);

  // try to reinsert values in reverse order:
  for (auto it = input.begin(); it != input.end(); it++) {
    cache.add(*it);
    QVERIFY2(!cache.last_operation(),
             "Values already exist in cache, add should work");
  }
}

void TestLRUCache::
test_find_method_should_return_whether_a_value_exists_in_the_lru_cache() {
  const std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  cache_t<int> cache;

  for (auto it = input.begin(); it != input.end(); it++) {
    auto result  { cache.find(*it) };
    auto expected{ std::make_pair<bool, std::list<int>::const_iterator>(
                                    false, std::list<int>::iterator{}) };
    QVERIFY2(result.first == expected.first,
             "find should return a pair {false, {}} when the value doesn't exists in cache");
    QVERIFY2(result.second == expected.second,
             "find should return a pair {false, {}} when the value doesn't exists in cache");
  }

  // insert in reverse order:
  populate(input.rbegin(), input.rend(), cache);
  for (auto it = input.begin(); it != input.end(); it++) {
    auto result  { cache.find(*it) };
    auto expected{ std::make_pair<bool, int>(true, static_cast<int>(*it)) };
    QVERIFY2(result.first == expected.first,
             "find should return a pair {true, value} when the value exists in cache");
    QVERIFY2(*result.second == expected.second,
             "find should return a pair {true, value} when the value exists in cache");
  }
}

void TestLRUCache::
test_find_method_should_reorder_the_lru_cache_by_most_recent_used() {
  std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  cache_t<int> cache;

  // insert in reverse order to make the first element the most recent:
  populate(input.rbegin(), input.rend(), cache);

  // iterate over the list in normal order to make the most recent to be the
  // last element
  for (auto it = input.begin(); it != input.end(); it++) {
    auto result  { cache.find(*it) };
    QCOMPARE(*result.second, *it);
  }

  // Reverse the input to behave as the expected values:
  std::reverse(input.begin(), input.end());
  QCOMPARE(cache.values(), input);

  // iterate over the list that is reversed to make the most recent be the
  // first element
  for (auto it = input.begin(); it != input.end(); it++) {
    auto result  { cache.find(*it) };
    QCOMPARE(*result.second, *it);
  }

  // Reverse back the input to behave as the expected values:
  std::reverse(input.begin(), input.end());
  QCOMPARE(cache.values(), input);
}

void TestLRUCache::
test_operator_subscript_should_add_a_value_if_not_exists() {
  std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  cache_t<int> cache;

  // iterate over the list in normal order to make the most recent to be the
  // last element
  for (auto it = input.rbegin(); it != input.rend(); it++) {
    const auto& result{ cache[*it] };
    QCOMPARE(result, *it);
  }

  QCOMPARE(cache.values(), input);
  QCOMPARE(cache.size(), input.size());
}

void TestLRUCache::
test_operator_subscript_should_search_for_a_value_if_exists() {
  std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  cache_t<int> cache;

  // iterate over the list in normal order to make the most recent to be the
  // last element
  for (auto it = input.rbegin(); it != input.rend(); it++) {
    const auto& result{ cache[*it] };
    QCOMPARE(result, *it);
  }

  QCOMPARE(cache.values(), input);
  QCOMPARE(cache.size(), input.size());

  // iterate over the list that is reversed to make the most recent be the
  // first element
  for (auto it = input.begin(); it != input.end(); it++) {
    const auto& result{ cache[*it] };
    QCOMPARE(result, *it);
    QCOMPARE(result, cache.most_recent());
    QVERIFY2(!cache.last_operation(),
             "Value should not be inserted, only searched");
  }

  QCOMPARE(cache.size(), input.size());

  // Reverse back the input to behave as the expected values:
  std::reverse(input.begin(), input.end());
  QCOMPARE(cache.values(), input);
}

void TestLRUCache::
test_most_recent_value() {
  std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  cache_t<int> cache;

  // iterate over the list in normal order to make the most recent to be the
  // last element
  for (auto it = input.rbegin(); it != input.rend(); it++) {
    (void)cache[*it];
    QCOMPARE(cache.most_recent(), *it);
  }

  // iterate over the list in normal order to make the most recent to be the
  // last element
  for (auto it = input.rbegin(); it != input.rend(); it++) {
    (void)cache.find(*it);
    QCOMPARE(cache.most_recent(), *it);
  }
}

void TestLRUCache::
test_least_recent_value() {
  std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  cache_t<int> cache;

  // iterate over the list in normal order to make the most recent to be the
  // last element
  for (auto it = input.rbegin(); it != input.rend(); it++) {
    (void)cache[*it];
    QCOMPARE(cache.least_recent(), input.back());
  }

  // iterate over the list in normal order to make the most recent to be the
  // last element
  for (auto it = input.rbegin(); it != std::prev(input.rend()); it++) {
    (void)cache.find(*it);
    QCOMPARE(cache.least_recent(), *(std::next(it)));
  }
}

void TestLRUCache::
test_clipped_getter() {
  const std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  const size_t max_capacity{ 50 };
  cache_t<int> cache;
  cache.set_capacity(max_capacity);

  // iterate over the list in normal order to make the most recent to be the
  // last element
  for (auto it = input.rbegin(); it != input.rend(); it++) {
    (void)cache[*it];
    auto distance { std::distance(input.rbegin(), it) };
    if (distance >= 50) {
      QCOMPARE(cache.clipped(), 1u);
    }
    else {
      QCOMPARE(cache.clipped(), 0u);
    }
  }
}

void TestLRUCache::
test_clip_method_should_keep_the_capacity_consistent() {
  std::list<std::string> input {
    "62", "171", "-572", "-898", "204", "94", "-286", "-750", "306", "680",
    "66", "-180", "161", "-76", "-861", "-574", "292", "-155", "909", "-379",
    "201", "-210", "-230", "194", "120", "-222", "-121", "964", "493", "-864",
    "253", "953", "-333", "455", "-424", "830", "-955", "155", "-415", "586",
    "-305", "-363", "-293", "-195", "-968", "728", "560", "569", "431", "436",
    "-839", "600", "521", "-464", "173", "221", "753", "893", "836", "671",
    "-552", "773", "-447", "-250", "-620", "-690", "-949", "-737", "-479", "-830",
    "179", "865", "775", "67", "428", "-513", "530", "69", "-718", "866",
    "708", "-100", "957", "-948", "24", "114", "-476", "244", "-591", "704",
    "-359", "-523", "628", "247", "-680", "236", "-683", "-383", "-665", "-470",
  };

  const size_t max_capacity{ 50 };
  test::structures::extended_cache cache;
  cache.set_capacity(max_capacity);

  populate(input.rbegin(), input.rend(), cache);
  QCOMPARE(cache.size(),          max_capacity);
  QCOMPARE(cache.items_in_map(),  max_capacity);
  QCOMPARE(cache.items_in_list(), max_capacity);

  input.resize(50);
  QCOMPARE(cache.values(), input);
}

void TestLRUCache::
test_countains_method_should_return_whether_a_value_is_present_or_not() {
  const std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  const size_t max_capacity{ 50 };
  cache_t<int> cache;
  cache.set_capacity(max_capacity);

  populate(input.rbegin(), input.rend(), cache);
  QCOMPARE(cache.size(), max_capacity);

  const auto& cache_ref{ cache };

  for (auto it = input.begin(); it != input.end(); it++) {
    auto distance{ std::distance(input.begin(), it) };
    if (distance >= 50) {
      QVERIFY2(!cache_ref.contains(*it),
               "Cache should not contain the other values");
    }
    else {
      QVERIFY2(cache_ref.contains(*it),
               "Cache should contain the first 50 values");
    }
  }
}

void TestLRUCache::
test_set_and_get_capacity() {
  const std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  const size_t max_capacity{ 50u };
  cache_t<int> cache;
  cache.set_capacity(max_capacity);

  populate(input.rbegin(), input.rend(), cache);
  QCOMPARE(cache.size(), max_capacity);

  cache.set_capacity(25);
  QCOMPARE(cache.size(),      25u);
  QCOMPARE(cache.capacity(),  25u);
  QCOMPARE(cache.clipped(),   max_capacity - 25u);

  cache.set_capacity(1);
  QCOMPARE(cache.size(),      1u);
  QCOMPARE(cache.capacity(),  1u);
  QCOMPARE(cache.clipped(),   25u - 1u);

  cache.set_capacity(100);
  QCOMPARE(cache.size(),      1u);
  QCOMPARE(cache.capacity(),  100u);
  QCOMPARE(cache.clipped(),   0u);
}

void TestLRUCache::
test_non_default_constructor() {
  const std::list<int> input {
    62, 171, -572, -898, 204, 94, -286, -750, 306, 680,
    66, -180, 161, -76, -861, -574, 292, -155, 909, -379,
    201, -210, -230, 194, 120, -222, -121, 964, 493, -864,
    253, 953, -333, 455, -424, 830, -955, 155, -415, 586,
    -305, -363, -293, -195, -968, 728, 560, 569, 431, 436,
    -839, 600, 521, -464, 173, 221, 753, 893, 836, 671,
    -552, 773, -447, -250, -620, -690, -949, -737, -479, -830,
    179, 865, 775, 67, 428, -513, 530, 69, -718, 866,
    708, -100, 957, -948, 24, 114, -476, 244, -591, 704,
    -359, -523, 628, 247, -680, 236, -683, -383, -665, -470,
  };

  const size_t max_capacity{ 50 };
  cache_t<int> cache{ max_capacity };

  populate(input.rbegin(), input.rend(), cache);
  QCOMPARE(cache.size(), max_capacity);

  cache.set_capacity(25);
  QCOMPARE(cache.size(),      25u);
  QCOMPARE(cache.capacity(),  25u);
  QCOMPARE(cache.clipped(),   max_capacity - 25u);

  cache.set_capacity(1);
  QCOMPARE(cache.size(),      1u);
  QCOMPARE(cache.capacity(),  1u);
  QCOMPARE(cache.clipped(),   25u - 1u);

  cache.set_capacity(100);
  QCOMPARE(cache.size(),      1u);
  QCOMPARE(cache.capacity(),  100u);
  QCOMPARE(cache.clipped(),   0u);
}

void TestLRUCache::
test_on_discarded_method() {
  const std::vector<std::string> input {
    "62", "171", "-572", "-898", "204", "94", "-286", "-750", "306", "680",
    "66", "-180", "161", "-76", "-861", "-574", "292", "-155", "909", "-379",
    "201", "-210", "-230", "194", "120", "-222", "-121", "964", "493", "-864",
    "253", "953", "-333", "455", "-424", "830", "-955", "155", "-415", "586",
    "-305", "-363", "-293", "-195", "-968", "728", "560", "569", "431", "436",
    "-839", "600", "521", "-464", "173", "221", "753", "893", "836", "671",
    "-552", "773", "-447", "-250", "-620", "-690", "-949", "-737", "-479", "-830",
    "179", "865", "775", "67", "428", "-513", "530", "69", "-718", "866",
    "708", "-100", "957", "-948", "24", "114", "-476", "244", "-591", "704",
    "-359", "-523", "628", "247", "-680", "236", "-683", "-383", "-665", "-470",
  };

  const size_t max_capacity{ 50 };
  test::structures::extended_cache cache;
  cache.set_capacity(50);

  populate(input.rbegin(), input.rend(), cache);
  QCOMPARE(cache.size(), max_capacity);
  QCOMPARE(std::vector<std::string>(input.rbegin(), input.rbegin() + 50),
           cache.discarded_values());
}

void TestLRUCache::
test_lru_cache_with_complex_datatypes() {
  using data_t = test::structures::complex_data_type;
  std::vector<data_t> values(10);
  cache_t<data_t> cache(5);

  for (const auto& value : values) {
    cache[value];
  }

  for (size_t ii = 0; ii < values.size(); ii++) {
    if (ii < 5) {
      QVERIFY2(!cache.contains(values[ii]),
               "Cache should not contain the first half of the elements");
    }
    else {
      QVERIFY2(cache.contains(values[ii]),
               "Cache should contain the second half of the elements");
      QVERIFY2(cache.contains(data_t{ static_cast<long>(ii) }),
               "Cache should contain the second half of the elements");
    }
  }


}

void TestLRUCache::
test_for_range_loop() {
  const std::vector<std::string> input {
    "alabama", "new york", "washington", "illinois", "california",
    "texas", "new mexico"
  };

  cache_t<std::string> cache;
  populate(input.rbegin(), input.rend(), cache);

  auto it{ input.begin() };
  const auto& cache_ref{ cache };
  for (const auto& location : cache) {
    QCOMPARE(location, *it);
    it++;
  }

  auto cit{ input.cbegin() };
  for (const auto& location : cache_ref) {
    QCOMPARE(location, *cit);
    cit++;
  }

  for (auto it = cache.rbegin(); it != cache.rend(); it++) { }
}

void TestLRUCache::
test_remove_element_from_cache() {
  const std::vector<std::string> input {
    "alabama", "new york", "washington", "illinois", "california",
    "texas", "new mexico"
  };

  cache_t<std::string> cache;
  populate(input.rbegin(), input.rend(), cache);

  cache.remove("texas");
  QVERIFY(!cache.contains("texas"));

  auto it{ input.begin() };
  for (const auto& location : cache) {
    QCOMPARE(location, *it);
    it++;
    if (it != input.end() && *it == "texas") {
      it++;
    }
  }

  for (const auto& location : input) {
    cache.remove(location).remove(location);
  }
}

void TestLRUCache::
test_move_copy() {
  const std::vector<std::string> input {
    "alabama", "new york", "washington", "illinois", "california",
    "texas", "new mexico"
  };

  {
    cache_t<std::string> cache, copied, moved;
    populate(input.rbegin(), input.rend(), cache);
    copied = cache;
    moved  = std::move(cache);

    QVERIFY(cache.empty());
    auto it { copied.begin() };
    auto mit { moved.begin() };
    for (const auto& location : input) {
      QCOMPARE(location, *it);
      QCOMPARE(location, *mit);
      it++;
      mit++;
    }
  }
  {
    cache_t<std::string> cache;
    populate(input.rbegin(), input.rend(), cache);
    cache_t<std::string> copied{ cache };
    cache_t<std::string> moved{ std::move(cache) };
    QVERIFY(cache.empty());
    auto it { copied.begin() };
    auto mit { moved.begin() };
    for (const auto& location : input) {
      QCOMPARE(location, *it);
      QCOMPARE(location, *mit);
      it++;
      mit++;
    }
  }
}

void TestLRUCache::
test_cache_as_pointers() {
  std::unique_ptr<cache_t<long>> cache_ptr = std::make_unique<cache_t<long>>();
  const std::list<long> input {
    34l, 3l, 5l, 67l, -345l, 9230797509l, -28394834l
  };

  cache_t<long>& cache { *cache_ptr};
  // insert in reverse order:
  populate(input.rbegin(), input.rend(), cache);
  QCOMPARE(cache.values(), input);
}

void TestLRUCache::
test_clear() {
  const std::list<long> input {
    34l, 3l, 5l, 67l, -345l, 9230797509l, -28394834l
  };
  cache_t<long> cache;

  // insert in reverse order:
  populate(input.rbegin(), input.rend(), cache);
  cache.clear();
  QVERIFY(cache.empty());
}

namespace test {
namespace structures {

void extended_cache::
on_discard(const value_t &value) {
  _discarded_values.push_back(value);
}

}
}
