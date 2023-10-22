#include <iostream>
#include <sstream>
#include <string>

using namespace std;

int main() {
  string equation;
  int result = 0;
  cout << "Type an equation:" << endl;
  cin >> equation;
  // Create an input stringstream to parse the equation
  stringstream equationStream(equation);
  char operatorSymbol;
  int operand;
  equationStream >> result; // Initialize result with the first number
  while (equationStream >> operatorSymbol >> operand) {
    if (operatorSymbol == '+') {
      result += operand;
    } else if (operatorSymbol == '-') {
      result -= operand;
    } else {
      cout << "Invalid operator: " << operatorSymbol << endl;
      return 1; // Exit with an error code
    }
  }
  cout << "Answer:" << endl << result << endl;
  return 0;
}
