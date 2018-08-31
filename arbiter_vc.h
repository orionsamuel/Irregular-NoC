#include "systemc.h"
#include "parameters.h"
#include <iostream>

using namespace std;

SC_MODULE(arbiter){
	sc_in<bool> clk;
	sc_signal<sc_int<32> > rd[10];
	sc_int<32> priority;
	sc_int<32> bufferCircular[10];
	sc_int<32> pointer;

	//Verifica em qual buffer se encontra a prioridade no momento
	void checkPriority(){			
		if(this->pointer < 5){
			this->pointer++;
		}else{
			this->pointer = 1;
		}
		if ((bufferCircular[0] == 1)  && (this->pointer == 0)){
			priority = 0;
			rd[0].write(1);
		}
		if ((bufferCircular[1] == 1)  && (this->pointer == 1)){
			priority = 1;
			rd[0].write(1);
		}
		if ((bufferCircular[2] == 1)  && (this->pointer == 2)){
			priority = 2;
			rd[0].write(1);
		}
		if ((bufferCircular[3] == 1)  && (this->pointer == 3)){
			priority = 3;
			rd[0].write(1);
		}
		if ((bufferCircular[4] == 1)  && (this->pointer == 4)){
			priority = 4;
			rd[3].write(1);
		}
		if ((bufferCircular[5] == 1)  && (this->pointer == 5)){
			priority = 5;
			rd[4].write(1);
		}
		if ((bufferCircular[6] == 1)  && (this->pointer == 6)){
			priority = 6;
			rd[0].write(1);
		}
		if ((bufferCircular[7] == 1)  && (this->pointer == 7)){
			priority = 7;
			rd[0].write(1);
		}
		if ((bufferCircular[8] == 1)  && (this->pointer == 8)){
			priority = 8;
			rd[3].write(1);
		}
		if ((bufferCircular[9] == 1)  && (this->pointer == 9)){
			priority = 9;
			rd[4].write(1);
		}
		//cout << this->pointer << endl;
	}


	SC_CTOR(arbiter){
		pointer = 0;
		SC_METHOD(checkPriority);
		sensitive << clk.pos();
	}
};