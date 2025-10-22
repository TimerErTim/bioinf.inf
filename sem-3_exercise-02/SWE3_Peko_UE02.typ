#import "../common/template.typ": documentation-template
#import "../common/components.typ": *

#show: documentation-template.with(title: "SWE3 - Übung 2", semester-term: "WS 2025/26", author: "Tim Peko", aufwand-in-h: 12)

#import "../common/visualization/mergesort.typ": *

= Aufgabe: Merge Sort <chapter-task-01>

== Lösungsidee

=== Merge Sort

Der Merge Sort Algorithmus funktioniert, indem wir immer sortierte Subarrays zu einem sortierten Superarray zusammenfügen. Dazu machen wir uns die sortierte Eigenschaft zu Nutze und fügen immer das kleinste Element des linken und rechten Subarrays zu dem Superarray hinzu. Dieser Vorgang wird in @merge-step-visualization demonstriert. Wichtig ist hierbei, dass ein zusätzlicher Buffer benötigt wird, der das gemerged Superarray speichert.

Um die gesamte Collection zu sortieren, brechen wir die Collection auf die kleinste möglichen Subarrays, die bereits sortiert sind, auf. Das sind die Subarrays, die nur ein Element enthalten. Danach wird der Merge Schritt für die immer größer werdenden (gemerged) Super-/Subarrays wiederholt, bis die gesamte Collection sortiert ist. Das ist in @mergesort-visualization visualisiert.

#figure(box(stroke: black, inset: 10pt, visualize_merge_step()), caption: [Visualisierung eines Merge Schrittes]) <merge-step-visualization>

#let mergesort_demo_data = (1, 2, 3, 4, 5, 6, 7, 8)
#figure(box(stroke: black, inset: 10pt, visualize_mergesort(mergesort_demo_data)), caption: [
  Visualisierung des Merge Sort Algorithmus
]) <mergesort-visualization>

==== Eigenschaften

Der Merge Sort Algorithmus ist
- ein stabiler Sortieralgorithmus, da er die relative Reihenfolge der Elemente mit gleichem Wert beibehält.
- kein in-place Sortieralgorithmus, da er zusätzlichen Speicher benötigt.
- ein vergleichsbasierter Sortieralgorithmus, da er nur auf Vergleichsoperatoren basiert.
- gut in Dateien aufteilbar, da er die Daten in kleinere Chunks aufteilt, die separat sortiert werden können.
- mit einer Zeitkomplexität von $O(n log n)$ im besten, durchschnittlichen und schlechtesten Fall.
- mit einer Platzkomplexität von $O(n)$ für den zusätzlichen Speicher.

#figure(
  box(stroke: black, inset: 10pt, visualize_mergesort_complexity()), 
  caption: [Komplexität des Merge Sort Algorithmus]
) <mergesort-complexity>

=== Implementierung In-Memory

Es existiert eine `main.cpp` Datei, die allerdings nur eine Dummy Main Funktion enthält. Sie dient lediglich dazu, das Projekt kompilieren zu können. Alternativ könnte in Visual Studio auch der Projekttyp auf `Static Library` gesetzt werden. Das wird hier zur leichteren Kompatibilität nicht gemacht.

==== Buffer Interfaces

Da in @chapter-task-02 auf die On-Disk Sortierung eingegangen wird, habe ich einen Merge Sort implementiert, der es uns erlaubt, einfach per Interface die Buffer Typen (In-Memory oder On-Disk) zu wechseln:

```cpp
// merge_sort.hpp

/// Generic interface for reading elements in merge operations.
template<typename T>
class IMergeReader {
public:
    virtual ~IMergeReader() = default;

    /// Returns the current value at the reader's position.
    virtual T get() = 0;

    /// Advances the reader to the next value.
    virtual bool advance() = 0;

    /// Checks if the reader is advanced to exhaustion.
    virtual bool is_exhausted() = 0;

    /// Converts this reader into an IMergeWriter, allowing the buffer to be reused for writing.
    virtual std::unique_ptr<IMergeWriter<T>> into_writer() = 0;
};

/// Generic interface for writing (appending) elements in merge operations.
template<typename T>
class IMergeWriter {
public:
    virtual ~IMergeWriter() = default;

    /// Appends a value at the writer's position.
    virtual bool append(const T& value) = 0;

    /// Converts this writer into an IMergeReader, allowing the written data to be read.
    virtual std::unique_ptr<IMergeReader<T>> into_reader() = 0;
};
```

==== Kernlogik <core-mergesort-logic>

Der Kern der Merge Sort Implementierung ist die `merge_step` Methode, die zwei Subarrays merged und in einen Zielbuffer schreibt. Das ist in @merge-step-visualization visualisiert. Chunksize ist dabei die Größe der bereits gemergeden und damit sortierten Subarrays.

```cpp
template <typename T>
bool merge_sorter::merge_step(IMergeReader<T> &reader_l, IMergeReader<T> &reader_r, IMergeWriter<T> &writer, size_t chunk_size_per_reader)
{
    size_t l_merged_count = 0; // Increment until chunk_size is reached
    size_t r_merged_count = 0; // Increment until chunk_size is reached
    bool l_exhausted = reader_l.is_exhausted();
    bool r_exhausted = reader_r.is_exhausted();

    // Zip and merge one chunk of size chunk_size_per_reader
    while (l_merged_count < chunk_size_per_reader && r_merged_count < chunk_size_per_reader && !l_exhausted && !r_exhausted)
    {
        T left_val = reader_l.get();
        T right_val = reader_r.get();

        if (left_val <= right_val)
        {
            writer.append(left_val);
            l_exhausted = !reader_l.advance();
            l_merged_count++;
        }
        else
        {
            writer.append(right_val);
            r_exhausted = !reader_r.advance();
            r_merged_count++;
        }
    }

    // Finish remaining elements from left reader for this chunk
    while (l_merged_count < chunk_size_per_reader && !l_exhausted)
    {
        writer.append(reader_l.get());
        l_exhausted = !reader_l.advance();
        l_merged_count++;
    }

    // Finish remaining elements from right reader for this chunk
    ... // Same but for other side

    return !l_exhausted || !r_exhausted;
}
```

In Vorbereitung auf On-Disk Sortierung, alterniert die `merge` Methode für jeden Teilchunk zwischen zwei Zielbuffern. @merge-iteration-flowchart visualisiert diesen Vorgang.

```cpp
template <typename T>
void merge_sorter::merge(IMergeReader<T> &sorted_l, IMergeReader<T> &sorted_r, IMergeWriter<T> &writer_l, IMergeWriter<T> &writer_r, size_t chunk_size)
{
    bool write_to_left = true; // Start with writer_l for first chunk

    while (true)
    {
        IMergeWriter<T>& writer = write_to_left ? writer_l : writer_r;

        if (!merge_step(sorted_l, sorted_r, writer, chunk_size)) {
            break;
        }

        // Switch to the other writer for the next chunk
        write_to_left = !write_to_left;
    }
}
```

#figure(
    quote(block: true, attribution: "Übungsangabe", image("assets/merge_iteration_flowchart.png")),
    caption: [Jeweils Chunks der Größe $n$ von A und B, werden zu einem Chunk der Größe $2n$ zusammengeführt. Die neuen
Chunks werden abgechselnd in C und D geschrieben.]
) <merge-iteration-flowchart>

Diese Schritte werden in der `sort` Methode so oft wiederholt und zwischen Quell- und Zielbuffern alterniert, bis die gesamte Collection sortiert ist. Dabei ist hier der finale merge Schritt noch ausständig.

```cpp
template <typename T>
void merge_sorter::sort(
    std::unique_ptr<IMergeReader<T>> &reader_l, ... &reader_r,
    std::unique_ptr<IMergeWriter<T>> &writer_l, ... &writer_r,
    size_t total_size)
{
    size_t chunk_size = 1;
    while (true)
    {
        merge(*reader_l, *reader_r, *writer_l, *writer_r, chunk_size);
        chunk_size *= 2;
        if (chunk_size >= total_size)
        {
            // We have finished the last iteration and writers contain sorted data
            break;
        }

        // Prepare next chunk sized merge run
        // Swap reader and writer buffer roles
        ... 
    }
}
```

Zusätzlich gibt es noch die Hilfsmethode `split`, die alternierend die Elemente der unsortierten Collection in zwei Hälften schreibt.

Die `complete_sort` Methode
+ Splittet die unsortierte Collection in zwei Hälften
+ Führt den finalen Merge Schritt aus
+ Setzt den Quellbuffer auf den neuen sortierten Buffer
Visualisierung in @complete-sort-flowchart.

#figure(
    quote(block: true, attribution: "Übungsangabe", image("assets/complete_sort_flowchart.png")),
    caption: [Flowchart der `complete_sort` Methode: Aufteilung und wiederholtes zusammenführen von immer größeren Chunks.]
) <complete-sort-flowchart>


```cpp
template <typename T>
void merge_sorter::complete_sort(
    std::unique_ptr<IMergeReader<T>> &unsorted_source,
    std::unique_ptr<IMergeWriter<T>> buffer1, ... buffer4)
{
    // Split the unsorted source into two halves
    auto total_size = split(*unsorted_source, *buffer1, *buffer2);

    // Sort the two halves
    auto reader_l = buffer1->into_reader();
    auto reader_r = buffer2->into_reader();
    sort<T>(reader_l, reader_r, buffer3, buffer4, total_size);

    // Merge the two sorted halves into the soure
    auto sorted_l = buffer3->into_reader();
    auto sorted_r = buffer4->into_reader();
    auto sorted_full = unsorted_source->into_writer();
    merge_step<T>(*sorted_l, *sorted_r, *sorted_full, total_size);

    // re-seat the source => reset the cursor to 0
    unsorted_source = sorted_full->into_reader();
}
```

==== `stream_reader` Fixes

Die per Moodle offiziell bereitgestellte `stream_reader` Klasse zeigt unerwartetes Verhalten bei der Verwendung: Die interne Methode `stream_reader::next()` gibt am Ende des Filestreams einen leeren String `""` zurück. Das liegt an der Implementierung der `stream_reader::has_next()` Methode:

```cpp
template<typename T>
inline bool stream_reader<T>::has_next() {
  return buffer.has_value() || (m_in && m_in.good() && !m_in.eof());
}
```

Diese spiegelt nämlich die Implementierung des `std::istream::operator >>` wider und daher wird bei `stream_reader::next()` versucht, ein neues `T` auszugeben, was am Ende des Streams fehlschlägt und einen leer initialisierten String zurückgibt.

Die Lösung wird durch einen Buffer implementiert:

```cpp
template<typename T>
inline bool stream_reader<T>::has_next() {
    if (buffer) return true;
    T tmp;
    if (m_in >> tmp) {           // only true when a token was actually read
        buffer = std::move(tmp); // stash it for next()
        return true;
    }
    return false;
}

template<typename T>
inline std::optional<T> stream_reader<T>::next() {
    if (buffer) {
        auto v = std::move(*buffer);
        buffer.reset();
        return v;
    }
    T tmp;
    if (m_in >> tmp) return tmp; // succeed → return token
    return std::nullopt;         // fail → no token
}
```

==== File Handling <file-handling-task-01>

Dateien werden mittels der `stream_reader` Klasse gelesen. Folgender Code wird dazu im Projekt verwendet:

```cpp
std::ifstream read_file(file_name);
stream_reader<std::string> reader(read_file);
std::vector<std::string> data;
while (reader.has_next())
{
    data.push_back(reader.get());
}
read_file.close();
```

Dateien werden folgendermaßen mit neuer Datenfolge beschrieben:

```cpp
std::ofstream write_file(file_name);
for (const auto &entry : data)
{
    write_file << entry << " ";
}
write_file.close();
```

==== In-Memory Buffers

Die `InMemoryReader` und `InMemoryWriter` Klassen werden verwendet, um In-Memory Datenfolgen zu lesen und zu schreiben. Sie verwenden einen `std::vector<T>` als Datenquelle und schreiben in diesen. 

Die Reader haben einen Cursor, der auf das aktuelle Element zeigt und bei jedem Aufruf von `advance()` um eins erhöht wird. Bei einer Konvertierung zu einem Writer, wird der backing Vector geleert und per `shared_ptr` an den entstehenden Writer übergeben.

Wird der Writer zurück in einen Reader konvertiert, wird der Cursor auf 0 gesetzt und der backing Vector ebenfalls per `shared_ptr` an den entstehenden Reader übergeben.

```cpp
template<typename T>
class InMemoryReader : public IMergeReader<T> {
private:
    std::shared_ptr<std::vector<T>> _data;
    size_t _cursor;

public:
    explicit InMemoryReader(std::shared_ptr<std::vector<T>> data, size_t cursor = 0)
        : _data(data), _cursor(cursor) {}

    T get() override {
        if (is_exhausted()) {
            throw std::underflow_error("No more elements to read");
        }
        return (*_data)[_cursor];
    }

    bool advance() override {
        if (is_exhausted()) {
            return false;
        }
        _cursor++;
        // Return true if we can further advance once more
        return _cursor < _data->size();
    }

    bool is_exhausted() override {
        return _cursor >= _data->size();
    }

    std::unique_ptr<IMergeWriter<T>> into_writer() override {
        _data->clear();
        return std::make_unique<InMemoryWriter<T>>(_data);
    }
};

template<typename T>
class InMemoryWriter : public IMergeWriter<T> {
private:
    std::shared_ptr<std::vector<T>> _data;

public:
    explicit InMemoryWriter(std::shared_ptr<std::vector<T>> data = nullptr)
        : _data(data ? data : std::make_shared<std::vector<T>>()) {}

    bool append(const T& value) override {
        _data->push_back(value);
        return true;
    }

    std::unique_ptr<IMergeReader<T>> into_reader() override {
        return std::make_unique<InMemoryReader<T>>(_data, 0);
    }
};
```

==== Zusammenfügen

Die komplette In-Memory Sortierung wird in der `sort_vec_in_memory` Methode zusammengeführt. Die `sort_file_in_memory` Methode liest mit den in @file-handling-task-01 gezeigten Methoden die Datei in einen `std::vector<std::string>` und führt dann die Sortierung mit `sort_vec_in_memory` durch, bevor sie die sortierte Datenfolge wieder in die Datei schreibt.

```cpp
void merge_sorter::sort_vec_in_memory(std::vector<value_t> &data)
{
    std::unique_ptr<IMergeReader<value_t>> input_reader(
      std::make_unique<InMemoryReader<value_t>>(
        std::make_shared<std::vector<value_t>>(data)
      )
    );

    complete_sort<value_t>(
        input_reader,
        std::make_unique<InMemoryWriter<value_t>>(),
        ...);

    // Write the data from input_reader back to data vector
    data.clear();
    while (!input_reader->is_exhausted())
    {
        data.push_back(input_reader->get());
        input_reader->advance();
    }
}
```

== Testfälle <task-01-test-cases>

Der Standard-Testfall
+ *Arrange* - Legt eine Datei mit zufälligen Strings an\ ```cpp
    std::string filename = "test_file.txt";
    file_manipulator::fill_randomly(filename, array_length, string_length);
    ```
+ *Act* - Sortiert diese per `merge_sorter::sort_file_in_memory(...)`\ ```cpp
    merge_sorter sorter;
    sorter.sort_file_in_memory(filename);
    ```
+ *Assert* - Verifiziert das Ergebnis hinsichtlich der Sortierung.\ ```cpp
    std::ifstream file(filename);
    stream_reader<std::string> reader(file);

    std::string prev = reader.get();
    while (reader.has_next()) {
        std::string current = reader.get();
        ASSERT_LE(prev, current) << "Elements are not in sorted order";
        prev = current;
    }
    ```

=== Liste der abgedeckten Testfälle

Die folgenden Testfälle überprüfen die `merge_sorter` Implementierung:
- Default Testfall - _Parametrisiert_
  - String Länge 2, 10, 20
  - Array Länge 10, 200, 5000, 100000
- Leere Datei
- Datei mit nicht alphanumerischen Zeichen
- Verkehrt sortierte Datei
- Datei mit Duplikaten
- Datei mit unterschiedlich langen Strings

Zusätzlich gibt es kleine Testfälle, die sowohl die `random.h` als auch die `stream_reader.h` Implementierung testen.

== Ergebnisse

#figure(image("assets/2025-10-14_test_results.png"), caption: [Ergebnisse der Testfälle für Beispiel 1]) <task-01-test-results>

Alle Testfälle bestehen erfolgreich, wie in @task-01-test-results zu sehen ist. Es fällt auf, dass die Laufzeit bei den parametrisierten Testfällen stark unterschiedlich ausfällt. Das liegt an der stark unterschiedlichen Größe der zu sortierenden Datei.

#pagebreak(weak: true)
= Aufgabe: On Disk <chapter-task-02>

== Anforderungen

Wir müssen folgende Anforderungen aus der Angabe erfüllen:

#quote(block: true, attribution: "Übungsangabe")[
Ein paar Implementierungshinweise:
+ Die Klasse merge_sorter soll die in der Übung besprochene Klasse file_manipulator für alle Dateioperationen
  verwenden. Diese Dateioperationen könnten sein: eine Datei kopieren, eine Datei mit Zufallswerten füllen, eine
  Datei in mehrere Dateien aufsplitten, den Inhalt einer Datei ausgeben.
+ Die Klasse file_manipulator operiert auf ifstreams und ofstreams. Die einzigen erlaubten Operationen auf diese
  Streams sind nur `<<` und `>>`.
+ Die Klasse `stream_reader<value_type>` ...
]



== Lösungsidee

Prinzipiell müssen nur noch die `IMergeReader` und `IMergeWriter` Interfaces für On-Disk implementiert werden. Um auch die Anforderungen zu erfüllen, nutzen wir dazu eine `file_manipulator` Klasse, die folgende statische Methoden bereitstellt:

+ `fill_randomly(std::string const& file_name, size_type n = 100, size_type len = 4)`: Füllt eine Datei mit Zufallswerten.
+ `append(std::ofstream& file, std::string const& str)`: Fügt eine Zeichenkette an die Datei an.
+ `print(std::string const& src_file_name, std::ostream& out = std::cout)`: Gibt den Inhalt der Datei auf die Konsole aus.
+ `delete_file(std::string const& file_name)`: Löscht eine Datei.

=== Kleine Optimierung

In der `merge_sorter::sort(...)` Methode können wir die Iteration früher abbrechen, als wir in @chapter-task-01 implementiert haben. Dazu verwenden wir die `chunk_size > total_size / 2` Bedingung statt `chunk_size >= total_size`. Das liegt daran, dass wir nur den Inhalt der beiden Writer, die jeweils die Hälfte der Daten entsprechen, sortieren müssen. Der letzte Merge Schritt wird dann über den finalen `merge_sorter::merge(...)` Aufruf in `merge_sorter::complete_sort(...)` durchgeführt.

=== Validierung und Fehlerbehandlung

- Öffnen einer Datei wird geprüft; bei Fehlern werden aussagekräftige Exceptions geworfen (z. B. in `FileMergeReader`/`-Writer`, `merge_sorter::sort_file_in_memory`).
- Lesen/Writing geschieht nur über `<<`/`>>` (Anforderung 2); Formatfehler führen zu `has_next()==false` und werden nicht stillschweigend als leere Tokens interpretiert.

=== On-Disk Buffers

Die On-Disk Implementierung erfolgt über zwei Klassen in `file_merge_buffer.cpp`:

- `FileMergeReader<T>`: Implementiert `IMergeReader<T>` für das Lesen aus Dateien. Nutzt intern `stream_reader<T>` und `std::ifstream`. Beim Öffnen wird geprüft, ob die Datei existiert - andernfalls wird eine `std::runtime_error` Exception geworfen.

- `FileMergeWriter<T>`: Implementiert `IMergeWriter<T>` für das Schreiben in Dateien. Nutzt `std::ofstream` und die `file_manipulator::append()` Methode. Beim Erstellen wird die Zieldatei geleert, um saubere Ausgangsbedingungen zu schaffen.

Beide Klassen unterstützen die `into_reader()`/`into_writer()` Konvertierung, die für das Wechseln zwischen Lese- und Schreibmodus auf derselben Datei erforderlich ist. Dies ermöglicht es, dass die Buffer-Dateien sowohl als Eingabe als auch als Ausgabe in verschiedenen Phasen des Merge-Sort-Algorithmus verwendet werden können.

```cpp
template<typename T>
class FileMergeReader : public IMergeReader<T> {
private:
    std::string _filename;
    std::unique_ptr<stream_reader<T>> _reader;
    std::unique_ptr<std::ifstream> _file;

public:
    T get() override {
        if (is_exhausted()) {
            throw std::underflow_error("No more elements to read");
        }
        return _reader->peek();
    }

    bool advance() override {
        if (is_exhausted()) {
            return false;
        }
        _reader->get(); // consume current element
        return _reader->has_next();
    }

    bool is_exhausted() override {
        return !_reader->has_next();
    }
};

template<typename T>
class FileMergeWriter : public IMergeWriter<T> {
private:
    std::string _filename;
    std::unique_ptr<std::ofstream> _file;
public:
    bool append(const T& value) override {
        if (!_file->is_open()) {
            return false;
        }
        file_manipulator::append(*_file, value);
        return true;
    }
};
```

== Tests

Wir setzen auch in diesem Beispiel auf den etablierten Standard-Testfall aus @task-01-test-cases. Dabei werden die Testfälle für On-Disk Sortierung hinzugefügt. Wir erzeugen beim Testfall $"string_length" = 100$ und $"array_length = 1000000"$ eine Datei mit knapp unter $100 "MB"$ Daten. Mit dem zusätzlichen Speichervorbauch der vier Buffer sind das insgesamt $\~300 "MB"$ Speicher.

Zusätzlich testen wir die Fehlerbehandlung für nicht vorhandene Dateien:

```cpp
TEST(MergeSortTest, TestNonexistentFileOnDiskThrows) {
    // Arrange
    std::string filename = "__no_such_file_exists__.txt";
    remove(filename.c_str());

    // Act + Assert
    merge_sorter sorter;
    ASSERT_THROW(sorter.sort_file_on_disk(filename), std::runtime_error);
}
```

== Ergebnisse

=== Testergebnisse

Die Tests verifizieren die korrekte Sortierung und robuste Fehlerbehandlung. Auch hier zeigt @task-02-test-results, dass die Laufzeit entsprechend mit der Größe der zu sortierenden Datenfolge wächst. Der Testlauf, der die $100 "MB"$ Datei sortiert, dauert \~10 Minuten. Die Struktur mit `IMergeReader`/`IMergeWriter` ermöglicht identische Kernlogik für In-Memory und On-Disk, was die Wartbarkeit verbessert.

#figure(
    image("assets/2025-10-16_test_results_task-02.png"),
    caption: [Ergebnisse der Testfälle für Beispiel 2]
) <task-02-test-results>

=== Benchmark

#let benchmark-data = json("assets/benchmarks_results.json")

Final wurde ein gegenüberstellender Benchmark durchgeführt, der die Laufzeit $T(n)$ und den Speicherbedarf $S(n)$ des Merge Sort Algorithmus für In-Memory und On-Disk vergleicht. Die Implementierung dazu wurde in Rust geschrieben, da ich hierfür mit dem Benchmarking Ecosystem vertrauter bin. Die Ergebnisse sind in @task-02-benchmarks-results zu sehen. Sie zeigen, dass die Laufzeit für On-Disk deutlich höher ist als für In-Memory, da der Algorithmus auf der Festplatte arbeiten muss. Der RAM-Bedarf fällt bei gleicher Einzelelementlänge $="len"$ geringer aus, bleibt aber auch nicht konstant.

Der Benchmark wurde auf folgender Hardware durchgeführt:
#figure(
    table(
        columns: (auto, auto),
        [*CPU*], [#benchmark-data.system.cpu_brand \@ #calc.round(benchmark-data.system.cpu_frequency_mhz / 1000, digits: 2) $"GHz"$],
        [*Logical Cores*], [#benchmark-data.system.cpu_cores_logical],
        [*RAM*], [#calc.round(benchmark-data.system.total_memory_kb / 1024 / 1024, digits: 2) $"GB"$],
        [*Arch*], [#benchmark-data.system.arch],
        [*Kernel*], [#benchmark-data.system.kernel_version],
        [*OS*], [#benchmark-data.system.long_os_version],
    ),
    caption: [Hardware-Spezifikationen des Benchmark-Systems]
)

#figure(
    box(stroke: black, inset: 10pt, 
        visualize_mergesort_benchmarks_results(benchmark-data)
    ),
    caption: [Benchmark Ergebnisse des Merge Sort Algorithmus auf einer #benchmark-data.system.cpu_brand CPU]
) <task-02-benchmarks-results>


