#include "grapher.h"

using namespace std::chrono;

Grapher::Grapher(std::string name, float *value, long long initial_time) {
  this->value = value;
  this->name = name;
  this->initial_time = initial_time;
}

std::string Grapher::get_parsed_data() {
  std::chrono::milliseconds time =
      duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  long long micros =
      std::chrono::duration_cast<std::chrono::microseconds>(time).count() -
      initial_time;

  std::stringstream ss;
  ss << micros;
  std::string s = ss.str();

  std::cout << s << std::endl;

  std::string out = name + "_" + s + "_" + std::to_string(*value) + "|";

  return out;
}