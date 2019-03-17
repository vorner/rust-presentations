title: Let the computer write tests for you
class: animation-fade
layout: true

<!--
This work is licensed under the Creative Commons Attribution-ShareAlike 4.0
International License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to Creative
Commons, PO Box 1866, Mountain View, CA 94042, USA.
-->

.bottom-bar[
  {{title}}
]

---

class: impact

# {{title}}

.left-column[
Michal Vaner (michal.vaner@avast.com)
]

---

# About me

* Programmer here at Avast.
* Working on performance-sensitive code.
* Processes untrusted inputs.
* Care about correctness and security.

---

# Writing tests is hard

* Every program contains at least one bug.
  - Including the examples in this talk!
* Nobody *likes* writing tests.
* Nobody has *time* to write tests.
* Humans are not *good* at writing tests.

???

* Don't spoil it to others and don't point the bugs out

---

# Story of the Monday Morning

* It's Monday morning.
* There was a party last night.
* The coffee didn't kick in yet.
* There was a fight over the scrambled eggs at the canteen.
* A merge request with qicksort implementation came.
* Let's see the code.

???

* Feel into the situation.
* Now let's decide if this code can go in.

---

# Review checklist

* It is readable.
* Looks quicksorty ‒ pivot, swapping, etc.
* Follows style guide & naming conventions.
  - Quicksort conventionally uses `l` & `r`.
* Has tests.
  - 100% test coverage.
  - Tests edge-cases.
  - Tests some larger input.
  - All tests are green.

---

# But it's broken!

* Doesn't recurse into the halves.
* Can go into an infinite loop.
* Can go into infinite recursion.
* None of the tests noticed.

???

* Show the code again

---

# Introducing property-based testing

* The computer generates test case data.
  - Structured data.
  - Author may specify a strategy.
  - A *lot* of test cases are run.
* Author feeds the data to algorithm.
* Then some *properties* of solution are checked.
  - Implicit: doesn't crash.
  - Often verifying solution is simpler than finding it.
  - Possible to compare to trivial (slow) implementation.
* If it fails:
  - The harness tries to *minimize* the input.
  - Stores the seed, to re-check it next time.

???

* Let's show the code.
* Run it and show it fails.
* Comparing to slow implementation is called model based implementation.
* Do we want to fix it? Maybe at the end of session?

---

# Pros & Cons

* Less work to write than thinking of hundreds of test cases.
* Good at spotting overlooked possibilities.
  - Likely overlooked in tests too.
* More CPU intensive ‒ takes longer to run.
* Isn't good at finding edge-cases.
* Suitable at finding algorithmic bugs.
  - For „small“ things.
  - Unit-test scope.
* Doesn't replace but complements conventional unit tests.

???

* Real world example in urlite?
* But bunch of other test cases too.

---

# Libraries

* Rust: proptest, quickcheck, model
* Haskell: quickcheck
* C++: RapidCheck
* Python: Hypothesis

---

# Next example: Morse decoder

* Using base-2 arithmetics.
  - `.` is 0, `-` is 1.
  - Prefix each letter with `1` to disambiguate (`.` vs `..`).
* And a lookup in a table.
* Let's see the code.

???

* Who sees the bug?
* HeartBleed style vulnerability if ever exposed to untrusted input.
  - The client can ask for a byte arbitrarily out of bounds.

---

# About me

* Currently employed at Avast
* Using many programming languages
* Using Rust for about 2 years
* Self-appointed member of the Rust evangelism strike force
  - Won't tell you to Rewrite it in Rust
* First version of presentation made 1.5 years ago at CZ.NIC

---

# What this is about

* Rust the Programming Language
* Why it is interesting
* Not learning the syntax
  - No time for that

---

# What is Rust?

.left-column[
* Compiled „Systems“ programming language
* Aims for safety/UB-freedom
* Fast, low-level, no GC
* Strongly Statically Typed
* Multiplatform, Multiparadigm
* Inspired by:
  - C++
  - Haskell
  - Erlang
  - ...
* Sponsored by Mozilla, but not owned
]
.right-column[
![Ferris](images/crab.svg)
]

???

* Basically like C++, but done right

---

# Problems

* Steep learning curve
  - Lifetimes & Borrow Checker
  - 8 different types of strings
  - Insists on handling errors, thread safety...
* Parts of ecosystem are not mature
  - Some libraries missing, some incomplete
  - Most didn't reach 1.0 (API stability promise)
* Slow compilation
* Doesn't hide complexity
* Very few job offers, not well known to managers
* Internal infrastructures not ready for it

---

# Contrast

* [Top weekend](https://medium.com/@hoffa/the-top-weekend-languages-according-to-githubs-code-6022ea2e33e8#.poomdv8cg) language based on commits in 2016
  - Couldn't find a recent study ☹
* Won 3 polls for Most Loved Programming Language in a row
* Successfully used in several projects and companies:
  - rustc
  - Firefox
  - DropBox
  - Cloudflare
  - rg (faster grep replacement)
  - ...
  - Avast is experimenting

---

# Why, theory 1

* People *love* safety
  - Language guaranteeing no UB is great
* People use strong passwords and don't write them on sticky notes
* And use 6-point seat belts
* 😕

---

# Why, theory 2

* Significant part of programmers are *masochists*
* Interesting theory and might not be completely wrong
  - Programming is often painful
  - And we like to brag about *how* painful it was
* Let's not dig into this 😇

---

# Why, theory 3

* It solves some pain points
* Overall positive outcome of pains
* Different ones for different backgrounds:
  - C/C++ is sometimes tiresome and hard
  - Node/Perl/Python/Lua/Java/… can be slow

???

* C/C++: Either because I still have to worry about not doing UB, or because the
  tooling is so poor

---

# Do I trust my code?

* How about flying a plane with my code in it?
  - 😨
  - 🚂🚃🚃🚃🚃🚃
* Most people don't trust their own code
  - The trust *lowers* with experience
* Compensated by applying ridiculous amount of tooling to it:
  - Code review
  - CppCheck, Coverity
  - valgrind
  - Several levels of tests
  - Fuzzing
  - Denial

---

# Example 1

```c++
uint64_t timeMsec(clockid_t id) {
	struct timespec ts;
	int result = clock_gettime(id, &ts);

	assert(result != -1);

	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}
```

---

# Example 1

```c++
uint64_t timeMsec(clockid_t id) {
	struct timespec ts;
	int result = clock_gettime(id, &ts);

	assert(result != -1);

*	return ts.tv_sec * 1000 + ts.tv_nsec / 1000000;
}
```

???

* On some architectures, the `tv_sec * 1000` can overflow

---

# Example 2

```python
import time
import database

# This program runs as a daemon and complains
# whenever it detects the database is broken.

while True:
    time.sleep(300)
    if database.broken():
        print("The database is broken at %s" % time.ctiem())
```

---

# Example 2

```python
import time
import database

# This program runs as a daemon and complains
# whenever it detects the database is broken.

while True:
    time.sleep(300)
    if database.broken():
*       print("The database is broken at %s" % time.ctiem())
```

???

* Typo

---

# Example 3

```c
// Read one bufferfull of data, please.
void read_buf(int fd, uint8_t *buffer, size_t buf_size) {
  size_t position = 0;

  while (buf_size > position) {
    position += read(fd, buffer + position,
                     buf_size - position);
  }
}
```

---

# Example 3

```c
// Read one bufferfull of data, please.
void read_buf(int fd, uint8_t *buffer, size_t buf_size) {
  size_t position = 0;

  while (buf_size > position) {
*   position += read(fd, buffer + position,
                     buf_size - position);
  }
}
```

???

* If it ever returns an error, strange things start happening
  - Losing one byte of data
  - Maybe underflowing the position

---

# Example 4

```cpp
struct A { uint64_t x = 42; };

struct B: A { string y = "hello"; };

void output(A arr[], size_t cnt) {
    for (size_t i = 0; i < cnt; i ++)
        cout << arr[i].x << endl;
}

int main() {
    B buffer[4] = {};
    output(buffer, 4);
    return 0;
}
```

---

# Example 4

```cpp
struct A { uint64_t x = 42; };

struct B: A { string y = "hello"; };

*void output(A arr[], size_t cnt) {
    for (size_t i = 0; i < cnt; i ++)
        cout << arr[i].x << endl;
}

int main() {
    B buffer[4] = {};
*   output(buffer, 4);
    return 0;
}
```

---

# Example 4 ‒ output

```
42
140734140591416
5
478560413032
```

---

# Kinds of errors

* Compile-time (doesn't compile)
* Run-time (crashes)
* Logic (gives wrong answers)

- Rust tries to push towards the first
- *If it compiles, it's correct*

---

# Shift of mentality

* Human brain is not enough for programming
* Papering over complexity doesn't make it go away
* Don't trust the programmer not to make stupid mistakes
* Better reject some correct programs than accept some incorrect ones
* Wrong assumptions about code should make something break at compile time

---

# Cool, but how?

* Type system from Haskell
  - Strict types, no automatic conversions
  - Prevents confusing stuff (`Duration` vs. `Instant` vs. `SystemTime`)
  - 🍎+🍊
* Enums with data
  - `Result<T, E>` ‒ no access to `Ok` value on error
  - Must match all possibilities
* Separated concepts (reference vs. `NULL`)
* No uninitialized values 👻
* Lifetimes, move semantics, sharing or mutability
* Pit-of-success APIs
  - `Mutex`: data live inside, no access without locking

???

* In C most is a type alias to int. In python… well, it has types, but nobody
really enforces them, or not unless you run it.

* In Java, just about anything can be NULL.

* Still allows bypass some of the checks inside `unsafe`

---

# Avoids surprising constructs

* No function/method overloading
* No inheritance
* No default parameters to functions
* Only „checked“ generics

- And all 4 of them replaced by trait system

---

# All without sacrificing performance

* Runtime model is very close to C++
  - No GC, no traditional runtime
  - Compiled to native, using LLVM
  - Explicit stack and heap
  - Non-growing stacks
* Zero-overhead abstractions
* Easy use of zero-copy
* Can be embedded into other languages

???

* Basically a different syntax for something like C++, but with additional phase
  that proves the program correct.
* Yes, including part of the C++ complexity (the one about memory management,
  etc ‒ not the one about too many interacting rules).
* Zero-overhead: Map with `()` values.
* Calling into and from C is without additional cost ‒ no switching of stacks,
  no hidden translation. Compared to eg. Go, something still needs to do the
  conversions (string in Go is a different thing than string in C), but then
  there's not the part with switching stacks, calling it a suspension point…

---

# Productivity & tooling

* High-level abstractions
  - iterators, collections, cross-platform abstractions
* `cargo` package manager
* Integrated support for tests and documentation
  - Including checking examples for compilation & running
* Promise of stability
  - New compiler release won't break your code
  - Versions of libraries get fixed
* Cross-compiler almost out of the box
* Code generation & metaprogramming support

---

# Example: serialization & deserialization

```rust
/// A statistic summary of one network connection.
#[derive(Clone, Debug, Serialize, Deserialize, Eq, PartialEq)]
#[serde(rename_all(kebab-case))]
enum Traffic {
    #[serde(
        serialize_with="ser_seconds",
        deserialize_with="de_seconds"
    )]
    duration: Duration,
    bytes_in: u64,
    bytes_out: u64,
}
```

???

* Close to what would a hand-crafted code for this exact structure be, no
  runtime reflection
* The generated code can be examined with cargo-expand

---

# Example: read the whole input into a buffer

.left-column[
* C: Let's juggle `read` and `realloc`
* C++: Many ways, no consensus about the right one
* Perl

```perl
my $input;
{
    local $/; # Slurp mode
    $input = <$fd>;
    # Wait… what about errors?
}
```
]

.right-column[
* Python

```python
input = fd.read()
```

* Rust

```rust
let mut input = Vec::new();
fd.read_to_end(&mut input)?;
```
]

---

# So, what do I get by learning Rust

* Language that feels consistent and pays attention to details
* Less surprises, more confidence
  - Even in big projects with millions of LOC, large teams and long life times
  - A lot of assumptions encoded in the types
* Good performance from idiomatic code
* Shorter total time of development
  - POC code is almost ready for production
* More exact thinking and modeling of intentions
  - The checks the compiler does become second nature even in other languages

???

* Down to stuff like coding style ‒ saves time arguing
* No inconsistencies like print vs. println in go (print inserts spaces between
  arguments, println only if the arguments aren't strings)

---

# How to learn

* The Rust Book: https://doc.rust-lang.org/book
* Documentation: https://doc.rust-lang.org
* Try to build something
  - Argue with the compiler for a while
* Many libraries & rustc itself have `E-Easy` issues, often with mentoring
* Friendly community:
  - gitter
  - IRC
  - User's forum
