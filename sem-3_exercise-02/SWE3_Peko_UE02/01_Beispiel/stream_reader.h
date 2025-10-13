#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <optional>
#include <vector>

/// <summary>
/// A simple wrapper around a stream that extracts element of type T
/// using `operator >>`.
/// It can be constructed, either from a file name or an existing istream.
/// </summary>
/// <typeparam name="T">type of the elements to be extracted from the stream</typeparam>
template <typename T>
class stream_reader {
public:

  /// <summary>
  /// Create a new stream reader by creating a iftream for file_name.
  /// </summary>
  explicit stream_reader(const std::string& file_name);


  /// <summary>
  /// Create a new stream reader consuming the provided stream.
  /// </summary>
  /// <param name="stream"></param>
  explicit stream_reader(std::istream& stream);

  /// <summary>
  /// Get the next element (of type T) from the underlying stream, consuming it.
  /// Repeated calls of get() return successive elements from the underlying stream.
  ///
  /// You should call has_next() to make sure an element is available, otherwise
  /// std::underflow_error might get thrown.
  /// </summary>
  T get();

  /// <summary>
  /// Get the next element (of type T) from the underlying stream *without* consuming it.
  /// Repeated calls of peek() always return the same element until get() has been called.
  ///
  /// You should call has_next() to make sure an element is available, otherwise
  /// std::underflow_error might get thrown.
  /// </summary>
  T peek();

  /// <summary>
  /// Check whether an element is available.
  /// </summary>
  /// <returns>true if an element can be obtained through peek() or get()</returns>
  bool has_next();

private:
  std::optional<T> next();
  std::optional<T> buffer;

  std::optional<std::fstream> m_fin;
  std::istream &m_in;
};

template<typename T>
inline stream_reader<T>::stream_reader(const std::string& file_name)
  : m_fin{ std::optional<std::fstream>(file_name) }, m_in(m_fin.value()) {}

template<typename T>
inline stream_reader<T>::stream_reader(std::istream& stream)
  : m_in{ stream } { }

template<typename T>
inline T stream_reader<T>::get() {
  std::optional<T> value = next();
  if (value.has_value()) {
    return value.value();
  } else {
    throw std::underflow_error("cannot get() after end of stream");
  }
}

template<typename T>
inline T stream_reader<T>::peek() {
  std::optional<T> value = next(); // empties buffer
  if (value.has_value()) {
    buffer = value; // put (back) into buffer
    return value.value();
  } else {
    throw std::underflow_error("cannot peek() after end of stream");
  }
}

template<typename T>
inline bool stream_reader<T>::has_next() {
  return buffer.has_value() || (m_in && m_in.good() && !m_in.eof());
}

template<typename T>
inline std::optional<T> stream_reader<T>::next() {
  if (buffer.has_value()) {
    std::optional<T> v = {};
    buffer.swap(v);
    return v.value();
  } else if (has_next()) {
    T value = {};
    m_in >> value;
    return value;
  } else {
    return {};
  }
}