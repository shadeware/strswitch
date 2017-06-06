#ifndef STRSWITCH_HPP_14695981039346656037
#define STRSWITCH_HPP_14695981039346656037

#ifndef STRSWITCH_CUSTOM_REGEX_MATCHER
#include <regex>
#endif

#ifndef STRSWITCH_DEFAULT_SEED
#define STRSWITCH_DEFAULT_SEED 14695981039346656037ULL
#endif

#ifndef STRSWITCH_CPP14_CONSTEXPR
#define STRSWITCH_CPP14_CONSTEXPR // disabled by default
#endif

namespace strswitch
{

// deduce size_t so we can depend on 0 headers
using sizet = decltype(sizeof(0));

// ----------------------------------------------------------------------------------------------

template <class Char>
STRSWITCH_CPP14_CONSTEXPR inline bool strings_equal(const Char* str1, const Char* str2, sizet length) noexcept
{
    for (sizet i = 0; i < length; ++i)
        if (str1[i] != str2[i])
            return false;
    return true;
}

// ----------------------------------------------------------------------------------------------

#ifndef STRSWITCH_CUSTOM_HASH
using hash_type = unsigned long long;
namespace detail
{
template <class Char>
static inline constexpr hash_type hash_impl(hash_type seed, const Char* str, sizet len, sizet i) noexcept
{
    return i == 0 ? seed : (hash_impl(seed, str, len, i - 1) ^ static_cast<hash_type>(str[i - 1])) * 1099511628211ULL;
}
} // namespace detail

template <class Char>
static inline constexpr hash_type hash(const Char* str, sizet length, hash_type seed) noexcept
{
    return detail::hash_impl(seed, str, length, length);
}

template <class Char, sizet N>
static inline constexpr hash_type hash(const Char (&str)[N], hash_type seed) noexcept
{
    return hash(str, N - 1, seed);
}
#endif

// ----------------------------------------------------------------------------------------------

template <class Char, sizet N>
STRSWITCH_CPP14_CONSTEXPR inline bool starts_with(const Char (&prefix_str)[N], const Char* str, sizet length) noexcept
{
    const auto prefix_length = N - 1;

    if (prefix_length > length)
        return false;

    return strswitch::strings_equal(prefix_str, str, prefix_length);
}

template <class Char, sizet N>
STRSWITCH_CPP14_CONSTEXPR inline bool ends_with(const Char (&suffix_str)[N], const Char* str, sizet length) noexcept
{
    const auto suffix_length = N - 1;

    if (suffix_length > length)
        return false;

    return strswitch::strings_equal(str + length - suffix_length, suffix_str, suffix_length);
}

// ----------------------------------------------------------------------------------------------

#ifndef STRSWITCH_CUSTOM_REGEX_MATCHER
template <class Char, sizet N>
static inline bool regex_matches(const Char (&pattern_str)[N], const Char* str, sizet length)
{
    std::regex reg(pattern_str);
    return std::regex_match(str, str + length, reg);
}
#endif

} // namespace strswitch

// ----------------------------------------------------------------------------------------------

#define STRSWITCH_EXPAND_CASES(seq)                                                                                    \
    switch (strswitch::hash(str_, length_, seed_))                                                                     \
    {                                                                                                                  \
        NTERMINATE_EXPAND(STRSWITCH_EXPAND_CASES1 seq)                                                                 \
    }

#define STRSWITCH_EXPAND_CASES1(str, ...)                                                                              \
    case strswitch::hash(str, seed_):                                                                                  \
        if (sizeof(str) / sizeof(*str_) - 1 == length_ && strswitch::strings_equal(str, str_, length_))                \
        {                                                                                                              \
            __VA_ARGS__;                                                                                               \
            continue;                                                                                                  \
        }                                                                                                              \
        break;                                                                                                         \
        STRSWITCH_EXPAND_CASES2

#define STRSWITCH_EXPAND_CASES2(str, ...)                                                                              \
    case strswitch::hash(str, seed_):                                                                                  \
        if (sizeof(str) / sizeof(*str_) - 1 == length_ && strswitch::strings_equal(str, str_, length_))                \
        {                                                                                                              \
            __VA_ARGS__;                                                                                               \
            continue;                                                                                                  \
        }                                                                                                              \
        break;                                                                                                         \
        STRSWITCH_EXPAND_CASES1

#define STRSWITCH_EXPAND_CASES10
#define STRSWITCH_EXPAND_CASES20

// ----------------------------------------------------------------------------------------------

#define STRSWITCH_EXPAND_STARTS_WITH(seq) NTERMINATE_EXPAND(STRSWITCH_EXPAND_STARTS_WITH1 seq)
#define STRSWITCH_EXPAND_STARTS_WITH1(str, ...)                                                                        \
    if (strswitch::starts_with(str, str_, length_))                                                                    \
    {                                                                                                                  \
        __VA_ARGS__;                                                                                                   \
        continue;                                                                                                      \
    }                                                                                                                  \
    STRSWITCH_EXPAND_STARTS_WITH2
#define STRSWITCH_EXPAND_STARTS_WITH2(str, ...)                                                                        \
    if (strswitch::starts_with(str, str_, length_))                                                                    \
    {                                                                                                                  \
        __VA_ARGS__;                                                                                                   \
        continue;                                                                                                      \
    }                                                                                                                  \
    STRSWITCH_EXPAND_STARTS_WITH1
#define STRSWITCH_EXPAND_STARTS_WITH10
#define STRSWITCH_EXPAND_STARTS_WITH20

// ----------------------------------------------------------------------------------------------

#define STRSWITCH_EXPAND_ENDS_WITH(seq) NTERMINATE_EXPAND(STRSWITCH_EXPAND_ENDS_WITH1 seq)
#define STRSWITCH_EXPAND_ENDS_WITH1(str, ...)                                                                          \
    if (strswitch::ends_with(str, str_, length_))                                                                      \
    {                                                                                                                  \
        __VA_ARGS__;                                                                                                   \
        continue;                                                                                                      \
    }                                                                                                                  \
    STRSWITCH_EXPAND_ENDS_WITH2
#define STRSWITCH_EXPAND_ENDS_WITH2(str, ...)                                                                          \
    if (strswitch::ends_with(str, str_, length_))                                                                      \
    {                                                                                                                  \
        __VA_ARGS__;                                                                                                   \
        continue;                                                                                                      \
    }                                                                                                                  \
    STRSWITCH_EXPAND_ENDS_WITH1
#define STRSWITCH_EXPAND_ENDS_WITH10
#define STRSWITCH_EXPAND_ENDS_WITH20

// ----------------------------------------------------------------------------------------------

#define STRSWITCH_EXPAND_MATCHES(seq) NTERMINATE_EXPAND(STRSWITCH_EXPAND_MATCHES1 seq)
#define STRSWITCH_EXPAND_MATCHES1(pattern, ...)                                                                        \
    if (strswitch::regex_matches(pattern, str_, length_))                                                              \
    {                                                                                                                  \
        __VA_ARGS__;                                                                                                   \
        continue;                                                                                                      \
    }                                                                                                                  \
    STRSWITCH_EXPAND_MATCHES2
#define STRSWITCH_EXPAND_MATCHES2(pattern, ...)                                                                        \
    if (strswitch::regex_matches(pattern, str_, length_))                                                              \
    {                                                                                                                  \
        __VA_ARGS__;                                                                                                   \
        continue;                                                                                                      \
    }                                                                                                                  \
    STRSWITCH_EXPAND_MATCHES1
#define STRSWITCH_EXPAND_MATCHES10
#define STRSWITCH_EXPAND_MATCHES20

// ----------------------------------------------------------------------------------------------

#define STRSWITCH_EXPAND_DEFAULT(...) __VA_ARGS__;
// ----------------------------------------------------------------------------------------------

#define STRSWITCH_EXPAND(seq) TERMINATE_EXPAND(STRSWITCH_EXPAND1 seq)
#define STRSWITCH_EXPAND1(func, ...) STRSWITCH_EXPAND_##func(__VA_ARGS__) STRSWITCH_EXPAND2
#define STRSWITCH_EXPAND2(func, ...) STRSWITCH_EXPAND_##func(__VA_ARGS__) STRSWITCH_EXPAND1
#define STRSWITCH_EXPAND10
#define STRSWITCH_EXPAND20

// ----------------------------------------------------------------------------------------------

#define TERMINATE_EXPAND_IMPL2(...) __VA_ARGS__##0
#define TERMINATE_EXPAND_IMPL1(...) TERMINATE_EXPAND_IMPL2(__VA_ARGS__)
#define TERMINATE_EXPAND(...) TERMINATE_EXPAND_IMPL1(__VA_ARGS__)

#define NTERMINATE_EXPAND_IMPL2(...) __VA_ARGS__##0
#define NTERMINATE_EXPAND_IMPL1(...) NTERMINATE_EXPAND_IMPL2(__VA_ARGS__)
#define NTERMINATE_EXPAND(...) NTERMINATE_EXPAND_IMPL1(__VA_ARGS__)

// ----------------------------------------------------------------------------------------------

#define STRSWITCH(str, length, seed, seq)                                                                              \
    do                                                                                                                 \
    {                                                                                                                  \
        const auto* str_ = str;                                                                                        \
        auto length_ = length;                                                                                         \
        constexpr auto seed_ = seed;                                                                                   \
        (void)str_;                                                                                                    \
        (void)length_;                                                                                                 \
        (void)seed_;                                                                                                   \
        STRSWITCH_EXPAND(seq)                                                                                          \
    } while (false);

// ----------------------------------------------------------------------------------------------

#endif // STRSWITCH_HPP_14695981039346656037
