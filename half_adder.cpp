#include "systemc.h"

SC_MODULE(ha_sc){
	sc_in<bool> x, y;
	sc_out<bool> s, v;

	void proc(){
		s = x ^ y;
		v = x & y;
	}

	SC_CTOR(ha_sc){
		SC_METHOD(proc);
		sensitive << x << y;
	}
};