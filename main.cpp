#include "systemc.h"
#include <iostream>
#include "router.h"
#include "parameters.h"
#include "packet.h"

using namespace std;

SC_MODULE(NoC){

	sc_in<bool> clk;

	router *rt[4];
	

	void chaveamento_externo(){
		rt[1]->in_portN = rt[0]->out_port[1];
		rt[1]->in_val[0].write(rt[0]->out_val[1]);
		rt[3]->in_portE = rt[2]->out_port[2];
		rt[3]->in_val[1].write(rt[2]->out_val[2]); 
	}


	SC_CTOR(NoC){
		rt[0] = new router("rt");
		rt[1] = new router("rt");
		rt[2] = new router("rt");
		rt[3] = new router("rt");
		
		rt[0]->clk(clk);
		rt[1]->clk(clk);
		rt[2]->clk(clk);
		rt[3]->clk(clk);

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

	rede.rt[0]->position = 0;
	rede.rt[1]->position = 1;
	rede.rt[2]->position = 2;
	rede.rt[3]->position = 3;

	rede.rt[0]->in_val[4].write(1);

	rede.rt[0]->tabela = table[0];
	rede.rt[0]->in_portL.type = 1;
	rede.rt[0]->in_portL.payload = 8;
	rede.rt[0]->in_portL.destiny = 4;

	rede.rt[1]->tabela = table[1];

	rede.rt[2]->in_val[4].write(1);

	rede.rt[2]->tabela = table[2];
	rede.rt[2]->in_portL.type = 1;
	rede.rt[2]->in_portL.payload = 10;
	rede.rt[2]->in_portL.destiny = 5;

	rede.rt[3]->tabela = table[3];
	
	sc_start(500, SC_NS);

	cout << rede.rt[1]->out_port[3].destiny << endl;
	cout << rede.rt[3]->out_port[0].payload << endl;	

}