#include <iostream>
#include "../src/pair.h"

using std::cout;
using std::endl;
using mini_STL::pair; 

int main() {
    pair<int, int> p1(3, 5);
    pair<int, int> p2(3, 5);
    pair<int, int> p3(3, 6);
    pair<int, int> p4(3, 2);

    cout << "p1(3, 5), p2(3, 5), p3(3, 6), p4(3, 2)\n\n";

    if(p1 == p2)
        cout << "p1 == p2" << endl;
    if(p1 != p3)
        cout << "p1 != p3" << endl;
    if(p1 <  p3)
        cout << "p1 <  p3" << endl;
    if(p1 >  p4)
        cout << "p1 >  p4" << endl;
    if(p1 <= p3)
        cout << "p1 <= p3" << endl;
    if(p1 >= p4)
        cout << "p1 >= p4" << endl;
    
    return 0;
}
