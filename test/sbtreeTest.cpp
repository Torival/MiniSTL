#include <set>
#include <vector>
#include <iostream>
#include "../src/sbtree.h"

using namespace std;

// 给定一个正整数n，需要输出一个长度为n的数组，数组元素是随机数，范围为0 – n-1，且元素不能重复。
vector<int> getrandom(int size);

int main() {

    set s;
    sbtree sb;
	clock_t start[2][3], finish[2][3];

    for(int j = 0; j < 2; ++j) {
        start[i][0] = clock();
        for (int i = 0; i < size; ++i){
            if(j == 0)
                s.insert(vec[i]);
            else
                sb.insert(vec[i]);
        }   
        finish[i][0] = clock();

        start[i][1] = clock();
        for (int i = 0; i < size; ++i){
            if(j == 0)
                s.find(vec[i]);
            else
                sb.find(vec[i]);
        }   
        finish[i][1] = clock();

        start[i][2] = clock();
        for (int i = 0; i < size; ++i){
            if(j == 0)
                s.erase(vec[vec.size() - 1 - i]);
            else
                sb.erase(vec[vec.size() - 1 - i]);
        }
        finish[i][2] = clock();

    }

    cout <<"set/sbtree\tinsert\terase\tfind\t"<<endl;
    for(int j = 0; j < 2; ++j) {
        if(j == 0) 
            cout <<"set\t\t";
        else
            cout <<"sbtree\t\t";

        cout << finish[i][0] - start[i][0] << "\t" <<
                finish[i][1] - start[i][1] << "\t" <<  
                finish[i][2] - start[i][2] << "\t" <<  endl;

    }

	getchar();
	return 0;
}


vector<int> getrandom(int size){
    srand((unsigned)time(NULL));

    vector<int> result;
    vector<bool> ispush(size, false);
    
    while(result.size < size) {
        int num = rand() % size;
        if(!ispush[num]){
            result.push_back(num);
            ispush[num] = true;
        }
    }

    return result;
}