This document explains preprocessor magic that stands behind `STRSWITCH`.

First, let's omit moral considerations of using preprocessor hacks in modern C++ and get down to business.

Second, let's state a problem: we have a function-like macro `f(x)` and we want to apply it to each of the given elements. The problem is, how do we represent the elements and how do we iterate over them?

# Preprocessor data types

There are a number of different preprocessor-level data types. For example, from VMD doc:

| Type | examples                                                                                          |
|------------|---------------------------------------------------------------------------------------------|
| array      | (4,(an_identifier,156,BOOST_VMD_TYPE_IDENTIFIER))                                           |
| list       | (78,(some_identifier,(BOOST_VMD_TYPE_TYPE,BOOST_PP_NIL)))                                   |
| seq        | (identifier)(89)(245)                                                                       |
| tuple      | (any_id,175,BOOST_VMD_TYPE_LIST,happy,21)                                                   |
| sequence   | tree 59 (56,BOOST_VMD_TYPE_SEQ) (128)(fire)(clown) (47,(BOOST_VMD_TYPE_TUPLE,BOOST_PP_NIL)) |

In fact, the only way to iterate over them is to hardcode iteration up to some limit, which we won't discuss. You can look at boost-preprocessor for examples, e.g. at http://www.boost.org/doc/libs/1_64_0/boost/preprocessor/repetition/for.hpp
However, there is just one exception - `seq`s.
Let's investigate iteration over them.

# Terminology
But first, let's use correct terminology. C preprocessor was never intended to do such things, thus `iteration` is just an abstract concept that has to be emulated by some hacks.
## Recursion
Preprocessor does not have any kind of loops, so the first thing that comes to mind is recursion (remember how in C++11 constexpr loops had to be implemented via recursion?). The only problem is that it's actually explicitly prohibited by standard. So, following snippet
```c
#define f(x) x f(x)
f(1)
```
won't expand into infinite series of ones. Instead, it will produce `1 f(1)`. Even if we try to make recursion finite or indirect, the main problem won't go away - macro name can be present only once per "call stack". Relevant quote from the standard:
> If the name of the macro being replaced is found during this scan of the replacement list (not including the
rest of the source file’s preprocessing tokens), it is not replaced. Furthermore, if any nested replacements
encounter the name of the macro being replaced, it is not replaced. These nonreplaced macro name preprocessing
tokens are no longer available for further replacement even if they are later (re)examined in contexts
in which that macro name preprocessing token would otherwise have been replaced.

## Rescanning
But actually recursion is not entirely correct way to think of macro expansion either. After all, function-like macros are not functions. As can be seen from the quote above, subsequent function calls are implemented in terms of `rescanning`.
Consider following example:
```c
#define foo() bar()
#define bar() 3
foo()
```
Result of the expansion is `3` as one would expect, but thinking "macro foo calls macro bar" is wrong. Instead, the process is essentially the following: when preprocessor encounters `foo()`, it simply replaces it with `bar()`, then rescanning takes place and `bar()` gets replaced with its replacement list which is `3`. These replacements are `nested`.

# The trick
And the reason we had to go all the way down into low-level semantics of macro expansion is that there is a trick - standard does not specify what `nested` means. Consider this snippet

```c
#define f(x) 1 x
#define g(x) 2 x

g(f)(g)(3)
```
The trace is:
```c
g(f)(g)(3)
2 f(g)(3)
2 1 g(3)
But is g nested with respect to f?
if yes: 2 1 g(3) <-- g is nested with respect to f, therefore it is not replaced (g -> f -> g)
if no: 2 1 3 <-- g is not nested with respect to f, therefore it is replaced
```
All common preprocessors (gcc, clang, icc, boost wave, even msvc) assume *no*. And the implication is that "call stack" gets reseted after each subsequent invocation, thus allowing us to use macro name more than once!

Now let's use this knowledge to implement naive traversal of `seq`:

## First attempt
```c
#define f(x) x f
f(1)(2)(3)
```
Unfortunately, it will expand to `1 f(2)(3)`. Trace is:
```c
f(1) <-- note that the rest of seq is ignored by common compilers
1 f  <-- even though `f` is not invoked here, we still did encounter its name more than once, thus banning it for further replacements.
```
But solution is simple - indirect recursion.

## Second attempt
```c
#define f(x) f1(x)
#define f1(x) x f2
#define f2(x) x f1
f(1)(2)(3)(4)(5)
```
This will result in `1 2 3 4 5 f2`. Trace is:
```c
f1(1)
1 f2 <-- no reason to ban f1 or f2
<reset call stack and do further replacement>
1 f2(2)
1 2 f1 <-- no reason to ban f1 or f2
<reset call stack and do further replacement>
<and so on>
```
It worked! Now we just need to get rid of trailing `f2`.

## Final version
```c
#define CONCAT_IMPL(a, b) a ## b
#define CONCAT(a, b) CONCAT_IMPL(a, b) // indirect version allowing argument expansion

#define f(x) CONCAT(f1(x), 0)
#define f1(x) x f2(x)
#define f2(x) x f1(x)
#define f10
#define f20

f(1)(2)(3)(4)(5)

```
which will finally result to `1 2 3 4 5`.
Tail of trace:
```c
<...>
CONCAT(1 2 3 4 5 f2, 0)
1 2 3 4 5 f20
1 2 3 4 5
```

# But why?
Unlike other preprocessor data types, `seqs` can be iterated over without limited hard-coding. Unlike templates, they
* don't cause code bloating
* consume much lesser time and memory during compilation
* are basically unlimited, while templates usually have some depth limit forced by compiler
* work in C
* work on a different translation level so they allow eviler magic

# What are the downsides of this method?
* It relies on gray area of standard.
* It relies on preprocessor, which is full of traps for inexperienced hacker.
* It is known to screw identation in some IDEs, most notably in MSVC.

