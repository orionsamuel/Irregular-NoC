#include "systemc.h"
#include <iostream>
#include "flow_control.h"

using namespace std;

int sc_main (int argc, char* argv[]){
	
	sc_clock clock("Clock", 10, SC_NS, 0.5);
	
	flow_control flow("flow");
	flow.clk(clock);
	flow.in_val.write(1);
	flow.wok.write(1);
	sc_start(30, SC_NS);
	return 0;
}