#include "systemc.h"
#include <iostream>
#include "flow_control.h"

using namespace std;

int sc_main (int argc, char* argv[]){
	
	sc_clock clock("Clock", 10, SC_NS, 0.5);
	sc_signal<sc_int<32> > in_val, wok,in_ack, wr;
	in_val = 1;
	wok = 1;
	flow_control flow("flow");
	flow.clk(clock);
	flow.in_val(in_val);
	flow.wok(wok);
	flow.in_ack(in_ack);
	flow.wr(wr);
	//flow = new flow_control("flow");
	sc_start(10, SC_NS);

	return 0;
}