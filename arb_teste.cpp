#include "systemc.h"
#include <iostream>
#include "arbiter.h"
#include "parameters.h"

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS, 0.5);
	sc_signal<sc_int<32> > portDestinyN, portDestinyE, portDestinyS, portDestinyW, portDestinyL, priority, rdN, rdE, rdS, rdW, rdL;
	portDestinyN = WEST;
	portDestinyS = EAST;

	arbiter arb("arbiter");
	arb.clk(clock);
	arb.portDestinyN(portDestinyN);
	arb.portDestinyE(portDestinyE);
	arb.portDestinyS(portDestinyS);
	arb.portDestinyW(portDestinyW);
	arb.portDestinyL(portDestinyL);
	arb.rdN(rdN);
	arb.rdE(rdE);
	arb.rdS(rdS);
	arb.rdW(rdW);
	arb.rdL(rdL);
	priority = arb.priority;
	arb.bufferCircular[NORTH] = 1;
	arb.bufferCircular[SOUTH] = 1;

	sc_start(10, SC_NS);

}