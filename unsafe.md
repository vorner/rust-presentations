title: Unsafe Rust
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

# Co je unsafe

* „Odemkne“ další, potenciálně nebezpečné, možnosti
  - Volání `unsafe` funkcí
  - Dereference pointerů
  - Mutable globals
  - Práce s `union`
* Všechny ochrany (borrow checker) aktivní
* Umožňuje vyvolat „Undefined behavior“

---

# Užití

* FFI
  - Volání funkcí cizího jazyka
  - „C“-čkové rozhraní
* Výkon
  - Přeskakování bezpečnostních kontrol
  - Např. nekontrolování indexů
* Nové abstrakce
  - Např. obousměrný spoják
* Hodně ve standardní knihovně

---

# Undefined behavior

* Základní předpoklady
  - Proměnné jsou inicializované
  - Reference je vždy platná
  - Neexistují dvě mutable reference na stejnou věc
  - Enum má jen vyjmenované hodnoty
  - …
* Optimalizace s nimi počítají

* [Seznam](https://doc.rust-lang.org/reference/behavior-considered-undefined.html)

---

# Undefined behavior

* Nedodržení předpokladů
* Nemusí se stát nic
* Ale může se stát cokoliv
* V safe Rustu to nejde
* V unsafe ručí autor
  - Pokud lze pomocí safe API vyvolat UB, je to „unsound“

---

# Závazek Unsafe

* V Safe Rustu nedovolí
* Ale omezující
* Unsafe dovoluje „vše“
* Programátor zaručí dodržení
* Jen v odůvodněných případech
* Podobně jako v Cčku
  - Jiné (přísnější) předpoklady

---

# Syntaxe

* Použití

```rust
unsafe {
    // Superpowers here
}

// Also dangerous!
```

---

# Syntaxe

* Unsafe při volání
* Číst návod
* Sekce `# Safety`, vykřičníček

```rust
/// Summons daemons
///
/// # Safety
///
/// Ensure proper circle of protection is drawn.
unsafe fn daemons() {
    // Superpowers
}
```

---

# Pointery

* Dereference je unsafe
* Cokoliv jiného je safe
* Na rozdíl od reference je „jen číslo“
  - Ne tak úplně (provenance, …)
* Převod na referenci
  - Musí být validní hodnota
  - Dodržení borrow checkingu

---

# Práce s pamětí

* Alokace
* Práce s neinicializovanou pamětí
* `UnsafeCell`, `MaybeUninit`, `transmute`

---

# FFI

* Import a export funkcí
* Konzervativně považované za `unsafe`
* Lze napsat ručně nebo generovat
  - `bindgen`
  - `cbindgen` (opačný směr)

```rust
extern "C" fn malloc(usize) -> *mut libc::void;
```

---

# FFI

* Dodržení všech předpokladů druhé strany
* Výjimky a panic přes FFI jsou UB
* Cesta do „celého zbytku světa“
  - V podstatě všechno používá C jako komunikační vrstvu

---

# Best practices

* „Zabalit“ do safe API
  - Uživatel nemusí vědět, že uvnitř je nějaké unsafe
  - Je třeba audit celého modulu
* Nemíchat s bussiness logikou
  - Oddělený crate na jednu `unsafe` věc a jen na ni
* Zdokumentovat a „dokázat“ korektnost

---

# Další informace

* The Rust Book
* [Rustonomicon](https://doc.rust-lang.org/nomicon/)
* [Rust Reference](https://doc.rust-lang.org/reference/index.html)
* [Unsafe code guidelines](https://rust-lang.github.io/unsafe-code-guidelines/)
