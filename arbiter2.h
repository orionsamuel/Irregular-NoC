#include "systemc.h"
#include "parameters.h"
#include <iostream>

using namespace std;

SC_MODULE(arbiter){
	sc_in<bool> clk;
	sc_signal<sc_int<32> > rd[5];
	sc_int<32> priority;
	sc_int<32> bufferCircular[5];
	sc_int<32> pointer;

	//Verifica em qual buffer se encontra a prioridade no momento
	void checkPriority(){			
		for(this->pointer = 1; this->pointer < 6; this->pointer++){
			if((bufferCircular[0] == 1) && (this->pointer = NORTH)){
				priority = NORTH;
				cout << priority << endl;
			}
			if((bufferCircular[1] == 1) && (this->pointer = EAST)){
				priority = EAST;
				cout << priority << endl;
			}
			if((bufferCircular[2] == 1) && (this->pointer = SOUTH)){
				priority = SOUTH;
				cout << priority << endl;
			}
			if((bufferCircular[3] == 1) && (this->pointer = WEST)){
				priority = WEST;
				cout << priority << endl;
			}
			if((bufferCircular[4] == 1) && (this->pointer = LOCAL)){
				priority = LOCAL;
				cout << priority << endl;
			}
		}		
		
	}


	SC_CTOR(arbiter){
		SC_METHOD(checkPriority);
		sensitive << clk.pos();
	}
};