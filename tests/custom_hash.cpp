#define STRSWITCH_CUSTOM_HASH
#include "strswitch.hpp"
#include "gtest/gtest.h"

namespace strswitch
{

using hash_type = unsigned long long;
namespace detail
{
template <class Char>
static inline constexpr hash_type hash_impl(hash_type seed, const Char* str, sizet len, sizet i) noexcept
{
    return i == 0 ? seed : (hash_impl(seed, str, len, i - 1) ^ static_cast<hash_type>(str[i - 1])) * 13ULL;
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

}

static std::string tutorial_with_custom_hash(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), tolower);

    // clang-format off
    STRSWITCH(str.data(), str.length(), STRSWITCH_DEFAULT_SEED,
              (CASES,
               ("hi", return "hi!")
               ("how are you?", return "fine";)
               ("good bye", { return "bye!"; } )
               ("does a set of all sets contain itself?", throw std::logic_error("...")))
              (STARTS_WITH, ("can i", return "you can"))
              (ENDS_WITH, ("!", return "don't shout at me!" ))
              (MATCHES, ("\\d+", return "I like this number!" ))
              (DEFAULT, return "sorry?")
              )
    // clang-format on

    throw std::logic_error("unreachable"); // there is return in default section
}

TEST(strswitch, tutorial_with_custom_hash)
{
    EXPECT_EQ(tutorial_with_custom_hash("hi"), "hi!");
    EXPECT_EQ(tutorial_with_custom_hash("how are you?"), "fine");
    EXPECT_EQ(tutorial_with_custom_hash("good bye"), "bye!");
    EXPECT_THROW(tutorial_with_custom_hash("does a set of all sets contain itself?"), std::logic_error);
    EXPECT_EQ(tutorial_with_custom_hash("can i go already?"), "you can");
    EXPECT_EQ(tutorial_with_custom_hash("die!"), "don't shout at me!");
    EXPECT_EQ(tutorial_with_custom_hash("123"), "I like this number!");
    EXPECT_EQ(tutorial_with_custom_hash("how are you doing this?"), "sorry?");
}
