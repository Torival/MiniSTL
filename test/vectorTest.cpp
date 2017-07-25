#include <iostream>
#include <string>
#include <ctime>

using std::cout;
using std::endl;
using std::string;

//#include <vector>
//#define vector std::vector

#include "../src/vector.h"
#define vector mini_STL::vector

void test(string str);

int main() {
	
//    test("STL::vector");

    test("mini_STL::vector");
    
    return 0;
}

template <class T>
void print(vector<T> vec) {
    for(auto& it : vec)
        cout << it << ' ';
    cout << endl;
}

void test(string str) {
    clock_t start, finish; 
    start = clock(); 
    
    vector<int> pod_vec1;
    vector<int> pod_vec2(2, 1);
    vector<int> pod_vec3(3, 2);
    vector<int> pod_vec4(pod_vec3.begin(), pod_vec3.end());
    
    cout << str << endl;
    cout << "TEST plain old data-------------------" << endl;
    
    print(pod_vec2);
    
    pod_vec2.push_back(3);
    print(pod_vec2);
        
    pod_vec2.insert(pod_vec2.begin(), 4); 
    print(pod_vec2);
        
    pod_vec2.insert(pod_vec2.begin() + 1, 2, 5);
    print(pod_vec2);
        
    pod_vec2.pop_back();
    print(pod_vec2);
        
    pod_vec2.insert(pod_vec2.end() - 1, pod_vec4.begin(), pod_vec4.end());
    print(pod_vec2);
    
    pod_vec2.erase(pod_vec2.begin()+1, pod_vec2.begin()+3);
    print(pod_vec2);
    
    pod_vec2.erase(pod_vec2.begin()+2);
    print(pod_vec2);

    
    
    
    vector<string> obj_vec1; 
    vector<string> obj_vec2(5, "123");
    vector<string> obj_vec3(3, "234");
    vector<string> obj_vec4(obj_vec3.begin(), obj_vec3.end());
    
    cout << "\n\nTEST object data-------------------" << endl;
    
    print(obj_vec2);
    
    obj_vec2.push_back("456");
    print(obj_vec2);
        
    obj_vec2.insert(obj_vec2.begin(), "567");
    print(obj_vec2);
       
    obj_vec2.insert(obj_vec2.begin() + 1, 2, "678");
    print(obj_vec2);
        
    obj_vec2.insert(obj_vec2.end() - 1, obj_vec4.begin(), obj_vec4.end());
    print(obj_vec2);
    
    obj_vec2.erase(obj_vec2.begin()+1, obj_vec2.begin()+3);
    print(obj_vec2);
    
    obj_vec2.erase(obj_vec2.begin()+2);
    print(obj_vec2);
     
    obj_vec2.pop_back();   
    print(obj_vec2);
        
    finish = clock();
    
    cout << "time : " << finish - start << "ms" << endl; 
}
