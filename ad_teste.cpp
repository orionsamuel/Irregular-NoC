#include "systemc.h"
#include "full_adder.cpp"
#include <iostream>

using namespace std;


int sc_main (int argc, char* argv[]){

	sc_signal<bool> x, y, vem, soma, vai;
	x = 1;
	y = 1;

	fa adder("FA");
	adder.x(x);
	adder.y(y);
	adder.vem(vem);
	adder.soma(soma);
	adder.vai(vai);

	sc_start(10, SC_NS);
	cout << soma << endl;
}