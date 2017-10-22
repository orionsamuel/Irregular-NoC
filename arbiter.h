#include "systemc.h"
#include "parameters.h"
#include <iostream>

using namespace std;

SC_MODULE(arbiter){
	sc_in<bool> clk;
	sc_in<sc_int<32> > portDestinyN, portDestinyE, portDestinyS, portDestinyW, portDestinyL;
	sc_out<sc_int<32> > rdN, rdE, rdS, rdW, rdL;
	sc_int<32> priority;
	sc_int<32> bufferCircular[5];
	sc_int<32> pointer;

	//Verifica em qual buffer se encontra a prioridade no momento
	void checkPriority(){
		for (int i = 0; i < 5; i++){
			pointer = i;
			if ((bufferCircular[NORTH] == 1) && (this->pointer == NORTH)){
				priority = portDestinyN.read();
				rdN.write(1);
			}else if ((bufferCircular[EAST] == 1) && (this->pointer == EAST)){
				priority = portDestinyE.read();
				rdE.write(1);
			}else if ((bufferCircular[SOUTH] == 1) && (this->pointer == SOUTH)){
				priority = portDestinyS.read();
				rdS.write(1);
			}else if ((bufferCircular[WEST] == 1) && (this->pointer == WEST)){
				priority = portDestinyW.read();
				rdW.write(1);
			}else if ((bufferCircular[LOCAL] == 1) && (this->pointer == LOCAL)){
				priority = portDestinyL.read();
				rdL.write(1);
			}
		}
	}


	SC_CTOR(arbiter){

		SC_METHOD(checkPriority);
		sensitive << clk.pos() << portDestinyN << portDestinyE << portDestinyS << portDestinyW << portDestinyL;
	}
};