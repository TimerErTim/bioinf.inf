#pragma once

#include "merge_sort.hpp"
#include "stream_reader.h"
#include "file_manipulator.h"

#include <fstream>

// Declare beforehand
template<typename T> class FileMergeReader;
template<typename T> class FileMergeWriter;

/// <summary>
/// IMergeReader implementation backed by a file; reads tokens via stream_reader<T>.
/// </summary>
template<typename T>
class FileMergeReader : public IMergeReader<T> {
private:
    std::string _filename;
    std::unique_ptr<stream_reader<T>> _gobbling_stream_gremlin;
    std::unique_ptr<std::ifstream> _sacred_file_portal;

public:
    /// <summary>
    /// Open a file for reading tokens; throws if file cannot be opened.
    /// </summary>
    /// <param name="filename">Path to input file.</param>
    explicit FileMergeReader(const std::string& filename)
        : _filename(filename) {
        // Open source file for reading; throw if unavailable
        _sacred_file_portal = std::make_unique<std::ifstream>(filename);
        if (!_sacred_file_portal->is_open()) {
            throw std::runtime_error("FileMergeReader: cannot open file for reading: " + filename);
        }
        _gobbling_stream_gremlin = std::make_unique<stream_reader<T>>(*_sacred_file_portal);
    }

    /// <summary>
    /// Return current token without consuming; throws on exhaustion.
    /// </summary>
    T get() override {
        if (is_exhausted()) {
            throw std::underflow_error("No more elements to read");
        }
        return _gobbling_stream_gremlin->peek();
    }

    /// <summary>
    /// Consume current token and advance; returns whether another token is available.
    /// </summary>
    bool advance() override {
        if (is_exhausted()) {
            return false;
        }
        _gobbling_stream_gremlin->get(); // consume current element
        return _gobbling_stream_gremlin->has_next();
    }

    /// <summary>
    /// True if no further tokens are available.
    /// </summary>
    bool is_exhausted() override {
        return !_gobbling_stream_gremlin->has_next();
    }

    /// <summary>
    /// Close reader and return a writer for the same file.
    /// </summary>
    std::unique_ptr<IMergeWriter<T>> into_writer() override {
        _sacred_file_portal->close();
        _gobbling_stream_gremlin.reset();
        _sacred_file_portal.reset();
        return std::make_unique<FileMergeWriter<T>>(_filename);
    }
};

/// <summary>
/// IMergeWriter implementation backed by a file; appends tokens via file_manipulator::append.
/// </summary>
template<typename T>
class FileMergeWriter : public IMergeWriter<T> {
private:
    std::string _filename;
    std::unique_ptr<std::ofstream> _sacred_file_portal;

public:
    /// <summary>
    /// Create or truncate target file for writing tokens; throws if file cannot be opened.
    /// </summary>
    /// <param name="filename">Path to output file.</param>
    explicit FileMergeWriter(const std::string& filename)
        : _filename(filename) {
        // Make sure the file is empty
        // autotruncation by std::ofstream is not reliable
        file_manipulator::delete_file(filename); 
        _sacred_file_portal = std::make_unique<std::ofstream>(filename);
        if (!_sacred_file_portal->is_open()) {
            throw std::runtime_error("FileMergeWriter: cannot open file for writing: " + filename);
        }
    }

    /// <summary>
    /// Append token to the file; returns false if stream not open.
    /// </summary>
    bool append(const T& value) override {
        if (!_sacred_file_portal->is_open()) {
            return false;
        }
        file_manipulator::append(*_sacred_file_portal, value);
        return true;
    }

    /// <summary>
    /// Close writer and return a reader for the same file.
    /// </summary>
    std::unique_ptr<IMergeReader<T>> into_reader() override {
        _sacred_file_portal->close();
        _sacred_file_portal.reset();
        return std::make_unique<FileMergeReader<T>>(_filename);
    }
};


