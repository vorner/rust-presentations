title: Nechte si testy napsat počítačem
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

# O přednášce

* Představíme si určité testovací metody
  - Randomizované
* Chceme psát kvalitní software
  - Nepadá
  - Dělá co má
  - Nejde „hacknout“
  - Proto píšeme testy. Ale…

---

# Psaní testů je těžké

* Každý program obsahuje alespoň jeden bug 🐛
  - Včetně příkladů k této přednášce!
* Psaní testů nikoho nebaví
* Nikdo na to nemá čas
* Lidé nepíší dobré testy

???

* Pokud ty bugy vidíte, neprozrazujte je ostatním
* Pokud jako programátor zapomenu na situaci, tak ji zapomenu i otestovat

---

# Příběh sobotního rána

* Je sobota ráno
* Včera byl večírek 🍺
* Kafe ještě nezabralo ☕
* Venku je ošklivě
* Ve frontě je merge request s quicksortem
* Nic lepšího na práci

???

* Alespoň ne nic, na co bychom se cítili
* Představme si tu zoufalou situaci
* A jdeme rozhodnout, jestli ten kód přijmeme nebo ne

---

```rust
pub fn qsort(data: &mut [u8]) {
    if data.len() <= 1 { return; }
    let mut l = 0;
    let mut r = data.len() - 1;
    let pivot = data[(l + r) / 2];

    while l < r {
        if data[l] < pivot {
            l += 1;
        } else if data[r] > pivot {
            r -= 1;
        } else {
            data.swap(l, r);
            l += 1; r -= 1;
        }
    }
}
```

---

# Má to testy ‒ a procházejí

```rust
#[test] fn sort_empty() {
    let mut d = [];
    qsort(&mut d);
    assert!(d.is_empty());
}
```

* `[]`
* `[42]`
* `[1, 2, 3]`
* `[3, 2, 1]`
* `[4, 2, 3, 1]`
* `[1, 8, 3, 7, 5, 6, 4, 2, 9]`

???

* Pustit
* Ukázat ten kód

---

# Review checklist

* Je to čitelné
* Vypadá to quicksortovitě (pivot, swap, …)
* Dodržuje styl & jmenné konvence
  - Učebnicový quicksort také používá `l` & `r`
* Má to testy
  - 100% test coverage
  - Testuje okrajové případy
  - Testuje i něco většího
  - Všechny testy jsou zelené
* LGTM! 🐙

???

* Pro většinu účelů, 5 je dostatečně dobrá aproximace nekonečna
  - A máme test s velikostí 9

---

# Ale je to rozbité! 🤦

* Nevolá se rekurzivně na „půlky“
* Při triviálním doplnění způsobí nekonečnou rekurzi
* Žádný z testů neodhalil

???

* Ukázat kód znovu

---

# Ukažme si Property Based Testing

* Počítač nageneruje náhodná vstupní data 🎲
  - Strukturovaná
  - Autor může specifikovat strategii
  - Mnohokrát opakuje
* Autor nakrmí algoritmus daty
* Nakonec se verifikují vlastnosti (properties) výsledku
  - Implicitně: nespadne
  - Ověřit výsledek bývá jednodušší než spočítat
  - Možnost porovnat s triviální (pomalou) implementací
* Pokud selže:
  - Framework se pokusí *minimalizovat* vstup
  - Uloží seed a vyzkouší stejný vstup i příště

???

* Nemusí se nutně ověřovat perfektní správnost
* Porovnávání s triviální/jinou implementací se nazývá „model based testing“

---


```rust
proptest! {
*   #[test] fn sort(mut data: Vec<u8>) {
        let original = data.clone();

*       qsort(&mut data[..]);

        prop_assert_eq!(original.len(), data.len());
        for val in &original {
            prop_assert!(data.contains(val));
        }
        for (prev, cur) in data.iter().tuple_windows() {
            prop_assert!(prev <= cur);
        }
    }
}
```

???

* Pustit a ukázat, že to padá
* Opravit jako domácí úkol pro posluchače
  - Tip: na wikipedii qsort určitě už je 😇

---

# Výhody a nevýhody

* Méně práce než vymýšlet a psát stovky testů
* Dobré pro objevení přehlédnutých situací
  - Pravděpodobně přehlédnuté i v testech
* Náročnější na CPU ‒ běží déle
* Není dobré na nalezení okrajových případů
* Vhodné na objevování algoritmických bugů
  - Na „malé“ věci
  - Velikostí podobné unit-testům
* Nenahrazuje klasické unit testy
  - Rozšiřuje je

???

* Příběh o trápení kolem IP adres

---

# Knihovny

* Rust: proptest, quickcheck, model
* Haskell: quickcheck
* C++: RapidCheck
* Python: Hypothesis

- Pro jiné jazyky určitě existují také

---

# Další ukázka: dekodér morzeovky

* Aritmetika v binární soustavě
  - `.` je 0, `-` je 1
  - Před každé písmeno přilepíme `1` pro odlišení (`.` vs `..`)
* Vyhledat v tabulce
* Podívejme se na kód

---

```c
static const char *tbl = " ETIANMSURWDKGOHVF?L?PJBXCYZQ??54?3???2"
                         "???????16???????7???8?90";

void demorse(char *buff) {
  size_t acc = 1;
  char *write = buff;
  for (char *c = buff; *c; c ++) {
    switch (*c) {
      case '.': acc *= 2; break;
      case '-': acc = acc * 2 + 1; break;
      default:
        *write ++ = tbl[acc - 1];
        acc = 1;
    }
  }
  *write = '\0';
}
```

???

* Kdo vidí bug?
* Pokud to vystavíme nedůvěryhodným vstupům, dostaneme díru stylu heart-bleed
  - Klient si může říct o byte libovolně mimo rozsah

---

```c
static const char *tbl = " ETIANMSURWDKGOHVF?L?PJBXCYZQ??54?3???2"
                         "???????16???????7???8?90";

void demorse(char *buff) {
  size_t acc = 1;
  char *write = buff;
  for (char *c = buff; *c; c ++) {
    switch (*c) {
      case '.': acc *= 2; break;
      case '-': acc = acc * 2 + 1; break;
      default:
*       *write ++ = tbl[acc - 1];
        acc = 1;
    }
  }
  *write = '\0';
}
```

---

# Fuzzing

* Krmí program náhodnými vstupy
  - Obvykle nestrukturované
  - Občas dostatečně validní, aby se dostaly hluboko do programu
  - Zároveň dostatečně nevalidní, aby tam způsobily problémy
* Snaží se způsobit divné chování
  - Crash
  - Obvykle pouštěno se sanitizery
* Nehledá úplně naslepo
  - Vezme již vyzkoušený vstup
  - Maličko upraví
  - Pozoruje jestli prošlo kódem jinudy
  - Pokud objevil nový průchod, přidá ke vstupům

---

```c
extern int LLVMFuzzerTestOneInput(const uint8_t *data, size_t size) {
  // 0-terminate the string
  char buf[size + 1];
  memcpy(buf, data, size);
  buf[size] = '\0';

* demorse(buf);

  return 0;
}
```

```sh
clang -g -O1 -std=c99 -fsanitize=fuzzer,address \
  morse.c fuzz.c -o fuzz
```

???

* Ukázat, pustit
* Vysvětlit výstup

---

# Výhody a nevýhody

* Dobré na hledání bezpečnostních děr
  - Kód pracující s nedůvěryhodným vstupem
  - Dekodéry, parsery, …
* Učí se jak se „zavrtat“ dovnitř programu
  - Pomocí sledování coverage
  - Učí se tím jak vstup vypadá
* Těžkotonážní v porovnání s property based testing
* *Velmi* náročné na CPU
  - Může potřebovat dny až týdny aby něco objevil ⏰

???

* Umí se naučit, že vstupem je URL a jak takové URL vypadá, nebo že pro náš
  program mají tečky a čárky nějaký speciální význam

---

# Nástroje

- Obecně mnoho
  * Je to celkem stará technika
  * Novější vývoj posouvá k praktičnosti

* AFL
  - Krmí to „z vnějšku“ procesu
* LibFuzzer
  - Založené na LLVM, používá sanitazery
  - Celkem jednoduché na použití
  - Pouští to „uvnitř“ procesu
* HongFuzz
  - Dokáže používat HW počítadla místo instrumentace

---

# Hybridní přístup

* Odvodit strukturovaný vstup na základě náhodných dat z fuzzeru
* Tak, aby malá změna vstupu změnila strukturu jen málo
  - Aby to nezmátlo to hledání zajímavých vstupů
* Např. generování syntakticky správných programů pro překladač

- Rust: Knihovna `arbitrary`

---

# K čemu to jde použít

* Zde je místo na Vaše nápady 😇
