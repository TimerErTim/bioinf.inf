#set page(numbering: "1 von 1", number-align: right, header: "24. April 2025")
#set heading(numbering: "1.1.")
#set text(font: "Calibri", lang: "de")

#align(center)[
  #text(17pt)[*FDS2 - Übung 8*]\
  #text(14pt)[SS 2025]

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

= Beispiel 1: ADT `bstree`

== Lösungsansatz

Der binäre Suchbaum (`bstree`) als Ganzes verwaltet einen Zeiger auf den Wurzelknoten und einen Zähler für die Anzahl der Knoten im Baum. Jeder Knoten enthält einen Wert vom Typ `int`, sowie Zeiger auf den linken und rechten Kindsknoten.

Die öffentlich angebotenen Methoden lassen sich in folgende Kategorien unterteilen:

1. *Konstruktoren und Destruktor*:
   - `bstree()`: Erzeugt einen leeren Baum
   - `bstree(bstree const& src)`: Kopiert einen bestehenden Baum
   - `~bstree()`: Gibt den Speicher aller Knoten frei

2. *Zugriffsmethoden*:
   - `apply`: Wendet eine Funktion auf jeden Knoten im Baum an (in-order Traversierung)
   - `at`: Gibt den Wert am angegebenen Index zurück (in-order Traversierung)
   - `contains`: Prüft, ob ein Wert im Baum vorhanden ist
   - `count`: Zählt Vorkommen eines bestimmten Wertes im Baum
   - `empty`: Prüft, ob der Baum leer ist
   - `equals`: Vergleicht zwei Bäume auf strukturelle Gleichheit
   - `size`: Gibt die Anzahl der Knoten im Baum zurück

3. *Modifikationsmethoden*:
   - `insert`: Fügt einen Wert in den Baum ein
   - `remove`: Entfernt ein Vorkommen eines Wertes aus dem Baum
   - `remove_all`: Entfernt alle Vorkommen eines Wertes aus dem Baum
   - `clear`: Entfernt alle Knoten aus dem Baum

4. *Ausgabemethoden*:
   - `print`: Eine flache Darstellung des Baums
   - `print_2d`: Eine zweidimensionale Darstellung des Baums von links nach rechts
   - `print_2d_upright`: Eine zweidimensionale Darstellung des Baums von oben nach unten

Für die rekursiven Operationen wurden private Hilfsmethoden implementiert, die die eigentliche Rekursion durchführen. Die öffentlichen Methoden dienen hauptsächlich als Wrapper, die die Gültigkeit der Eingabeparameter prüfen und dann die entsprechenden rekursiven Methoden aufrufen.

Bei der Implementierung der `remove` Methode wurden drei Fälle unterschieden:
1. Löschen eines Leaf-Knotens: Der Knoten wird einfach entfernt
2. Löschen eines Knotens mit einem Kind: Das Kind ersetzt den Knoten
3. Löschen eines Knotens mit zwei Kindern: Der Knoten wird durch den kleinsten Wert im rechten Teilbaum ersetzt

Die `print` Methoden geben die Baumstruktur in verschiedenen Formaten aus.

== Testfälle

#let image_display(
  image_path,
  caption: none,
  width: 20em,
  height: auto
) = {
  block(
    figure(
      block(stroke: black, image(image_path, width: width, height: height)),
      caption: caption
    )
  )
}

Die Testfälle sind in der Datei `main01.cpp` implementiert und geben die Ergebnisse auf der Konsole aus. Sie wurden in folgende Gruppen unterteilt.

=== Leerer Baum

#image_display(
  "assets/empyt_tree_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]

=== Einzelner Knoten

#image_display(
  "assets/single_node_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]

=== Einfügen und Struktur

#image_display(
  "assets/insertion_structure_test_output1.png",
  height: 95%,
  width: auto
)

#image_display(
  "assets/insertion_structure_test_output2.png",
  height: 100%,
  width: auto
)

#image_display(
  "assets/insertion_structure_test_output3.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]


=== Copy Constructor

#image_display(
  "assets/copy_constructor_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]

=== Entfernen Edge Cases

#image_display(
  "assets/removal_edges_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]

=== Entfernen von Werten

#image_display(
  "assets/remove_all_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]

=== Funktion anwenden

#image_display(
  "assets/apply_function_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]

=== Ausführliches Indexing

#image_display(
  "assets/indexing_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]


=== Baum leeren

#image_display(
  "assets/clear_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]

=== Bäume vergleichen

#image_display(
  "assets/equals_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]

=== Edge Cases

#image_display(
  "assets/edge_cases_test_output.png",
  width: 80%
)

*Ergebnis*: #text(green)[PASSED]



#align(right + bottom)[
  Aufwand in h: 5
]
