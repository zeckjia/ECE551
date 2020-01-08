#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <vector>

template<typename T>
std::ostream & operator<<(std::ostream & s, const std::vector<T> & v) {
  for (size_t i = 0; i < v.size(); i++) {
    s << v[i];
  }
  return s;
}

template<typename A>
void read_sort_print(A & f) {
  std::vector<std::string> lines;
  std::string curr;

  while (getline(f, curr)) {
    curr.push_back('\n');
    lines.push_back(curr);
  }
  std::sort(lines.begin(), lines.end());
  std::cout << lines;
}

int main(int argc, char ** argv) {
  if (argc == 1) {
    read_sort_print<std::istream>(std::cin);
    return EXIT_SUCCESS;
  }

  for (int i = 1; i < argc; i++) {
    std::ifstream f(argv[i]);
    if (!f.is_open()) {
      std::cerr << "Could not open the file!" << std::endl;
      exit(EXIT_FAILURE);
    }
    read_sort_print<std::ifstream>(f);
    f.close();
  }
  return EXIT_SUCCESS;
}
