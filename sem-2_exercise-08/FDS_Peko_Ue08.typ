#set page(numbering: "1 von 1", number-align: right, header: "24. April 2025")
#set heading(numbering: "1.1.")
#set text(font: "Calibri", lang: "de")

#align(center)[
  #text(17pt)[*FDS2 - Übung 7*]\
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

= Beispiel 1: Objekterkennung

== Lösungsansatz

Eine neue Funktion `count_and_color_objects(...)` erhält ein Quellbild (aus dem die Objekte gezählt werden sollen) und ein Zielbild, in dem die farbliche Markierung gespeichert wird. Der Schwellenwert `theta` $theta$ wird als Parameter übergeben und sorgt für das Abgrenzen der Objekte. Jedes erkannte Objekt wird mit einer neuen Zufallsfarbe eingefärbt.

Es wird über alle Pixel des Quellbildes iteriert und die Helligkeit jedes Pixels mit dem Schwellenwert $theta$ verglichen. Zudem wird in einem Array mitgespeichert, ob die jeweiligen Pixel bereits besucht wurden. Ist dem nicht der Fall und die Helligkeit des Pixels ist größer oder gleich $theta$, wird das Pixel als neues Objekt erkannt und von diesem Pixel ausgehend per Flood-Fill Algorithmus mit einer neuen Zufallsfarbe eingefärbt.

Der Flood-Fill Algorithmus färbt den Pixel und ruft sich selbst rekursiv für alle umliegenden Pixel auf, wenn das Pixel noch nicht besucht wurde und die Helligkeit größer oder gleich $theta$ ist.

Ein naiver rekursiver Flood-Fill Algorithmus funktioniert nicht, da man schnell den Stack mit einem Stack-Overflow sprengt. Es muss für jeden _Pixel_ ein neuer Stack-Frame erstellt werden. Um dies zu vermeiden, wird die Scanline Methode verwendet. Diese markiert eine ganze Zeile auf einmal, und erzeugt daher nur einen Stack-Frame pro _Bildzeile_.

Referenzierte Dateien können unter `assets/` gefunden werden.

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

Die Testfälle sind in der Datei `main01.cpp` zu finden. Dessen Ausführung ergibt folgende Konsolenausgabe:

#block(align(left, figure(
  block(stroke: black, align(left, image("assets/testcases_console_output.png"))),
  caption: "Konsolenausgabe der Testfälle"
)))

=== Testfall 1: Leeres Bild

*Input:*\
leeres Bild (width = 0, height = 0)\
$theta = 0.2$

*Output:*\
```txt
count = 0
Kein geschriebenes Bild
```

*Ergebnis*: #text(green)[success]

=== Testfall 2: Einfarbig unterhalb Threshold

*Input:*\
Komplett schwarzes Bild\
$theta = 0.1$

#image_display(
  "assets/black_picture.png",
  "Schwarzes Bild - Input für Testfall 2"
)

*Output:*\
```txt
count = 0
```

#image_display(
  "assets/testcase-2_output.png",
  "Output von Testfall 2"
)

*Ergebnis*: #text(green)[success]

=== Testfall 3: Einfarbig oberhalb Threshold

*Input:*\
Komplett weißes Bild\
$theta = 0.5$

#image_display(
  "assets/white_picture.png",
  "Weißes Bild - Input für Testfall 3"
)

*Output:*\
```txt
count = 1
```

#image_display(
  "assets/testcase-3_output.png",
  "Output von Testfall 3"
)

*Ergebnis*: #text(green)[success]

=== Testfall 4: Mehrere getrennte Objekte

*Input:*\
Schwarzer Hintergrund mit 3 weißen Punkten (Helligkeit > $theta$)\
$theta = 0.5$

#image_display(
  "assets/3_dots_picture.png",
  "Schwarzes Bild mit 3 weißen Punkten - Input für Testfall 4"
)

*Output:*\
```txt
count = 3
```

#image_display(
  "assets/testcase-4_output.png",
  "Output von Testfall 4"
)

*Ergebnis*: #text(green)[success]

=== Testfall 5: Objekt mit Löchern

*Input:*\
Kreis aus hellen Pixeln mit schwarzen Inneren\
$theta = 0.5$

#image_display(
  "assets/circle_hole_picture.png",
  "Hohler weißer Kreis - Input für Testfall 5"
)

*Output:*\
```txt
count = 1
```

#image_display(
  "assets/testcase-5_output.png",
  "Output von Testfall 5"
)

*Ergebnis*: #text(green)[success]

=== Testfall 6: Ungültiger Schwellenwert

*Input:*\
Komplett weißes Bild\
$theta = 1.1$

#image_display(
  "assets/white_picture.png",
  "Weißes Bild - Input für Testfall 6"
)

*Output:*\
```txt
count = 0
Kein geschriebenes Bild
```

*Ergebnis*: #text(green)[success]




#align(right + bottom)[
  Aufwand in h: 4
]
