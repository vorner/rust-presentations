title: Joining the Rust and C worlds
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

<!--
# Audience requirements

* Have used Rust at least a bit and understands the syntax
* Have used some other language, C or C++ preferred
* Have Rust and C compiler installed and come with a laptop
-->

---

class: impact

# {{title}}

.left-column[
Michal Vaner (michal.vaner@avast.com)
]

???

* Intro, greetings, welcome.
* Questions, Tennis balls.

---

# Agenda

* What is this FFI anyway and why do I need it?
* Calling C from Rust.
* Calling Rust from C.
* Binding generators.
* Common tips and traps.

???

* Time for quiz:
  - How many (computer) languages do you know/use?
  - Did want to use multiple in the same program?
  - Ideas how this is done?
* Anything specific in mind to use this on already?

---

# What is this FFI good for?

* Way to interface with C ABI.
  - C ABI is common ground.
  - Almost any language can interact with C.
* Rust can call C functions.

???
* Access to a lot of functionality

--
* Rust can call other languages functions.
  - If the function pretends to be a C function.

???
* Even more functionality
* Usually C or C++, though

--
* Rust can pretend to be C.

???
* Not having runtime helps
  - Can be embedded into other languages

--
* Rust can be used to write *native extensions*:
  - For languages like Python, Ruby, Java…
  - Just C libraries with the right functions.

???
* There are libraries helping with the boilerplate
  - But we'll have a look at the lower-level stuff
  - So we know how it works

---

# Basics of passing through the FFI

* Only some types are allowed:
  - Primitive types.
  - Structs marked with `#[repr(C)]`.
  - Some others.
* Correctly annotated functions.
* FFI is considered `unsafe`.
  - C can do anything.
  - It's up to the programmer to check preconditions.

???
* Mostly types that exist in C.
* Must have the same representation as in C.
* Annotations to choose the calling conventions.
  - Where the parameters go onto the stack, etc.

---

# Example ‒ C part

```c
#include <stdint.h>

void many_hello_worlds(int how_many) {
    int i;
    for (i = 0; i < how_many; i ++) {
        puts("Hello world");
    }
}
```

???

* Just a contrived example for demonstration purposes.
* Explanation for those who don't know C.

---

# Example ‒ Rust part

```rust
use std::os::raw::c_int;

extern {
    fn many_hello_worlds(how_many: c_int);
}

fn main() {
    unsafe {
        many_hello_worlds(how_many);
    }
}
```

---

# Example ‒ Rust part

```rust
use std::os::raw::c_int;

extern {
*   fn many_hello_worlds(how_many: c_int);
}

fn main() {
    unsafe {
        many_hello_worlds(how_many);
    }
}
```

???

* Just listing the existing function, no body.
* Equivalent signature, just translated to different syntax.

---

# Example ‒ Rust part

```rust
use std::os::raw::c_int;

extern {
    fn many_hello_worlds(how_many: c_int);
}

fn main() {
    unsafe {
*       many_hello_worlds(how_many);
    }
}
```

???

* Called like any other Rust function.
* No magic happening behind the scenes:
  - Rust uses the same runtime model as C, with stack, etc...
  - Places arguments onto the stack.
  - Jumps to the right place in code.
* Unsafe:
  - Compiler can't know what the function does, so it can't check.
  - Checking is up to the programmer.

---

# Exercise

* Compute MD5 of a file.
* Library for MD5 is in C.
* Load the file in Rust, feed the library.
* Skeleton of the program at **TODO**.

???

* In fact, there's Rust [md5](https://github.com/vorner/md5) library.
  - We use md5 as an example for simplicity.
  - But there are other libraries not directly available in Rust.
* The skeleton embeds the C code directly into the project.
* Could link it from other library too.
* An hour or so for trying it out.
* Will be helping along.

---

# Exercise ‒ results

* Did you manage?
* What problems did you have?
  - How did you solve them?
  - Did others have the same problem? And the same solution?

---

# Snip

* The missing part (other topics) go here:
  - The other direction.
  - Binding generators.
  - Tips and tricks.

---

# Wrap-up

* Will you use it? On what project?
* Materials for further study:
  - [The presentation](https://github.com/vorner/rust-presentations/blob/master/workshop-ffi.md)
  - [The Rustonomicon](https://doc.rust-lang.org/nomicon/ffi.html)
  - [The Rust FFI guide](https://michael-f-bryan.github.io/rust-ffi-guide/)
  - [FFI Omnibus](jakegoulding.com/rust-ffi-omnibus/)
