#include "systemc.h"
#include <iostream>
#include "router.h"
#include "parameters.h"


SC_MODULE(NoC){

	sc_in<bool> clk;

	router *rt0, *rt1, *rt2, *rt3;
	routing_table table0, table1;

	void chaveamento_externo(){
		rt1->in_portN = rt0->out_port[1];
		rt1->in_val[0].write(rt0->out_val[1]);
		rt3->in_portE = rt2->out_port[2];
		rt3->in_val[1].write(rt2->out_val[2]); 
	}


	SC_CTOR(NoC){
		rt0 = new router("rt0");
		rt1 = new router("rt1");
		rt2 = new router("rt2");
		rt3 = new router("rt3");

		rt0->clk(clk);
		rt1->clk(clk);
		rt2->clk(clk);
		rt3->clk(clk);

		SC_METHOD(chaveamento_externo);
		sensitive << clk.pos();

	}
};

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS);

	routing_table table[4];

	table[0].push_back({3, WEST, 5});
	table[0].push_back({4, SOUTH, 2});
	table[0].push_back({3, NORTH, 8});
	table[0].push_back({4, EAST, 1});
	table[0].push_back({0, LOCAL, 0});

	table[1].push_back({3, NORTH, 5});
	table[1].push_back({4, EAST, 2});
	table[1].push_back({3, SOUTH, 8});
	table[1].push_back({4, WEST, 1});
	table[1].push_back({1, LOCAL,0});

	table[2].push_back({5, SOUTH, 5});
	table[2].push_back({4, EAST, 2});
	table[2].push_back({5, NORTH, 8});
	table[2].push_back({4, WEST, 1});
	table[2].push_back({2, LOCAL,0});

	table[3].push_back({4, NORTH, 5});
	table[3].push_back({5, NORTH, 2});
	table[3].push_back({5, SOUTH, 8});
	table[3].push_back({4, EAST, 1});
	table[3].push_back({3, LOCAL,0});


	NoC rede("NoC");
	rede.clk(clock);

	rede.rt0->position = 0;
	rede.rt1->position = 1;
	rede.rt2->position = 2;
	rede.rt3->position = 3;

	rede.rt0->in_val[4].write(1);

	rede.rt0->tabela = table[0];
	rede.rt0->in_portL.type = 1;
	rede.rt0->in_portL.payload = 8;
	rede.rt0->in_portL.destiny = 4;

	rede.rt1->tabela = table[1];

	rede.rt2->in_val[4].write(1);

	rede.rt2->tabela = table[2];
	rede.rt2->in_portL.type = 1;
	rede.rt2->in_portL.payload = 10;
	rede.rt2->in_portL.destiny = 5;

	rede.rt3->tabela = table[3];
	
	sc_start(500, SC_NS);

	cout << rede.rt1->out_port[3].destiny << endl;
	cout << rede.rt3->out_portN.destiny << endl;			

}