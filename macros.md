title: Makra v Rustu
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
Michal Vaner (vorner@vorner.cz)
]

---

# Představení

* Michal Vaner
* https://github.com/vorner
* https://vorner.github.io/
* Evangelista Rustu
* Tady na výpomoc
* Nabízím i (komerční) konzultace

---

# Proč makra

- Declarative macros

* Opakující se kód
  - A nelze použít jiná metoda
* DSL

```rust
bitflags! {
    struct Flags: u32 {
        const A = 0b00000001;
        const B = 0b00000010;
        const C = 0b00000100;
        const ABC = Self::A.bits | Self::B.bits | Self::C.bits;
    }
}
```

---

# Ukázka opakujícího se kódu

```rust
#[derive(Copy, Clone, Debug)]
struct Point(f64, f64);

impl Add for Point {
    type Output = Self;
    fn add(self, other: Self) -> Self {
        Self (self.0 + other.0, self.1 + other.1)
    }
}

impl Sub for Point {
    type Output = Self;
    fn sub(self, other: Self) -> Self {
        Self (self.0 - other.0, self.1 - other.1)
    }
}
```

---

# Alternativy

* Generika (pomocí traitů)
  - Musí sedět typy
* `build.rs`
  - Neumí „reagovat“ na vstup
* Procedurální makra
  - Samostatný crate
  - Možná libovolná manipulace
  - I derive a attribute makra
  - Obvykle pomocí [`quote`] a [`syn`]

[`quote`]: https://docs.rs/quote/latest/quote/
[`syn`]: https://docs.rs/quote/latest/syn/
  - Velmi flexibilní, ale pracné

---

# `build.rs`

```rust
fn main() -> Result<(), Error> {
    fs::write(
        format!("{}/x.rs", env::var("OUT_DIR").unwrap()),
        r#"
            fn hello() -> usize {
                42
            }
        "#,
    )?;
    Ok(())
}
```

```rust
include!(concat!(env!("OUT_DIR"), "/snazzy.items.rs"));
```

---

# Procedurální makra

```rust
#[proc_macro_derive(MyTrait)]
pub fn my_derive(input: TokenStream) -> TokenStream {
    let input: DeriveInput = syn::parse(input).unwrap();
    let name = &input.ident;
    let (impl_generics, ty_generics, where_clause) =
      input.generics.split_for_impl();

    ...

    quote!(
        impl #impl_generics MyTrait for #name #ty_generics {
            ...
        }
    ).into()
}
```

---

# Použití

* Volání s vykřičníkem
* Libovolné závorky (ale párové)
* Nahradí za "rozbalení" makra
* Pracuje nad syntaktickými stromy
* Hygienické

---

# Ukázka

```rust
fn main() {
    println!("Hello world");
}
```

---

# Ukázka ‒ `cargo expand`

```rust
#![feature(prelude_import)]
#[prelude_import]
use std::prelude::rust_2021::*;
#[macro_use]
extern crate std;
fn main() {
    {
        ::std::io::_print(
           ::core::fmt::Arguments::new_v1(&["Hello, world!\n"], &[])
        );
    };
}
```

---

# Jak psát

* Napřed napsat ručně
* Poté "zmakrovat"
* Základní anatomie

```rust
macro_rules! useless_plus {
    (add $a: expr => $b: expr) => {
        $a + $b
    }
}

let x = useless_plus!(add 42 => 12);
```

---

# Metaproměnné a jejich typy

* `block`: `{ ... }`
* `expr`: `1 + 2`
* `ident`: `my_variable`
* `item`: `fn hello() { ... }`
* `meta`: `#[derive(...)]`
* `tt`: Cokoliv
* `ty`: `std::collections::LinkedList<usize>`
* ...

- [Kompletní popis](https://veykril.github.io/tlborm/decl-macros/minutiae/fragment-specifiers.html)

---

# Více patternů

```rust
macro_rules! mac {
    ($a: expr) => { ... };
    ($a: expr, $b: expr) => { ... };
}
```

---

# Opakování

* Opakované části vzoru
* Specifikátory `*`, `+` a `?`

```rust
macro_rules! sum_many {
    ($($a: expr, )+) => {
        [ $( &$a, )+ ].into_iter().sum()
    }
}
```

---

# Rekurze

```rust
macro_rules! sum_many {
    ($($a: expr, )+) => {
        [ $( &$a, )+ ].into_iter().sum()
    };
    ($($a: expr)+,) => {
        sum_many!($( $a, )+)
    };
}
```

---

# Použití z vnějšku

* `#[macro_export]`
* `$crate`
* Plné cesty a reexporty

```rust
#[doc(hidden)]
pub mod reexports {
    pub use std;
}

#[macro_export]
macro_rules boom {
    () => { $crate::reexports::std::panic::panic!("Boom!") }
}

```

---

# Další zdroje

* [Kapitola z The Rust Book](https://doc.rust-lang.org/stable/book/ch19-06-macros.html)
* [The Little Book of Rust Macros](https://veykril.github.io/tlborm/introduction.html)
