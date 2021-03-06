#ifndef UTILS_TRAITS_HPP
#define UTILS_TRAITS_HPP

#include "utils_compiler.hpp"

#include <type_traits>
#include <iterator>

#include <vector>
#include <list>
#include <set>
#include <unordered_set>
#include <map>
#include <unordered_map>
#include <array>
#include <variant>
#include <tuple>
#include <optional>


namespace utils::traits {
    /**
     *  Found type, if has value, contains found index.
     */
    using found_t = std::optional<size_t>;


    ////////////////////////////////////////////////////////////////////////////
    /// True if type T is plain byte
    ////////////////////////////////////////////////////////////////////////////
    template<typename T>
    struct is_byte {
        static constexpr bool value =
               std::is_same<T,   signed char>::value
            || std::is_same<T, unsigned char>::value;
    };

    template<typename T>
    inline constexpr bool is_byte_v = is_byte<T>::value;

    ////////////////////////////////////////////////////////////////////////////
    /// True if type T is iterator
    ////////////////////////////////////////////////////////////////////////////   
    template<typename T>
    struct is_iterator {
        private:
            static char test(...);

            template <typename U,
                      typename = typename std::iterator_traits<U>::difference_type,
                      typename = typename std::iterator_traits<U>::pointer,
                      typename = typename std::iterator_traits<U>::reference,
                      typename = typename std::iterator_traits<U>::value_type,
                      typename = typename std::iterator_traits<U>::iterator_category
                      > static long test(U&&);
        public:
            static constexpr bool value = std::is_same <
                                          decltype(test(std::declval<T>())), long >::value;
    };

    template<typename T>
    inline constexpr bool is_iterator_v = is_iterator<T>::value;

    /// True if variable T is a container with iterator support.
    /// i.e. has std::begin and std::end
    template<typename T, typename U = void>
    struct is_iterable : public std::false_type { };

    template<typename T>
    struct is_iterable<T, std::void_t<utils::traits::is_iterator<decltype(std::begin(std::declval<T&>()))>,
                                      utils::traits::is_iterator<decltype(std::end(std::declval<T&>()))>
    >> : public std::true_type { };

    template<typename T>
    inline constexpr bool is_iterable_v = is_iterable<T>::value;

    ////////////////////////////////////////////////////////////////////////////
    /// True if F can be invoked with an argument type T, T& or T&&
    ////////////////////////////////////////////////////////////////////////////

    template<typename F, typename... Args>
    inline constexpr bool is_invocable_v = std::is_invocable_v<F, Args...>
                                        || std::is_invocable_v<F, Args&...>
                                        || std::is_invocable_v<F, Args&&...>;

    ////////////////////////////////////////////////////////////////////////////
    /// True if type T is a member of the variant VARIANT_T
    ////////////////////////////////////////////////////////////////////////////
    template<typename T, typename VARIANT_T>
    struct is_variant_member;

    template<typename T, typename... ALL_T>
    struct is_variant_member<T, std::variant<ALL_T...>>
        : public std::disjunction<std::is_same<T, ALL_T>...> {};

    template<typename T, typename... ALL_T>
    inline constexpr bool is_variant_member_v = is_variant_member<T, ALL_T...>::value;

    ////////////////////////////////////////////////////////////////////////////
    /// True if type T is a std:: container type
    ////////////////////////////////////////////////////////////////////////////
    // Basic is_container template; specialize to derive from std::true_type for all desired container types
    template<typename T>
    struct is_container : public std::false_type { };

    // Mark vector as a container
    template<typename T, typename TAllocator>
    struct is_container<std::vector<T, TAllocator>> : public std::true_type { };

    // Mark list as a container
    template<typename T, typename TAllocator>
    struct is_container<std::list<T, TAllocator>> : public std::true_type { };

    // Mark initializer_list as a container
    template<typename T>
    struct is_container<std::initializer_list<T>> : public std::true_type { };

    // Mark set as a container
    template<typename T, typename TCompare, typename TAllocator>
    struct is_container<std::set<T, TCompare, TAllocator>> : public std::true_type { };

    // Mark unordered_set as a container
    template<typename T, typename THash, typename TPred, typename TAllocator>
    struct is_container<std::unordered_set<T, THash, TPred, TAllocator>> : public std::true_type { };

    // Mark multiset as a container
    template<typename T, typename TCompare, typename TAllocator>
    struct is_container<std::multiset<T, TCompare, TAllocator>> : public std::true_type { };

    // Mark unordered_multiset as a container
    template<typename T, typename THash, typename TPred, typename TAllocator>
    struct is_container<std::unordered_multiset<T, THash, TPred, TAllocator>> : public std::true_type { };

    // Mark map as a container
    template<typename TKey, typename TValue, typename TCompare, typename TAllocator>
    struct is_container<std::map<TKey, TValue, TCompare, TAllocator>> : public std::true_type { };

    // Mark unordered_map as a container
    template<typename TKey, typename TValue, typename THash, typename TPred, typename TAllocator>
    struct is_container<std::unordered_map<TKey, TValue, THash, TPred, TAllocator>> : public std::true_type { };

    // Mark std::array as a container
    template<typename T, size_t N>
    struct is_container<std::array<T, N>> : public std::true_type { };

    // Mark T[N] as a container
    template<typename T, size_t N>
    struct is_container<T[N]> : public std::true_type { };

    // Basic is maplike template
    template<typename T, typename U = void>
    struct is_maplike : public std::false_type { };

    // Mark type that has key_type, map_type and operator[key_type] as map
    template<typename T>
    struct is_maplike<T, std::void_t<typename T::key_type,
                                     typename T::mapped_type,
                                     decltype(std::declval<T&>()[std::declval<const typename T::key_type&>()])>>
      : public std::true_type { };

    template<typename T>
    inline constexpr bool is_maplike_v = is_maplike<T>::value;

    ////////////////////////////////////////////////////////////////////////////
    /// Check if function can be called on Type
    ////////////////////////////////////////////////////////////////////////////
    namespace internal {
        template<template<class...> class, class, class...>
        struct can_apply : std::false_type{};

        template<template<class...>class Z, class...Ts>
        struct can_apply<Z, std::void_t<Z<Ts...>>, Ts...> : std::true_type{};
    }

    template<template<class...>class Z, class...Ts>
    using can_apply = internal::can_apply<Z, void, Ts...>;

    /**
     *  Creates a `has_<F>_v` function that returns a boolean ::value
     *  to indicate if the given object has the function F.
     *
     *  e.g. CREATE_HAS_FUNCTION(find) creates:
     *      dot_find_r<T, U> that calls T.F()
     *      has_find that checks if dot_find can be applied on T
     *      has_find_v that returns has_find::value
     */
    #define UTILS_TRAITS_CREATE_HAS_FUNCTION(F) \
        template<class T, class U> \
        using dot_ ## F ## _r = decltype(std::declval<T>().F(U())); \
        template<class T, class U> \
        using has_ ## F = can_apply<dot_ ## F ## _r, T, U>; \
        template<typename T, class U> \
        inline constexpr bool has_ ## F ## _v = has_ ## F<T, U>::value;

    UTILS_TRAITS_CREATE_HAS_FUNCTION(find)
    UTILS_TRAITS_CREATE_HAS_FUNCTION(contains)

    ////////////////////////////////////////////////////////////////////////////
    /// If the type T is a reference type, provides the member typedef type
    /// which is the type referred to by T with its topmost cv-qualifiers removed.
    /// Otherwise type is T with its topmost cv-qualifiers removed.
    ////////////////////////////////////////////////////////////////////////////
    #if !UTILS_CPP_LANG_CHECK(UTILS_CPP_VERSION_20)
        template< class T >
        struct remove_cvref {
            typedef std::remove_cv_t<std::remove_reference_t<T>> type;
        };

        template< class T >
        using remove_cvref_t = typename remove_cvref<T>::type;
    #endif

    ////////////////////////////////////////////////////////////////////////////
    /**
     *  \brief   Cast enum type to underlining data type.
     *  \param   e
     *      The enum value to cast.
     */
    template <typename E> ATTR_MAYBE_UNUSED ATTR_NODISCARD
    static inline constexpr auto to_underlying(const E e) noexcept {
        return static_cast<std::underlying_type_t<E>>(e);
    }
}



#endif // UTILS_TRAITS_HPP
