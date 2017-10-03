#ifndef SOL_HELPERS_HPP
#define SOL_HELPERS_HPP

/*
Helper functions to augment sol lua library.

These may already exist and I am just doing it wrong
or not finding it in the documentation.
 */

#include "sol/sol.hpp"

#include <vector>

namespace ogle {
/// Template function to convert lua table of T
/// into std::vector<T>. Uses table iter.second
/// to get the value.
template <class T> std::vector<T> get_vector(sol::table table) {
  std::vector<T> ts;
  for (auto &iter : table) {
    ts.push_back(iter.second.as<T>());
  }

  return ts;
}
  
}

#endif // SOL_HELPERS_HPP