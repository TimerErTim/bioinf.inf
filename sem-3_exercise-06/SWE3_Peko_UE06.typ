#import "../common/template.typ": documentation-template
#import "../common/components.typ": *

#show: documentation-template.with(
  title: "SWE3 - Übung 6",
  semester-term: "WS 2025/26",
  author: "Tim Peko",
  aufwand-in-h: "3",
)

#import "../common/visualization/code_metrics.typ": *
#import "../common/visualization/test_results.typ": *
#import "../common/visualization/linked_lists.typ": *

= Aufgabe: Doppelt verkettete Liste

== Lösungsidee

Die Aufgabe besteht darin, eine generische doppelt verkettete Liste (`DoublyLinkedList<T>`) zu implementieren, die einen bidirektionalen Iterator zur Verfügung stellt. Die Implementierung folgt modernen C++-Prinzipien und ist mit der C++ Standard Library kompatibel.

=== Interne Struktur der Liste

Die Liste verwendet eine klassische Sentinel-Knoten-Architektur:

#figure(
  rect(inset: 0.5cm, visualize-doubly-linked-list-sentinel-structure()),
  caption: [Visualisierung der Sentinel-Knoten Architektur],
)

Jeder Knoten (`Node<T>`) enthält:
- `data`: Das gespeicherte Element vom Typ `T`
- `prev`: Zeiger auf den vorherigen Knoten
- `next`: Zeiger auf den nächsten Knoten

Die Sentinel-Knoten (`head_` und `tail_`) sind immer vorhanden und enthalten keine echten Daten. Sie dienen als Markierungen für den Anfang und das Ende der Liste und vereinfachen die Implementierung von Einfüge- und Löschoperationen erheblich.

=== Designentscheidungen

*1. Sentinel-Knoten:* Durch die Verwendung von Sentinel-Knoten am Anfang und Ende der Liste werden Sonderfälle bei Einfüge- und Löschoperationen vermieden. `begin()` zeigt immer auf `head_->next`, `end()` zeigt immer auf `tail_`.

*2. Template-Implementierung:* Die Liste ist vollständig templatebasiert, um beliebige Datentypen (primitive Typen, Objekte, Smart Pointer) zu unterstützen.

*3. Gecachte Größe:* Die Größe wird in `size_` gecached, um `size()` in O(1) zu ermöglichen.

*4. Move-Semantik:* Alle relevanten Methoden unterstützen Move-Semantik für effiziente Handhabung von nicht-kopierbaren oder teuren Objekten.

*5. STL-Kompatibilität:* Die Iteratoren sind vollständig STL-konform und ermöglichen die Verwendung mit Standard-Algorithmen wie `std::find`, `std::count`, `std::for_each` etc.

*6. Öffentlicher Namespace:* Die Klassen sind im globalen Namespace definiert, um die Verwendung zu vereinfachen.

== Iterator-Implementierung

=== Bidirektionaler Iterator

Die Wahl eines *bidirektionalen Iterators* (`std::bidirectional_iterator_tag`) ist natürlich für eine doppelt verkettete Liste:

*Begründung:*
1. Die doppelte Verkettung ermöglicht natürlich Vorwärts- und Rückwärtstraversierung
2. Ein Random-Access-Iterator wäre nicht sinnvoll, da Indexzugriff O(n) erfordern würde
3. Ein Forward-Iterator würde die Rückwärtstraversierung nicht unterstützen

=== Iterator-Operationen

```cpp
// STL-konformer Iterator-Typ-Alias
using iterator_category = std::bidirectional_iterator_tag;
using value_type        = T;
using difference_type   = std::ptrdiff_t;
using pointer           = T*;
using reference         = T&;

// Operationen
*it         // Dereferenzierung
it->member  // Zeigerzugriff
++it, it++  // Inkrement (vorwärts)
--it, it--  // Dekrement (rückwärts)
it1 == it2  // Gleichheit
it1 != it2  // Ungleichheit
```

=== Iterator-Struktur

```txt
            Node<T>* current_
                   |
                   v
    +------+     +------+     +------+
    | prev |<--->| Node |<--->| next |
    +------+     +------+     +------+
                 | data |
                 +------+
```

Der Iterator speichert einen Zeiger auf den aktuellen Knoten und navigiert durch Zugriff auf `prev` und `next`.

== Komplexitätsanalyse

#table(
  columns: (auto, auto, auto),
  inset: 8pt,
  align: horizon,
  [*Operation*], [*Komplexität*], [*Begründung*],
  [`push_front`], [O(1)], [Direkter Zugriff auf `head_`],
  [`push_back`], [O(1)], [Direkter Zugriff auf `tail_`],
  [`pop_front`], [O(1)], [Direkter Zugriff auf `head_->next`],
  [`pop_back`], [O(1)], [Direkter Zugriff auf `tail_->prev`],
  [`size`], [O(1)], [Gecachte Größe],
  [`find`], [O(n)], [Lineare Suche erforderlich],
  [`begin/end`], [O(1)], [Direkter Zugriff auf Sentinel-Knoten],
  [`foreach`], [O(n)], [Traversierung aller Elemente],
  [`insert`], [O(1)], [Nur Zeiger-Operationen],
  [`erase`], [O(1)], [Nur Zeiger-Operationen],
)

== Foreach-Methode

Die `foreach`-Methode akzeptiert jeden aufrufbaren Typ durch Template-Parameter:

```cpp
// Template-based (efficient, allows inlining)
template <typename Func>
void foreach(Func func);

// Usage examples:
list.foreach([](int& x) { x *= 2; });           // Lambda
list.foreach(MyFunctor{});                       // Functor
list.foreach(&myFunction);                       // Function pointer
```

*Vorteile des Template-Ansatzes:*
- Keine Laufzeit-Overhead durch `std::function`
- Compiler kann Aufrufe inlinen
- Beliebige aufrufbare Typen werden akzeptiert

#pagebreak(weak: true)
== Design-Überlegung: Iteration und Modifikation

=== Problemstellung

Kann die Liste *während* der Iteration verändert werden? Insbesondere: Können Elemente gelöscht werden?

=== Analyse

```txt
GEFAHRENSITUATION:
                   Iterator it
                        |
                        v
    [A] <---> [B] <---> [C] <---> [D]
               ^
         erase(it) → it wird ungültig!
```

Bei einer naiven Implementierung würde das Löschen des Knotens, auf den der Iterator zeigt, zu einem *dangling pointer* führen. Der Iterator zeigt dann auf freigegebenen Speicher.

=== Sichere Lösung: remove_if-Muster

Die implementierte Lösung verwendet das *remove_if*-Muster:

```cpp
template <typename Predicate>
size_type remove_if(Predicate pred) {
    size_type removed = 0;
    iterator it = begin();
    while (it != end()) {
        if (pred(*it)) {
            it = erase(it);  // erase returns iterator to NEXT element
            ++removed;
        } else {
            ++it;
        }
    }
    return removed;
}
```

*Schlüsselprinzip:* `erase()` gibt einen Iterator auf das *nächste gültige Element* zurück.

=== Alternative: Stabile Iteratoren

Eine fortgeschrittenere Lösung wäre die Implementierung *stabiler Iteratoren*:

```txt
STABILE ITERATOR-ARCHITEKTUR:

  DoublyLinkedList
        |
        v
  +-------------+
  | active_iter |---> Iterator 1 ---> Iterator 2 ---> ...
  +-------------+           |              |
                           v              v
    [A] <---> [B] <---> [C] <---> [D]
```

Bei dieser Architektur würde:
1. Die Liste alle aktiven Iteratoren tracken
2. Bei `erase()` alle betroffenen Iteratoren auf das nächste Element umleiten
3. Der Speicher-Overhead und die Komplexität deutlich steigen

*Fazit:* Für die meisten Anwendungsfälle ist das `remove_if`-Muster ausreichend und effizienter.

#pagebreak(weak: true)
== Teststrategie

Die Tests folgen dem AAA-Prinzip (Arrange-Act-Assert) und decken folgende Kategorien ab:

=== Konstruktor-Tests
- Default-Konstruktor (leere Liste)
- Initializer-List-Konstruktor
- Copy-Konstruktor (tiefe Kopie, Unabhängigkeit)
- Move-Konstruktor (Ownership-Transfer)
- Copy- und Move-Zuweisung
- Self-Assignment

=== Basis-Operationen
- `push_front`: Einfügen am Anfang, Move-Semantik
- `push_back`: Einfügen am Ende, Move-Semantik
- `pop_front`/`pop_back`: Entfernen, Exceptions bei leerer Liste
- `size`: Korrekte Zählung nach allen Operationen
- `empty`: Korrekte Rückgabe
- `clear`: Vollständiges Leeren, Wiederverwendbarkeit

=== Such-Operationen
- `find`: Existierendes/nicht existierendes Element, erstes Vorkommen bei Duplikaten
- `contains`: Boolean-Wrapper um find

=== Iterator-Tests
- `begin()`/`end()`: Korrekte Positionierung
- Inkrement/Dekrement (Präfix und Postfix)
- Dereferenzierung und Pfeil-Operator
- Gleichheits-/Ungleichheitsvergleiche
- Bidirektionale Traversierung
- STL-Typ-Aliase und Iterator-Kategorie

=== Foreach-Tests
- Lambda-Funktionen (lesend und modifizierend)
- Funktor-Objekte
- Funktionszeiger
- Const-Listen (nur lesend)

=== Insert/Erase-Tests
- Einfügen am Anfang, Ende, in der Mitte
- Löschen einzelner Elemente
- Bereich löschen
- `remove_if` mit verschiedenen Prädikaten

=== Edge Cases
- Große Listen (10.000+ Elemente)
- Verschiedene Datentypen (int, string, custom structs)
- Move-Only-Typen (`std::unique_ptr`)
- Wiederholtes Clear und Refill
- Iterator-Stabilität nach Push-Operationen

=== STL-Kompatibilität
- `std::find`, `std::find_if`
- `std::count`
- `std::transform`
- `std::accumulate`
- `std::all_of`, `std::any_of`, `std::none_of`
- `std::for_each`

== Ergebnisse

Die Implementierung erfüllt alle geforderten Anforderungen:

1. *Template-basiert:* Unterstützt beliebige Datentypen
2. *O(1) Operationen:* `push_front`, `push_back` sind konstant
3. *Bidirektionaler Iterator:* STL-konform mit korrektem `iterator_tag`
4. *foreach-Methode:* Akzeptiert Lambdas, Funktoren und Funktionszeiger
5. *Sichere Modifikation:* `remove_if` ermöglicht sicheres Löschen während Iteration
6. *Umfassende Tests:* Über 100 Unit-Tests decken alle Funktionalitäten und Edge Cases ab. Die Ergebnisse sind in @test-cases aufgelistet.

=== Code-Metriken

#let code-counts = json("assets/code_metrics.json")

#figure(
  table(
    columns: 4,
    table.header[*Typ*][*Codezeilen*][*Kommentarzeilen*][*Leerzeilen*],
    ..code-counts
      .pairs()
      .map(((key, value)) => (
        [_#(key)_],
        [#value.code],
        [#value.comments],
        [#value.blanks],
      ))
      .flatten(),
      align: (x, y) => if x == 0 { right } else { auto }
  ),
  caption: [Code-Metriken für das Übungsprojekt],
) <code-metrics-table>

Weil die tabellarische Code-Metrik Darstellung in @code-metrics-table noch nciht genug ist, sind die gleichen Daten nochmal grafisch in @code-metrics-graph dargestellt.

#figure(
  rect(code-metrics-bar-graph(code-counts), inset: 0.5cm),
  caption: [Grafische Darstellung der Code-Metriken]
) <code-metrics-graph>


=== Testfälle <test-cases>
#let test-results = json("assets/test_resuls.json")

Das Google-Test Framework wird in der `main.cpp` aufgerufen und erlaubt so, die Testfälle aus der kompilierten Binärdatei auszuführen. Mit dem\ 
`--gtest_output=json:assets/test_resuls.json`-Flag wird die Ausgabe in Datei geschrieben, die grob in @test-results-overview und detailliert in @test-results-detailed dargestellt wird.

#figure(
  table-test-results-overview(test-results),
  caption: [Übersicht der Testfälle und Ergebnisse]
) <test-results-overview>

#[
  #show figure: set block(breakable: true)
  #figure(
    table-test-results-detailed(test-results),
    caption: [Detailierte Ergebnisse der Testfälle]
  ) <test-results-detailed>
]


