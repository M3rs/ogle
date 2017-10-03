#include <iostream>
#include <vector>
#include <string>

#include "sol.hpp"

struct Vec {
  float x;
  float y;
  float z;

  Vec() : x(0.0f), y(0.0f), z(0.0f) {}
  Vec(float x, float y, float z) : x(x), y(y), z(z) {}
};

template<class Value>
std::vector<Value> get_vector(sol::table table)
{
  std::vector<Value> items;
  for (auto& iter : table) {
    items.push_back(iter.second.as<Value>());
  }

  return items;
}

int main() {
  sol::state lua;
  lua.open_libraries(sol::lib::base, sol::lib::package);
  lua.new_usertype<Vec>("vec3",
                        sol::constructors<Vec(), Vec(float, float, float)>(),
                        "x", &Vec::x, "y", &Vec::y, "z", &Vec::z);
  lua.script_file("test.lua");
  //lua.script_file("other.lua");
  lua.script("x()");
  int x = 5;

  int w = lua["window"]["w"];
  int h = lua["window"]["h"];

  std::cout << w << ", " << h << std::endl;

  auto names = get_vector<std::string>(lua["items"]);
  auto items = get_vector<float>(lua["vertices"]);



}
