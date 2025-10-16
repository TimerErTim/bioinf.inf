#import "../common/template.typ": documentation-template
#import "../common/components.typ": *

#show: documentation-template.with(title: "SWE3 - Übung 2", semester-term: "WS 2025/26", author: "Tim Peko", aufwand-in-h: 12)

#import "../common/visualization/mergesort.typ": *

= Aufgabe: Merge Sort <chapter-task-01>

== Lösungsidee

=== Merge Sort

Der Merge Sort Algorithmus funktioniert, indem wir immer sortierte Subarrays zu einem sortierten Superarray zusammenfügen. Dazu machen wir uns die sortierte Eigenschaft zu Nutze und fügen immer das kleinste Element des linken und rechten Subarrays zu dem Superarray hinzu. Dieser Vorgang wird in @merge-step-visualization demonstiert. Wichtig ist hierbei, dass ein zusätzlicher Buffer benötigt wird, der das gemerged Superarray speichert.

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

Es exisitert eine `main.cpp` Datei, die allerdings nur eine Dummy Main Funktion enthält. Sie dient lediglich dazu, das Projekt kompilieren zu können. Alternativ könnte in Visual Studio auch der Projekttyp auf `Static Library` gesetzt werden. Das wird hier zur leichteren Kompabilität nicht gemacht.

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

In Vorbereitung auf On-Disk Sortierung, alterniert die `merge` Methode für jeden Teilchunk zwischen zwei Zielbuffern.

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

Diese Schritter werden in der `sort` Methode so oft wiederholt und zwischen Quell- und Zielbuffern alterniert, bis die gesamte Collection sortiert ist. Dabei ist hier der finale merge Schritt noch ausständig.

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
void merge_sorter::sort_vec_in_memory(std::vector<std::string> &data)
{
    std::unique_ptr<IMergeReader<std::string>> input_reader(
      std::make_unique<InMemoryReader<std::string>>(
        std::make_shared<std::vector<std::string>>(data)
      )
    );

    complete_sort<std::string>(
        input_reader,
        std::make_unique<InMemoryWriter<std::string>>(),
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

== Testfälle

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
- Verkehrt sortierte Datei
- Datei mit Duplikaten
- Datei mit unterschiedlich langen Strings

Zusätzlich gibt es kleine Testfälle, die sowohl die `random.h` als auch die `stream_reader.h` Implementierung testen.

== Ergebnisse

#figure(image("assets/2025-10-14_test_results.png"), caption: [Ergebnisse der Testfälle für Beispiel 1]) <task-01-test-results>

Alle Testfälle bestehen erfolgreich, wie in @task-01-test-results zu sehen ist. Es fällt auf, dass die Laufzeit bei den parametrisierten Testfällen stark unterschiedlich ausfällt. Das liegt an der stark unterschiedlichen Größe der zu sortierenden Datei.

= Aufgabe: On Disk <chapter-task-02>

== Lösungsidee

== Ergebnisse

== Testfälle

