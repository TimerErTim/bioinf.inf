#pragma once

#include "merge_sort.hpp"

#include <vector>
#include <memory>
#include <stdexcept>

// Forward declarations to resolve circular dependency
template<typename T> class InMemoryReader;
template<typename T> class InMemoryWriter;


template<typename T>
class InMemoryReader : public IMergeReader<T> {
private:
    std::shared_ptr<std::vector<T>> _data;
    size_t _cursor;

public:
    explicit InMemoryReader(std::shared_ptr<std::vector<T>> data, size_t cursor = 0)
        : _data(std::move(data)), _cursor(cursor) {}

    T peek() override {
        if (_cursor >= _data->size()) {
            throw std::underflow_error("No more elements to read");
        }
        return (*_data)[_cursor];
    }

    bool advance() override {
        if (_cursor >= _data->size()) {
            return false;
        }
        _cursor++;
        return _cursor < _data->size();
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
