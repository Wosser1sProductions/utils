#ifndef UTILS_BITS_HPP
#define UTILS_BITS_HPP

#include "utils_compiler.hpp"
#include "utils_test.hpp"
#include "utils_traits.hpp"
#include "utils_algorithm.hpp"

#include <cstdint>
#include <limits>
#include <numeric>

/*
 *  Refer to: https://gcc.gnu.org/onlinedocs/gcc/Other-Builtins.html
 */
#ifdef UTILS_COMPILER_MSVC
    #include <intrin.h>
    #define UTILS_BITS_CLZ_ULL  utils::bits::internal::_clz_template
    #define UTILS_BITS_FFS_LL   utils::bits::internal::_ffs_template
    #define UTILS_BITS_CNT_LL   utils::bits::internal::_cnt_template
#else
    #define UTILS_BITS_CLZ_ULL  __builtin_clzll
    #define UTILS_BITS_FFS_LL   __builtin_ffsll
    #define UTILS_BITS_CNT_LL   __builtin_popcountll
#endif

#define UTILS_BITS_ASSERT_SHIFT_SIGNED_SIZE 0


namespace utils::bits {
    namespace internal {
        #ifdef UTILS_COMPILER_MSVC
            template <typename T> ATTR_MAYBE_UNUSED ATTR_NODISCARD HEDLEY_PRIVATE
            static inline constexpr uint_fast32_t _clz_template(T x) {
                if constexpr (sizeof(T) * 8 == 32) {
                    #if defined(__lzcnt32)
                        return static_cast<uint_fast32_t>(__lzcnt32(x));
                    #elif defined(_BitScanReverse)
                        unsigned long ret;
                        _BitScanReverse(&ret, x);
                        return (int)(31 ^ ret);
                    #endif
                } else if constexpr (sizeof(T) * 8 == 64) {
                    #if defined(__lzcnt64)
                        return static_cast<uint_fast32_t>(__lzcnt64(x));
                    #elif defined(_BitScanReverse64)
                        unsigned long ret;
                        _BitScanReverse64(&ret, x);
                        return (int)(63 ^ ret);
                    #endif
                } else {
                    if (HEDLEY_UNLIKELY(x == 0)) return 0u;
                    uint_fast32_t r = 0;
                    while (x)
                        x >>= 1, ++r;
                    return (sizeof(T) * 8) - r;
                }
            }

            template <typename T> ATTR_MAYBE_UNUSED ATTR_NODISCARD HEDLEY_PRIVATE
            static inline constexpr uint_fast32_t _ffs_template(T x) {
                if (HEDLEY_UNLIKELY(x == 0)) return 0u;
                uint_fast32_t r = 1;
                while ((x & 1) == 0)
                    x >>= 1, ++r;
                return r;
            }

            template <typename T> ATTR_MAYBE_UNUSED ATTR_NODISCARD HEDLEY_PRIVATE
            static inline constexpr uint_fast32_t _cnt_template(T x) {
                if (HEDLEY_UNLIKELY(x == 0)) return 0u;
                uint_fast32_t r = 0;
                while (x)
                    r += (x & 1) == 1, x >>= 1;
                return r;
            }
        #endif
    }

    /**
     * \brief   Return the size in bits of the given type.
     *          Uses sizeof(T) * CHAR_BITS or
     *          std::numeric_limits<uint8_t>::digits
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    inline constexpr size_t size_of(void) noexcept {
        return sizeof(T) * std::numeric_limits<uint8_t>::digits;
    }

    /**
     *  \brief  Create a mask matching the bit at the 1-based \p pos.
     *
     *  \param  pos
     *      The position (1-based) to mask.
     *  \return Returns a \p T with the pos' bit active.
     */
    template<class T = uint64_t> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr T mask_one(uint_fast32_t pos) {
        return !pos || pos > utils::bits::size_of<T>()
             ? 0 : (T(1ull) << (pos - 1));
    }

    /**
     *  \brief  Create a mask matching \p n MSBs.
     *
     *  \param  n
     *      The amount of MSBs to mask.
     *  \return Returns a mask with \p n bits active on the left.
     */
    template<class T = uint64_t> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr T mask_msb(uint_fast32_t n) {
        using uT = typename std::make_unsigned<T>::type;
        return n >= utils::bits::size_of<T>()
             ? ~uT(0ull) : !n ? 0 : (~uT(0ull) << (utils::bits::size_of<T>() - n));
    }

    /**
     *  \brief  Create a mask matching \p n LSBs.
     *
     *  \param  n
     *      The amount of LSBs to mask.
     *  \return Returns a mask with \p n bits active on the right.
     */
    template<class T = uint64_t> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr T mask_lsb(uint_fast32_t n) {
        using uT = typename std::make_unsigned<T>::type;
        return n >= utils::bits::size_of<T>()
             ? ~uT(0ull) : ~(~uT(0ull) << n);
    }

    /**
     *  \brief  Select a single bit at \p pos.
     *
     *  \param  value
     *      The value to select in.
     *  \param  pos
     *      The position to select at (1-based).
     *  \return Returns 1 or 0 depending if the bit was active or not.
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr T select_one(const T value, uint_fast32_t pos) {
        return !!(value & utils::bits::mask_one<T>(pos));
    }

    /**
     *  \brief  Select \p n MSBs and return them as LSB value.
     *
     *  \param  value
     *      The value to select in.
     *  \param  n
     *      The amount of MSBs to select.
     *  \return Returs the \p n bit value from the left, as \p n LSBs.
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr T select_msb(const T value, uint_fast32_t n) {
        return (value >> (utils::bits::size_of<T>() - n))
             & utils::bits::mask_lsb<T>(n);
    }

    /**
     *  \brief  Select \p n LSBs and return them as LSB value.
     *
     *  \param  value
     *      The value to select in.
     *  \param  n
     *      The amount of LSBs to select.
     *  \return Returs the \p n bit value from the right, as \p n LSBs.
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr T select_lsb(const T value, uint_fast32_t n) {
        return value & utils::bits::mask_lsb<T>(n);
    }

    /**
     *  \brief  Set the bit at \p pos to the value of \p set.
     *
     *  \param  value
     *      The value to change a bit in.
     *  \param  set
     *      The value to set the bit to.
     *  \param  pos
     *      The position to set the bit at.
     *  \return Returns \p value with the bit at \p pos set to \p set.
     */
    template<class T> ATTR_MAYBE_UNUSED
    static inline constexpr T& set_one(T& value, const T set, uint_fast32_t pos) {
        value &= ~utils::bits::mask_one<T>(pos);
        value |= (set & 1) << (pos - 1);
        return value;
    }

    /**
     *  \brief  Set the \p n MSBs to the value of \p set.
     *
     *  \param  value
     *      The value to change MSBs in.
     *  \param  set
     *      The value to set the bits to.
     *  \param  n
     *      The amount of bits to set.
     *  \return Returns \p value with \p n MSBs set to \p set.
     */
    template<class T> ATTR_MAYBE_UNUSED
    static inline constexpr T& set_msb(T& value, const T set, uint_fast32_t n) {
        value &= ~utils::bits::mask_msb<T>(n);
        value |= utils::bits::select_lsb<T>(set, n) << (utils::bits::size_of<T>() - n);
        return value;
    }

    /**
     *  \brief  Set the \p n LSBs to the value of \p set.
     *
     *  \param  value
     *      The value to change LSBs in.
     *  \param  set
     *      The value to set the bits to.
     *  \param  n
     *      The amount of bits to set.
     *  \return Returns \p value with \p n LSBs set to \p set.
     */
    template<class T> ATTR_MAYBE_UNUSED
    static inline constexpr T& set_lsb(T& value, const T set, uint_fast32_t n) {
        value &= ~utils::bits::mask_lsb<T>(n);
        value |= utils::bits::select_lsb<T>(set, n);
        return value;
    }

    /**
     *  \brief  Generic bitwise AND all args.
     *
     *  \return Return arg1 & ... & argn
     */
    template<
        typename... T,
        typename C = std::common_type_t<T...>
    > ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr C and_all(T const&... args) {
        static_assert(std::is_integral_v<C>, "utils::bits::and_all: Integrals required.");
        return (... & args);
    }

    /**
     *  \brief  Generic bitwise OR all args.
     *
     *  \return Return arg1 | ... | argn
     */
    template<
        typename... T,
        typename C = std::common_type_t<T...>
    > ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr C or_all(T const&... args) {
        static_assert(std::is_integral_v<C>, "utils::bits::or_all: Integrals required.");
        return (... | args);
    }

    /**
     *  \brief  Generic bitwise XOR all args.
     *
     *  \return Return arg1 ^ ... ^ argn
     */
    template<
        typename... T,
        typename C = std::common_type_t<T...>
    > ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr C xor_all(T const&... args) {
        static_assert(std::is_integral_v<C>, "utils::bits::xor_all: Integrals required.");
        return (... ^ args);
    }


    /**
     * \brief  Find First Set
     *         This function identifies the least significant index or position of the
     *         bits set to one in the word.
     *
     * \param  value
     *      Value to find least significant index
     * \retval bitIndex
     *      Index of least significat bit at one
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr uint_fast32_t ffs(const T value) {
        static_assert(std::is_integral_v<T>, "utils::bits::ffs: Integral required.");
        using uT = typename std::make_unsigned<T>::type;
        return UTILS_BITS_FFS_LL(uT(value));
    }

    /**
     * \brief  Find MSB Set (first from left)
     *         This function identifies the position of the MSB starting from LSB.
     *
     * \param  value
     *      Value to find msb index
     * \retval bitIndex
     *      Index of msb
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr uint_fast32_t msb(const T value) {
        static_assert(std::is_integral_v<T>, "utils::bits::msb: Integral required.");
        using uT = typename std::make_unsigned<T>::type;
        #ifdef UTILS_COMPILER_MSVC
            return uint_fast32_t(utils::bits::size_of<uT>() - UTILS_BITS_CLZ_ULL(uT(value) | 1)) - (value == 0);
        #else
            return uint_fast32_t(64 - UTILS_BITS_CLZ_ULL(uT(value) | 1)) - (value == 0);
        #endif
    }

    /**
     * \brief  Popcount
     *         This function counts the amount of set bits.
     *
     * \param  value
     *      Value to count the set bits in.
     * \retval bitIndex
     *      The amount of bits that are `1`.
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr uint_fast32_t popcount(const T value) {
        static_assert(std::is_integral_v<T>, "utils::bits::popcount: Integral required.");
        using uT = typename std::make_unsigned<T>::type;
        return UTILS_BITS_CNT_LL(uT(value));
    }

    /**
     *  \brief  Bitwise rotate the given \p value to the left by \p n bits.
     *
     *  \param  value
     *      The value to rotate.
     *  \param  n
     *      The amount of bit places to rotate left with.
     *  \return The result will be of type T but with \p bit_length bits used,
     *          other bits exceeding the length will become zeroes.
     *          The value will be shifted to the left, and any bits exceeding
     *          \p bit_length will be appended to the right.
     */
    template <
        class T,
        size_t bit_length = utils::bits::size_of<T>()
    > ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr T rotl(const T value, const uint_fast32_t n = 1) {
        static_assert(std::is_integral_v<T>, "utils::bits::rotl: Integral required.");

        constexpr T mask_size = utils::bits::mask_lsb(bit_length);
        const     T mask_data = utils::bits::mask_lsb(n);
        return ((value << n) & mask_size) | ((value >> (bit_length - n)) & mask_data);
    }

    /**
     *  \brief  Bitwise rotate the given \p value to the right by \p n bits.
     *
     *  \param  value
     *      The value to rotate.
     *  \param  n
     *      The amount of bit places to rotate right with.
     *  \return The result will be of type T but with \p bit_length bits used,
     *          other bits exceeding the length will become zeroes.
     *          The value will be shifted to the right, and any bits shifted out
     *          will be appended to the left starting at \p bit_length.
     */
    template <
        class T,
        size_t bit_length = utils::bits::size_of<T>()
    > ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr T rotr(const T value, const uint_fast32_t n = 1) {
        static_assert(std::is_integral_v<T>, "utils::bits::rotr: Integral required.");

        const     T mask_right = utils::bits::mask_lsb(bit_length - n);
        const     T mask_data  = utils::bits::mask_lsb(n);
        return ((value >> n) & mask_right) | ((value & mask_data) << (bit_length - n));
    }

    /**
     *  \brief  Determine if given value is a power of 2.
     *
     *  \param  value
     *      The value to check.
     *  \return Returns true if value is the result of (1 << x).
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr bool is_power_of_2(const T value) {
        static_assert(std::is_integral_v<T>, "utils::bits::is_power_of_2: Integral required.");
        return value && !(value & (value - 1));
    }

    /**
     *  \brief  Check if given value is an odd number.
     *
     *  \param  value
     *      The value to check.
     *  \return Returns true if the value is odd.
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr bool is_odd(const T value) {
        static_assert(std::is_integral_v<T>, "utils::bits::is_odd: Integral required.");
        return (value & T(1)) != T(0);
    }

    /**
     *  \brief  Check if given value is an even number.
     *
     *  \param  value
     *      The value to check.
     *  \return Returns true if the value is even (divisable by 2).
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr bool is_even(const T value) {
        static_assert(std::is_integral_v<T>, "utils::bits::is_even: Integral required.");
        return !utils::bits::is_odd(value);
    }

    /**
     *  \brief  Round the given value to the next multiple of \p multiple,
     *          greater or equal to \p value itself.
     *
     *  \param  value
     *      The number to round.
     *  \param  multiple
     *      The multiple to round to.
     *  \return Returns the nearest multiple greater than \p value.
     */
    ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr int64_t round_to_multiple(const int64_t value, const int64_t multiple) noexcept(false) {
        ASSERT(multiple);
        const int64_t isPositive = int64_t(value >= 0ll);
        return ((value + isPositive * (multiple - 1ll)) / multiple) * multiple;
    }

    /**
     *  @brief  Round the given bits to the next byte.
     *
     *  @param  bits
     *          The amount of bits to round to a byte.
     *  @return Returns the next byte if bits has 1-7 surplus bits
     *          or the current byte if no surplus bits.
     */
    ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr uint64_t round_to_byte(const uint64_t bits) {
        return (bits + 7ull) / 8ull;
    }

    /**
     *  @brief  Create a signed value of type T
     *          with the given value that is only src_bits long.
     *
     *          This abuses the fact that right-shifting a signed number
     *          with an MSB of 1, will fill in the new MSBs as 1, making the
     *          original value signed in the new (bigger) size.
     *  @tparam T
     *          Target type (signed).
     *  @param  value
     *          The value to make signed in the available bits of T.
     *  @param  src_bits
     *          The amount of bits that are used in value.
     *  @return Returns a signed or unsigned number,
     *          depending on whether value[src_bits : 0] was considered signed.
     */
    template<
        class T,
        size_t Tlen = utils::bits::size_of<T>()
    > ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline T shift_signed(const uint64_t value, const size_t src_bits) {
        #if UTILS_BITS_ASSERT_SHIFT_SIGNED_SIZE
            // ASSERT that src_bits is not bigger than sizeof(T)
            ASSERT(src_bits <= Tlen);
            const size_t bit_length = Tlen - src_bits;
            return T(value << bit_length) >> bit_length;
        #else // If src_bits is bigger than sizeof(T), don't shift
            const uint64_t bit_length = src_bits <= Tlen ? Tlen - src_bits : 0;
            return T(value << bit_length) >> bit_length;
        #endif
    }

    /**
     *  @brief  Same as shift_signed, but with known bit size
     *          of value at compile time.
     *  @tparam T
     *          Target type (signed).
     *  @tparam bits
     *          Amount of bits in value.
     *  @param  value
     *          The value to make signed in the available bits of T.
     *  @return Returns a signed or unsigned number,
     *          depending on whether value[src_bits : 0] was considered signed.
     */
    template<class T, uint_fast8_t bits> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline T extend_sign(const uint64_t value) {
        static_assert(bits > 0, "utils::bits::extend_sign: Amount of bits greater than 0 required.");
        struct { T value:bits; } s;
        return s.value = value;
    }

    /**
     *  \brief  Determine the amount of bits needed to represent the given T.
     *
     *  \param  value
     *      The T value to check.
     *  \return
     *      Returns the amount of bits required to represent the value if reshifted to size_of<T> bits.
     */
    template<class T> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline uint_fast32_t bits_needed(T value) {
        static_assert(std::is_integral_v<T>, "utils::bits::bits_needed: Integral required.");

        uint_fast32_t bits = 0;

        // 1. Mask value with amount of current bits : (value & ((1 << bits) - 1))
        // 2. Shift left to size of 16 bits          : << (16 - bits)
        // 3. Cast to int16_t
        // 4. Shift back to original size
        // 5. Check if value represented with <bits> bits and re-shifted to 16 bits
        //    equals the starting value.
        // 6. Repeat until a minimal amount of bits has been found to represent the int16_t.
        //    (between 1 and 16)

        while (utils::bits::shift_signed<T>(value, ++bits) != value);

        return bits;
    }

    /**
     *  \brief  Accumulate the boolean vector to the target type \p T.
     *          The MSB will be the first value in the vector.
     *
     *          Optional ASSERT to limit size of vector to size of target type.
     *          If `bool_vec.size()` is bigger, the MSBs will be shifted out.
     *
     *  \tparam T
     *      The target type.
     *  \param  bool_vec
     *      The vector to put its contents into a \p T.
     *  \return Returns a `T` where the MSB is the first item in \p bool_vec,
     *          and the LSB is the last one.
     */
    template<
        typename T, typename U = bool,
        typename = typename std::enable_if_t<std::is_integral_v<T>>,
        typename = typename std::enable_if_t<std::is_same_v<bool, U>
                                          || std::is_integral_v<U>>
    > ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline T to_binary(const std::vector<U>& bool_vec) {
        ASSERT(bool_vec.size() <= utils::bits::size_of<T>());
        return std::accumulate(bool_vec.begin(), bool_vec.end(), T(0),
                               [](T x, T y) { return (x << 1) | (y & 1); });
    }

    /**
     *  \brief  Create a string representation of the bits in \p value, from MSB to LSB.
     *
     *          TODO In C++20, is_integral_v can be left out and
     *          std::bit_cast<T> can be used as new value.
     *
     *  \param  value
     *      The value to create a string for.
     *  \return Returns a string representation consisting of '0's and '1's.
     */
    template<
        typename T,
        typename = typename std::enable_if_t<std::is_integral_v<T>>
    > ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline std::string to_string(T value) {
        const size_t length = utils::bits::size_of<T>();
        std::string out(length, '0');

        for (auto it = out.rbegin(); value && it != out.rend(); value >>= 1, it++) {
            if (value & 1)
                *it = '1';
        }

        return out;
    }

    /**
     *  \brief  Create a string representation of the bits in \p value, from LSB to MSB.
     *
     *  \param  value
     *      The value to create a string for.
     *  \return Returns a string representation consisting of '0's and '1's.
     */
    template<
        typename T,
        typename = typename std::enable_if_t<std::is_integral_v<T>>
    > ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline std::string to_string_lsb(T value) {
        std::string out{utils::bits::to_string(value)};
        utils::algorithm::reverse(out);
        return out;
    }
}

#undef UTILS_BITS_CLZ_ULL
#undef UTILS_BITS_FFS_LL
#undef UTILS_BITS_CNT_LL
#endif // UTILS_BITS_HPP
