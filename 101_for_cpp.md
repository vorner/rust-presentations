title: Rust 101 for C++ developers
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

* Employed by Avast
* Using both C++ and Rust
  - Professionally and for fun
* Rust is currently favourite language
  - Maintaining a few Rust libraries
  - Expect some bias in the talk
  - Won't tell you to Rewrite It In Rust
* Maybe both communities will hate me after today

---

# Why talk about Rust to C++ folks?

* C++ can do things impossible in other languages
  - „The last resort language“
  - Rust is a direct competitor for this niche
  - Knowing the enemy
* Rust is heavily inspired by C++
  - Both in positive and negative way
  - Probably the closest relative
  - Smaller investment for C++ people to learn
* Expanding the horizons
  - Techniques and patterns used in one can help in the other

---

# About the talk

* Mostly on the intuitive level
  - No time for fine rigorous details
  - Rust doesn't have a standard
* About real-life implementations of C++
  - The C++ standard allows weird implementations
* Will not *teach* Rust syntax
  - Maybe motivate to have a look yourself
* Will skim few selling points of Rust

---

# Why I learned Rust

* Heard about Rust and it's safety guarantees
  - No segfaults, no data races
  - But without GC
* Pretty audacious claims
  - Not even GC languages protect against data races
  - 🦄
* Had a look to *disprove* the claims
  - And failed at that

???

* It's uncommon for something to fulfill the marketing claims

---

# The 10k miles overview

* Rust is *mostly* C++
  - If done today from scratch
* With Haskell's type system
* The best-practices *enforced* by the compiler
* Syntax & feature cleanup
* It wasn't *meant* that way from start
  - The motivation was safety, not C++ replacement

---

# Basic info

.left-column[
* Multi-paradigm
  - *Mostly* imperative
  - Allows functional, OOP, …
* Strong static typing
* Started by Graydon Hoare in 2006
  - Personal project
* Mozilla started to sponsor in 2009
* 1.0 released on May 15 2015
* Community development
  - With semi-formal RFC process
]

.right-column[
```rust
fn main() {
    println!("Hello world");
}
```
]

???

* You've probably heard something, but if not...

---

# Execution model

* The same for both languages
* Memory:
  - Explicit heap, static storage, threads with stacks
  - Stacks are non-relocable and continuous
  - No GC, semi-manual management
* Ahead of the time compilation to native code
* Threads map to OS threads
* Types compose without implicit indirection
  - Members are in-line
* Methods are just syntax sugar for functions
* Minimal run-time introspection

---

# Runtime

* Thin, used on-demand
* Pay only for what you use
* Possible to use without the standard library
* Contains:
  - Before-main initialization
  - Memory allocation
  - Stack unwinding
  - Runtime type information + dynamic dispatch

---

# Other similarities

* RAII
* Both dynamic and static method dispatch
  - Monomorphisation
* Compile-time meta programming
  - Different in each
* Both have undefined behaviour
  - An empty infinite loop is not UB in Rust
  - Only `unsafe` lets you write UB in Rust
* The same threading model
  - Memory orderings
* Compile times

---

# Consequences

* Similar performance characteristics
  - In speed, RAM consumption and code size
* C ffi is very cheap
  - Allows building native extensions for other languages
* The same runtime tools work *mostly* out of the box
  - Valgrind, gdb, perf, heaptrack, …
  - Slight differences in name mangling
* Ability to combine Rust and C++
  - Thin glue code can be auto-generated
  - Allows for cross-language optimisations, like inlining

---

# Highlighted differences

* Syntax
* Drops 47 year old baggage of C compatibility
* Drops *a lot* of complexity
* Attitude to safety & human abilities
* Introduces very strong type system
* Compilation model
* Checked generics
* Error messages are first-class goal

---

# Attitude towards the programmer

* To err is human
  - Especially on Monday morning before coffee ☕
* Cognitive bandwidth is limited
  - Save it for the problem, not language
* Compiler proves certain correctness guarantees
  - Frees some brain power of the programmer
  - Can't be broken by refactoring
* It's better to refuse correct program than compile an incorrect one
  - There are escape hatches

---

# A word from the Father of Rust

*Basically I've an anxious, pessimist personality; most systems I try to build
are a reflection of how terrifying software-as-it-is-made feels to me. I'm
seeking peace and security amid a nightmare of chaos. I want to help programmers
sleep well, worry less.*

  *Graydon Hoare*

---

# Safe by default

* Borrow checker to enforce lifetimes
* Extra `unsafe` power enabled by a keyword
  - Most programs don't need to use
  - Mostly for building new abstractions or FFI
  - No UB without `unsafe`
* Variables default to immutable
* Visibility defaults to private
* All variables need to be initialized before use
* Encourages „pit of success“ APIs
  - Allows encoding certain properties into types
  - Example: HTTP body can be sent just once, after the headers
  - Example: `Mutex` contains protected data

???

* Fighting the borrow checker is a phase of learning Rust
  - Automatic prover
  - It is *usually* right
  - One gets use to it
  - Teaches certain design patterns
* Probably missing something

---

# Mutex in C++

```cpp
class foo {
private:
    std::string data; // Protected by the mutex below
    std::mutex mutex;
public:
    void set_data(std::string new_data) {
        std::lock_guard<std::mutex> guard(mutex);
        data = std::move(new_data);
    }
    std::string get_data() const {
        std::lock_guard<std::mutex> guard(mutex);
        return data;
    }
}
```

---

# Mutex in Rust

```rust
use std::sync::Mutex;

struct Foo {
    Mutex<String> data;
}

impl Foo {
    fn set_data(&self, data: String) {
        let mut guard = self.data.lock().unwrap();
        *guard = data;
    }
    fn data(&self) -> String {
        self.data.lock().unwrap().clone()
    }
}
```

???

* Adheres to Rust codestyle conventions (eg. getter without `get_`).
* Unwrap because of lock poisoning
* The setter would probably be written in a single command too, just showing off
  some syntax.
* Is 1:1 rewrite, including where move semantics or copying of things happen
* Doesn't allow the access to the inner string without locking. In C++ we could
  make a mistake, for example overlook something during refactoring.

---

# Removal of complexity

* No inheritance
* No overloading
  - Results in simple scoping rules
* No default parameters
* Pointer arithmetics as a method
* No exceptions
  - Has panics which behave similarly
  - Strong exception guarantee is not mandated

--

+ Avoids hellfire combos
  - Combinations of features that do something *weird*
  - Like overloading & templates
  - Or inheritance & pointer to array decay

???

* Makes the code a bit more wordy
* Time lost by writing longer code is gained by not figuring out what went wrong
* But also less surprising and easier to read
  - Because things don't happen without a clue in the code

---

# A story with refactoring

* Found this in a header file:

```cpp
static const std::string = "42";
```

???

* Let's do a bit more practical intermezzo, demonstrating the above

--

* That looks *old style* and has other problems
* Let's just have an `unsigned` constant instead
* The compiler would show an error at every use
  - Compiler-driven refactoring

---

# Except...

* No, this really old C pothole ⚠

```cpp
std::string msg = "The answer is " + answer;
std::cout << msg << std::endl;
```

???

* Yes, this *does* produce a warning. If they are turned on.
* And no, this is *not* a modern C++ style. But I didn't *intend* to write
  that.

---

# Type system

* Inspired by Haskell
* Algebraic types
  - Enums (with payload)
  - Something like tagged union
  - When matching, all variants must be handled
  - Closed ‒ may not contain unlisted variants
  - Convention: errors by `Result<T, E>`
* Traits
  - Used for adding methods to types
  - Building block for generics
* No auto-conversions
  - Has coercions, but doesn't *change* the object

???

* But shoehorned into static dispatch of methods & imperative language
* Type elision allowed only inside functions, not in signatures
  - When looking at docs or headers, one wants to know the types, not the read
    the whole body to figure it out
  - Type elision works *backwards* too (eg. figuring the type of vector element
    by what is being inserted)

---

# Example: read the whole file to memory

```rust
use std::io::{Error, Read};
use std::fs::{File, Path};

fn file_to_mem<P: AsRef<Path>>(path: P) -> Result<Vec<u8>, Error> {
    let mut f = File::open(path)?;
    let mut buff = Vec::new();
    f.read_to_end(&mut buff)?;
    Ok(buff)
}
```

???

* Error handling:
  - Need to extract the Ok variant
  - Possible by pattern matching, methods, or the question mark → visible, but
    terse
* Generics, traits
  - On the input
  - The `read_to_end` is method of the `Read` trait, `File` implements that. But
    many other things do.
* Elision: The element of Vec on `Vec::new` is elided (from the `read_to_end`)
* The &mut make it visible the method may/will change the content

---

# Looking up an element, C++

```cpp
std::unordered_map<int64_t, int64_t> map;

// ...

if (const auto &elem = map.find(42); elem != map.end()) {
    std::cout << "Value of 42 is " << elem->second << std::endl;
} else {
    std::cout << "We don't have value of 42" << std::endl;
}
```

---

# Looking up an element, *broken* C++

```cpp
std::unordered_map<int64_t, int64_t> map;

// ...

// Help! If 42 is not there, the aliens might kill all the kittens!
// Or make all the dead kittens alive again!
std::cout << "Value of 42 is " << map.find(42)->second;
```

---

# Looking up an element, Rust

```rust
use std::collections::HashMap;

let mut map: HashMap<i64, i64> = HashMap::new();

// ...

match map.get(&42) {
    Some(elem) => println!("Value of 42 is {}", elem),
    None => println!("We don't have value of 42"),
}
```

???

* The returned `Option<&i64>` forces the programmer to acknowledge the
  possibility of missing element.

---

# Looking up an element, *broken* Rust

```rust
use std::collections::HashMap;

let mut map: HashMap<i64, i64> = HashMap::new();

// ...

println!("The value of 42 is {}", map.get(&42).unwrap());
```

* If value is missing, it panics in *defined* manner
* The `unwrap` is visible in the code and conveys the intention

---

# OOP

* Allows for adding methods to *any* types
  - Including primitive types
* Constructors are not special
  - Simply a static method returning an instance
  - By convention called `new`
* Destructors by implementing the `Drop` trait
* Copying by implementing the `Clone` trait
  - Can be auto-generated by the compiler (opt-in)
* Assignment and similar *moves*
  - Not copies, unlike in C++
  - Original ceases to exist
  - Equivalent of `memcpy`

---

# Moving a vector, C++

```cpp
void consume_vector(std::vector<int> data) {
    // ...
}

int main() {
    std::vector<int> v;
    v.push_back(10);
    v.push_back(20);
    // A new vector is created, takes over v's allocation.
    // v becomes empty, but still exists.
    consume_vector(std::move(v));
    // You still can do this.
    v.push_back(30);
    return 0;
    // Destructor of v runs here.
}
```

---

# Moving a vector, Rust

```rust
fn consume_vector(data: vec<isize>) {
    // ...
}

fn main() {
    let mut v = Vec::new();
    v.push(10);
    v.push(20);
    // No new vector is created. v's stack image is relocated
    // to a different address, still pointing to the old data.
    consume_vector(v);
    // Here, v no longer exists. Trying to access it is compilation
    // error.
    // v.push(30);
    // Therefore, no destructor runs here.
}
```

---

# Moving a vector, idiomatic Rust

* There's a macro to create a vector

```rust
fn consume_vector(data: vec<isize>) {
    // ...
}

fn main() {
    consume_vector(vec![10, 20]);
}
```

---

# Ecosystem & tooling

* `cargo` ‒ Compilation & package manager
  - Central repository of packages
  - Easy to use libraries
  - Common to use many small libraries
* Common tooling out of the box
  - Writing & running tests
  - Documentation generation
  - Documentation code examples are compiled and tested
* Allows building further tools
  - `cargo update` & `cargo outdated`
  - `cargo audit`
  - `cargo bloat`

---

# Commonly accepted conventions

* Naming style is warned about by compiler
* Official coding style
* Naming guidelines

+ Helps reading some else's code
+ Saves time arguing about bikeshedding matters

---

# Compile-time programming

* `const fn`
* Trait programming
  - Example: Serde
* Macros
  - Work on typed token trees
  - *Mostly* hygienic (names don't clash)
  - Pattern matching + recursion
* Procedural macros
  - A function that gets bunch of tokens and produces tokens
  - Compiled first, then made available to the compiler
  - In macro, attribute or derive positions
* `build.rs`

???

* Macros have similar semantics as C++ templates or lisp

---

# Auto-generating boilerplate

```rust
#[derive(
    Copy, Clone, Debug, Eq, PartialEq, Ord,
    PartialOrd, Hash, Serialize, Deserialize,
)]
#[serde(rename_all = "UPPERCASE", deny_unknown_fields)]
struct Point {
    x: i64,
    y: i64,
}
```

* `cargo expand` will show the generated code
* Possible to write by hand
* Some provided by compiler
* The `Serialize` and `Deserialize` are procedural macros

---

# Learning materials

* [The Rust Book](https://doc.rust-lang.org/stable/book/)
* [Rust For C++ programmers](https://github.com/nrc/r4cppp)
* [The Rustonomicon](https://doc.rust-lang.org/stable/nomicon/)
  - About the advanced and unsafe parts
* [Entirely Too Many Linked Lists](https://rust-unofficial.github.io/too-many-lists/index.html)
  - Because linked lists in Rust are *hard*

+ Come work to Avast
