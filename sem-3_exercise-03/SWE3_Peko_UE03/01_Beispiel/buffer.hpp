#pragma once

#include <iostream>

class buffer_bad {
public:
  // constructor
  buffer_bad(size_t size) : m_size(size), m_data(new int[size]) {
    // m_size = size
    // m_data = new int[size]
  }

  // destructor
  ~buffer_bad() { delete[] m_data; }

  int &operator[](size_t index) { return m_data[index]; }

  // add element
  void add(int element);

  // get element
  int get(size_t index);

private:
  // m_size = 0, default initialization
  size_t m_size{};
  // without {}, the var is uninit and may contain anything
  int *m_data{};
};

class buffer_good {
public:
  // constructor
  buffer_good(size_t size) : m_size(size), m_data(new int[size]) {
    std::cout << "buffer_good constructor called" << std::endl;
    // m_size = size
    // m_data = new int[size]
  }

  // copy constructor
  buffer_good(const buffer_good& other) : m_size(other.m_size), m_data(new int[other.m_size]) {
    std::cout << "buffer_good copy constructor called" << std::endl;
    for (size_t i = 0; i < m_size; ++i) {
      m_data[i] = other.m_data[i];
    }
  }

  // move constructor
  buffer_good(buffer_good&& other) noexcept : m_size(other.m_size), m_data(other.m_data) {
    std::cout << "buffer_good move constructor called" << std::endl;
    other.m_size = 0;
    other.m_data = nullptr;
  }

  // assignment operator
  buffer_good& operator=(buffer_good other) noexcept {
    std::cout << "buffer_good assignment operator called" << std::endl;
    std::swap(m_size, other.m_size);
    std::swap(m_data, other.m_data);
    return *this;
  }

  // destructor
  ~buffer_good() {
    std::cout << "buffer_good destructor called" << std::endl;
    delete[] m_data;
  }

  int &operator[](size_t index) { return m_data[index]; }

  // add element
  void add(int element);

  // get element
  int get(size_t index);

private:
  // m_size = 0, default initialization
  size_t m_size{};
  // without {}, the var is uninit and may contain anything
  int *m_data{};
};
