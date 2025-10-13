#include "file_manipulator.h"

#include <fstream>
#include "random.h"

using fm = file_manipulator;

void fm::copy(std::string const& destination_file_name, std::string const& source_file_name) {

  std::ifstream source_stream(source_file_name);
  std::ofstream destination_stream(destination_file_name);
  value_type    value;

  while ((source_stream >> value) && destination_stream) {
    destination_stream << value << ' ';
  }
}


void fm::fill_randomly(std::string const& file_name, size_type n, size_type len) {
    std::ofstream out(file_name);

    while (out && (n-- > 0)) {
        out << random_string(len) << ' ';
    }
    out.close();
}


void fm::write(std::string const& file_name, std::string const& str) {
  std::ofstream out(file_name);
  out << str;
}


fm::size_type fm::split(
  std::string const& src_file_name,
  std::vector<std::string> const& destination_file_names) {

  std::ifstream                 source_stream(src_file_name);
  std::vector <std::ofstream*> destination_streams;

  for (const auto& name : destination_file_names) {
    destination_streams.push_back(new std::ofstream(name));
  }

  size_type  n(0);
  value_type value;

  while (source_stream >> value) {
    std::ofstream& o = *(destination_streams[n % destination_streams.size()]);
    if (o) {
      o << value << ' ';
      ++n;
    }
  }

  for (auto& destination_stream : destination_streams) {
    delete destination_stream;
  }

  return n;
}


std::ostream& fm::print(std::string const& src_file_name, std::ostream& out) {
  std::ifstream in(src_file_name);
  value_type    v;

  while (in >> v) {
    out << v << ' ';
  }

  out << '\n';

  return out;
}
