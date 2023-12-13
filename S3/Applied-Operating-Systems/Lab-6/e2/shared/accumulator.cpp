#include <fstream>
#include <string>

using namespace std;

int main(int argc, char **argv) {
	string file = "./files/" + static_cast<string>(argv[1]);
	size_t size = stoi(static_cast<string>(argv[2]));
	
	fstream fin(file, fstream::in);
	fstream fout(file, fstream::app);

	for (size_t i = 0; i < size; ++i) {
		long long tmp;
		fin >> tmp;
		fout << tmp * 2 << '\n';
	}

	return 0;
}
