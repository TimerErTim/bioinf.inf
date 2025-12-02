#set page(numbering: "1 von 1", number-align: right, header: "11. April 2025")
#set heading(numbering: "1.1.")
#set text(font: "Calibri")

#align(center)[
  #text(17pt)[*FDS2 - Übung 6*]\
  #text(14pt)[SS 2025]

  #text(16pt)[Tim Peko]
]

#context [
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

= Beispiel 1: Sudoku

== Lösungsansatz

- *`sudoku::read`*:\ Liest ein Sudoku $n^2 times n^2$ der Ordnung $n$ aus einem `std::istream`. Das Format beginnt mit der Ordnung $n$, gefolgt von $n^2$ Zeilen mit jeweils $n^2$ Zahlen (0 für leere Zellen). Es wird ein Grid gemäß der Ordnung initialisiert und dann die Zellen zuerst der Spalte nach, dann Zeile für Zeile eingelesen.
- *`sudoku::is_valid`*:\ Überprüft, ob das Setzen einer Zahl in einer Zelle gemäß den Sudoku-Regeln (Zeile, Spalte, Block) gültig ist, indem über die querenden Zellen iteriert und nach einem Duplikat der zu setzenden Zahl gesucht wird.
- *`sudoku::simplify`*:\ Reduziert den Suchraum vor der Exhaustion durch Constraint Propagation. Dazu werden zwei Strategien angewendet:
  + `check_single_possibility`: Füllt Zellen, für die nur noch eine einzige Zahl möglich ist.
  + `check_unique_in_unit`: Füllt Zellen, wenn eine bestimmte Zahl nur an einer einzigen Stelle in einer Zeile, Spalte oder einem Block platziert werden kann.
  Die `apply_constraints` Funktion wendet diese Strategien iterativ solange an, bis keine weiteren Vereinfachungen mehr möglich sind.
- *`sudoku::solve`*:\ Implementiert einen rekursiven Backtracking-Algorithmus (Exhaustion).
  + Sucht die nächste leere Zelle.
  + Wenn keine leere Zelle gefunden wird, ist das Sudoku gelöst.
  + Probiert für die leere Zelle alle Zahlen von 1 bis n.
  + Wenn eine Zahl gültig ist, wird sie gesetzt und `solve` rekursiv aufgerufen.
  + Wenn der rekursive Aufruf erfolgreich ist, wird `true` zurückgegeben. Das Sudoku ist gelöst.
  + Wenn nicht, wird die Zahl zurückgesetzt (Backtracking) und die nächste Zahl probiert.
  + Wenn keine Zahl funktioniert, wird `false` zurückgegeben. Das Sudoku ist nicht lösbar.

Die `soduku` Klasse wurde in einer eigenen Datei implementiert. Die `main01.cpp` Datei definiert die `main` Funktion und liest die Sudoku-Datei ein. Der Pfad zu dieser Datei kann optional über die Kommandozeile angegeben werden. Die Ausgabe erfolgt auf die Standardausgabe.

Referenzierte Dateien können unter `assets/` gefunden werden.

== Testfälle

=== Testfall 1: Standard Sudoku

*Input:*\
datei: `sudoku-I-3.txt`

#let soduku(order, grid) = [
  #let bold_stroke = 1.5pt
  #let slim_stroke = 0.5pt
  #let n = order
  #let grid = grid
  #let stroke = (x, y) => (
    left: if calc.rem(x, n) == 0 { bold_stroke } else { slim_stroke },
    right: if x + 1 == n * n { bold_stroke } else { slim_stroke },
    top: if calc.rem(y, n) == 0 { bold_stroke } else { slim_stroke },
    bottom: if y + 1 == n * n { bold_stroke } else { slim_stroke },
  )

  #box(table(columns: n * n, stroke: stroke, fill: (x, y) => if calc.even(x)
      and calc.even(y)
      or calc.odd(x) and calc.odd(y) { color.lighten(gray, 75%) }, ..grid
      .flatten()
      .map(it => [
        #if it > 0 {
          it
        }
      ])))
]
#soduku(3, (
  (0, 1, 0, 0, 6, 5, 4, 0, 0),
  (0, 0, 0, 0, 8, 4, 1, 0, 0),
  (4, 0, 0, 0, 0, 0, 0, 7, 0),
  (0, 5, 0, 1, 9, 0, 0, 0, 0),
  (0, 0, 3, 0, 0, 0, 7, 0, 0),
  (0, 0, 0, 0, 3, 7, 0, 5, 0),
  (0, 8, 0, 0, 0, 0, 0, 0, 3),
  (0, 0, 2, 6, 5, 0, 0, 0, 0),
  (0, 0, 9, 8, 1, 0, 0, 2, 0),
))

*Output:*
```txt
-------------------
|  1  |  6 5|4    |
|     |  8 4|1    |
|4    |     |  7  |
-------------------
|  5  |1 9  |     |
|    3|     |7    |
|     |  3 7|  5  |
-------------------
|  8  |     |    3|
|    2|6 5  |     |
|    9|8 1  |  2  |
-------------------

-------------------
|9 1 7|3 6 5|4 8 2|
|2 3 5|7 8 4|1 9 6|
|4 6 8|9 2 1|3 7 5|
-------------------
|7 5 6|1 9 8|2 3 4|
|8 2 3|5 4 6|7 1 9|
|1 9 4|2 3 7|6 5 8|
-------------------
|5 8 1|4 7 2|9 6 3|
|3 7 2|6 5 9|8 4 1|
|6 4 9|8 1 3|5 2 7|
-------------------
```

#soduku(3, (
  (9, 1, 7, 3, 6, 5, 4, 8, 2),
  (2, 3, 5, 7, 8, 4, 1, 9, 6),
  (4, 6, 8, 9, 2, 1, 3, 7, 5),
  (7, 5, 6, 1, 9, 8, 2, 3, 4),
  (8, 2, 3, 5, 4, 6, 7, 1, 9),
  (1, 9, 4, 2, 3, 7, 6, 5, 8),
  (5, 8, 1, 4, 7, 2, 9, 6, 3),
  (3, 7, 2, 6, 5, 9, 8, 4, 1),
  (6, 4, 9, 8, 1, 3, 5, 2, 7),
))

Ergebnis: #text(green)[success]

=== Testfall 2: Ungültige Eingabe (Zu wenig Reihen)

*Input:*
datei: `sudoku-testcase-2.txt`
```txt
3
|0|1|0|0|6|5|4|0|0|
|0|0|0|0|8|4|1|0|0|
```

*Output:*
```txt
error: not enough rows in sudoku grid. expected 9 rows.

Could not solve the Sudoku.
```

Ergebnis: #text(green)[success]

=== Testfall 3: Ungültige Eingabe (Falsche Anzahl Zahlen pro Zeile)

*Input:*
datei: `sudoku-testcase-3.txt`
```txt
2
|1|0|3|0|
|0|2|0|
|3|0|0|0|
|0|4|0|0|1|
```

*Output:*
```txt
error: not enough numbers in row |0|2|0|. expected 4 numbers, got 3.

Could not solve the Sudoku.
```

Ergebnis: #text(green)[success]

=== Testfall 4: Ungültige Eingabe (Ungültiger Wert)

*Input:*
datei: `sudoku-testcase-4.txt`
```txt
2
|1|0|3|0|
|0|4|0|5|
|2|3|4|0|
|0|0|1|0|
```

*Output:*
```txt
error: invalid value 5 at row |0|4|0|5|, col 3. must be between 0 and 4.

Could not solve the Sudoku.
```

Ergebnis: #text(green)[success]

=== Testfall 5: Unlösbares Sudoku

*Input:*
datei: `sudoku-testcase-5.txt`
#soduku(3, (
  (2, 0, 0, 9, 0, 0, 0, 0, 0),
  (0, 0, 0, 0, 0, 0, 0, 6, 0),
  (0, 0, 0, 0, 0, 1, 0, 0, 0),
  (5, 0, 2, 6, 0, 0, 4, 0, 7),
  (0, 0, 0, 0, 0, 4, 1, 0, 0),
  (0, 0, 0, 0, 9, 8, 0, 2, 3),
  (0, 0, 0, 0, 0, 3, 0, 8, 0),
  (0, 0, 5, 0, 1, 0, 0, 0, 0),
  (0, 0, 7, 0, 0, 0, 0, 0, 0),
))

*Output:*
```txt
-------------------
|2    |9    |     |
|     |     |  6  |
|     |    1|     |
-------------------
|5   2|6    |4   7|
|     |    4|1    |
|     |  9 8|  2 3|
-------------------
|     |    3|  8  |
|    5|  1  |     |
|    7|     |     |
-------------------


failure: failed to solve the Sudoku.

Could not solve the Sudoku.
```

Ergebnis: #text(green)[success]


=== Testfall 6: Falsches Sudoku

*Input:*
datei: `sudoku-testcase-6.txt`
#soduku(2, (
  (1, 2, 3, 4),
  (3, 4, 1, 2),
  (2, 1, 4, 3),
  (4, 3, 1, 2),
))

*Output:*
```txt
error: invalid initial sudoku state. duplicate number 1 found at row 1, col 2.

Could not solve the Sudoku.
```


#align(right + bottom)[
  Aufwand in h: 9
]
