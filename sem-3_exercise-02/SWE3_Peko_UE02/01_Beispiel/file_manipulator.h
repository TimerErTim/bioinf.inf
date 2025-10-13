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
  /// Copy the contents of one file to another. Please not that this uses the value_type to get
  /// successive elements and then copies them over.
  /// </summary>
  static void copy(std::string const& destination_file_name, std::string const& source_file_name);

  /// <summary>
  /// Create a file, filled n strings of size len. If the file exists, it will be overwritten.
  /// </summary>
  /// <param name="file_name">name of the destination file</param>
  /// <param name="n">number of elements</param>
  /// <param name="len">length of each element</param>
  static void fill_randomly(std::string const& file_name, size_type n = 100, size_type len = 4);

  /// <summary>
  /// Create a file with the given contents. If the file exists, it will be overwritten.
  /// </summary>
  /// <param name="file_name">name of the file</param>
  /// <param name="str">new contents of the file</param>
  static void write(std::string const& file_name, std::string const& str);

  /// <summary>
  /// Extract elements of type value_type from src file and distribute them in a round-robin
  /// fashion over the given destination files. Destinations files will be created or overwritten.
  /// </summary>
  /// <param name="src_file_name">name of the source file</param>
  /// <param name="destination_file_names">vector with names of the target files</param>
  /// <returns></returns>
  static size_type split(std::string const& src_file_name, std::vector<value_type> const& destination_file_names);

  /// <summary>
  /// Read contents of type value_type from a file and print it to a stream (default std::cout).
  /// </summary>
  /// <param name="src_file_name">name of the file to print</param>
  /// <param name="out">destination stream (default std::cout)</param>
  /// <returns>the destination stream (out)</returns>
  static std::ostream& print(std::string const& src_file_name, std::ostream& out = std::cout);
};