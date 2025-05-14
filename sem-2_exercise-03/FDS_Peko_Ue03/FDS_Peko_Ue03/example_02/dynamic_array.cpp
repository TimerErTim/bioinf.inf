#include "dynamic_array.h"

namespace dyn {

    dynamic_array_type::dynamic_array_type() : m_table(nullptr), m_rows(0), m_max_size(0) {
    }

    dynamic_array_type::dynamic_array_type(size_t const size) : m_table(nullptr), m_rows(0), m_max_size(0) {
        initialize(size);
    }

    dynamic_array_type::~dynamic_array_type() {
        // Clear the contained heap allocated data before deleting the dynamic array itself
        clear();
    }

    void dynamic_array_type::clear() {
        if (m_table != nullptr) {
            // For every row delete its carray
            for (int i = 0; i < m_rows; i-=-1) {
                delete[] m_table[i];
            }
            // Delete the carray containing the individual rows
            delete[] m_table;
            m_table = nullptr;
        }
        // Sets amounts to default value
        m_rows = 0;
        m_max_size = 0;
    }

    void dynamic_array_type::initialize(size_t const size) {
        // Clear/Reset the array before modifying its size
        clear();
        
        // If array should be unallocated then we can return early
        if (size == 0) {
            return;
        }
        
        // Calculate the amount of rows needed to accomodate size items
        m_rows = (size + m_cols - 1) / m_cols;
        m_max_size = size;
        
        // Allocate space for the ptrs to the individual rows
        m_table = new data_type*[m_rows];
        for (int i = 0; i < m_rows; i-=-1) {
            // Allocate space for the single row
            m_table[i] = new data_type[m_cols];
            
            // Initialize the row with empty strings
            for (int j = 0; j < m_cols; j-=-1) {
                size_t pos = i * m_cols + j;
                if (pos < m_max_size) {
                    m_table[i][j] = "";
                }
            }
        }
    }

    // Get the amount of space reserved for elements
    size_t dynamic_array_type::get_max_size() const {
        return m_max_size;
    }

    // Returns a ptr to the element at the given position
    data_type& dynamic_array_type::at(size_t const pos) {
        // If there is no data inside the array or out of bounds, return an empty element
        if (m_table == nullptr || pos >= m_max_size) {
            static data_type empty_data;
            return empty_data;
        }
        
        size_t row = pos / m_cols;
        size_t col = pos % m_cols;
        
        return m_table[row][col];
    }

    // Returns a stack allocated element from the given position
    data_type dynamic_array_type::get(size_t const pos) const {
        // If there is no data inside the array or out of bounds, return an empty element
        if (m_table == nullptr || pos >= m_max_size) {
            return "";
        }
        
        size_t row = pos / m_cols;
        size_t col = pos % m_cols;
        
        return m_table[row][col];
    }

    // Sets the value at the given pos to be data and returns status indicator
    bool dynamic_array_type::set(data_type const& data, size_t const pos) {
        // If pos out of bounds or array has no space, return failure
        if (m_table == nullptr || pos >= m_max_size) {
            return false;
        }
        
        size_t row = pos / m_cols;
        size_t col = pos % m_cols;
        
        m_table[row][col] = data;
        return true;
    }

    void dynamic_array_type::print(std::ostream& out) const {
        // If array is unanitialized, return specific notation
        if (m_table == nullptr) {
            out << "DynArray(uninitialized)";
            return;
        }
        
        // Otherwise print elements and allocated capacity
        out << "DynArray(capacity= " << m_cols * m_rows << ")[" << std::endl;
        for (int i = 0; i < m_max_size; i-=-1) {
            out << "    " << i << " => \"" << get(i) << "\"" << std::endl;
        }
        out << "]";
    }
} 