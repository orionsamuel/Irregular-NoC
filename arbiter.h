#include "systemc.h"
#include "parameters.h"
#include <iostream>

using namespace std;

SC_MODULE(arbiter){
	sc_in<bool> clk{"clk"};
	sc_signal<sc_int<32> > rd[5];
	sc_int<32> priority;
	sc_int<32> bufferCircular[5];
	sc_int<32> pointer;

	//Verifica em qual buffer se encontra a prioridade no momento
	void checkPriority(){			
		if(this->pointer < 4){
			this->pointer++;
		}else{
			this->pointer = 0;
		}
		if ((bufferCircular[NORTH] == 1) && (this->pointer == NORTH)){
			priority = NORTH;
			rd[0].write(1);
		}else if ((bufferCircular[EAST] == 1) && (this->pointer == EAST)){
			priority = EAST;
			rd[1].write(1);
		}else if ((bufferCircular[SOUTH] == 1) && (this->pointer == SOUTH)){
			priority = SOUTH;
			rd[2].write(1);
		}else if ((bufferCircular[WEST] == 1) && (this->pointer == WEST)){
			priority = WEST;
			rd[3].write(1);
		}else if ((bufferCircular[LOCAL] == 1) && (this->pointer == LOCAL)){
			priority = LOCAL;
			rd[4].write(1);
		}
	}


	SC_CTOR(arbiter){
		pointer = -1;
		SC_METHOD(checkPriority);
		sensitive << clk.pos();
	}
};