#import "../common/template.typ": documentation-template
#import "../common/components.typ": *

#show: documentation-template.with(title: "SWE3 - Übung 4", semester-term: "WS 2025/26", author: "Tim Peko", aufwand-in-h: "-")

// #import "../common/visualization/rational_numbers.typ": *

= Aufgabe 1: Klasse `rational_t` erweitern

In `01_Beispiel` wurde die bisherige, auf `int` spezialisierte Klasse durch einen generischen Datentyp `rational_t<T>` ersetzt und um die geforderten Funktionalitäten erweitert.

== Implementierung

- `rational_t<T>` ist vollständig als Template im Header `rational_t.hpp` implementiert (inline), inklusive:
  - `value_type`-Export (`using value_type = T`)
  - Methode `inverse()` (Kehrwert mit Fehler bei Null)
  - Normalisierung: Vorzeichen am Nenner wird nach oben gezogen, `0/x -> 0/1`. Eine Reduktion auf Normalform erfolgt für `T = int` mittels `std::gcd`. Für andere Bereiche (z. B. Matrizen) wird keine GGT-Reduktion erzwungen.
  - Operatoren als zweistellige `friend`-Funktionen (Barton–Nackman): `+,-,*,/` sowie Vergleiche über Kreuzmultiplikation (`==,!=,<,<=,>,>=`), außerdem `<<`/`>>`.

- Variante 2 (Concept): Ein Concept beschreibt die Minimalanforderungen an `T` (siehe unten). Es wird bewusst keine `ops`-Schicht verwendet.

- `matrix_t<T>`: zu Testzwecken als 1×1-Matrix umgesetzt (`matrix_t.hpp`). Die Operatoren sind als zweistellige `friend`-Funktionen inline implementiert. `matrix_t<T>::one()` und `::zero()` liefern Einheits- bzw. Nullmatrix.

- Gemischte Operatoren mit `int` links sind für `rational_t<int>` definiert (`int + rational_t<int>` etc.).

== Anforderungen an den Typ T

Damit `rational_t<T>` funktioniert, stellt die Implementierung möglichst geringe Anforderungen an `T`. Das Concept fordert nur:

- Konstruktion aus `int` (`T{0}`, `T{1}`)
- Arithmetik: `+`, `-`, `*`, `/`, unäres `-`
- Vergleich: `==`, `<` (andere Relationen werden daraus abgeleitet)
- Streams: `<<`, `>>`

Zusätzlich gilt:

- Für `T` vom ganzzahligen Typ (z. B. `int`) wird auf Normalform reduziert (`std::gcd`).
- Für allgemeine Bereiche ohne Euklidische Division (z. B. `matrix_t<T>`) erfolgt nur Vorzeichen-/Null-Kanonisierung; Gleichheit und Ordnung funktionieren korrekt über Kreuzmultiplikation.

- Für `matrix_t<T>` (1×1):
  - Addition, Subtraktion, Multiplikation, Division und Vergleich leiten auf den enthaltenen Skalar `T` weiter.
  - Damit kann `rational_t<matrix_t<T>>` Kehrwerte bilden und Ausdrücke auswerten, ohne zusätzliche algebraische Anforderungen an allgemeine Matrizen zu stellen.

Hinweis: Eine echte Reduktion auf vollständig gekürzte Form erfordert in allgemeinen Bereichen eine Division durch den gcd. Für `int` ist dies gegeben; anderenfalls wird nur Vorzeichen-/Null-Normalisierung angewandt und Vergleiche erfolgen über Kreuzmultiplikation.

== Tests

Es existiert ein gemeinsames Google-Test-Projekt (`test.cpp`) mit Testfällen pro Teilaspekt:

- Konstruktion, Normalisierung, Streams, Arithmetik, Vergleich, Kettenausdrücke (für `rational_t<int>`)
- Kehrwert (`inverse`) inkl. Fehlerfall bei Null
- Tests für `rational_t<matrix_t<int>>` (1×1), inkl. Kehrwert und Arithmetik

Jeder Test enthält aussagekräftige Namen und prüft erwarteten gegen tatsächlichen Output (über `as_string()`/Vergleiche). Fehlersituationen (z. B. Nenner `0`, Invertieren von `0`) werden explizit getestet und dokumentiert.

== Lösungsidee

Die Klasse kapselt die notwendigen Operationen über eine dünne `ops`-Schicht, sodass die Anforderungen an `T` minimal bleiben. Normalisierung ist zweistufig: (1) Vorzeichen und Null-Kanonisierung für alle `T`; (2) optionale Reduktion über `gcd` dort, wo dies sinnvoll definiert ist (`int`, 1×1-Matrix mit Einheits-gcd). Die Operatoren sind als zweistellige `friend`-Funktionen realisiert (Barton–Nackman), um symmetrische Auflösung und Inlining zu fördern.

// Variante 2 mit Concepts wird verwendet; es gibt keine `ops`-Abstraktionsschicht.

