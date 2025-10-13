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

    /// Converts this reader into an IMergeWriter, allowing the read data to be written.
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

class merge_sorter {
public:
    using value_t = std::string;
    using size_t = std::size_t;

    void sort_file_in_memory(const std::string& file_name);
    void sort_vec_in_memory(std::vector<std::string>& data);
private:
    template<typename T>
    void sort(
        std::unique_ptr<IMergeReader<T>>& reader_l,
        std::unique_ptr<IMergeReader<T>>& reader_r,
        std::unique_ptr<IMergeWriter<T>>& writer_l,
        std::unique_ptr<IMergeWriter<T>>& writer_r,
        size_t total_size
    );

    template<typename T>
    void complete_sort(
        std::unique_ptr<IMergeReader<T>>& unsorted_source,
        std::unique_ptr<IMergeWriter<T>> buffer1,
        std::unique_ptr<IMergeWriter<T>> buffer2,
        std::unique_ptr<IMergeWriter<T>> buffer3,
        std::unique_ptr<IMergeWriter<T>> buffer4
    );

    template<typename T>
    void merge(IMergeReader<T>& sorted_l, IMergeReader<T>& sorted_r, IMergeWriter<T>& writer_l, IMergeWriter<T>& writer_r, size_t chunk_size);

    /// Returns the number of elements read from the reader.
    template<typename T>
    long long split(IMergeReader<T>& reader, IMergeWriter<T>& writer_l, IMergeWriter<T>& writer_r);

    template<typename T>
    bool merge_step(IMergeReader<T>& reader_l, IMergeReader<T>& reader_r, IMergeWriter<T>& writer, size_t chunk_size);
};