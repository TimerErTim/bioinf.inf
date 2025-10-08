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

= Beispiel 1: Heapsort

== Lösungsansatz

Die drei zu implementierenden Algorithmen sind:
- `brute_search`: Die naive, brute-force Suche.
- `bm_search`: Der Boyer-Moore-Algorithmus, der durch eine Heuristik (Bad-Character-Rule) in der Praxis oft sehr schnell
  ist.
- `kmp_search`: Der Knuth-Morris-Pratt-Algorithmus, der durch eine Vorverarbeitung des Musters (LPS-Array) unnötige
  Vergleiche vermeidet.

Jeder dieser Algorithmen wird als statische `search`-Methode in einer eigenen Klasse gekapselt. Diese Methode akzeptiert
den zu durchsuchenden Text, das Muster, eine Startposition und einen `data_collector` zur Instrumentierung.

== Source Code


== Testfälle

=== Leeres Array

=== Negative Elemente

=== Absteigend sortiertes Array

=== Bereits sortiertes Array

=== Duplikate

#align(right + bottom)[
  Aufwand in h: 8
]
