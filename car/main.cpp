
#include <iostream>
#include <string>
#include "car.cpp"

using std::string;
using std::cout;

int main(){
	string n = "fred";
	Car g(n,10.2);
	cout << g << '\n';
	g.engineOn();
	cout << g << '\n';
	g.speedUp();
	cout << g << '\n';
	g.speedDown();
	cout << g << '\n';
	g.engineOff();
	cout << g << '\n';
	return 0;
}
