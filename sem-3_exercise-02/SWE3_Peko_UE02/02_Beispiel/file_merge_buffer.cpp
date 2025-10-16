#pragma once

#include "merge_sort.hpp"
#include "stream_reader.h"
#include "file_manipulator.h"

#include <fstream>

// Declare beforehand
template<typename T> class FileMergeReader;
template<typename T> class FileMergeWriter;

template<typename T>
class FileMergeReader : public IMergeReader<T> {
private:
    std::string _filename;
    std::unique_ptr<stream_reader<T>> _reader;
    std::unique_ptr<std::ifstream> _file;

public:
    explicit FileMergeReader(const std::string& filename)
        : _filename(filename) {
        // Open source file for reading; throw if unavailable
        _file = std::make_unique<std::ifstream>(filename);
        if (!_file->is_open()) {
            throw std::runtime_error("FileMergeReader: cannot open file for reading: " + filename);
        }
        _reader = std::make_unique<stream_reader<T>>(*_file);
    }

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

    std::unique_ptr<IMergeWriter<T>> into_writer() override {
        _file->close();
        _reader.reset();
        _file.reset();
        return std::make_unique<FileMergeWriter<T>>(_filename);
    }
};

template<typename T>
class FileMergeWriter : public IMergeWriter<T> {
private:
    std::string _filename;
    std::unique_ptr<std::ofstream> _file;

public:
    explicit FileMergeWriter(const std::string& filename)
        : _filename(filename) {
        // Make sure the file is empty
        // autotruncation by std::ofstream is not reliable
        file_manipulator::delete_file(filename); 
        _file = std::make_unique<std::ofstream>(filename);
        if (!_file->is_open()) {
            throw std::runtime_error("FileMergeWriter: cannot open file for writing: " + filename);
        }
    }

    bool append(const T& value) override {
        if (!_file->is_open()) {
            return false;
        }
        file_manipulator::append(*_file, value);
        return true;
    }

    std::unique_ptr<IMergeReader<T>> into_reader() override {
        _file->close();
        _file.reset();
        return std::make_unique<FileMergeReader<T>>(_filename);
    }
};


