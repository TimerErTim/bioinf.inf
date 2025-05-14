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

Für die Implementierung des binären Suchbaums (`bstree`) wurde eine Struktur angelegt, die einen Baum mit Knoten realisiert. Jeder Knoten enthält einen Wert vom Typ `int`, sowie Zeiger auf den linken und rechten Teilbaum. Der Baum als Ganzes verwaltet einen Zeiger auf den Wurzelknoten und einen Zähler für die Anzahl der Knoten im Baum.

Die Hauptmethoden sind:

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
   - `print`: Gibt den Baum in-order aus
   - `print_2d`: Visualisiert den Baum zweidimensional
   - `print_2d_upright`: Visualisiert den Baum zweidimensional aufrecht

Für die rekursiven Operationen wurden private Hilfsmethoden implementiert, die die eigentliche Rekursion durchführen. Die öffentlichen Methoden dienen hauptsächlich als Wrapper, die die Gültigkeit der Eingabeparameter prüfen und dann die entsprechenden rekursiven Methoden aufrufen.

Bei der Implementierung der `remove`-Methode wurden drei Fälle unterschieden:
1. Löschen eines Blattknotens: Der Knoten wird einfach entfernt
2. Löschen eines Knotens mit einem Kind: Das Kind ersetzt den Knoten
3. Löschen eines Knotens mit zwei Kindern: Der Knoten wird durch den kleinsten Wert im rechten Teilbaum ersetzt

== Testfälle

#let image_display(
  image_path,
  caption
) = {
  block(
    figure(
      block(stroke: black, image(image_path, width: 20em)),
      caption: caption
    )
  )
}

Die Testfälle sind in der Datei `main01.cpp` implementiert. Dabei wurden folgende Aspekte getestet:

=== Testfall 1: Leerer Baum

*Input:*\
Leerer Baum ohne Einfügeoperationen

*Tests:*
- `empty()`: Prüfen, ob der Baum leer ist
- `size()`: Prüfen, ob die Größe 0 ist
- `contains(5)`: Prüfen, ob ein beliebiger Wert enthalten ist
- `at(0, value)`: Prüfen, ob auf einen Index zugegriffen werden kann
- `remove(5)`: Prüfen, ob ein Wert entfernt werden kann
- `clear()`: Prüfen, ob der leere Baum geleert werden kann

*Output:*\
Alle Tests bestanden

*Ergebnis*: #text(green)[success]

=== Testfall 2: Einfügen von Werten

*Input:*\
Einfügen der Werte 10, 5, 15, 3, 7, 12, 20

*Tests:*
- `empty()`: Prüfen, ob der Baum nicht mehr leer ist
- `size()`: Prüfen, ob die Größe 7 ist
- `contains()`: Prüfen, ob alle eingefügten Werte enthalten sind
- Visualisierung des Baums mit `print()`, `print_2d()` und `print_2d_upright()`

*Output:*\
Alle Tests bestanden, Baumstruktur entspricht den Erwartungen:
```
     20
 15
     12
10
     7
 5
     3
```

*Ergebnis*: #text(green)[success]

=== Testfall 3: Kopieren eines Baums

*Input:*\
Kopie eines Baums mit den Werten 10, 5, 15

*Tests:*
- `size()`: Prüfen, ob beide Bäume die gleiche Größe haben
- `equals()`: Prüfen, ob beide Bäume strukturell gleich sind
- Unabhängigkeit: Nach Einfügen in den ersten Baum prüfen, ob beide Bäume unterschiedlich sind

*Output:*\
Alle Tests bestanden

*Ergebnis*: #text(green)[success]

=== Testfall 4: Entfernen von Knoten

*Input:*\
Baum mit den Werten 10, 5, 15, 3, 7, 12, 20

*Tests:*
- Entfernen eines Blattknotens (3)
- Entfernen eines Knotens mit einem Kind (5)
- Entfernen eines Knotens mit zwei Kindern (15)
- Entfernen des Wurzelknotens (10)
- Versuch, einen nicht vorhandenen Wert zu entfernen (100)

*Output:*\
Alle Tests bestanden

*Ergebnis*: #text(green)[success]

=== Testfall 5: Anwenden einer Funktion

*Input:*\
Baum mit den Werten 10, 5, 15

*Tests:*
- Anwenden der Funktion `add_one` auf alle Knoten
- Prüfen, ob alle Werte um 1 erhöht wurden (11, 6, 16)

*Output:*\
Alle Tests bestanden

*Ergebnis*: #text(green)[success]

=== Testfall 6: Indexbasierter Zugriff

*Input:*\
Baum mit den Werten 10, 5, 15

*Tests:*
- Zugriff auf gültige Indices (0, 1, 2)
- Versuch, auf ungültige Indices zuzugreifen (-1, 3)

*Output:*\
Alle Tests bestanden

*Ergebnis*: #text(green)[success]

=== Testfall 7: Zählen von Vorkommen

*Input:*\
Baum mit den Werten 10, 5, 10, 15, 10 (Duplikate)

*Tests:*
- Zählen mehrfacher Vorkommen (10)
- Zählen einzelner Vorkommen (5)
- Zählen nicht vorhandener Werte (100)

*Output:*\
Alle Tests bestanden

*Ergebnis*: #text(green)[success]

=== Testfall 8: Entfernen aller Vorkommen

*Input:*\
Baum mit den Werten 10, 5, 10, 15, 10 (Duplikate)

*Tests:*
- Entfernen aller Vorkommen eines mehrfach vorhandenen Wertes (10)
- Entfernen aller Vorkommen eines einmalig vorhandenen Wertes (5)
- Versuch, alle Vorkommen eines nicht vorhandenen Wertes zu entfernen (100)

*Output:*\
Alle Tests bestanden

*Ergebnis*: #text(green)[success]

=== Testfall 9: Leeren des Baums

*Input:*\
Baum mit den Werten 10, 5, 15

*Tests:*
- Prüfen, ob `clear()` die richtige Anzahl entfernter Knoten zurückgibt
- Prüfen, ob der Baum nach dem Leeren leer ist

*Output:*\
Alle Tests bestanden

*Ergebnis*: #text(green)[success]


#align(right + bottom)[
  Aufwand in h: 5
]
