title: Lockfree data structures without a GC
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

# About the talk

* ???
* What do we want to do
* Why we want to do it
* Why the talk
  - Something like examples what can be done
  - Probably don't want to do, use a Mutex
  - Good mental exercise
* TODO: Images

---

# The problem

* Take out a pointer
* Want to free it
* Does someone still use it?
* Can't leave it up for a GC
* GC-based languages have an advantage
  - Overall performance can be faster even with the GC

---

# Example: TODO

* Treiber's stack
* Do an insertion
* Do a deletion
  - Show that we can't free it, some other thread might be using

---

# Generation-based reclamation strategy

* Old and new generation
* Thread pins a generation
* Creates a new one if old is the same as new
* Eventually everyone migrates
* Data released in older than old can be freed, nobody uses

TODO: Images, threads pointing to the generation

---

# Properties

* Low CPU overhead
  - Just pinning the generation
  - Amortized over all modifications in a pin
* Can delay reclamation
  - Not good for RAII resources
* Stuck thread can prevent all reclamation
  - No guaranteed limit on memory overhead
  - Small enough in practice

---

# In practice

* crossbeam
  - stack, queues, channels
  - contrie
* TODO: Something else?

---

# Hazard pointers

* Global registry of pointers in use
  - „Don't you dare free my pointer“
* Using a pointer:
  1. Read the pointer
  2. Put it into registry
  3. Read again to verify, rollback if not
* Freeing a pointer:
  1. Check the _whole_ registry
  2. Not found → delete
  3. Found → delegate deletion to that one

TODO: Images

---

# Properties

* Overhead per operation
* Relatively more expensive writes
  - Can push the expense to random read
* Implementation difficulties:
  - Extending the registry
  - Shrinking the registry
* Freeing not delayed
  - Limited memory overhead (only the registry)
  - RAII-friendly

---

# In practice

* TODO: Find libraries

---

# Arc-swap

* Hybrid approach
  - Limited hazard-pointer registry (no growing/shrinking)
  - Fallback to generation-inspired spin lock
* Guaranteed wait-free reads
* Lock-free writes under specific conditions

---

* TODO: Some conclusion?

---

# Sources

* Articles for original research
* Wikipedia articles
* Crossbeam / stjepan's blog
