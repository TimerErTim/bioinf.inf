#set page(paper: "a4")
#set text(font: "Calibri", size: 11pt)
#set heading(numbering: "1.")

#align(center)[
  #text(size: 16pt, weight: "bold")[
    FDS2 - Übung 2: Bitmap Implementation
  ]
  #v(0.5cm)
  #text(size: 14pt)[
    Tim Peko
  ]
  #v(0.2cm)
  #text(size: 12pt)[
    SS 2025
  ]
]

= Lösungsansatz

== Datenstruktur

Die Implementierung basiert auf einer einfachen Struktur, die die Dimensionen des Bildes und einen Zeiger auf die Pixeldaten speichert:

```cpp
struct bitmap {
    long_type width;
    long_type height;
    pixel_type* pixels;
};
```

Die Pixeldaten werden als eindimensionales Array gespeichert, wobei die Pixel zeilenweise angeordnet sind. Der Zugriff auf ein Pixel an der Position (x, y) erfolgt über den Index `y * width + x`.

== Speicherorganisation

Das BMP-Format speichert Bilder zeilenweise von unten nach oben, wobei jede Zeile auf ein Vielfaches von 4 Bytes aufgefüllt wird (Padding). Die Pixeldaten werden im BGR-Format gespeichert (Blau, Grün, Rot).

Dabei war wichtig zu beachten, dass die verwendeten C++ `structs` die Byte-Ausrichtung 1 haben müssen, da die BMP-Dateien in dieser Ausrichtung gespeichert werden. `#pragma pack(push, 1)` wurde verwendet, um die Byte-Ausrichtung zu gewährleisten.

== Implementierte Funktionen

=== Erzeugung von Bitmaps

Die Funktionen `generate_bitmap` macht sich den Konstruktor von `bitmap` zunutze, um ein neues Bitmap-Objekt zu erzeugen und mittels `new` den Speicher auf dem Heap zu reservieren.

=== Größenänderung

Bei der Größenänderung werden die Pixeldaten neu allokiert, die alten Pixeldaten werden mittels `free` freigeben und die neuen Pixeldaten werden mittels `new` angelegt.

=== Bildmanipulation

Verschiedene Funktionen zur Bildmanipulation wurden implementiert:
- `detect_edges`: Kantenerkennung mittels Sobel-Operator
- `fill`: Füllen des Bildes mit einer Farbe
- `invert`: Invertieren der Farben
- `to_gray`: Umwandlung in Graustufen auf Basis des Luminanz-Werts

=== Kantenerkennung

Der Algorithmus berechnet den Gradienten des Bildes in x- und y-Richtung mithilfe von Sobel-Operatoren und kombiniert diese zu einem Gesamtgradienten, der die Stärke der Kante angibt.

Vor der Anwendung des Sobel-Operators wird das Bild in Graustufen umgewandelt.

=== Graustufen

Die Umwandlung in Graustufen erfolgt mithilfe des Luminanz-Werts.
$ L = sqrt(0.299 dot R^2 + 0.587 dot G^2 + 0.114 dot B^2) $
Dieser wird für jeden Pixel berechnet und alle Farbkanäle werden durch diesen Grauwert ersetzt.
$ R' = G' = B' = L $

=== Invertieren

Die Invertierung der Farben erfolgt, indem bei jedem Farbkanal der maximal mögliche Wert $W$ mit dem aktuellen Wert $C_i$ subtrahiert wird:
$
 C_i ' = W - C_i 
$

= Testfälle
Die Implementierung wurde mit verschiedenen Testfällen überprüft. Die Testfälle sind in der Datei `test_bitmap.cpp` zu finden.

== Erzeugung von Bitmaps

Es werden verschiedene Bitmaps mit unterschiedlichen Größen und Farben erzeugt und deren Größe überprüft.

*Ergebnis*: #text(green)[Erfolgreich]

== Kopieren von Bitmaps

Es wird ein Bitmap erzeugt und ein Kopie erstellt. Die ursprüngliche Bitmap wird dann gelöscht und die Kopie wird überprüft.

*Ergebnis*: #text(green)[Erfolgreich]

== Lesen und Schreiben von BMP-Dateien

Es wird ein Bitmap erzeugt und in eine BMP-Datei geschrieben. Die Datei wird dann wieder gelesen und die Pixel werden überprüft.

*Ergebnis*: #text(green)[Erfolgreich]

== Anwendung der Bildmanipulationsfunktionen

Es wird ein Bitmap erzeugt und die Bildmanipulationsfunktionen werden aufgerufen. Die Pixel werden überprüft, ob sie die erwarteten Werte haben.

*Ergebnis*: #text(green)[Erfolgreich]

== Beispiel Anwendung (`example::main`)

Es wird eine gegebene BMP-Datei geladen und die Bildmanipulationsfunktionen werden aufgerufen. Die Ergebnisse werden in eine neue BMP-Datei geschrieben.

Dieser Testfall wird durch manuelles Überprüfen der Ergebnisse bestätigt.

*Ergebnis*: #text(green)[Erfolgreich]

= Fazit

Die Tests zeigen, dass die Implementierung korrekt funktioniert und die geforderten Funktionen erfüllt.

#align(right)[
  #text(style: "italic")[
    Aufwand in Stunden: 8
  ]
]

