#include "test_settings.hpp"

#ifdef ENABLE_TESTS
#include "../utils_lib/external/doctest.hpp"

#include "../utils_lib/utils_bits.hpp"


TEST_CASE("Test utils::bits::size_of") {
    REQUIRE(utils::bits::size_of< int8_t>()  == 8);
    REQUIRE(utils::bits::size_of<uint8_t>()  == 8);
    REQUIRE(utils::bits::size_of< int16_t>() == 16);
    REQUIRE(utils::bits::size_of<uint16_t>() == 16);
    REQUIRE(utils::bits::size_of< int32_t>() == 32);
    REQUIRE(utils::bits::size_of<uint32_t>() == 32);
    REQUIRE(utils::bits::size_of< int64_t>() == 64);
    REQUIRE(utils::bits::size_of<uint64_t>() == 64);
}

TEST_CASE("Test utils::bits::mask") {
    REQUIRE(utils::bits::mask_one(0) == 0);
    REQUIRE(utils::bits::mask_msb(0) == 0);
    REQUIRE(utils::bits::mask_lsb(0) == 0);
    REQUIRE(utils::bits::mask_one(64) == (1ull << 63));
    REQUIRE(utils::bits::mask_msb(64) == ~0ull);
    REQUIRE(utils::bits::mask_lsb(64) == ~0ull);
    REQUIRE(utils::bits::mask_one(65) == 0);
    REQUIRE(utils::bits::mask_msb(65) == ~0ull);
    REQUIRE(utils::bits::mask_lsb(65) == ~0ull);

    int64_t msb = int64_t(1ull << 63), lsb = 1ull;

    for (uint32_t i = 1; i < 65; i++, msb >>= 1, lsb = (lsb << 1) | 1) {
        CHECK(utils::bits::mask_one(i) == (1ull << (i-1)));
        CHECK(utils::bits::mask_msb(i) == uint64_t(msb));
        CHECK(utils::bits::mask_lsb(i) == uint64_t(lsb));
    }
}

TEST_CASE("Test utils::bits::select") {
    REQUIRE(utils::bits::select_one(0, 1) == 0);
    REQUIRE(utils::bits::select_msb(0, 1) == 0);
    REQUIRE(utils::bits::select_lsb(0, 1) == 0);
    REQUIRE(utils::bits::select_one(1, 1) == 1);
    REQUIRE(utils::bits::select_msb(1, 1) == 0);
    REQUIRE(utils::bits::select_lsb(1, 1) == 1);
    REQUIRE(utils::bits::select_one((1ull << 63), 1) == 0);
    REQUIRE(utils::bits::select_msb((1ull << 63), 1) == 1);
    REQUIRE(utils::bits::select_lsb((1ull << 63), 1) == 0);
}

TEST_CASE("Test utils::bits::set") {
    int x = 0;
    REQUIRE(utils::bits::set_one(x, 1,  1)   == 0x00000001);
    REQUIRE(utils::bits::set_one(x, 1, 32)   == 0x80000001);
    REQUIRE(utils::bits::set_one(x, 0,  1)   == 0x80000000);
    REQUIRE(utils::bits::set_msb(x, 0x07, 4) == 0x70000000);
    REQUIRE(utils::bits::set_msb(x, 0x0F, 5) == 0x78000000);
    REQUIRE(utils::bits::set_lsb(x, 0x01, 1) == 0x78000001);
    REQUIRE(utils::bits::set_lsb(x, 0x55, 8) == 0x78000055);
    REQUIRE(utils::bits::set_lsb(x, 0x00, 8) == 0x78000000);
    REQUIRE(utils::bits::set_lsb(x, 0x30, 6) == 0x78000030);
    REQUIRE(utils::bits::set_lsb(x, 0x11, 5) == 0x78000031);
}

TEST_CASE("Test utils::bits::and_all") {
    REQUIRE(utils::bits::and_all( 0) ==  0);
    REQUIRE(utils::bits::and_all( 1) ==  1);
    REQUIRE(utils::bits::and_all(-1) == -1);

    REQUIRE(0x0000 == utils::bits::and_all(0xFF00, 0x00FF));
    REQUIRE(0x1234 == utils::bits::and_all(0xFFFF, 0x1234));
    REQUIRE(0x1004 == utils::bits::and_all(0x100F, 0x1234, 0xFFFC));
}

TEST_CASE("Test utils::bits::or_all") {
    REQUIRE(utils::bits::or_all( 0) ==  0);
    REQUIRE(utils::bits::or_all( 1) ==  1);
    REQUIRE(utils::bits::or_all(-1) == -1);

    REQUIRE(0xFFFF == utils::bits::or_all(0xFF00, 0x00FF));
    REQUIRE(0xFFFF == utils::bits::or_all(0xFFFF, 0x1234));
    REQUIRE(0xFFFF == utils::bits::or_all(0x100F, 0x1234, 0xFFFC));
    REQUIRE(0xF050 == utils::bits::or_all(0x8000, 0x4000, 0x2000, 0x1000, 0x0050));
}

TEST_CASE("Test utils::bits::xor_all") {
    REQUIRE(utils::bits::xor_all( 0) ==  0);
    REQUIRE(utils::bits::xor_all( 1) ==  1);
    REQUIRE(utils::bits::xor_all(-1) == -1);
    REQUIRE(utils::bits::xor_all(1, 1) == 0);

    REQUIRE(0xFFFF == utils::bits::xor_all(0xFF00, 0x00FF));
    REQUIRE(0xEDCB == utils::bits::xor_all(0xFFFF, 0x1234));
    REQUIRE(0xFDC7 == utils::bits::xor_all(0x100F, 0x1234, 0xFFFC));
    REQUIRE(0xF050 == utils::bits::xor_all(0x8000, 0x4000, 0x2000, 0x1000, 0x0050));
}

TEST_CASE("Test utils::bits::ffs" ) {
    REQUIRE(utils::bits::ffs<int>(-1)     == 1);
    REQUIRE(utils::bits::ffs<int>(1)      == 1);
    REQUIRE(utils::bits::ffs<int>(0)      == 0);
    REQUIRE(utils::bits::ffs<int>(0x8001) == 1);

    for (uint64_t i = 65, mask = 1ull << 63; i--; mask >>= 1) {
        CHECK(utils::bits::ffs(mask) == i);
    }
}

TEST_CASE("Test utils::bits::msb" ) {
    REQUIRE(utils::bits::msb<int>(1)      == 1);
    REQUIRE(utils::bits::msb<int>(-1)     == utils::bits::size_of<int>());
    REQUIRE(utils::bits::msb<int>(0)      == 0);
    REQUIRE(utils::bits::msb<int>(0x8001) == 16);

    for (uint64_t i = 65, mask = 1ull << 63; i--; mask >>= 1) {
        CHECK(utils::bits::msb(mask) == i);
    }
}

TEST_CASE("Test utils::bits::popcount" ) {
    REQUIRE(utils::bits::popcount<int>(0)      == 0);
    REQUIRE(utils::bits::popcount<int>(1)      == 1);
    REQUIRE(utils::bits::popcount<int>(-1)     == utils::bits::size_of<int>());
    REQUIRE(utils::bits::popcount<int>(0x8001) == 2);
    REQUIRE(utils::bits::popcount<int>(0xFF)   == 8);

    for (uint64_t i = 64, mask = 1ull << 63; i--; mask >>= 1) {
        CHECK(utils::bits::popcount(mask) == 1);
    }
}

TEST_CASE("Test utils::bits::rotl" ) {
    REQUIRE(0x00 == utils::bits::rotl(0));
    REQUIRE(0xFF == utils::bits::rotl<uint8_t>(0xFF, 8));
    REQUIRE(0xA5 == utils::bits::rotl<uint8_t>(0xA5, 8));
    REQUIRE(0xA5 == utils::bits::rotl<uint8_t>(0x5A, 4));

    REQUIRE(0x01 == utils::bits::rotl(uint8_t(0x80), 1));
    REQUIRE(0x08 == utils::bits::rotl(uint8_t(0x04), 1));
    REQUIRE(0x10 == utils::bits::rotl(uint8_t(0x08), 1));
    REQUIRE(0xF0 == utils::bits::rotl(uint8_t(0x0F), 4));
    REQUIRE(0x0F == utils::bits::rotl(uint8_t(0xF0), 4));
    REQUIRE(0x81 == utils::bits::rotl(uint8_t(0x18), 4));
    REQUIRE(0x03 == utils::bits::rotl(uint8_t(0x81), 1));
    REQUIRE(0xBEEFDEAD == utils::bits::rotl(0xDEADBEEF, 16));

    REQUIRE(int32_t(0xFFFFFF01) == utils::bits::rotl(int32_t(0xFFFFFF80), 1));
    REQUIRE(int32_t(0x00000001) == utils::bits::rotl<int32_t,  8>(int32_t(0x12345680), 1));
    REQUIRE(int32_t(0x00000000) == utils::bits::rotl<int32_t, 16>(int32_t(0xDEAD0000), 1));

    REQUIRE((uint64_t(1) << 63) == utils::bits::rotl(uint64_t(1) << 62, 1));
    REQUIRE( uint64_t(1)        == utils::bits::rotl(uint64_t(1) << 63, 1));
}

TEST_CASE("Test utils::bits::rotr" ) {
    REQUIRE(0x00 == utils::bits::rotr(0));
    REQUIRE(0xFF == utils::bits::rotr<uint8_t>(0xFF, 8));
    REQUIRE(0xA5 == utils::bits::rotr<uint8_t>(0xA5, 8));
    REQUIRE(0xA5 == utils::bits::rotr<uint8_t>(0x5A, 4));

    REQUIRE(0x40 == utils::bits::rotr(uint8_t(0x80), 1));
    REQUIRE(0x02 == utils::bits::rotr(uint8_t(0x04), 1));
    REQUIRE(0x04 == utils::bits::rotr(uint8_t(0x08), 1));
    REQUIRE(0xF0 == utils::bits::rotr(uint8_t(0x0F), 4));
    REQUIRE(0x0F == utils::bits::rotr(uint8_t(0xF0), 4));
    REQUIRE(0x81 == utils::bits::rotr(uint8_t(0x18), 4));
    REQUIRE(0xC0 == utils::bits::rotr(uint8_t(0x81), 1));
    REQUIRE(0xBEEFDEAD == utils::bits::rotr(0xDEADBEEF, 16));

    REQUIRE(int32_t(0x7FFFFFC0) == utils::bits::rotr(int32_t(0xFFFFFF80), 1));
    REQUIRE(int32_t(0x00000040) == utils::bits::rotr<int32_t,  8>(int32_t(0x12345680), 1));
    REQUIRE(int32_t(0x00000000) == utils::bits::rotr<int32_t, 16>(int32_t(0xDEAD0000), 1));

    REQUIRE((uint64_t(1) << 62) == utils::bits::rotr(uint64_t(1) << 63, 1));
    REQUIRE((uint64_t(1) << 63) == utils::bits::rotr(uint64_t(1), 1));
}

TEST_CASE("Test utils::bits::is_power_of_2" ) {
    REQUIRE(utils::bits::is_power_of_2<int>(1)      == true);
    REQUIRE(utils::bits::is_power_of_2<int>(-1)     == false);
    REQUIRE(utils::bits::is_power_of_2<int>(0)      == false);
    REQUIRE(utils::bits::is_power_of_2<int>(0x8000) == true);

    for (uint64_t i = 65, mask = 1ull << 63; --i; mask >>= 1) {
        CHECK(utils::bits::is_power_of_2(mask));
    }
}

TEST_CASE("Test utils::bits::is_odd" ) {
    REQUIRE(utils::bits::is_odd(1)      == true);
    REQUIRE(utils::bits::is_odd(-1)     == true);
    REQUIRE(utils::bits::is_odd(0)      == false);
    REQUIRE(utils::bits::is_odd(0x8000) == false);
    REQUIRE(utils::bits::is_odd(2)      == false);
}

TEST_CASE("Test utils::bits::is_even" ) {
    REQUIRE(utils::bits::is_even(1)      == false);
    REQUIRE(utils::bits::is_even(-1)     == false);
    REQUIRE(utils::bits::is_even(0)      == true);
    REQUIRE(utils::bits::is_even(0x8000) == true);
    REQUIRE(utils::bits::is_even(2)      == true);
}

TEST_CASE("Test utils::bits::round_to_multiple" ) {
    REQUIRE(utils::bits::round_to_multiple(-1, 10)     == 0);
    REQUIRE(utils::bits::round_to_multiple(0 , 10)     == 0);
    REQUIRE(utils::bits::round_to_multiple(0x8000, 16) == 0x8000);

    REQUIRE_FUNCTION_ABORTS_FALSE(utils::bits::round_to_multiple, 1, 1);
    REQUIRE_FUNCTION_ABORTS(utils::bits::round_to_multiple, 1, 0);

    for (int64_t i = 1; i < 10; i++) {
        CHECK(utils::bits::round_to_multiple(i, 10) == 10);
        CHECK(utils::bits::round_to_multiple(-i, 10) == 0);
    }

    CHECK(utils::bits::round_to_multiple(10, 10) == 10);
    CHECK(utils::bits::round_to_multiple(-10, 10) == -10);
}

TEST_CASE("Test utils::bits::round_to_byte" ) {
    REQUIRE(utils::bits::round_to_byte(    0) == 0);
    REQUIRE(utils::bits::round_to_byte(  4*8) == 4);
    REQUIRE(utils::bits::round_to_byte(  255) == 32);
    REQUIRE(utils::bits::round_to_byte(255*8) == 255);

    for (size_t i = 1; i <= 8; i++) {
        CHECK(utils::bits::round_to_multiple(int64_t(i), 8) == 8);
        CHECK(utils::bits::round_to_byte(i)                 == 1);
    }
}

TEST_CASE("Test utils::bits::shift_signed" ) {
    REQUIRE(utils::bits::shift_signed<int8_t>(0, 0) == 0);
    REQUIRE(utils::bits::shift_signed<int8_t>(0, 8) == 0);
    REQUIRE(utils::bits::shift_signed<int8_t>((-1) & 0xFF, 8) == -1);
    REQUIRE(utils::bits::shift_signed<int8_t>((-1) & 0xFF, 8) == -1);

    REQUIRE(utils::bits::shift_signed<uint8_t>(0, 0) == 0);
    REQUIRE(utils::bits::shift_signed<uint8_t>(0, 8) == 0);
    REQUIRE(utils::bits::shift_signed<uint8_t>((-1) & 0xFF, 8) == 0xFF);
    REQUIRE(utils::bits::shift_signed<uint8_t>((-1) & 0xFF, 8) == 0xFF);

    for (size_t i = 1, mask = 1; i <= 8; i++, mask = (mask << 1) | 1) {
        CHECK(utils::bits::shift_signed<int8_t>((-1) & 0xFF, i) == -1);
        CHECK(utils::bits::shift_signed<int8_t>(size_t(-1) & mask, i) == -1);
        CHECK(utils::bits::shift_signed<int8_t>(0, i) == 0);
        CHECK(utils::bits::shift_signed<int8_t>(1, i) == (i > 1 ? 1 : -1));
        CHECK(utils::bits::shift_signed<int16_t>(1, i + 1) == 1);

        CHECK(utils::bits::shift_signed<uint8_t>((-1) & 0xFF, i) == mask);
        CHECK(utils::bits::shift_signed<uint8_t>(size_t(-1) & mask, i) == mask);
        CHECK(utils::bits::shift_signed<uint8_t>(0, i) == 0);
        CHECK(utils::bits::shift_signed<uint8_t>(1, i) == 1);
        CHECK(utils::bits::shift_signed<uint16_t>(1, i + 1) == 1);
    }

    CHECK(utils::bits::shift_signed<int16_t>(0b1000,  4) == -8);
    CHECK(utils::bits::shift_signed<int16_t>(0b1000,  5) ==  8);
    CHECK(utils::bits::shift_signed<int16_t>(0xFFFF, 16) == -1);
    CHECK(utils::bits::shift_signed<int16_t>(0x7FFF, 16) == 32767);
    CHECK(utils::bits::shift_signed<int16_t>(0x7FFF, 15) == -1);
    CHECK(utils::bits::shift_signed<int16_t>(0xDEAD, 16) == -8531);

    CHECK(utils::bits::shift_signed<uint16_t>(0b1000,  4) == 0b1000);
    CHECK(utils::bits::shift_signed<uint16_t>(0b1000,  5) == 0b1000);
    CHECK(utils::bits::shift_signed<uint16_t>(0xFFFF, 16) == 0xFFFF);
    CHECK(utils::bits::shift_signed<uint16_t>(0x7FFF, 16) == 0x7FFF);
    CHECK(utils::bits::shift_signed<uint16_t>(0x7FFF, 15) == 0x7FFF);
    CHECK(utils::bits::shift_signed<uint16_t>(0xDEAD, 16) == 0xDEAD);

    #if UTILS_BITS_ASSERT_SHIFT_SIGNED_SIZE
        CHECK_FUNCTION_ABORTS_FALSE(utils::bits::shift_signed< int8_t>, 0, 8);
        CHECK_FUNCTION_ABORTS      (utils::bits::shift_signed< int8_t>, 0, 9);
        CHECK_FUNCTION_ABORTS_FALSE(utils::bits::shift_signed<int16_t>, 0, 16);
        CHECK_FUNCTION_ABORTS      (utils::bits::shift_signed<int16_t>, 0, 17);
        CHECK_FUNCTION_ABORTS_FALSE(utils::bits::shift_signed<int32_t>, 0, 32);
        CHECK_FUNCTION_ABORTS      (utils::bits::shift_signed<int32_t>, 0, 33);

        CHECK_FUNCTION_ABORTS_FALSE(utils::bits::shift_signed< uint8_t>, 0, 8);
        CHECK_FUNCTION_ABORTS      (utils::bits::shift_signed< uint8_t>, 0, 9);
        CHECK_FUNCTION_ABORTS_FALSE(utils::bits::shift_signed<uint32_t>, 0, 32);
        CHECK_FUNCTION_ABORTS      (utils::bits::shift_signed<uint32_t>, 0, 33);
    #else
        CHECK(utils::bits::shift_signed< int16_t>(0xBEEF, 17) ==  int16_t(0xBEEF));
        CHECK(utils::bits::shift_signed<uint16_t>(0xBEEF, 17) == uint16_t(0xBEEF));
    #endif
}

TEST_CASE("Test utils::bits::extend_sign" ) {
    REQUIRE(utils::bits::extend_sign<int8_t, 1>(0) == 0);
    REQUIRE(utils::bits::extend_sign<int8_t, 8>(0) == 0);
    REQUIRE(utils::bits::extend_sign<int8_t, 8>((-1) & 0xFF) == -1);
    REQUIRE(utils::bits::extend_sign<int8_t, 8>((-1) & 0xFF) == -1);

    CHECK(utils::bits::extend_sign<int16_t,  4>(0b1000) == -8);
    CHECK(utils::bits::extend_sign<int16_t,  5>(0b1000) ==  8);
    CHECK(utils::bits::extend_sign<int16_t, 16>(0xFFFF) == -1);
    CHECK(utils::bits::extend_sign<int16_t, 16>(0x7FFF) == 32767);
    CHECK(utils::bits::extend_sign<int16_t, 15>(0x7FFF) == -1);
    CHECK(utils::bits::extend_sign<int16_t, 16>(0xDEAD) == -8531);
}

TEST_CASE("Test utils::bits::bits_needed" ) {
    REQUIRE(utils::bits::bits_needed<uint8_t >(0) == 1);
    REQUIRE(utils::bits::bits_needed<uint8_t >(1) == 1);
    REQUIRE(utils::bits::bits_needed<uint8_t >(0xFF) == 8);
    REQUIRE(utils::bits::bits_needed<uint16_t>( 8) == 4);
    REQUIRE(utils::bits::bits_needed< int16_t>( 8) == 5);
    REQUIRE(utils::bits::bits_needed<uint16_t>(uint16_t(-8)) == 16);
    REQUIRE(utils::bits::bits_needed< int16_t>(-8) == 4);

    for (uint64_t i = 65, mask = 1ull << 63; --i; mask >>= 1) {
        CHECK(utils::bits::bits_needed<uint64_t>(mask) == i);
        CHECK(utils::bits::bits_needed<int64_t>(int64_t(mask)) == (i < 63 ? i + 1 : 64));
    }
}

TEST_CASE("Test utils::bits::to_binary" ) {
    const std::vector<bool> empty  {};
    const std::vector<bool> test_f { false };
    const std::vector<bool> test_t { true  };
    const std::vector<bool> test_8 { true, false, true, false, true, false, true, false };
    const std::vector<bool> test_9 { true, false, true, false, true, false, true, false, true };
    const std::vector<int>  test_i8{ 1, 0, 255, 0, 3, 0, 5, 0};

    CHECK(utils::bits::to_binary<uint8_t> (empty ) == 0x00);
    CHECK(utils::bits::to_binary<uint8_t> (test_f) == 0x00);
    CHECK(utils::bits::to_binary<uint8_t> (test_t) == 0x01);
    CHECK(utils::bits::to_binary<uint8_t> (test_8) == 0xAA);
    CHECK_FUNCTION_ABORTS(utils::bits::to_binary<uint8_t>, test_9);
    CHECK(utils::bits::to_binary<uint16_t>(test_9) == 0x155);
    CHECK(utils::bits::to_binary<int>(test_i8) == 0xAA);
}

TEST_CASE("Test utils::bits::to_string" ) {
    constexpr uint32_t test0 = 0x00000000;
    constexpr uint32_t test1 = 0x00000001;
    constexpr uint32_t test2 = 0x80000000;
    constexpr uint32_t test3 = 0xAAAAAAAA;
    constexpr uint32_t test4 = 0xFFFFFFFF;
    constexpr uint32_t test5 = 0x005500AA;

    CHECK(utils::bits::to_string(test0) == "00000000000000000000000000000000");
    CHECK(utils::bits::to_string(test1) == "00000000000000000000000000000001");
    CHECK(utils::bits::to_string(test2) == "10000000000000000000000000000000");
    CHECK(utils::bits::to_string(test3) == "10101010101010101010101010101010");
    CHECK(utils::bits::to_string(test4) == "11111111111111111111111111111111");
    CHECK(utils::bits::to_string(test5) == "00000000010101010000000010101010");
}

#endif
