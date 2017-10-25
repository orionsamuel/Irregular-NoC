#include "systemc.h"
#include <iostream>
#include "router.h"
#include "parameters.h"
#include <vector>

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS);

	routing_table table0;

	table0.push_back({3, WEST, 5});
	table0.push_back({4, EAST, 2});
	table0.push_back({3, NORTH, 8});
	table0.push_back({4, EAST, 1});

	flit in_portN;
	flit in_portE;
	flit in_portS;
	flit in_portW;
	flit in_portL;

	flit out_portN;
	flit out_portE;
	flit out_portS;
	flit out_portW;
	flit out_portL;

	in_portL.type = 1;
	in_portL.payload = 8;
	in_portL.destiny = 4;


	router rt("ROUTER");
	rt.clk(clock);

	rt.in_val[4].write(1);

	rt.tabela = table0;

	rt.in_portN = in_portN;
	rt.in_portE = in_portE;
	rt.in_portS = in_portS;
	rt.in_portW = in_portW;
	rt.in_portL = in_portL;

	out_portN = rt.out_portN;
	out_portE = rt.out_portE;
	out_portS = rt.out_portS;
	out_portW = rt.out_portW;
	out_portL = rt.out_portL;


	sc_start();

	//cout << in_portL.destiny << endl;

}