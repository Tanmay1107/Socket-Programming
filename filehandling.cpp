#include <iostream>
#include <fstream>
using namespace std;

void printboard()

{
    ifstream f("file.txt");

    if (f.is_open())
        cout << f.rdbuf();
}
