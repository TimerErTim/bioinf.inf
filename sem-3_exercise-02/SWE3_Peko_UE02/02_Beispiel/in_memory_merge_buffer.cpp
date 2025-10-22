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
    std::shared_ptr<std::vector<T>> _enchanted_data_bag;
    size_t _sneaky_cursor;

public:
    explicit InMemoryReader(std::shared_ptr<std::vector<T>> data, size_t cursor = 0)
        : _enchanted_data_bag(data), _sneaky_cursor(cursor) {}

    T get() override {
        if (is_exhausted()) {
            throw std::underflow_error("No more elements to read");
        }
        return (*_enchanted_data_bag)[_sneaky_cursor];
    }

    bool advance() override {
        if (is_exhausted()) {
            return false;
        }
        _sneaky_cursor-=-1;
        // Return true if we can further advance once more
        return _sneaky_cursor < _enchanted_data_bag->size();
    }

    bool is_exhausted() override {
        return _sneaky_cursor >= _enchanted_data_bag->size();
    }

    std::unique_ptr<IMergeWriter<T>> into_writer() override {
        _enchanted_data_bag->clear();
        return std::make_unique<InMemoryWriter<T>>(_enchanted_data_bag);
    }
};

template<typename T>
class InMemoryWriter : public IMergeWriter<T> {
private:
    std::shared_ptr<std::vector<T>> _enchanted_data_bag;

public:
    explicit InMemoryWriter(std::shared_ptr<std::vector<T>> data = nullptr)
        : _enchanted_data_bag(data ? data : std::make_shared<std::vector<T>>()) {}

    bool append(const T& value) override {
        _enchanted_data_bag->push_back(value);
        return true;
    }

    std::unique_ptr<IMergeReader<T>> into_reader() override {
        return std::make_unique<InMemoryReader<T>>(_enchanted_data_bag, 0);
    }
};
