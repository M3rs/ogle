#include <iostream>
//#include <experimental/string_view>
#include <string_view>

//void greet(std::experimental::string_view sv)

void cgreet(const char* name)
{
	std::cout << name << std::endl;
}
void greet(std::string_view sv)
{
	std::cout << sv << std::endl;
	cgreet(sv.data());
}

int main() {
	std::string n = "andy";
	greet(n);
	greet("stan");

}
