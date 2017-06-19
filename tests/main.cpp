#include "strswitch.hpp"
#include "gtest/gtest.h"

#include <string>

TEST(strings_equal, shallow_test)
{
    EXPECT_FALSE(strswitch::strings_equal("123", "456", 3));
    EXPECT_TRUE(strswitch::strings_equal("123", "123", 3));
}

TEST(hash, shallow_test)
{
    EXPECT_NE(strswitch::hash("1", 1, 0), strswitch::hash("2", 1, 0));
    EXPECT_NE(strswitch::hash("12", 0), strswitch::hash("21", 0));
    EXPECT_NE(strswitch::hash("1", 1, STRSWITCH_DEFAULT_SEED), strswitch::hash("2", 1, STRSWITCH_DEFAULT_SEED));
    EXPECT_NE(strswitch::hash("12", STRSWITCH_DEFAULT_SEED), strswitch::hash("21", STRSWITCH_DEFAULT_SEED));

    EXPECT_EQ(strswitch::hash("", 0, 0), 0u);
    EXPECT_EQ(strswitch::hash("", 0), 0u);
    EXPECT_EQ(strswitch::hash("", 0, STRSWITCH_DEFAULT_SEED), STRSWITCH_DEFAULT_SEED);
    EXPECT_EQ(strswitch::hash("", STRSWITCH_DEFAULT_SEED), STRSWITCH_DEFAULT_SEED);

    EXPECT_EQ(strswitch::hash("1", 1, STRSWITCH_DEFAULT_SEED), strswitch::hash("1", STRSWITCH_DEFAULT_SEED));
    EXPECT_EQ(strswitch::hash("123", 3, STRSWITCH_DEFAULT_SEED), strswitch::hash("123", STRSWITCH_DEFAULT_SEED));
}

TEST(starts_with, shallow_test)
{
    EXPECT_FALSE(strswitch::starts_with("123", "456", 3));
    EXPECT_FALSE(strswitch::starts_with("123", "456", 0));

    EXPECT_FALSE(strswitch::starts_with("123", "1234", 2));
    EXPECT_TRUE(strswitch::starts_with("123", "1234", 3));
    EXPECT_TRUE(strswitch::starts_with("123", "1234", 4));
}

TEST(ends_with, shallow_test)
{
    EXPECT_FALSE(strswitch::ends_with("123", "456", 3));
    EXPECT_FALSE(strswitch::ends_with("123", "456", 0));

    EXPECT_FALSE(strswitch::ends_with("234", "1234", 2));
    EXPECT_TRUE(strswitch::ends_with("234", "234", 3));
    EXPECT_TRUE(strswitch::ends_with("234", "1234", 4));
}

TEST(regex_matches, shallow_test)
{
    EXPECT_TRUE(strswitch::regex_matches("\\d+", "0", 1));
    EXPECT_TRUE(strswitch::regex_matches("\\d+", "456", 3));
    EXPECT_FALSE(strswitch::regex_matches("\\d+", "", 0));
    EXPECT_FALSE(strswitch::regex_matches("\\d+", " ", 1));
    EXPECT_FALSE(strswitch::regex_matches("\\d+", ".", 1));
    EXPECT_FALSE(strswitch::regex_matches("\\d+", "_", 1));
    EXPECT_FALSE(strswitch::regex_matches("\\d+", "+", 1));

    EXPECT_TRUE(strswitch::regex_matches("\\w", "1", 1));
    EXPECT_FALSE(strswitch::regex_matches("\\w", "12", 2));

    EXPECT_FALSE(strswitch::regex_matches("\\w+", "+", 1));
    EXPECT_FALSE(strswitch::regex_matches("\\w+", " ", 1));
    EXPECT_FALSE(strswitch::regex_matches("\\w+", "", 0));
    EXPECT_FALSE(strswitch::regex_matches("\\w+", ".a.", 3));
    EXPECT_FALSE(strswitch::regex_matches("\\w+", "a.a", 3));
    EXPECT_TRUE(strswitch::regex_matches("\\w+", "a", 1));

    EXPECT_TRUE(strswitch::regex_matches("\\w*", "", 0));
    EXPECT_TRUE(strswitch::regex_matches("\\w*", "-", 0));
    EXPECT_TRUE(strswitch::regex_matches("\\w*", "a", 1));
    EXPECT_TRUE(strswitch::regex_matches("\\w*", "abc", 3));

    EXPECT_TRUE(strswitch::regex_matches("\\w*", "1", 1));
    EXPECT_TRUE(strswitch::regex_matches("\\w*", "123", 3));
    EXPECT_FALSE(strswitch::regex_matches("\\w*", "+", 1));
}

int empty_switch(const std::string& str)
{
    STRSWITCH(str.data(), str.length(), STRSWITCH_DEFAULT_SEED, )

    return 0;
}

TEST(strswitch, empty_switch)
{
    EXPECT_EQ(empty_switch(""), 0);
    EXPECT_EQ(empty_switch("123"), 0);
}

int simple_case(const std::string& str)
{
    // clang-format off
    STRSWITCH(str.data(), str.length(), STRSWITCH_DEFAULT_SEED,
              (CASES,
               ("1", return 1))
              )
    // clang-format on

    return 0;
}

TEST(strswitch, simple_case)
{
    EXPECT_EQ(simple_case("1"), 1);
    EXPECT_EQ(simple_case("01"), 0);
    EXPECT_EQ(simple_case("10"), 0);
    EXPECT_EQ(simple_case("0"), 0);
    EXPECT_EQ(simple_case(""), 0);
}

int simple_cases(const std::string& str)
{
    // clang-format off
    STRSWITCH(str.data(), str.length(), STRSWITCH_DEFAULT_SEED,
              (CASES,
               ("1", return 1)
               ("2", return 2)
               ("3", return 4))
              )
    // clang-format on

    return 0;
}

TEST(strswitch, simple_cases)
{
    EXPECT_EQ(simple_cases("1"), 1);
    EXPECT_EQ(simple_cases("01"), 0);
    EXPECT_EQ(simple_cases("10"), 0);
    EXPECT_EQ(simple_cases("0"), 0);
    EXPECT_EQ(simple_cases(""), 0);

    EXPECT_EQ(simple_cases("2"), 2);
    EXPECT_EQ(simple_cases("3"), 4);

    EXPECT_EQ(simple_cases("4"), 0);
}

int with_single_default(const std::string& str)
{
    // clang-format off
  STRSWITCH(str.data(), str.length(), STRSWITCH_DEFAULT_SEED,
            (DEFAULT, return 1)
            )
    // clang-format on

    return 0;
}

TEST(strswitch, with_single_default)
{
    EXPECT_EQ(with_single_default(""), 1);
    EXPECT_EQ(with_single_default("123"), 1);
}

int simple_case_with_default(const std::string& str)
{
    // clang-format off
    STRSWITCH(str.data(), str.length(), STRSWITCH_DEFAULT_SEED,
              (CASES,
               ("1", return 1))
               (DEFAULT, return -1)
              )
    // clang-format on

    return 0;
}

TEST(strswitch, simple_case_with_default)
{
    EXPECT_EQ(simple_case_with_default("1"), 1);
    EXPECT_EQ(simple_case_with_default("01"), -1);
    EXPECT_EQ(simple_case_with_default("10"), -1);
    EXPECT_EQ(simple_case_with_default("0"), -1);
    EXPECT_EQ(simple_case_with_default(""), -1);
}

int simple_cases_with_default(const std::string& str)
{
    // clang-format off
    STRSWITCH(str.data(), str.length(), STRSWITCH_DEFAULT_SEED,
              (CASES,
               ("1", return 1)
               ("2", return 2)
               ("3", return 4))
              (DEFAULT, return -1)
              )
    // clang-format on

    return 0;
}

TEST(strswitch, simple_cases_with_default)
{
    EXPECT_EQ(simple_cases_with_default("1"), 1);
    EXPECT_EQ(simple_cases_with_default("01"), -1);
    EXPECT_EQ(simple_cases_with_default("10"), -1);
    EXPECT_EQ(simple_cases_with_default("0"), -1);
    EXPECT_EQ(simple_cases_with_default(""), -1);

    EXPECT_EQ(simple_cases_with_default("2"), 2);
    EXPECT_EQ(simple_cases_with_default("3"), 4);

    EXPECT_EQ(simple_cases_with_default("4"), -1);
}

int simple_cases_with_default_and_custom_seed(const std::string& str)
{
    // clang-format off
    STRSWITCH(str.data(), str.length(), 0,
              (CASES,
               ("1", return 1)
               ("2", return 2)
               ("3", return 4))
              (DEFAULT, return -1)
              )
    // clang-format on

    return 0;
}

TEST(strswitch, simple_cases_with_default_and_custom_seed)
{
    EXPECT_EQ(simple_cases_with_default_and_custom_seed("1"), 1);
    EXPECT_EQ(simple_cases_with_default_and_custom_seed("01"), -1);
    EXPECT_EQ(simple_cases_with_default_and_custom_seed("10"), -1);
    EXPECT_EQ(simple_cases_with_default_and_custom_seed("0"), -1);
    EXPECT_EQ(simple_cases_with_default_and_custom_seed(""), -1);

    EXPECT_EQ(simple_cases_with_default_and_custom_seed("2"), 2);
    EXPECT_EQ(simple_cases_with_default_and_custom_seed("3"), 4);

    EXPECT_EQ(simple_cases_with_default_and_custom_seed("4"), -1);
}

int case_before_starts_with(const std::string& str)
{
    // clang-format off
  STRSWITCH(str.data(), str.length(), 0,
            (CASES, ("1", return 1))
            (STARTS_WITH, ("1", return 2))
            (DEFAULT, return -1)
            )
    // clang-format on

    return 0;
}

TEST(strswitch, case_before_starts_with)
{
    EXPECT_EQ(case_before_starts_with(""), -1);
    EXPECT_EQ(case_before_starts_with("1"), 1);
}

int case_after_starts_with(const std::string& str)
{
    // clang-format off
  STRSWITCH(str.data(), str.length(), 0,
            (STARTS_WITH, ("1", return 2))
            (CASES, ("1", return 1))
            (DEFAULT, return -1)
            )
    // clang-format on

    return 0;
}

TEST(strswitch, case_after_starts_with)
{
    EXPECT_EQ(case_after_starts_with(""), -1);
    EXPECT_EQ(case_after_starts_with("1"), 2);
}

int starts_with_empty(const std::string& str)
{
    // clang-format off
  STRSWITCH(str.data(), str.length(), 0,
            (STARTS_WITH, ("", return 2))
            (STARTS_WITH, ("1", return 3))
            (CASES, ("1", return 1))
            (DEFAULT, return -1)
            )
    // clang-format on

    return 0;
}

TEST(strswitch, starts_with_empty)
{
    EXPECT_EQ(starts_with_empty(""), 2);
    EXPECT_EQ(starts_with_empty("1"), 2);
    EXPECT_EQ(starts_with_empty("12"), 2);
}

int ends_with_empty(const std::string& str)
{
    // clang-format off
  STRSWITCH(str.data(), str.length(), 0,
            (ENDS_WITH, ("", return 2))
            (ENDS_WITH, ("1", return 3))
            (CASES, ("1", return 1))
            (DEFAULT, return -1)
            )
    // clang-format on

    return 0;
}

TEST(strswitch, ends_with_empty)
{
    EXPECT_EQ(ends_with_empty(""), 2);
    EXPECT_EQ(ends_with_empty("1"), 2);
    EXPECT_EQ(ends_with_empty("12"), 2);
}

int ends_with(const std::string& str)
{
    // clang-format off
  STRSWITCH(str.data(), str.length(), 0,
            (CASES, ("1", return 1))
            (ENDS_WITH, ("1", return 2))
            (DEFAULT, return -1)
            )
    // clang-format on

    return 0;
}

TEST(strswitch, ends_with)
{
    EXPECT_EQ(ends_with(""), -1);
    EXPECT_EQ(ends_with("1"), 1);
    EXPECT_EQ(ends_with("12"), -1);
    EXPECT_EQ(ends_with("21"), 2);
    EXPECT_EQ(ends_with("11"), 2);
}

int regex_matches(const std::string& str)
{
    // clang-format off
  STRSWITCH(str.data(), str.length(), 0,
            (CASES, ("1", return 1))
            (MATCHES, ("\\d+", return 2))
            (DEFAULT, return -1)
            )
    // clang-format on

    return 0;
}

TEST(strswitch, regex_matches)
{
    EXPECT_EQ(regex_matches(""), -1);
    EXPECT_EQ(regex_matches("1"), 1);
    EXPECT_EQ(regex_matches("12"), 2);
    EXPECT_EQ(regex_matches("21"), 2);
    EXPECT_EQ(regex_matches("11"), 2);
}

int u8_literals(const std::string& str)
{
    // clang-format off
    STRSWITCH(str.data(), str.length(), 0,
              (CASES,
               (u8"cat", return 1)
               (u8"кот", return 2)
               (u8"ネコ", return 4))
               (STARTS_WITH, (u8"ネ", return 5))
               (ENDS_WITH, (u8"コ", return 6))
              (DEFAULT, return -1)
              )
    // clang-format on

    return 0;
}

TEST(strswitch, u8_literals)
{
    EXPECT_EQ(u8_literals(u8"cat"), 1);
    EXPECT_EQ(u8_literals(u8"кот"), 2);
    EXPECT_EQ(u8_literals(u8"ネコ"), 4);
    EXPECT_EQ(u8_literals(u8"ネк"), 5);
    EXPECT_EQ(u8_literals(u8"кコ"), 6);
    EXPECT_EQ(u8_literals("21"), -1);
}

int wide_literals(const std::wstring& str)
{
    // clang-format off
    STRSWITCH(str.data(), str.length(), 0,
              (CASES,
               (L"cat", return 1)
               (L"кот", return 2)
               (L"ネコ", return 4))
              (STARTS_WITH, (L"ネ", return 5))
              (ENDS_WITH, (L"コ", return 6))
              (DEFAULT, return -1)
              )
    // clang-format on

    return 0;
}

TEST(strswitch, wide_literals)
{
    EXPECT_EQ(wide_literals(L"cat"), 1);
    EXPECT_EQ(wide_literals(L"кот"), 2);
    EXPECT_EQ(wide_literals(L"ネコ"), 4);
    EXPECT_EQ(wide_literals(L"ネк"), 5);
    EXPECT_EQ(wide_literals(L"кコ"), 6);
    EXPECT_EQ(wide_literals(L"21"), -1);
}

int utf16_literals(const std::u16string& str)
{
    // clang-format off
    STRSWITCH(str.data(), str.length(), 0,
              (CASES,
               (u"cat", return 1)
               (u"кот", return 2)
               (u"ネコ", return 4))
              (STARTS_WITH, (u"ネ", return 5))
              (ENDS_WITH, (u"コ", return 6))
              (DEFAULT, return -1)
              )
    // clang-format on

    return 0;
}

TEST(strswitch, utf16_literals)
{
    EXPECT_EQ(utf16_literals(u"cat"), 1);
    EXPECT_EQ(utf16_literals(u"кот"), 2);
    EXPECT_EQ(utf16_literals(u"ネコ"), 4);
    EXPECT_EQ(utf16_literals(u"ネк"), 5);
    EXPECT_EQ(utf16_literals(u"кコ"), 6);
    EXPECT_EQ(utf16_literals(u"21"), -1);
}

int utf32_literals(const std::u32string& str)
{
    // clang-format off
    STRSWITCH(str.data(), str.length(), 0,
              (CASES,
               (U"cat", return 1)
               (U"кот", return 2)
               (U"ネコ", return 4))
              (STARTS_WITH, (U"ネ", return 5))
              (ENDS_WITH, (U"コ", return 6))
              (DEFAULT, return -1)
              )
    // clang-format on

    return 0;
}

TEST(strswitch, utf32_literals)
{
    EXPECT_EQ(utf32_literals(U"cat"), 1);
    EXPECT_EQ(utf32_literals(U"кот"), 2);
    EXPECT_EQ(utf32_literals(U"ネコ"), 4);
    EXPECT_EQ(utf32_literals(U"ネк"), 5);
    EXPECT_EQ(utf32_literals(U"кコ"), 6);
    EXPECT_EQ(utf32_literals(U"21"), -1);
}

static std::string tutorial(std::string str)
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

TEST(strswitch, tutorial)
{
  EXPECT_EQ(tutorial("hi"), "hi!");
  EXPECT_EQ(tutorial("how are you?"), "fine");
  EXPECT_EQ(tutorial("good bye"), "bye!");
  EXPECT_THROW(tutorial("does a set of all sets contain itself?"), std::logic_error);
  EXPECT_EQ(tutorial("can i go already?"), "you can");
  EXPECT_EQ(tutorial("die!"), "don't shout at me!");
  EXPECT_EQ(tutorial("123"), "I like this number!");
  EXPECT_EQ(tutorial("how are you doing this?"), "sorry?");
}

int main(int argc, char** argv)
{
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
