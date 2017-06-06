#define STRSWITCH_CPP14_CONSTEXPR constexpr
#include "strswitch.hpp"
#include "gtest/gtest.h"

template <std::size_t Length>
constexpr int constexpr_tutorial(const char (&str)[Length])
{
    // clang-format off
  STRSWITCH(str, Length - 1, STRSWITCH_DEFAULT_SEED,
            (CASES,
             ("1", return 1)
             ("2", return 2)
             ("3", return 4))
            (STARTS_WITH, ("5", return 5))
            (ENDS_WITH, ("6", return 6))
            (DEFAULT, return -1)
            )
    // clang-format on

    return 0;
}

template <int Value>
static int force_compile_value = Value;

TEST(strswitch, constexpr_tutorial)
{
    EXPECT_EQ(force_compile_value<constexpr_tutorial("1")>, 1);
    EXPECT_EQ(force_compile_value<constexpr_tutorial("01")>, -1);
    EXPECT_EQ(force_compile_value<constexpr_tutorial("10")>, -1);
    EXPECT_EQ(force_compile_value<constexpr_tutorial("0")>, -1);
    EXPECT_EQ(force_compile_value<constexpr_tutorial("")>, -1);

    EXPECT_EQ(force_compile_value<constexpr_tutorial("2")>, 2);
    EXPECT_EQ(force_compile_value<constexpr_tutorial("3")>, 4);

    EXPECT_EQ(force_compile_value<constexpr_tutorial("4")>, -1);

    EXPECT_EQ(force_compile_value<constexpr_tutorial("5")>, 5);
    EXPECT_EQ(force_compile_value<constexpr_tutorial("50")>, 5);

    EXPECT_EQ(force_compile_value<constexpr_tutorial("6")>, 6);
    EXPECT_EQ(force_compile_value<constexpr_tutorial("06")>, 6);
}

