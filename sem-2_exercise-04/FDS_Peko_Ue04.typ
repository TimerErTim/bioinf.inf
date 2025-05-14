#set page(numbering: "1 von 1", number-align: right, header: "28. März 2025")
#set heading(numbering: "1.1.")
#set text(font: "Calibri")

#align(center)[
  #text(17pt)[*FDS2 - Übung 4*]\
  #text(14pt)[SS 2025]

  #text(16pt)[Tim Peko]
]

#context[
  #let show_outline = counter(page).final().first() > 3

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

= Beispiel 1: Permutation

== Lösungsansatz

Die rekursive Funktion `permute` generiert alle möglichen Permutationen eines Vektors durch systematisches Vertauschen der Elemente. Der Algorithmus verwendet einen rekursiven Ansatz, bei dem in jedem Schritt ein Element fixiert und die restlichen Elemente permutiert werden.

== Testfälle

Die Testfälle werden automatisch in der `main` Funktion ausgeführt.

=== Testfall 1: [1, 2, 3]

*Input:*
```txt
{1, 2, 3}
```

*Output:*
```txt
{1, 2, 3}
{1, 3, 2}
{2, 1, 3}
{2, 3, 1}
{3, 2, 1}
{3, 1, 2}
```
Ergebnis: #text(green)[success]

=== Testfall 2: [1, 2]

*Input:*
```txt
{1, 2}
```

*Output:*
```txt
{1, 2}
{2, 1}
```

Ergebnis: #text(green)[success]

= Beispiel 2: Mischen

== Lösungsansatz

Die rekursive Funktion `random_shuffle` implementiert den Fisher-Yates Shuffle Algorithmus in einer rekursiven Variante. Der Algorithmus wählt in jedem Schritt zufällig ein Element aus dem verbleibenden Array und tauscht es mit dem aktuellen Element.

== Testfälle

=== Testfall 1: [1, 2, 3, 4, 5]

*Input:*
```txt
{1, 2, 3, 4, 5}
```

*Output:*
```txt
{2, 1, 3, 5, 4}
```

Ergebnis: #text(green)[success]

=== Testfall 2: [1, 2]

*Input:*
```txt
{1, 2}
```

*Output:*
```txt
{2, 1}
```

Ergebnis: #text(green)[success]

= Beispiel 3: Primzahlen erkennen

== Lösungsansatz

Die indirekt rekursive Funktion `is_prime` prüft die Primzahleigenschaft durch rekursive Division mit allen möglichen Teilern bis zur Quadratwurzel der Zahl.

== Testfälle

=== Testfall 1: 17

*Input:*
```txt
17
```

*Output:*
```txt
true
```

Ergebnis: #text(green)[success]

=== Testfall 2: 4

*Input:*
```txt
4
```

*Output:*
```txt
false
```

Ergebnis: #text(green)[success]

=== Testfall 3: 2

*Input:*
```txt
2
```

*Output:*
```txt
true
```

Ergebnis: #text(green)[success]

=== Testfall 4: 1

*Input:*
```txt
1
```

*Output:*
```txt
false
```

Ergebnis: #text(green)[success]

= Beispiel 4: Große Zahlen vergleichen

== Lösungsansatz

Die rekursive Funktion `compare` vergleicht zwei Zahlen zur Basis 1000, die als einfachverkettete Listen dargestellt sind. Der Vergleich erfolgt rekursiv von links nach rechts.

== Testfälle

=== Testfall 1: 1002003 vs 1002004

*Input:*
```txt
num1 = 1002003
num2 = 1002004
```

*Output:*
```txt
-1
```

Ergebnis: #text(green)[success]

=== Testfall 2: 1002003 vs 1002003

*Input:*
```txt
num1 = 1002003
num2 = 1002003
```

*Output:*
```txt
0
```

Ergebnis: #text(green)[success]

=== Testfall 3: 2000000000049 vs 5

*Input:*
```txt
num1 = 2000000000049
num2 = 5
```

*Output:*
```txt
1
```

Ergebnis: #text(green)[success]

= Beispiel 5: Bisektion

== Lösungsansatz
Die rekursive Funktion `bisection` findet eine Nullstelle einer stetigen Funktion im gegebenen Intervall durch wiederholte Intervallhalbierung.

== Testfälle

Die Testfälle wurden mithilfe von GeoGebra überprüft.

=== Function 1: $f(x) = 2x^3 - 3x^2 + 0.5$

*Input:*
```txt
f(x) = 2x^3 - 3x^2 + 0.5
interval = [-2, 0]
```

*Output:*
```txt
-0.365997
```

Ergebnis: #text(green)[success]

*Input:*
```txt
f(x) = 2x^3 - 3x^2 + 0.5
interval = [-2, 4]
```

*Output:*
```txt
1.36598
```

Ergebnis: #text(green)[success]

=== Function 2: $f(x) = x + e^x$

*Input:*
```txt
f(x) = x + e^x
interval = [-1, 1]
```

*Output:*
```txt
-0.567169
```

Ergebnis: #text(green)[success]

*Input:*
```txt
f(x) = x + e^x
interval = [0, 1]
``` 

*Output:*
```txt
Error: Function has same signs at interval bounds
```

Ergebnis: #text(green)[success]

=== Function 3: $f(x) = 5/2x^2 - 7x + 4$

*Input:*
```txt
f(x) = 2.5x^2 - 7x + 4
interval = [-1, 4]
```

*Output:*
```txt
Error: Function has same signs at interval bounds
```

Ergebnis: #text(green)[success] 

*Input:*
```txt
f(x) = 2.5x^2 - 7x + 4
interval = [1, 4]
```

*Output:*
```txt
1.99998
```

Ergebnis: #text(green)[success]

*Input:*
```txt
f(x) = 2.5x^2 - 7x + 4
interval = [-1, 1]
```

*Output:*
```txt
0.800018
```

Ergebnis: #text(green)[success]



#align(right + bottom)[
  Aufwand in h: 8
]