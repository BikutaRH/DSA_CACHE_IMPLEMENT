#include <iostream>
using namespace std;

class A{
    int a = 5;
    int b = 6;
public:
    A() {
        cout << "1" << endl;
    }
    ~A() {
        cout << "destroy 1";
    }
};
class B : public A {
    int c = 7;
    int d = 8;
public:
    B(){
        cout << "2" << endl;
    }
    ~B(){
        cout << "detroy 2";
    }
};
int main(){
    A* newA = new B();
    delete newA;
    return 0;
}

