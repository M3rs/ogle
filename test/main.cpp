#include <iostream>

#include "sol.hpp"

struct Vec {
	float x;
	float y;
	float z;

	Vec():x(0.0f),y(0.0f),z(0.0f) {}
	Vec(float x, float y, float z) : x(x), y(y), z(z) {}
};

int main()
{
	sol::state lua;
	lua.open_libraries(sol::lib::base, sol::lib::package);
	lua.new_usertype<Vec>("vec3",
			sol::constructors<Vec(), Vec(float, float, float)>(),
			"x", &Vec::x,
			"y", &Vec::y,
			"z", &Vec::z
	);
	lua.script_file("test.lua");
	lua.script("x()");
	int x = 5;

	int n = lua["n"];

	std::cout << n << std::endl;

}
