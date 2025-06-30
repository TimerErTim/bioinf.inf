#set page(numbering: "1 von 1", number-align: right, header: "SS 2025")
#set heading(numbering: "1.1.")
#set text(font: "Calibri", lang: "de")

#align(center)[
  #text(17pt)[*FDS2 - Übung 10*]\
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

= Beispiel 1: Mustersuchalgorithmen

== Lösungsansatz

Die drei zu implementierenden Algorithmen sind:
- `brute_search`: Die naive, brute-force Suche.
- `bm_search`: Der Boyer-Moore-Algorithmus, der durch eine Heuristik (Bad-Character-Rule) in der Praxis oft sehr schnell
  ist.
- `kmp_search`: Der Knuth-Morris-Pratt-Algorithmus, der durch eine Vorverarbeitung des Musters (LPS-Array) unnötige
  Vergleiche vermeidet.

Jeder dieser Algorithmen wird als statische `search`-Methode in einer eigenen Klasse gekapselt. Diese Methode akzeptiert
den zu durchsuchenden Text, das Muster, eine Startposition und einen `data_collector` zur Instrumentierung.

=== Instrumentierung mit `data_collector`

Ein zentraler Bestandteil der Aufgabe ist die Analyse der Algorithmen. Dazu wird die Klasse `data_collector` implementiert.
Ihre Aufgaben sind:
- Zählen der gesamten Zeichenvergleiche während einer Suche.
- Aufzeichnen, wie oft jedes Zeichen im Text erfolgreich bzw. erfolglos verglichen wurde.
- Exportieren der gesammelten Daten in eine CSV-Datei für die externe Analyse und Visualisierung.

Die `search`-Methoden rufen Methoden des `data_collector` auf, um die Vergleiche zu protokollieren.

=== Projektstruktur

Die Lösung ist wie folgt strukturiert:
- `pattern_search.h/.cpp`: Header- und Implementierungsdateien für die Suchalgorithmen.
- `data_collector.h/.cpp`: Schnittstelle und Implementierung des Datensammlers.
- `example01.cpp`: Das Hauptprogramm, das die Experimente durchführt, die Testdaten generiert und die Algorithmen aufruft.

== Analyseaufgaben

Die Analyse konzentriert sich auf das Laufzeitverhalten der Algorithmen in Abhängigkeit von der Beschaffenheit des
Alphabets. Folgende Alphabete werden untersucht:
- *Binäres Alphabet*: `{0, 1}`
- *Alphabet der Basenpaare*: `{A, C, G, T}`
- *Alphabet der Aminosäuren*: 23 Symbole
- *ASCII-Zeichensatz*: 256 Symbole

Mithilfe eines Zufallszahlengenerators werden Texte und Muster verschiedener Längen erzeugt, um die in der Vorlesung
besprochenen theoretischen Eigenschaften der Algorithmen (z.B. Best-Case, Worst-Case) experimentell zu überprüfen.

== Experimentelle Ergebnisse

In diesem Abschnitt werden die Ergebnisse der durchgeführten Analysen dokumentiert. Die Rohdaten aus den CSV-Dateien
werden mithilfe von Python mit Matplotlib visualisiert und hier dargestellt.

=== Vergleich der Vergleichsoperationen

#let image_display(image_path, caption: none, width: 20em, height: auto) = {
  show figure: set block(width: 100%)

  block(figure(
    box(stroke: black, width: width, height: height, image(image_path, width: 100%, height: auto)),
    caption: caption,
  ))
}

Hier wird eine Grafik eingefügt, die die Anzahl der Zeichenvergleiche für jeden Algorithmus über verschiedene Text- und
Musterlängen für ein bestimmtes Alphabet darstellt.

#image_display("assets/comparison_chart.png", caption: "Anzahl der Vergleiche für alle Algorithmen und Alphabete", width: 90%)

*Analyse*: Die Ergebnisse bestätigen die theoretischen Erwartungen. Der Brute-Force-Algorithmus zeigt durchgehend die
höchste Anzahl an Vergleichen, insbesondere bei größeren Texten und kleineren Alphabeten. Boyer-Moore ist beinahe
ausnahmslos signifikant effizienter als KMP. Der Unterschied wird mit steigender Alphabetgröße immer eindeutiger. KMP
hat lediglich bei binären Alphabeten einen kleinen Vorteil.

=== Einfluss der Alphabetgröße

Hier wird eine Grafik eingefügt, die zeigt, wie sich die Größe des Alphabets auf die Effizienz der Algorithmen im
Worst-Case-Szenario auswirkt.

#image_display(
  "assets/alphabet_size_impact.png",
  caption: "Effizienz in Abhängigkeit von der Alphabetgröße (Worst-Case, große Texte)",
  width: 100%,
)

*Analyse*: Diese Grafik bestätigt, dass der Boyer-Moore-Algorithmus stark von einer größeren Alphabetgröße profitiert.
Die Sprünge mittels der Bad-Character-Heuristik werden größer, was die Anzahl der Vergleiche drastisch reduziert. KMP
zeigt eine relativ stabile Leistung, die weniger von der Alphabetgröße abhängt. Es scheint jedoch parallel zu
Brute-Force zu verlaufen.

=== Heatmap der Zeichenvergleiche

Hier werden Heatmaps eingefügt, die visualisieren, welche Teile des Textes im Worst-Case-Szenario am häufigsten
verglichen wurden. Die Visualisierung erfolgt mittels einer Hilbert-Kurve, um die Lokalität der Zugriffe in einer
2D-Darstellung zu erhalten.

#image_display(
  "assets/heatmaps/ascii_large_brute_force_worst_heatmap.png",
  caption: "Heatmap der Textzugriffe für Brute-Force (ASCII, large, worst-case)",
  width: 80%,
)
#image_display(
  "assets/heatmaps/ascii_large_boyer_moore_worst_heatmap.png",
  caption: "Heatmap der Textzugriffe für Boyer-Moore (ASCII, large, worst-case)",
  width: 80%,
)
#image_display(
  "assets/heatmaps/ascii_large_kmp_worst_heatmap.png",
  caption: "Heatmap der Textzugriffe für KMP (ASCII, large, worst-case)",
  width: 80%,
)

*Analyse*: Der Vergleich der Heatmaps für den großen ASCII-Testfall im Worst-Case ist aufschlussreich. *Brute-Force*
zeigt ein extrem dichtes durchgehendes Muster, was den ineffizienten, zeichenweisen Scan widerspiegelt. *Boyer-Moore*
zeigt ein deutlich verbessertes Muster mit großen Lücken, was auf die weiten Sprünge der Bad-Character-Heuristik
zurückzuführen ist. *KMP* weist ebenfalls ein gleichmäßiges komplett durchlaufendes Muster auf.

Der Vergleich der Heatmaps im Average-Case gibt einen guten Eindruck, wie die Algorithmen in der Praxis arbeiten.

#image_display(
  "assets/heatmaps/ascii_large_brute_force_average_heatmap.png",
  caption: "Heatmap der Textzugriffe für Brute-Force (ASCII, large, average-case)",
  width: 80%,
)
#image_display(
  "assets/heatmaps/ascii_large_boyer_moore_average_heatmap.png",
  caption: "Heatmap der Textzugriffe für Boyer-Moore (ASCII, large, average-case)",
  width: 80%,
)
#image_display(
  "assets/heatmaps/ascii_large_kmp_average_heatmap.png",
  caption: "Heatmap der Textzugriffe für KMP (ASCII, large, average-case)",
  width: 80%,
)

Man erkennt hier schön den abnehmenden Trend zu Ende des Textes. Da wir hier den Durschnittsfall analysieren und dieser per Zufall erzeugt wird, wird die Wahrscheinlichkeit, das Suchmuster noch nicht gefunden zu haben, gegen Ende des Textes immer geringer.

Der Vergleich im Average-Case unter einem binären Alphabet zeigt den Vorteil von *KMP* gegenüber den anderen Algorithmen.

#image_display(
  "assets/heatmaps/binary_large_brute_force_average_heatmap.png",
  caption: "Heatmap der Textzugriffe für Brute-Force (binary, large, average-case)",
  width: 80%,
)

#image_display(
  "assets/heatmaps/binary_large_boyer_moore_average_heatmap.png",
  caption: "Heatmap der Textzugriffe für Boyer-Moore (binary, large, average-case)",
  width: 80%,
)

#image_display(
  "assets/heatmaps/binary_large_kmp_average_heatmap.png",
  caption: "Heatmap der Textzugriffe für KMP (binary, large, average-case)",
  width: 80%,
)

  #align(right + bottom)[
  Aufwand in h: 8
]
