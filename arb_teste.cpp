#include "systemc.h"
#include <iostream>
#include "arbiter.h"
#include "parameters.h"

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS, 0.5);

	arbiter arb("arbiter");
	arb.clk(clock);
	arb.bufferCircular[NORTH] = 1;
	arb.bufferCircular[EAST] = 1;
	arb.bufferCircular[SOUTH] = 1;
	arb.bufferCircular[WEST] = 1;
	arb.bufferCircular[LOCAL] = 1;

	sc_start(50, SC_NS);
}