#include "router.h"
#include "parameters.h"
#include "packet.h"
#include <iostream>

using namespace std;

SC_MODULE(NoC){

	sc_clock clk; 

	router *r0, *r1, *r2;
	packet pkt;

	

	SC_CTOR(NoC):

		clk ("clock", 10, SC_NS){

		pkt.destiny = 2;			
			
		r0 = new router("r0");
		r1 = new router("r1");
		r2 = new router("r2");

		//Mapeamento dos roteadores
		r0->position = 0;
		r1->position = 1;
		r2->position = 2;

		for (int i = 0; i < SIZE_FLIT_PACKET; i++){
			r0->in_portL = pkt.flit_packet[i];
		}

		r0->clk(clk);
		r1->clk(clk);
		r2->clk(clk);

		r0->coreNumbers = 3;
		r1->coreNumbers = 3;
		r2->coreNumbers = 3;

		r0->out_portW = r1->in_portN;
		r0->out_portE = r2->in_portN;
		r1->in_val_N(r0->out_val_W);
		r2->in_val_N(r0->out_val_E);

		r1->out_portN = r0->in_portW;
		r1->out_portE = r2->in_portW;
		r0->in_val_W(r0->out_val_N);
		r2->in_val_W(r0->out_val_E);

		r2->out_portN = r0->in_portE;
		r2->out_portW = r1->in_portE;
		r0->in_val_E(r0->out_val_N);
		r1->in_val_E(r0->out_val_W);

		routing_table table0;
		routing_table table1;
		routing_table table2;

		table0.push_back({1, WEST, 1});
		table0.push_back({1, EAST, 2});
		table0.push_back({2, WEST, 2});
		table0.push_back({1, EAST, 1});
		table1.push_back({0, NORTH, 1});
		table1.push_back({0, EAST, 2});
		table1.push_back({2, NORTH, 2});
		table1.push_back({2, EAST, 1});
		table2.push_back({0, NORTH, 1});
		table2.push_back({0, WEST, 2});
		table2.push_back({1, NORTH, 2});
		table2.push_back({1, WEST, 1});

		r0->tabela = table0;
		r1->tabela = table1;
		r2->tabela = table2;

		cout << r2->out_portL.destiny << endl;
	}

};

int sc_main (int argc, char* argv[]){
	
	NoC *rede();
	sc_start(10000, SC_NS);

	return 0;
}
