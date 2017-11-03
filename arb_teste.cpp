#include "systemc.h"
#include <iostream>
#include "arbiter2.h"
#include "parameters.h"

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS, 0.5);

	arbiter arb("arbiter");
	arb.clk(clock);
	arb.bufferCircular[0] = 1;
	arb.bufferCircular[1] = 1;
	arb.bufferCircular[2] = 1;
	arb.bufferCircular[3] = 1;
	arb.bufferCircular[4] = 1;

	sc_start(100, SC_NS);
}