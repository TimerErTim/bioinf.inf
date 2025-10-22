#pragma once

#include <vector>
#include <string>
#include <memory>

// Forward declarations to break circular dependency between interfaces
template<typename T> class IMergeReader;
template<typename T> class IMergeWriter;

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

/// <summary>
/// Orchestrates merge sort over pluggable readers/writers (in-memory or on-disk).
/// Uses iterative two-way merging with doubling run-size.
/// </summary>
class merge_sorter {
public:
    using value_t = std::string;
    using size_t = std::size_t;

    /// <summary>
    /// Read tokens from a file into memory, sort them, and write back.
    /// </summary>
    /// <param name="file_name">Path to the file to be sorted.</param>
    void sort_file_in_memory(const std::string& file_name);
    /// <summary>
    /// Sort a vector of tokens in-memory using four in-memory buffers.
    /// </summary>
    /// <param name="data">Container to sort in-place.</param>
    void sort_vec_in_memory(std::vector<value_t>& data);
    /// <summary>
    /// Sort tokens stored in a file using on-disk buffers only.
    /// </summary>
    /// <param name="file_name">Path to the file to be sorted.</param>
    void sort_file_on_disk(const std::string& file_name);
private:
    /// <summary>
    /// Perform iterative merge passes on two sorted readers, alternating output to two writers.
    /// Doubles <c>chunk_size</c> each pass and swaps reader/writer roles in between.
    /// </summary>
    /// <typeparam name="T">Element type.</typeparam>
    /// <param name="reader_l">Left input reader.</param>
    /// <param name="reader_r">Right input reader.</param>
    /// <param name="writer_l">Left output writer.</param>
    /// <param name="writer_r">Right output writer.</param>
    /// <param name="total_size">Total number of elements across both inputs.</param>
    template<typename T>
    void sort(
        std::unique_ptr<IMergeReader<T>>& reader_l,
        std::unique_ptr<IMergeReader<T>>& reader_r,
        std::unique_ptr<IMergeWriter<T>>& writer_l,
        std::unique_ptr<IMergeWriter<T>>& writer_r,
        size_t total_size
    );

    /// <summary>
    /// Complete sort pipeline using four buffers: split, iterative sort, final merge back to source.
    /// </summary>
    /// <typeparam name="T">Element type.</typeparam>
    /// <param name="unsorted_source">Source reader to be sorted (re-seated to sorted data on return).</param>
    /// <param name="buffer1">Temporary writer buffer.</param>
    /// <param name="buffer2">Temporary writer buffer.</param>
    /// <param name="buffer3">Temporary writer buffer.</param>
    /// <param name="buffer4">Temporary writer buffer.</param>
    template<typename T>
    void complete_sort(
        std::unique_ptr<IMergeReader<T>>& unsorted_source,
        std::unique_ptr<IMergeWriter<T>> buffer1,
        std::unique_ptr<IMergeWriter<T>> buffer2,
        std::unique_ptr<IMergeWriter<T>> buffer3,
        std::unique_ptr<IMergeWriter<T>> buffer4
    );

    /// <summary>
    /// Alternating run merge: merge runs of size <c>chunk_size</c> from two sorted inputs into
    /// two writers by alternating target after each merged run.
    /// </summary>
    /// <typeparam name="T">Element type.</typeparam>
    /// <param name="sorted_l">Left sorted reader.</param>
    /// <param name="sorted_r">Right sorted reader.</param>
    /// <param name="writer_l">Left writer target.</param>
    /// <param name="writer_r">Right writer target.</param>
    /// <param name="chunk_size">Max elements per run taken from each side.</param>
    template<typename T>
    void merge(IMergeReader<T>& sorted_l, IMergeReader<T>& sorted_r, IMergeWriter<T>& writer_l, IMergeWriter<T>& writer_r, size_t chunk_size);

    /// Returns the number of elements read from the reader.
    /// <summary>
    /// Split input elements alternately into two writers (round-robin: L, R, L, R, ...).
    /// </summary>
    /// <typeparam name="T">Element type.</typeparam>
    /// <param name="reader">Source reader.</param>
    /// <param name="writer_l">Left writer.</param>
    /// <param name="writer_r">Right writer.</param>
    /// <returns>Total number of elements observed.</returns>
    template<typename T>
    long long split(IMergeReader<T>& reader, IMergeWriter<T>& writer_l, IMergeWriter<T>& writer_r);

    /// <summary>
    /// Merge a single run from each input reader into a writer.
    /// Consumes up to <c>chunk_size</c> per side or until exhaustion.
    /// </summary>
    /// <typeparam name="T">Element type.</typeparam>
    /// <param name="reader_l">Left input reader.</param>
    /// <param name="reader_r">Right input reader.</param>
    /// <param name="writer">Destination writer.</param>
    /// <param name="chunk_size">Max elements to take from each reader.</param>
    /// <returns>True if any reader still has elements; otherwise false.</returns>
    template<typename T>
    bool merge_step(IMergeReader<T>& reader_l, IMergeReader<T>& reader_r, IMergeWriter<T>& writer, size_t chunk_size);
};