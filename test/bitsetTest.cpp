#include <iostream>
#include <string>
using std::string;
using std::cout;
using std::endl;

//#include <bitset>
//using std::bitset;

#include "../src/bitset.h"
using mini_STL::bitset;


int main() {
    string s1 = "1110011100010111110110000001011000101001101101010000";
    string s2 = "1101100011110110101001110011111101100000010110011111";
    bitset<52> bs1(s1);
    bitset<52> bs2(s2);

    cout << "s1: " << s1 << endl;
    cout << "s2: " << s2 << endl;
    cout << endl;

    cout << "s1.count() -> " << bs1.count() << endl;
    cout << "s2.count() -> " << bs2.count() << endl;
    cout << endl;

    cout << "s1.any() -> " << bs1.any() << endl;
    cout << "s2.any() -> " << bs2.any() << endl;

    cout << "s1.all() -> " << bs1.all() << endl;
    cout << "s2.all() -> " << bs2.all() << endl;
    cout << endl;

    cout << "s1.none() -> " << bs1.none() << endl;
    cout << "s2.none() -> " << bs2.none() << endl;
    cout << endl;

    cout << "s1.test(10) -> " << bs1.test(10) << endl;
    cout << "s2.test(10) -> " << bs2.test(10) << endl;
    cout << endl;

    cout << "s1[15] -> " << bs1[15] << endl;
    cout << "s2[17] -> " << bs2[17] << endl;
    cout << endl;


    cout << "s1 == s1 -> " << (bs1 == bs1) << endl;
    cout << "s1 == s2 -> " << (bs1 == bs2) << endl;
    cout << "s1 != s1 -> " << (bs1 != bs1) << endl;
    cout << "s1 != s2 -> " << (bs1 != bs2) << endl;
    cout << endl;

    cout << "s1.to_string() -> " << bs1.to_string() << endl;
    cout << "s2.to_string() -> " << bs2.to_string() << endl;
    cout << endl;

    bitset<52> bs3(bs1);
    bitset<52> bs4(bs1);
    bitset<52> bs5(bs1);
    bs3 &= bs2;
    bs4 |= bs2;
    bs5 ^= bs2;
    (~bs1);
    cout << "s1 &= s2     -> " << bs3.to_string() << endl;
    cout << "s1 |= s2     -> " << bs4.to_string() << endl;
    cout << "s1 ^= s2     -> " << bs5.to_string() << endl;
    cout << "~s1          -> " << bs1.to_string() << endl;
    cout << endl;

   
    bs1 <<= 13;
    cout << "s1 <<= 13    -> " << bs1.to_string() << endl;
    bs2 <<= 13;
    cout << "s2 <<= 13    -> " << bs2.to_string() << endl;
    bs1 >>= 15;
    cout << "s1 >>= 15    -> " << bs1.to_string() << endl;
    bs2 >>= 15;
    cout << "s2 >>= 15    -> " << bs2.to_string() << endl;
    
    cout << endl;
    
    bs1.set(17);
    cout << "s1.set(17)   -> " << bs1.to_string() << endl;
    bs1.reset(17);
    cout << "s1.reset(17) -> " << bs1.to_string() << endl;
    bs1.flip(17);
    cout << "s1.flip(17)  -> " << bs1.to_string() << endl;
    cout << endl;

    /*
    bs1[13] = false;
    cout << "s1[13] = false -> " << bs1[13] << endl;
    bs1[13] = bs1[28];
    cout << "s1[13] = s1[28] -> " << bs1[13] << endl;
    cout << endl;*/

    cout << "s1 & s2      -> " << (bs1 & bs2).to_string() << endl;
    cout << "s1 | s2      -> " << (bs1 | bs2).to_string() << endl;
    cout << "s1 ^ s2      -> " << (bs1 ^ bs2).to_string() << endl;
    cout << endl;
    return 0;
}
