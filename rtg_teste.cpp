#include "systemc.h"
#include <iostream>
#include <vector>
#include "routing.h"
#include "parameters.h"

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS, 0.5);
	sc_signal<sc_int<32> > destiny, portDestiny;
	destiny = 8;
	routing_table table0;

	table0.push_back({8, WEST, 5});
	table0.push_back({4, EAST, 2});
	table0.push_back({8, NORTH, 8});
	table0.push_back({1, EAST, 1});

	routing rtg("routing");
	rtg.clk(clock);
	rtg.destiny(destiny);
	rtg.portDestiny(portDestiny);
	rtg.tabela = table0;

	sc_start(10, SC_NS);

	cout << rtg.tabela.size() << endl;
	cout << rtg.counter << endl;
	cout << portDestiny << endl;

}