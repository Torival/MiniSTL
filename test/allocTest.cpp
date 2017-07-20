#include <iostream>
#include <vector>
#include <cstdlib>
#include <iomanip>
#include <ctime>

#include "../src/construct.h"
#include "../src/uninitialized.h"
#include "../src/alloc.h"

using std::cin;
using std::cout;
using std::endl;
using std::vector;
using std::setw;
using std::setiosflags;
using std::ios;

using namespace mini_STL;

#define TEST_COUNT 500

void head();
void tail();

int main(int argc, char** argv) {
    vector<void*> ptr;
    vector<int> sz;
    default_alloc alloc;
    int size, flag;

    srand((unsigned)time(NULL));
    for (int i = 0; i < TEST_COUNT; i++) {

        flag = rand() & 1;
        cout << setiosflags(ios::left) << setw(25) << "memory_pool left bytes : " << alloc.left();

        if (ptr.size() < 5 || flag) {
            size = rand() % 129 + 1;
            cout << setw(12) << "\tallocate : " << size << endl;
        } else {
            size = rand() % ptr.size();
            cout << setw(12) << "\tdeallocate : " << sz[size] << endl;
        }

        head();
        alloc.test_free_list();

        if (ptr.size() < 5 || flag) {
            sz.push_back(size);
            ptr.push_back(alloc.allocate(size));
        } else {
            alloc.deallocate(ptr[size], sz[size]);
            ptr.erase(ptr.begin() + size);
            sz.erase(sz.begin() + size);
        }

        tail();
        cout << endl;

    }

    return 0;
}

void head() {
    cout << "+-------+-------+-------+-------+-------+-------+-------+-------"
         << "+-------+-------+-------+-------+-------+-------+-------+-------+" << endl;
    cout << "|8\t|16\t|24\t|32\t|40\t|48\t|56\t|64\t|72\t|80\t|88\t|96\t|104    |112\t|120\t|128\t|" << endl;

    cout << "+-------+-------+-------+-------+-------+-------+-------+-------"
         << "+-------+-------+-------+-------+-------+-------+-------+-------+" << endl;
}

void tail() {
    cout << "+-------+-------+-------+-------+-------+-------+-------+-------"
         << "+-------+-------+-------+-------+-------+-------+-------+-------+" << endl;
}
