#include <iostream>
using namespace std;

int main() {
  cout << "Hello World! \n";
  cout << "My name is Adam Amir! \n";
  cout << "This is my first C++ project! \n";
  cout << "The following is a blank line: \n";
  cout << "\n";
  cout << "Here's another way to insert a blank line:" << endl;
  cout << "Other cool stuffs: \n";
  cout << "\tTab \\Backslash \"Quote\"" << endl;
  int number = 17;
  // int is a var
  cout << number << endl;
  // declare multiple vars
  int x, y, z;
  x = y = z = 50;
  cout << x + y + z << endl;
  const int myNum = 15;
  // myNum = 10; myNum will always be 15, can't be redeclared because myNum is a constant
  return 0;
}