#include <iostream>

using namespace std;

int main()
{
    int number {25};
    int *pnumber {&number}; // указатель pnumber хранит адрес переменной number
    cout << "number addr: " << pnumber << endl;
    cout << "pnumber addr: " << &pnumber << endl;
    cout << "Value = " << *pnumber << endl; 
}