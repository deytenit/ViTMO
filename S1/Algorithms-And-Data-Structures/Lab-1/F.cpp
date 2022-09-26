#include <iostream>

using namespace std;

const string sep = "-----------------------------------------\n";

struct fridge {
    struct item {
        string title;
        unsigned int amount;

        void add(const int& filler) {
            if (amount + filler < 0) {
                throw invalid_argument("Item amount will be negative.");
            }

            amount += filler;
        }

        [[nodiscard]] int get() const {
            return amount;
        }

        friend ostream& operator<<(ostream& out, const item& it) {
            return out << it.amount;
        }

        friend istream& operator>>(istream& in, item& it) {
            return in >> it.amount;
        }
    };

    item items[4] = {{"milk", 0}, {"meat", 0}, {"cheese", 0}, {"sweets", 0}};

    void log() const {
        cout << "1. Milk - " << items[0] << " li.\n"
             << "2. Meat - " << items[1] << " kg.\n"
             << "3. Cheese - " << items[2] << " sli.\n"
             << "4. Sweets - " << items[3] << " amo." << endl;
    }
};

int main() {
    fridge fridges[2];

    cout << "Enter default values for 2 fridges:\n"
         << "[milk, li.] [meat, kg.] [cheese, sli.] [sweets, amo.]" << endl;

    for (auto& fridge : fridges) {
        cout << "|-> ";
        cin >> fridge.items[0] >> fridge.items[1] >> fridge.items[2] >> fridge.items[3];
    }

    while (true) {
        cout << sep
             << "Enter fridge number interface you want to get in.\n"
             << "[1-2] (other values will terminate program)" << endl;
        int fr;

        cout << "|-> ";
        cin >> fr;

        --fr;

        if (fr < 0 || fr > 1) {
            return 0;
        }

        cout << sep
             << "Fridge manipulation interface.\n"
             << "Enter command name:\n"
             << " - 1: Print out fridge contents.\n"
             << " - 2: Add/Remove item.\n"
             << " - 3: Move item to another fridge." << endl;

        int cmd;

        cout << "|-> ";
        cin >> cmd;

        if (cmd == 1) {
            fridges[fr].log();
        } else if (cmd == 2) {
            fridges[fr].log();

            cout << sep
                 << "Enter mutation request:\n"
                 << "[item id] [amount]" << endl;

            int it, amount;
            cout << "|-> ";
            cin >> it >> amount;

            --it;

            fridges[fr].items[it].add(amount);

        } else if (cmd == 3) {
            fridges[fr].log();

            cout << sep
                 << "Enter move request:\n"
                 << "[item id] [amount] [fridge id]" << endl;

            int it, amount, lfr;
            cout << "|-> ";
            cin >> it >> amount >> lfr;

            --it;
            --lfr;

            fridges[fr].items[it].add(-amount);
            fridges[lfr].items[it].add(amount);
        }
    }
}