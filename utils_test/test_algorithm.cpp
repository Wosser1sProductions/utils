#include "test_settings.hpp"

#ifdef ENABLE_TESTS
#include "../utils_lib/external/doctest.hpp"

#include "../utils_lib/utils_algorithm.hpp"

#include "../utils_lib/utils_random.hpp"
#include <map>


TEST_CASE("Test utils::algorithm::contains") {
    std::vector<int> empty, test(10);
    std::iota(test.begin(), test.end(), 0);

    CHECK_FALSE(utils::algorithm::contains(empty, 0));
    CHECK_FALSE(utils::algorithm::contains(test, 11));

    for (size_t i = 0; i < 10; i++) {
        const auto found = utils::algorithm::contains(test, i);
        CHECK(found);
        CHECK(*found == i);
    }

    {
        const auto found = utils::algorithm::contains("abcdefgh", 'e');
        CHECK(found);
        CHECK(*found == 4);
    }

    {
        std::map<std::string, int> mfind{{"a", 0}, {"b", 1}, {"c", 2}, {"d", 3}};
        const auto found = utils::algorithm::contains(mfind, std::string("c"));
        CHECK(found);
        CHECK(*found == 2);
    }
}

TEST_CASE("Test utils::algorithm::all") {
    REQUIRE(utils::algorithm::all());

    REQUIRE(utils::algorithm::all(true));
    REQUIRE(utils::algorithm::all(-1));
    REQUIRE(utils::algorithm::all(1));
    REQUIRE(utils::algorithm::all(1.125f));
    REQUIRE(utils::algorithm::all(3.625));

    REQUIRE_FALSE(utils::algorithm::all(false));
    REQUIRE_FALSE(utils::algorithm::all(0));

    REQUIRE_FALSE(utils::algorithm::all( true, true, true, false, true, true,  true, true, true, true));
    REQUIRE_FALSE(utils::algorithm::all( true, true, true, false, true, true,  true, true, true, true));
    REQUIRE_FALSE(utils::algorithm::all(false, true, true,  true, true, true,  true, true, true, true));
    REQUIRE_FALSE(utils::algorithm::all( true, true, true, false, true, true, false, true, true, true));
    REQUIRE_FALSE(utils::algorithm::all( true, true, true, false, true, true,  true, true, true, false));

    REQUIRE(utils::algorithm::all(true, true, true, true, true, true,  true, true, true, true));
}

TEST_CASE("Test utils::algorithm::any") {
    REQUIRE_FALSE(utils::algorithm::any());

    REQUIRE(utils::algorithm::any(true));
    REQUIRE(utils::algorithm::any(-1));
    REQUIRE(utils::algorithm::any(1));
    REQUIRE(utils::algorithm::any(1.125f));
    REQUIRE(utils::algorithm::any(3.625));

    REQUIRE_FALSE(utils::algorithm::any(false));
    REQUIRE_FALSE(utils::algorithm::any(0));

    REQUIRE(utils::algorithm::any( true, true, true, false, true, true,  true, true, true, true));
    REQUIRE(utils::algorithm::any( true, true, true, false, true, true,  true, true, true, true));
    REQUIRE(utils::algorithm::any(false, true, true,  true, true, true,  true, true, true, true));
    REQUIRE(utils::algorithm::any( true, true, true, false, true, true, false, true, true, true));
    REQUIRE(utils::algorithm::any( true, true, true, false, true, true,  true, true, true, false));
    REQUIRE(utils::algorithm::any(false, false, false, true, false, false, false, false, false, false));

    REQUIRE_FALSE(utils::algorithm::any(false, false, false, false, false, false, false, false, false, false));
}

TEST_CASE("Test utils::algorithm::none") {
    REQUIRE(utils::algorithm::none());

    REQUIRE_FALSE(utils::algorithm::none(true));
    REQUIRE_FALSE(utils::algorithm::none(-1));
    REQUIRE_FALSE(utils::algorithm::none(1));
    REQUIRE_FALSE(utils::algorithm::none(1.125f));
    REQUIRE_FALSE(utils::algorithm::none(3.625));

    REQUIRE(utils::algorithm::none(false));
    REQUIRE(utils::algorithm::none(0));

    REQUIRE_FALSE(utils::algorithm::none( true, true, true, false, true, true,  true, true, true, true));
    REQUIRE_FALSE(utils::algorithm::none( true, true, true, false, true, true,  true, true, true, true));
    REQUIRE_FALSE(utils::algorithm::none(false, true, true,  true, true, true,  true, true, true, true));
    REQUIRE_FALSE(utils::algorithm::none( true, true, true, false, true, true, false, true, true, true));
    REQUIRE_FALSE(utils::algorithm::none( true, true, true, false, true, true,  true, true, true, false));
    REQUIRE_FALSE(utils::algorithm::none(false, false, false, true, false, false, false, false, false, false));

    REQUIRE(utils::algorithm::none(false, false, false, false, false, false, false, false, false, false));
}

TEST_CASE("Test utils::algorithm::all_equal") {
    REQUIRE(utils::algorithm::all_equal());

    REQUIRE(utils::algorithm::all_equal(true));
    REQUIRE(utils::algorithm::all_equal(-1));
    REQUIRE(utils::algorithm::all_equal(1));
    REQUIRE(utils::algorithm::all_equal(1.125f));
    REQUIRE(utils::algorithm::all_equal(3.625));

    REQUIRE(utils::algorithm::all_equal(false));
    REQUIRE(utils::algorithm::all_equal(0));

    REQUIRE_FALSE(utils::algorithm::all_equal( true, true, true, false, true, true,  true, true, true, true));
    REQUIRE_FALSE(utils::algorithm::all_equal( true, true, true, false, true, true,  true, true, true, true));
    REQUIRE_FALSE(utils::algorithm::all_equal(false, true, true,  true, true, true,  true, true, true, true));
    REQUIRE_FALSE(utils::algorithm::all_equal( true, true, true, false, true, true, false, true, true, true));
    REQUIRE_FALSE(utils::algorithm::all_equal( true, true, true, false, true, true,  true, true, true, false));
    REQUIRE_FALSE(utils::algorithm::all_equal(false, false, false, true, false, false, false, false, false, false));

    REQUIRE(utils::algorithm::all_equal(false, false, false, false, false, false, false, false, false, false));
    REQUIRE(utils::algorithm::all_equal(true, true, true, true, true, true, true, true, true, true));

    REQUIRE(utils::algorithm::all_equal(321123.7774, 321123.7774, 321123.7774, 321123.7774));
    REQUIRE_FALSE(utils::algorithm::all_equal(321123.7774, 321123.777400001, 321123.7774, 321123.7774));

    REQUIRE(utils::algorithm::all_equal(42, 42, 42, 42, 42, 42, 42, 42, 42));
    REQUIRE_FALSE(utils::algorithm::all_equal(42, 42, 42.4, 42, 42, 42, 42, 42));
    REQUIRE_FALSE(utils::algorithm::all_equal(42, 42, 42, 42, 42, 42, 43, 42, 52));
}

TEST_CASE("Test utils::algorithm::all_within") {
    REQUIRE(utils::algorithm::all_within(0, 10));
    REQUIRE(utils::algorithm::all_within(0, 10, 0));
    REQUIRE(utils::algorithm::all_within(0, 10, 10));
    REQUIRE(utils::algorithm::all_within(0, 10, 1, 2, 3, 4));
    REQUIRE(utils::algorithm::all_within(10, 0, 1, 2, 10, 4));
    REQUIRE_FALSE(utils::algorithm::all_within(0, 10, 1, 2, 11, 4));
    REQUIRE_FALSE(utils::algorithm::all_within(10, 0, 1, 2, 11, 4));

    REQUIRE(utils::algorithm::all_within(1, 1, 1, 1, 1, 1));
    REQUIRE_FALSE(utils::algorithm::all_within(1, 1, 1, 1, 0, 1));

    REQUIRE(utils::algorithm::all_within(0.0, 1.0, 1e-10, 1e-6, 0.2, 0.555, 0.99999, 1.0));

    auto c = utils::random::generate_x<int>(10, 0, 10);
    REQUIRE(utils::algorithm::within(0, 10, c));
    REQUIRE(utils::algorithm::within(0, 10, c.begin(), c.end()));
    c.push_back(12);
    CHECK_FALSE(utils::algorithm::within(0, 10, c));
    CHECK_FALSE(utils::algorithm::within(0, 10, c.begin(), c.end()));

    c = utils::random::generate_x<int>(10, 15, 0);
    CHECK(utils::algorithm::within(0, 15, c));
    CHECK(utils::algorithm::within(0, 15, c.begin(), c.end()));
    CHECK(utils::algorithm::within(15, 0, c));
    CHECK(utils::algorithm::within(15, 0, c.begin(), c.end()));
}

TEST_CASE("Test utils::algorithm::sum") {
    constexpr int sum = 55;
    const std::vector<int>    e{};
    const std::vector<int>    x{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const std::vector<double> y{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    REQUIRE(utils::algorithm::sum(e) == 0);
    REQUIRE(utils::algorithm::sum(e.begin(), e.end()) == 0);
    REQUIRE(utils::algorithm::sum(x) == sum);
    REQUIRE(utils::algorithm::sum(x.begin(), x.end()) == sum);
    REQUIRE(utils::algorithm::sum(y) == doctest::Approx(sum));
    REQUIRE(utils::algorithm::sum(y.begin(), y.end()) == doctest::Approx(sum));
}

TEST_CASE("Test utils::algorithm::product") {
    constexpr int prod = 3628800;
    const std::vector<int>    e{};
    const std::vector<int>    x{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    const std::vector<double> y{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    REQUIRE(utils::algorithm::product(e) == 0);
    REQUIRE(utils::algorithm::product(e.begin(), e.end()) == 0);
    REQUIRE(utils::algorithm::product(x) == prod);
    REQUIRE(utils::algorithm::product(x.begin(), x.end()) == prod);
    REQUIRE(utils::algorithm::product(y) == doctest::Approx(prod));
    REQUIRE(utils::algorithm::product(y.begin(), y.end()) == doctest::Approx(prod));
}

TEST_CASE("Test utils::algorithm::reverse") {
    std::vector<int>    e{};
    std::vector<int>    x{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::vector<double> y{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

    utils::algorithm::reverse(e);
    utils::algorithm::reverse(x);
    utils::algorithm::reverse(y);

    REQUIRE(e.size() ==  0);
    REQUIRE(x.size() == 10);
    REQUIRE(y.size() == 10);

    for (size_t i = 0; i < 10; i++) {
        CHECK(x[i] == (10-i));
        CHECK(y[i] == doctest::Approx(10.0-i));
    }

    utils::algorithm::reverse(x.begin(), x.end());
    for (size_t i = 0; i < 10; i++) {
        CHECK(x[i] == (i+1));
    }
}

static void __test_func(int& x) { x /= 5; }

TEST_CASE("Test utils::algorithm::repeat") {
    int test = 0;
    utils::algorithm::repeat<5>([&](){ test++; });
    REQUIRE(test == 5);

    utils::algorithm::repeat([&](){ test *= 10; });
    REQUIRE(test == 50);

    utils::algorithm::repeat<2>(__test_func, test);
    REQUIRE(test == 2);

    utils::algorithm::repeat<1>([&](){ test--; CHECK(true); });
    utils::algorithm::repeat<0>([&](){ test--; });
    REQUIRE(test == 1);
}

TEST_CASE("Test utils::algorithm::for_each") {
    std::vector<int> test(10);
    std::iota(test.begin(), test.end(), 0);
    int cnt = 0;

    utils::algorithm::for_each(test.begin(), test.end(), [&](int val) {
        CHECK(val == cnt++);
    });

    cnt = 0;
    utils::algorithm::for_each(test, [&](int val) {
        CHECK(val == cnt++);
    });
}

TEST_CASE("Test utils::algorithm::min_element") {
    auto     test = utils::random::generate_x<int>(20, 0, 10);
    const int min = utils::random::Random::get(-10, -1);
    auto   it_min = utils::random::Random::get(test);
    *it_min = min;

    const auto found_it  = utils::algorithm::min_element(test.begin(), test.end());
    const auto found_it2 = utils::algorithm::min_element(test);
    CHECK( found_it  == it_min);
    CHECK(*found_it  == min);
    CHECK( found_it2 == it_min);
    CHECK(*found_it2 == min);
}

TEST_CASE("Test utils::algorithm::max_element") {
    auto     test = utils::random::generate_x<int>(20, 0, 10);
    const int max = utils::random::Random::get(11, 100);
    auto   it_max = utils::random::Random::get(test);

    *it_max = max;

    const auto found_it  = utils::algorithm::max_element(test.begin(), test.end());
    const auto found_it2 = utils::algorithm::max_element(test);
    CHECK( found_it  == it_max);
    CHECK(*found_it  == max);
    CHECK( found_it2 == it_max);
    CHECK(*found_it2 == max);
}

TEST_CASE("Test utils::algorithm::is_ascending") {
    std::vector<int> test(10);
    std::iota(test.begin(), test.end(), 0);

    REQUIRE(utils::algorithm::is_ascending(test));
    REQUIRE(utils::algorithm::is_ascending(test.begin(), test.end()));
    REQUIRE_FALSE(utils::algorithm::is_ascending(test.rbegin(), test.rend()));

    std::reverse(test.begin(), test.end());

    REQUIRE_FALSE(utils::algorithm::is_ascending(test));
    REQUIRE_FALSE(utils::algorithm::is_ascending(test.begin(), test.end()));
    REQUIRE(utils::algorithm::is_ascending(test.rbegin(), test.rend()));
}

TEST_CASE("Test utils::algorithm::is_descending") {
    std::vector<int> test(10);
    std::iota(test.begin(), test.end(), 0);

    REQUIRE_FALSE(utils::algorithm::is_descending(test));
    REQUIRE_FALSE(utils::algorithm::is_descending(test.begin(), test.end()));
    REQUIRE(utils::algorithm::is_descending(test.rbegin(), test.rend()));

    std::reverse(test.begin(), test.end());

    REQUIRE(utils::algorithm::is_descending(test));
    REQUIRE(utils::algorithm::is_descending(test.begin(), test.end()));
    REQUIRE_FALSE(utils::algorithm::is_descending(test.rbegin(), test.rend()));
}

TEST_CASE("Test utils::algorithm::sort") {
    std::vector<int> test(10);
    std::iota(test.begin(), test.end(), 0);

    REQUIRE(utils::algorithm::is_ascending(test));
    utils::algorithm::sort::insertion(test.begin(), test.end());
    REQUIRE(utils::algorithm::is_ascending(test));

    REQUIRE(utils::algorithm::is_ascending(test));
    utils::algorithm::sort::quick(test.begin(), test.end());
    REQUIRE(utils::algorithm::is_ascending(test));

    utils::random::Random::shuffle(test.begin(), test.end());
    utils::algorithm::sort::insertion(test.begin(), test.end());
    REQUIRE(utils::algorithm::is_ascending(test));

    utils::random::Random::shuffle(test.begin(), test.end());
    utils::algorithm::sort::quick(test.begin(), test.end());
    REQUIRE(utils::algorithm::is_ascending(test));
}

TEST_CASE("Test utils::algorithm::enumerate") {
    std::vector<int> test(10);
    std::iota(test.begin(), test.end(), 0);
    int cnt = 0;

    for (auto [i, val] : utils::algorithm::enumerate(test)) {
        CHECK(i == cnt++);
        CHECK(i == val);
    }

    const auto randoffset = utils::random::Random::get<size_t>(1, 100);

    for (auto [i, val] : utils::algorithm::enumerate(test, randoffset)) {
        CHECK(i == size_t(val) + randoffset);
    }

    REQUIRE(cnt == 10);
}

#endif
