# strswitch
Strings Witch - fast and tiny library for string pattern matching

[![Build Status](https://travis-ci.org/shadeware/strswitch.svg?branch=master)](https://travis-ci.org/shadeware/strswitch)
[![Build status](https://ci.appveyor.com/api/projects/status/g7s556d4jf8rvo6a?svg=true)](https://ci.appveyor.com/project/shadeware/strswitch)
[![Coverage Status](https://coveralls.io/repos/github/shadeware/strswitch/badge.svg?branch=master)](https://coveralls.io/github/shadeware/strswitch?branch=master)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

Strings Witch is a library that enhance switch with ability to handle strings.

Work in progress! - should work already, but API will probably change

# Usage example
Simple chat bot
```cpp
std::string str = "123";
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
```

# Features

* Very fast: performance is close to performance of native integer switch
* Very easy for compiler: unlike templates can easily handle thousands or even millions of cases
* Very lightweight: single header with <1k LoC
* Very ugly (for now)
