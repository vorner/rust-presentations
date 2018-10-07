<!--
This work is licensed under the Creative Commons Attribution-ShareAlike 4.0
International License. To view a copy of this license, visit
http://creativecommons.org/licenses/by-sa/4.0/ or send a letter to Creative
Commons, PO Box 1866, Mountain View, CA 94042, USA.
-->

title: Why you want to learn Rust
class: animation-fade
layout: true

<!-- This slide will serve as the base layout for all your slides -->
.bottom-bar[
  {{title}}
]

---

class: impact

# {{title}}

.left-column[
Michal Vaner (vorner@vorner.cz)
]

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
  - 8 different types for strings
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
  - Couldn't find a recent study :-(
* Won 3 polls for Most Loved Programming Language in a row
* Successfully used in several projects and companies:
  - rustc
  - Firefox
  - DropBox
  - Cloudflare
  - ...
  - Avast is experimenting

---

# Why, theory 1

* People *love* safety
* Therefore, language guaranteeing no UB is great
* Also, people use strong passwords and never write them down on sticky notes
* And use 6-point seat belts

---

# Why, theory 2

* Statistically significant part of programmers are *masochists*
* Interesting theory and might not be completely wrong
  - Programming is often painful
  - And we like to brag about *how* painful it was
* Hmm, probably not...

---

# Why, theory 3

* It solves some pain points
* So despite the pains it brings, it comes out positive
* Possibly different ones for different people:
  - I write in C/C++ because I have no other choice, but it's hard/tiresome.
  - I write in Node/Perl/Lua/Java, but it's slow. I'd make a native extension,
    but I'm afraid of C/C++.

---

# Do I trust my code?

* I'm sitting in a plane and I know it's controlled by my code. How do I feel
  about it?
* Most people don't trust their code (and the trust *lowers* with experience).
* Compensated by applying ridiculous amount of tooling to it:
  - Code review
  - CppCheck, Coverity
  - valgrind
  - Several levels of tests
  - fuzzing

---

# Example 1

```c++
uint64_t timeMsec(clockid_t id) {
	struct timespec ts;
	int result = clock_gettime(id, &ts);
	assert(result != -1);
	return ts.tv_sec * 1000
		+ ts.tv_nsec / 1000000;
}
```

???

* On some architectures, the `tv_sec * 1000` can overflow

---

# Example 2

```python
import time
import database

"""
This program runs as a daemon and complains
whenever it detects the database is broken.
"""

while True:
  time.sleep(300)
  if database.broken():
    print("The database is broken at %s" % time.ctiem())
```

???

* Typo

---

# Example 3

```c
// Read a bufferfull of data, please.
void read_buf(int fd, uint8_t *buffer,
              size_t buf_size) {
  size_t position = 0;
  while (buf_size > position) {
    position += read(fd, buffer + position,
		     buf_size - position);
  }
}
```

???

* If it ever returns an error, it loops forever

---

# Kinds of errors

* Compile-time (doesn't compile)
* Run-time (crashes)
* Logic (gives wrong answers)

- Rust tries to push towards the first
- *If it compiles, it's correct*

---

# Cool, but how?

* Strict types ‒ no automatic conventions, distinct types for distinct things
  - `Duration` vs. `Instant` vs. `SystemTime`
* `enum`s (something like tagged union), avoids invalid combinations
* Separation of indirection and non-existence (no `NULL`)
* `Result<T, E>` ‒ doesn't give you the value unless you do something with the
  error.
* Doesn't allow uninitialized values.
* Lifetimes, sharing `XOR` mutability.
* Mandatory `Mutex` ‒ data is inside, doesn't let you forget to lock.

- Allows to bypass some checks with `unsafe`.

???

In C most is a type alias to int. In python… well, it has types, but nobody
really enforces them, or not unless you run it.

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
#[derive(Debug, Serialize, Deserialize)]
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
}
```
]

.right-column[
* Rust

```rust
let mut input = Vec::new();
fd.read_to_end(&mut input)
    .expect("Didn't work :-(");
```
]

---

# So, what do I get by learning Rust

* Language that feels consistent and pays attention to details
* Pit of success
  - Doing it the right way is usually easier
* Confidence in the code
  - Even in big projects with millions of LOC, large teams and long life times
* Good performance from idiomatic code
* Shorter total time of development
  - POC code is almost ready for production
* More exact thinking and modeling of intentions
  - The checks the compiler does become second nature even in other languages

???

* Down to stuff like coding style ‒ saves time arguing
* No inconsistencies like print vs. println in go (print inserts spaces between
  arguments, println only if the arguments aren't strings)
