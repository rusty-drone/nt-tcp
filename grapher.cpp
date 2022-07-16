#include "grapher.h"

using namespace std::chrono;

template <typename T> Grapher<T>::Grapher(std::string name, T *value) {
  this->value = value;
  this->name = name;
}

template <typename T> std::string Grapher<T>::get_parsed_data() {
  //   std::chrono::milliseconds time =
  //       duration_cast<milliseconds>(system_clock::now().time_since_epoch());

  //   long long micros =
  //       std::chrono::duration_cast<std::chrono::microseconds>(time).count();

  //   std::stringstream ss;
  //   ss << micros;
  //   std::string s = ss.str();

  //   std::string out = name + "_" + s + "_" + std::to_string(*value) + "|";

  return " ";
}