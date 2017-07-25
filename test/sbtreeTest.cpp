#include <set>
#include <vector>
#include <ctime>
#include <iostream>
#include "../src/sbtree.h"

using namespace std;
using mini_STL::sbtree; 


// 获取一个长度为 size 的数组，数组元素是随机数，范围为1 – n，且元素不重复。
vector<int> getrandom(int size);

int main() {
	srand((unsigned)time(NULL));

    cout << "TEST---------- (Insert, Erase, Find)\n";
    
    for(int k = 0; k < 3; ++k) {
	    clock_t start[2][4], finish[2][4];
    	vector<int> vec;
		set<int> s;
    	sbtree<int> sb;
		
		string s1[2];
	    string s2[2];	
	    
	    int size = 10000000 + rand() % 9999999;
	    vec = getrandom(size);
	    
		cout << "\nTEST "<< k <<" size: "<< size <<"\n";
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
	            if(j == 0) {
	                if(s.find(vec[i]) != s.end())
	                    s1[0] += "1";
	            } else {
	                if(sb.find(vec[i]))
	                    s2[0] += "1";
	        	}
	        }   
	        finish[j][1] = clock();
	
	        start[j][2] = clock();
	        for (int i = 0; i < size; i += 3){
	            if(j == 0)
	                s.erase(vec[vec.size() - 1 - i]);
	            else
	                sb.erase(vec[vec.size() - 1 - i]);
	        }
	        finish[j][2] = clock();
	
	
			start[j][3] = clock();
	        for (int i = 0; i < size; ++i){
	            if(j == 0) {
	                if(s.find(vec[i]) != s.end())
	                    s1[1] += "1";
	            } else {
	                if(sb.find(vec[i]))
	                    s2[1] += "1";
	        	}
	        }   
	        finish[j][3] = clock();
	    }
	
	    cout <<"set/sbtree\tinsert\tfind(all)\terase\tfind(del)\t"<<endl;
	    for(int i = 0; i < 2; ++i) {
	        if(i == 0) 
	            cout <<"set\t\t";
	        else
	            cout <<"sbtree\t\t";
	
	        cout << finish[i][0] - start[i][0] << "\t" <<
	                finish[i][1] - start[i][1] << "\t\t" <<
					finish[i][2] - start[i][2] << "\t" <<  
	                finish[i][3] - start[i][3] << "\t" <<  endl;
	
	    }
	
	    cout << "find all count: (rbtree, " << s1[0].size() << "),  (sbtree, " << s2[0].size() << ")\n"
			 << "find del count: (rbtree, " << s1[1].size() << "),  (sbtree, " << s2[1].size() << ")" << endl;
	}
	
	cout << "\nTEST---------- (Select, Rank)\n";
    
    for(int k = 0; k < 3; ++k) {
    	sbtree<int> sb;	
	    
	    int size = 100000 + rand() % 9999999;
	    vector<int>vec;
		
		vec = getrandom(size);
		
//	    for(int i = 0; i < vec.size(); ++i)
//	    	cout << vec[i]<<" ";
//	    
		
	    cout << "\nTEST "<< k <<" size: "<< size <<"\n";
	    for (int i = 0; i < size; ++i){
	        sb.insert(vec[i]);
	    } 
	        
	    int num =1 + rand()%vec.size();

	    cout << "select(" << num << "): " << sb.select(num) <<"    "
	    	 << "rank("   << num << "): " << sb.rank(num)   << endl;
	}			
    return 0;
}

vector<int> getrandom(int size){
    
    
     vector<int> result(size);
    for(int i = 0; i < result.size(); ++i)
        result[i] = i+1;
        
    for(int i = 0; i < result.size() - 1; ++i)
        swap(result[i], result[i + rand() %(size - i)]);
    
    return result;
}
