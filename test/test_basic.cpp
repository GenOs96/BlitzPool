#include "thread_pool.hpp"
#include <iostream>
#include <chrono>

using namespace std;

int Func(int a){
    cout << "In Func a: " << a*a << endl;
    return a*a; 
}

int main(){
    ThreadPool tp;
    int a = 10, b = 11, c = 12, d = 13;
    auto add = [](int a, int b) -> int {
            auto sum = a+b;
            cout << "sum:" << sum << endl;
            return sum; 
        };
    auto sub = [](int a, int b) -> int {
            auto sub = b-a;
            cout << "sub: " << sub << endl;
            return sub;
        };
    
    future<int> f1 = tp.executeTask(add, a, c);
    this_thread::sleep_for(chrono::seconds(1));

    future<int> f2 = tp.executeTask(sub, b, d);
    this_thread::sleep_for(chrono::milliseconds(100));
    
    cout << "In main function: " << f1.get() << ", " << f2.get() << endl;
    return 0;
}
