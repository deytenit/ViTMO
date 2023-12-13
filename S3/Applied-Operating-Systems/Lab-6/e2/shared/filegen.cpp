#include <fstream>
#include <string>

using namespace std;

int main(int argc, char **argv) {
	size_t size = stoi(static_cast<string>(argv[1]));
	size_t amount = stoi(static_cast<string>(argv[2]));

	for (size_t i = 1; i <= amount; ++i) {
		fstream fout("./files/" + to_string(i), fstream::out);

		for (size_t j = 1; j <= size; ++j) {
			fout << j << '\n';
		}
	}
	return 0;
}
