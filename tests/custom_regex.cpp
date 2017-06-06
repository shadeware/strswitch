#define STRSWITCH_CUSTOM_REGEX_MATCHER
#include "strswitch.hpp"
#include "gtest/gtest.h"

#include <stdexcept>

namespace strswitch
{
template <class Char, sizet N>
static inline bool regex_matches(const Char (&/*pattern_str*/)[N], const Char* /*str*/, sizet /*length*/)
{
    throw std::runtime_error("not implemented");
}
}

static std::string tutorial_with_custom_regex(std::string str)
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

TEST(strswitch, tutorial_with_custom_regex)
{
    EXPECT_EQ(tutorial_with_custom_regex("hi"), "hi!");
    EXPECT_EQ(tutorial_with_custom_regex("how are you?"), "fine");
    EXPECT_EQ(tutorial_with_custom_regex("good bye"), "bye!");
    EXPECT_THROW(tutorial_with_custom_regex("does a set of all sets contain itself?"), std::logic_error);
    EXPECT_EQ(tutorial_with_custom_regex("can i go already?"), "you can");
    EXPECT_EQ(tutorial_with_custom_regex("die!"), "don't shout at me!");
    EXPECT_THROW(tutorial_with_custom_regex("123"), std::runtime_error);
    EXPECT_THROW(tutorial_with_custom_regex("how are you doing this?"), std::runtime_error);
}
