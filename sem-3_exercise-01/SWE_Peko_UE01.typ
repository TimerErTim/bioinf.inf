#set page(numbering: "1 von 1", number-align: right, header: "WS 2025/26")
#set heading(numbering: "1.1.")
#set text(font: "Calibri", lang: "de")

#align(center)[
  #text(17pt)[*SWE3 - Übung 1*]\
  #text(14pt)[WS 2025/26]

  #text(16pt)[Tim Peko]
]

#context[
  #let show_outline = counter(page).final().first() > 5

  #if show_outline [
    #show outline.entry: it => [
      #set text(size: 14pt - it.element.level * 1.5pt)
      #it
    ]
    #outline(title: "Inhaltsverzeichnis")
    #pagebreak()
  ]

  #if not show_outline [
    #v(2em)
  ]
]

#show raw: it => {
  if it.block {
    block(stroke: gray + 1pt, radius: 5pt, inset: (top: 5pt, right: 10pt, bottom: 10pt, left: 10pt))[
      #place(top + right, dx: 5pt, text(size: 8pt, fill: gray.darken(50%))[
        #it.lang
      ])
      #v(5pt)
      #it
    ]
  } else {
    it
  }
}

#let image_display(image_path, caption: none, width: 20em, height: auto) = {
  show figure: set block(width: 100%)

  block(figure(
    box(stroke: black, width: width, height: height, image(image_path, width: 100%, height: auto)),
    caption: caption,
  ))
}

#import "@preview/cetz:0.3.4": canvas, draw, tree
#let tree_display(data, caption: none, width: 20em, height: auto) = {
  show figure: set block(width: 100%)

  block(figure(
    box(stroke: black, width: width, height: height, inset: 5pt,
      canvas({
        import draw: *

        set-style(content: (padding: -2pt),
          fill: gray.lighten(70%),
          stroke: gray.lighten(70%))

        tree.tree(data, spread: 2.5, grow: 1.5, draw-node: (node, ..) => {
          circle((), radius: .45, stroke: none)
          content((), node.content)
        }, draw-edge: (from, to, ..) => {
          line((a: from, number: .6, b: to),
              (a: to, number: .6, b: from), mark: (end: ">"))
        }, name: "tree")
      })
    ),
    caption: caption,
  ))
}

= Beispiel 1: Heapsort

== Lösungsansatz

Heapsort sortiert ein Feld in-place, indem zuerst ein Max-Heap aufgebaut und anschließend das Maximum wiederholt an das Ende des aktiven Bereichs getauscht wird. Danach wird die Heap-Eigenschaft im verkleinerten Bereich per "Sinken" (shift down) wiederhergestellt.

- *Phase 1 – Heapaufbau (Heapify)*: Beginnend beim letzten Elternknoten wird für jeden Knoten rückwärts die Heap-Eigenschaft durch `shift_down` hergestellt. Das ist linear in der Feldlänge $O(n)$.
- *Phase 2 – Sortieren*: Solange der aktive Bereich nicht leer ist, wird das Wurzelelement (Maximum) mit dem letzten Element des aktiven Bereichs getauscht und der aktive Bereich um eins verkleinert. Anschließend wird die Wurzel per `shift_down` wieder abgesunken. Diese Phase kostet $O(n log(n))$.

Eigenschaften:
- *Laufzeit*: $O(n)$ für den Aufbau, $O(n log(n))$ insgesamt im Worst-/Average-Case.
- *Speicher*: in-place, $O(1)$ zusätzlicher Speicher.
- *Stabilität*: nicht stabil.

=== Heap-Eigenschaft

Ein Heap ist ein binärer Baum, der die Heap-Eigenschaft erfüllt. Diese besagt, dass der Wert jedes Knotens größer oder gleich dem Wert seiner Kinder ist. Für einen Max-Heap bedeutet das, dass der Wert des Wurzelknotens größer oder gleich dem Wert seiner Kinder ist.

=== Heap in-place

Heapsort ist ein in-place-Algorithmus, d.h. er benötigt keinen zusätzlichen Speicher. Alle Operationen werden direkt auf dem zu sortierenden Array durchgeführt.

=== Index-Berechnung für Heap-Navigation

In einem als Array repräsentierten binären Heap können die Indizes von Eltern- und Kindknoten durch einfache arithmetische Operationen berechnet werden:

- *Elternknoten*: Für einen Knoten bei Index `i` befindet sich der Elternknoten bei Index $floor((i-1)/2)$
- *Linkes Kind*: Für einen Knoten bei Index `i` befindet sich das linke Kind bei Index $2i+1$
- *Rechtes Kind*: Für einen Knoten bei Index `i` befindet sich das rechte Kind bei Index $2i+2$

Diese Beziehungen ergeben sich aus der vollständigen binären Baumstruktur, bei der die Knoten ebenenweise von links nach rechts nummeriert werden (beginnend bei Index 0).

Beispiel für Array `[9, 8, 5, 2, 2]`:
- Index 0 (Wert 9): Kinder bei Index 1 und 2
- Index 1 (Wert 8): Eltern bei Index 0, Kinder bei Index 3 und 4
- Index 2 (Wert 5): Eltern bei Index 0, keine Kinder
- Index 3 (Wert 2): Eltern bei Index 1, keine Kinder
- Index 4 (Wert 2): Eltern bei Index 1, keine Kinder


=== Heapsort-Ablauf am Beispiel

Zur Veranschaulichung des Heapsort-Algorithmus wird der Sortiervorgang am Beispiel des Arrays `[5, 2, 8, 2, 9]` schrittweise dargestellt:

+ Ausgangszustand (unsortiertes Array):\ #tree_display(
  (
    [5], ([2], [8], [9]), ([2])
  ),
  caption: "Unsortiertes Array als Baum",
  width: 40%
)
+ Nach Heapaufbau (Max-Heap):\ #tree_display(
  (
    [9], ([8], [5], [2]), ([2])
  ),
  caption: "Max-Heap nach build_heap",
  width: 40%
)

+ Nach erstem Tausch (9 ans Ende):\ #tree_display(
  (
    [2], ([8], [5]), ([2])
  ),
  caption: "Nach Tausch von 9 mit letztem Element",
  width: 40%
)

+ Nach shift_down der Wurzel:\ #tree_display(
  (
    [8], ([5], [2]), ([2])
  ),
  caption: "Nach Wiederherstellung der Heap-Eigenschaft",
  width: 40%
)

+ Sortiertes Ergebnis:\ Das Verfahren wird fortgesetzt, bis alle Elemente sortiert sind: `[2, 2, 5, 8, 9]`

== Source Code

Im Folgenden sind die relevanten Quelltext-Snippets eingebunden, auf deren Basis der Algorithmus erklärt wird.

=== Signaturen (heapsort.hpp)

```cpp
#pragma once

#include <vector>
#include <iostream>
#include <string>

class heap_sorter
{
public:
	using content_t = std::vector<int>;
	using index_t = content_t::size_type;
	using size_t = content_t::size_type;
	using value_t = content_t::value_type;

	static void sort(content_t &c);
	static void print_content(const content_t &c);
	static void print_as_tree(const content_t &c);

private:
	static void build_heap(content_t &c);
	static void shift_down(content_t &c, index_t start, size_t len);
	static void print_as_tree(
		const content_t &c,
		const index_t i,
		const size_t len,
		const size_t depth,
		std::ostream &out = std::cout);
};
```

=== Hilfsfunktionen und Absenken (heapsort.cpp)

```cpp
using index_t = heap_sorter::index_t;
static constexpr index_t left(index_t i)
{
	return (i * 2) + 1;
}
static constexpr index_t right(index_t i)
{
	return (i * 2) + 2;
}
static constexpr index_t parent(index_t i)
{
	return (i - 1) / 2;
}

void heap_sorter::shift_down(content_t &c, index_t start, size_t len)
{
	index_t i = start;
	while (left(i) < len)
	{
		index_t largest = i;
		index_t l = left(i);
		index_t r = right(i);

		if (l < len && c[l] > c[largest])
		{
			largest = l;
		}
		if (r < len && c[r] > c[largest])
		{
			largest = r;
		}
		if (largest == i)
		{
			break;
		}
		std::swap(c[i], c[largest]);
		i = largest;
	}
}
```

=== Heapaufbau (heapsort.cpp)

```cpp
void heap_sorter::build_heap(content_t &c)
{
	if (c.empty())
		return;
	// Start from the last parent node and move upwards
	for (index_t i = c.size() / 2; i-- > 0;)
	{
		shift_down(c, i, c.size());
		print_as_tree(c);
		std::cout << std::endl;
	}
}
```

=== Sortierphase (heapsort.cpp)

```cpp
void heap_sorter::sort(content_t &c)
{
	build_heap(c);
	std::cout << "built heap, sorting..." << std::endl;
	for (index_t i = c.size(); i-- > 0;)
	{
		shift_down(c, 0, i + 1);
		std::cout << "shifted down" << std::endl;
		print_as_tree(c, 0, i + 1, 0);
		std::swap(c[0], c[i]);
		std::cout << "swapped" << std::endl;
		print_content(c);
	}
}
```

=== Baum-Ausgabe (heapsort.cpp)

```cpp
void heap_sorter::print_as_tree(const content_t &c, const index_t i, const size_t len, const size_t depth, std::ostream &out)
{
	if (i >= len)
	{
		return;
	}

	print_as_tree(c, right(i), len, depth + 2, out);
	for (size_t d = 0; d < depth; ++d)
	{
		out << "  ";
	}
	out << c[i] << std::endl;
	print_as_tree(c, left(i), len, depth + 2, out);
}
```

== Testfälle

Nachfolgend sind die vorhandenen Testfälle aus `main.cpp` beschrieben. Jeder Testfall gibt vor und nach dem Sortieren den Zustand aus und verifiziert das Ergebnis per Vergleich mit dem erwarteten Vektor.

=== Leeres Array

- *Ziel*: Prüfen, dass ein leeres Feld unverändert bleibt und keine Fehler auftreten.
- *Relevanz*: Randfall für korrekte Abbruchbedingungen im Algorithmus (`build_heap` kehrt sofort zurück).

#image_display("assets/2025-10-08_empty_array_test.png", caption: "Leeres Array", width: 40%)

=== Negative Elemente

- *Ziel*: Sicherstellen, dass auch negative Werte korrekt sortiert werden.
- *Relevanz*: Vergleichsoperatoren funktionieren unabhängig vom Vorzeichen, Heapsort ist wertunabhängig.

#image_display("assets/2025-10-08_negative_elements_test.png", caption: "Negative Elemente", width: 40%)

=== Absteigend sortiertes Array

- *Ziel*: Worst-Case-nahe Ordnung prüfen; das Ergebnis muss vollständig aufsteigend sein.
- *Relevanz*: Belastet die `shift_down`-Operationen besonders stark.

#image_display("assets/2025-10-08_descending_sorted_array_test.png", caption: "Absteigend sortiertes Array", width: 40%)

=== Bereits sortiertes Array

- *Ziel*: Prüfen, dass ein bereits aufsteigend sortiertes Feld unverändert bleibt.
- *Relevanz*: Keine unnötigen Änderungen, Korrektheit der Swap-/Heap-Grenzenlogik.

#image_display("assets/2025-10-08_already_sorted_array_test.png", caption: "Bereits sortiertes Array", width: 40%)

=== Duplikate

- *Ziel*: Sicherstellen, dass gleiche Werte korrekt gruppiert werden und die Gesamtordnung stimmt.
- *Relevanz*: Heapsort ist nicht stabil, aber die Sortierung nach Wert muss korrekt sein.

#image_display("assets/2025-10-08_duplicates_test.png", caption: "Duplikate", width: 40%)

#align(right + bottom)[
  Aufwand in h: 6
]

= Beispiel 2: Heapsort Komplexität

== Lösungsansatz

== Source Code

== Analysen
