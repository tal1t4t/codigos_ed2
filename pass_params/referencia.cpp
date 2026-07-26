#include <iostream>
#include <ostream>
using namespace std;

void f2(int*& y){
    y = y + 2;
    cout << "y = " << y << endl;
    cout << "end. de y = " << &y << endl << endl;
}

void f1(int& x){ 
    x = x + 2;
    cout << "x = " << x << endl;
    cout << "end. de x = " << &x << endl << endl;
}

int main(){
    int a = 5;
    int *b = &a;

    cout << "a = " << a << endl;
    cout << "end. de a = " << &a << endl << endl;

    cout << "b = " << b << endl;
    cout << "*b = " << *b << endl;
    cout << "end. de b = " << &b << endl << endl;

    f1(a);
    f2(b);

    cout << "a = " << a << endl;
    cout << "end. de a = " << &a << endl << endl;

    cout << "b = " << b << endl;
    cout << "*b = " << *b << endl;
    cout << "end. de b = " << &b << endl << endl;

}