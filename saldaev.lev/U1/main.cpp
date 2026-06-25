#include <fstream>
#include <iostream>
#include <limits>
#include <string>

struct Person
{
  size_t id;
  std::string info;
};

namespace saldaev
{
  bool IdExcist(Person *persons, size_t s, size_t id)
  {
    for (size_t i = 0; i < s; ++i) {
      if (persons[i].id == id) {
        return true;
      }
    }
    return false;
  }
}

int main(int argc, char *argv[])
{
  if (argc > 3) {
    std::cerr << "wrong amoung of arguments";
    return 0;
  }

  std::fstream ifile, ofile;

  bool ifl = false;
  bool ofl = false;

  for (int i = 1; i < argc; ++i) {
    const char *arg = argv[i];

    if (arg[0] == 'i' && arg[1] == 'n' && arg[2] == ':') {
      if (ifl) {
        std::cerr << "wrong arguments";
        return 1;
      }
      const char *filename = arg + 3;
      ifile.open(filename);
      if (!ifile.is_open()) {
        std::cerr << "failed to open input file";
        return 2;
      }
      ifl = true;
    } else if (arg[0] == 'o' && arg[1] == 'u' && arg[2] == 't' && arg[3] == ':') {
      if (ofl) {
        std::cerr << "wrong arguments";
        return 1;
      }
      const char *filename = arg + 4;
      ofile.open(filename);
      if (!ofile.is_open()) {
        std::cerr << "failed to open output file";
        return 2;
      }
      ofl = true;
    } else {
      std::cerr << "wrong arguments";
      return 1;
    }
  }

  std::istream &in = ifl ? ifile : std::cin;
  std::ostream &out = ofl ? ofile : std::cout;

  Person *base = new Person[10];
  size_t cap = 10;
  size_t curr = 0;
  size_t ignored = 0;

  size_t id = 0;
  std::string info;
  while (true) {
    if (in >> id) {
      if (std::getline(in, info)) {
        size_t pos = 0;
        while (pos < info.size() && (info[pos] == ' ' || info[pos] == '\t')) {
          ++pos;
        }

        if (pos == info.size()) {
          ++ignored;
          continue;
        }

        info = info.substr(pos);

        if (saldaev::IdExcist(base, curr, id)) {
          ++ignored;
          continue;
        }

        if (curr == cap) {
          size_t newcap = cap * 2;
          Person *newbase = new Person[newcap];
          for (size_t j = 0; j < curr; ++j) {
            newbase[j] = base[j];
          }
          delete[] base;
          base = newbase;
          cap = newcap;
        }
        base[curr++] = {id, info};
        continue;
      }
    }

    if (in.eof()) {
      break;
    } else {
      in.clear();
      std::cin.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    }
    ++ignored;
  }

  if (ifl) {
    ifile.close();
  }

  for (size_t i = 0; i < curr; ++i) {
    out << base[i].id << ' ' << base[i].info << '\n';
  }
  if (curr == 0) {
    out << '\n';
  }

  delete[] base;
  if (ofl) {
    ofile.close();
  }

  std::cerr << curr << ' ' << ignored << '\n';
}
