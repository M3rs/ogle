#include <iostream>
#include <fstream>
#include <string>
#include <set>
#include <vector>
#include <algorithm>

using namespace std;

int main()
{
	ifstream f;
	f.open("data.txt");
	string line;
	set<string> lines;
	vector<string> data;
	while (getline(f, line)) {
		if (line == "") continue;

		lines.insert(line);
		data.push_back(line);
	}

	for (auto& vert : lines) {
		cout << "v " << vert << endl;
	}

	for (auto& v : data) {
		auto fv = lines.find(v);
		if (fv != lines.end()) {
			auto pos = distance(lines.begin(), find(lines.begin(), lines.end(), v));
			cout << "i " << pos << endl;
		}
	}
	
}
