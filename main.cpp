#if 0

#include <cstdint>
#include <cstring>
#include <regex>

#include "strswitch.hpp"



//#define VARIADIC_APPEND(suffix, ...) TERMINATE_EXPAND(VARIADIC_APPEND_IMPL1(suffix, __VA_ARGS__))
//#define VARIADIC_APPEND_IMPL1(suffix, arg, ...) suffix ## arg, VARIADIC_APPEND_IMPL2(suffix,
//__VA_ARGS__)
//#define VARIADIC_APPEND_IMPL2(suffix, arg, ...) suffix ## arg, VARIADIC_APPEND_IMPL1(suffix,
//__VA_ARGS__)

//#define VARIADIC_APPEND_IMPL10
//#define VARIADIC_APPEND_IMPL20

// VARIADIC_APPEND(lol, 1, 2, 3)

int res = 0;

void source()
{
    // clang-format off
    STRSWITCH("1", 1, STRSWITCH_DEFAULT_SEED,
          (CASES,
          ("1", { res = 1; })
          ("2",  res = 2;)
          ("20", res = 20)
          ("300", res = 299, ++res))
          (STARTS_WITH, ("prefix_", throw "prefix_";) ("pref_", throw "pref_";))
          (ENDS_WITH, ("_suffix", throw "_suffix";) ("_suff", throw "_suff";))
          (STARTS_WITH, ("_", throw "_";))
          (MATCHES, ("\\d+", throw "number")("\\w+", throw "word";))
          (DEFAULT, {res = -1; })
    )
    // clang-format on
}

int main() { source(); }

#endif
