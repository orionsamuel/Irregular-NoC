#include "systemc.h"
#include <iostream>
#include "router.h"
#include "parameters.h"


SC_MODULE(NoC){

	sc_in<bool> clk;

	router *rt0, *rt1;
	routing_table table0, table1;

	void chaveamento_externo(){
		rt1->in_portN = rt0->out_portE;
		rt1->in_val[0].write(rt0->out_val[1]);
	}


	SC_CTOR(NoC){
		rt0 = new router("rt0");
		rt1 = new router("rt1");

		rt0->clk(clk);
		rt1->clk(clk);

		SC_METHOD(chaveamento_externo);
		sensitive << clk.pos();

	}
};

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS);

	routing_table table0, table1;

	table0.push_back({3, WEST, 5});
	table0.push_back({4, SOUTH, 2});
	table0.push_back({3, NORTH, 8});
	table0.push_back({4, EAST, 1});
	table0.push_back({0, LOCAL, 0});

	table1.push_back({3, NORTH, 5});
	table1.push_back({4, EAST, 2});
	table1.push_back({3, SOUTH, 8});
	table1.push_back({4, WEST, 1});
	table1.push_back({1, LOCAL,0});


	NoC rede("NoC");
	rede.clk(clock);

	rede.rt0->position = 0;
	rede.rt1->position = 1;

	rede.rt0->in_val[4].write(1);

	rede.rt0->tabela = table0;
	rede.rt0->in_portL.type = 1;
	rede.rt0->in_portL.payload = 8;
	rede.rt0->in_portL.destiny = 4;

	rede.rt1->tabela = table1;
	
	sc_start(500, SC_NS);

	cout << rede.rt1->out_portW.destiny << endl;	

}