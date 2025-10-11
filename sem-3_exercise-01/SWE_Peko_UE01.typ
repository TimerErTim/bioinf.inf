#set page(numbering: "1 von 1", number-align: right, header: [
  WS 2025/26 #h(1fr) Tim Peko
])
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
    block(stroke: gray.lighten(90%), fill: gray.lighten(95%), radius: 5pt, width: 100%, inset: (top: 5pt, right: 10pt, bottom: 10pt, left: 10pt))[
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

#let image_display(image_path, caption: none, width: 20em, height: auto, label_: none) = {
  show figure: set block(width: 100%)

  block[
    #figure(
      box(stroke: black, width: width, height: height, image(image_path, width: 100%, height: auto)),
      caption: caption,
    ) #if label_ != none { label(label_) }
  ]
}

#let annotated_graphic(content, caption: none, width: 20em, height: auto, label_: none) = {
  //show figure: set block(width: 100%)
  block[
    #figure(box(stroke: black, width: width, height: height, inset: 5pt, content), caption: caption) #if label_ != none { label(label_) }
  ]
}

#import "@preview/cetz:0.3.4": canvas, draw, tree
#let tree_display(data, caption: none, width: 20em, height: auto) = {
  show figure: set block(width: 100%)

  block(figure(box(stroke: black, width: width, height: height, inset: 5pt, canvas({
    import draw: *

    set-style(content: (padding: -2.5pt), fill: gray.lighten(70%), stroke: gray.lighten(70%))

    tree.tree(data, spread: 2.5, grow: 1.5, draw-node: (node, ..) => {
      circle((), radius: .45, stroke: none)
      content((), node.content)
    }, draw-edge: (from, to, ..) => {
      line((a: from, number: .6, b: to), (a: to, number: .6, b: from), mark: (end: ">"))
    }, name: "tree")
  })), caption: caption))
}

= Heapsort <heapsort-task-01>

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

Zur Veranschaulichung des Heapsort-Algorithmus wird der Sortiervorgang am Beispiel des Arrays `[5, 2, 2, 8, 9]` schrittweise dargestellt:

+ Ausgangszustand (unsortiertes Array):\ #tree_display(([5], ([2], [8], [9]), ([2])), caption: "Unsortiertes Array als Baum", width: 40%)
+ Nach Heapaufbau (Max-Heap):\ #tree_display(([9], ([8], [5], [2]), ([2])), caption: "Max-Heap nach build_heap", width: 40%)

+ Nach erstem Tausch (9 ans Ende):\ #tree_display(([2], ([8], [5]), ([2])), caption: "Nach Tausch von 9 mit letztem Element", width: 40%)

+ Nach shift_down der Wurzel:\ #tree_display(([8], ([5], [2]), ([2])), caption: "Nach Wiederherstellung der Heap-Eigenschaft", width: 40%)

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

= Heapsort Komplexität

#import "@preview/lilaq:0.5.0" as lq
#let complexity_n = (100, 200, 500, 1000, 2000, 5000, 10000, 15000, 20000, 30000, 40000, 60000, 80000, 100000)
#let complexity_compares = (1030, 2455, 7427, 16854, 37710, 107713, 235329, 370337, 510720, 800704, 1101442, 1721334, 2363025, 3019617)
#let complexity_swaps = (582, 1357, 4039, 9077, 20160, 57119, 124174, 194942, 268385, 419914, 576753, 899780, 1233633, 1574933)

== Lösungsansatz

Für diese Aufgabe wird auf die Lösung der Aufgabe aus @heapsort-task-01 zurückgegriffen. In der Umsetzung machen wir uns globale Variablen für die Vergleichs- und Tauschanzahl zunutze und erstellen Hilfsfunktionen, die diese Variablen entsprechend inkrementieren. Diese verwenden wir in der Heapsort Implementierung.

Außerdem wird in der `main.cpp` Funktion `run_for_size` implementiert, die für eine gegebene Array-Größe und Anzahl von Iterationen die durchschnittliche Vergleichs- und Tauschanzahl ermittelt und ausgibt. Diese Funktion wird für verschiedene Array-Größen aufgerufen und die Ergebnisse in Vektoren gespeichert. Die Ergebnisse sind in @analysis-task-02 zu sehen.

== Source Code

Counter inkrementierende Hilfsfunktionen:
```cpp
// heapsort.hpp

extern long long count_cmp;
extern long long count_swap;

class heap_sorter
{
private:
  static bool less(const content_t &c, index_t i, index_t j) {
    count_cmp++;
    return c[i] < c[j];
  }

  static void swap(content_t &c, index_t i, index_t j) {
    count_swap++;
    std::swap(c[i], c[j]);
  }
};
```

`run_for_size` und `main` Funktion:
```cpp
// main.cpp

long long count_cmp = 0;
long long count_swap = 0;

std::pair<long long, long long> run_for_size(int size, int iterations)
{
    // Reset counters
    count_cmp = 0;
    count_swap = 0;

    // Take iterations testsamples
    for (int i = 0; i < iterations; i++)
    {
        std::vector<int> array = generate_random_array(size);
        heap_sorter::sort(array);
    }

    // Calculate average
    auto average_comparisons = count_cmp / iterations;
    auto average_swaps = count_swap / iterations;

    return std::make_pair(average_comparisons, average_swaps);
}

int main()
{
    int sizes[] = {
      100, 200, 500, 1000, 2000, 5000, 10000,
      15000, 20000, 30000, 40000, 60000, 80000, 100000
    };
    int iterations = 25;

    for (int size : sizes)
    {
        auto result = run_for_size(size, iterations);
        // Do something with the result, like printing it
    }

    return 0;
}

```

Geänderte Funktionen in der Heapsort Implementierung:
```cpp
// heapsort.cpp

void heap_sorter::sort(content_t &c)
{
	build_heap(c);
	for (index_t i = c.size(); i-- > 0;)
	{
		shift_down(c, 0, i + 1);
		swap(c, 0, i);
	}
}

void heap_sorter::shift_down(content_t &c, index_t start, size_t len)
{
	index_t i = start;
	while (left(i) < len)
	{
		index_t largest = i;
		index_t l = left(i);
		index_t r = right(i);

		if (l < len && less(c, largest, l))
		{
			largest = l;
		}
		if (r < len && less(c, largest, r))
		{
			largest = r;
		}
		if (largest == i)
		{
			break;
		}
		swap(c, i, largest);
		i = largest;
	}
}
```

== Analyse <analysis-task-02>

=== Komplexität

Das ermitteln der Vergleichs- & Tauschanzahl für die verschiedenen Array-Größen liefert die folgenden Ergebnisse:

#image_display(
  "assets/2025-10-09_counting_output.png",
  caption: "Ausgabe der Vergleichs- & Tauschanzahl",
  width: 80%,
  label_: "counting-output",
)

Grafisch dargestellt in @complexity-graph lässt sich deutlich eine Komplexität von $O(n log(n))$ erkennen. Zum Vergleich wird noch die Skalierungsformen $O(n)$ als Referenz in der Grafik abgebildet. Dabei wird hier nur die Zeitkomplexität betrachtet. Die Speicherkomplexität ist $O(1)$, weil der Algorithmus in-place arbeitet. Das bedeutet, er benötigt keinen zusätzlichen Speicherplatz. Also Konstant im Bezug auf die Array-Größe $n$.

#annotated_graphic(lq.diagram(
  width: 100%,
  height: 100%,
  title: [Heapsort: Comparisons - Swaps],
  xlabel: [Array Size $n$],
  ylabel: [$T(n)$],
  //xscale: "log",
  //yscale: "log",
  lq.plot(complexity_n, complexity_compares, label: "Comparisons", mark: lq.marks.x),
  lq.plot(complexity_n, complexity_swaps, label: "Swaps", mark: lq.marks.x),
  lq.plot(complexity_n, n => 3.1 * n * calc.log(n), label: [$O(n log(n))$], mark: none),
  lq.plot(complexity_n, n => 13 * n, label: [$O(n)$], mark: none),
), caption: "Komplexität von Comparisons und Swaps", width: 90%, height: 10cm, label_: "complexity-graph")

#figure(table(
  columns: (auto, auto, auto),
  table.header[*Array Size $n$*][*Comparisons*][*Swaps*],
  ..complexity_n.zip(complexity_compares, complexity_swaps).flatten().map(it => [#it]),
), caption: [Comparisons und Swaps abhängig von Array Size $n$]) <complexity-table>

@complexity-table zeigt die in der @complexity-graph dargestellten Werte.

#align(right + bottom)[
  Aufwand in h: 6
]
