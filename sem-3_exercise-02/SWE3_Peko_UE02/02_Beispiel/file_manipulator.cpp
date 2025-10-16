#include "file_manipulator.h"

#include <fstream>
#include "random.h"

using fm = file_manipulator;

void fm::delete_file(std::string const& file_name) {
  std::remove(file_name.c_str());
}


void fm::fill_randomly(std::string const& file_name, size_type n, size_type len) {
    std::ofstream out(file_name);

    while (out && (n-- > 0)) {
        append(out, random_string(len));
    }
    out.close();
}


void fm::append(std::ofstream& file, std::string const& str) {
  file << str << ' ';
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
