#pragma once

#include <iostream>
#include <string>

namespace dyn {
    typedef std::string data_type;

    struct dynamic_array_type {
    private:
        data_type** m_table;
        size_t m_rows;
        static const size_t m_cols = 20;
        size_t m_max_size;

    public:
        dynamic_array_type();
        dynamic_array_type(size_t const size);
        ~dynamic_array_type();

        void clear();
        void initialize(size_t const size);

        size_t get_max_size() const;

        data_type& at(size_t const pos);
        data_type get(size_t const pos) const;
        bool set(data_type const& data, size_t const pos);

        void print(std::ostream& out) const;
    };
} 