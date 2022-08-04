#ifndef GRAPHER_H_
#define GRAPHER_H_

#include <chrono>
#include <iostream>
#include <sstream>
#include <string>

class Grapher {
public:
  Grapher(std::string name, float *value, long long initial_time);
  std::string get_parsed_data();

  float *value;
  std::string name;
  long long initial_time;
};

#endif // GRAPHER_H_