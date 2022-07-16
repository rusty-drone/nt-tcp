#ifndef GRAPHER_H_
#define GRAPHER_H_

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

template <typename T> class Grapher {
public:
  Grapher(std::string name, T *value);
  std::string get_parsed_data();

private:
  T *value;
  std::string name;
};

#endif // GRAPHER_H_