#include <set>
#include <vector>
#include <ctime>
#include <iostream>
#include "../src/sbtree.h"

using namespace std;
using mini_STL::sbtree; 

#define TESTCOUNT 100000
// 给定一个正整数n，需要输出一个长度为n的数组，数组元素是随机数，范围为0 – n-1，且元素不能重复。
vector<int> getrandom(int size);

int main() {

    set<int> s;
    sbtree<int> sb;
    clock_t start[2][3], finish[2][3];
    string s1 = "";
    string s2 = "";
    vector<int> vec;
    int size = TESTCOUNT;
    
    vec = getrandom(size);

    for(int j = 0; j < 2; ++j) {
        start[j][0] = clock();
        for (int i = 0; i < size; ++i){
            if(j == 0)
                s.insert(vec[i]);
            else
                sb.insert(vec[i]);
        }   
        finish[j][0] = clock();

        start[j][1] = clock();
        for (int i = 0; i < size; ++i){
            if(j == 0)
                if(s.find(vec[i]) != s.end())
                    s1+= "1";
            else
                if(sb.find(vec[i]))
                    s2+= "1";
        }   
        finish[j][1] = clock();

        start[j][2] = clock();
        for (int i = 0; i < size; ++i){
            if(j == 0)
                s.erase(vec[vec.size() - 1 - i]);
            else
                sb.erase(vec[vec.size() - 1 - i]);
        }
        finish[j][2] = clock();

    }

    cout <<"set/sbtree\tinsert\terase\tfind\t"<<endl;
    for(int i = 0; i < 2; ++i) {
        if(i == 0) 
            cout <<"set\t\t";
        else
            cout <<"sbtree\t\t";

        cout << finish[i][0] - start[i][0] << "\t" <<
                finish[i][1] - start[i][1] << "\t" <<  
                finish[i][2] - start[i][2] << "\t" <<  endl;

    }

    cout << s1.size() << ","<<s2.size();
    return 0;
}


vector<int> getrandom(int size){
    srand((unsigned)time(NULL));
    
     vector<int> result(size);
    for(int i = 0; i < result.size(); ++i)
        result[i] = i;
        
    for(int i = 0; i < result.size() - 1; ++i)
        swap(result[i], result[i + rand() %(size - i)]);
    
    return result;
}
