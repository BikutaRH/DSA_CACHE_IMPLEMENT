#include <iostream>
using namespace std;

class A{
public:
    int a = 5;
    int b = 6;
public:
    A() {
        cout << a << endl;
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
    cout << newA->a;
    delete newA;
    return 0;
}

