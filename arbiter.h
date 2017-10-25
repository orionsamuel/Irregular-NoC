#include "systemc.h"
#include "parameters.h"
#include <iostream>

using namespace std;

SC_MODULE(arbiter){
	sc_in<bool> clk{"clk"};
	sc_out<sc_int<32> > rdN{"rdN"};
	sc_out<sc_int<32> > rdE{"rdE"};
	sc_out<sc_int<32> > rdS{"rdS"};
	sc_out<sc_int<32> > rdW{"rdW"};
	sc_out<sc_int<32> > rdL{"rdL"};
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
			rdN.write(1);
		}else if ((bufferCircular[EAST] == 1) && (this->pointer == EAST)){
			priority = EAST;
			rdE.write(1);
		}else if ((bufferCircular[SOUTH] == 1) && (this->pointer == SOUTH)){
			priority = SOUTH;
			rdS.write(1);
		}else if ((bufferCircular[WEST] == 1) && (this->pointer == WEST)){
			priority = WEST;
			rdW.write(1);
		}else if ((bufferCircular[LOCAL] == 1) && (this->pointer == LOCAL)){
			priority = LOCAL;
			rdL.write(1);
		}
	}


	SC_CTOR(arbiter){
		pointer = -1;
		SC_METHOD(checkPriority);
		sensitive << clk.pos();
	}
};