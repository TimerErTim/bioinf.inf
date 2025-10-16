// @@ -0,0 +1,57 @@
#pragma once

#include <string>
#include <iostream>
#include <vector>

class file_manipulator {

public:
  using size_type = std::size_t;

  /// <summary>
  /// type of elements handled in the files
  /// </summary>
  using value_type = std::string;

  /// <summary>
  /// Delete a file.
  /// </summary>
  static void delete_file(std::string const& file_name);

  /// <summary>
  /// Create a file, filled n strings of size len. If the file exists, it will be overwritten.
  /// </summary>
  /// <param name="file_name">name of the destination file</param>
  /// <param name="n">number of elements</param>
  /// <param name="len">length of each element</param>
  static void fill_randomly(std::string const& file_name, size_type n = 100, size_type len = 4);

  /// <summary>
  /// Append a string to a file.
  /// </summary>
  /// <param name="file_name">name of the file</param>
  /// <param name="str">string to append to the file</param>
  static void append(std::ofstream& file, std::string const& str);

  /// <summary>
  /// Read contents of type value_type from a file and print it to a stream (default std::cout).
  /// </summary>
  /// <param name="src_file_name">name of the file to print</param>
  /// <param name="out">destination stream (default std::cout)</param>
  /// <returns>the destination stream (out)</returns>
  static std::ostream& print(std::string const& src_file_name, std::ostream& out = std::cout);
};