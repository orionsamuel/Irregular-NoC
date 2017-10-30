#include "systemc.h"
#include <iostream>
#include "router.h"
#include "parameters.h"


using namespace std;

SC_MODULE(NoC){

	sc_in<bool> clk;

	router *rt[4];

	routing_table table[4];

	packet pkt[2];	

	

	void chaveamento_externo(){
		for(int i = 0; i < 3; i++){
			if(i == 1){
				rt[i]->in_port[1] = rt[i+1]->out_port[3];
				rt[i]->in_val[1].write(rt[i+1]->out_val[3]);
				rt[i+1]->in_port[3] = rt[i]->out_port[1];
				rt[i+1]->in_val[3].write(rt[i]->out_val[1]);
			}else if((i % 2) == 1){
				rt[i]->in_port[1] = rt[i+1]->out_port[3];
				rt[i]->in_val[1].write(rt[i+1]->out_val[3]);
				rt[i+1]->in_port[3] = rt[i]->out_port[1];
				rt[i+1]->in_val[3].write(rt[i]->out_val[1]);
			}else if((i % 2) == 0){
				rt[i]->in_port[2] = rt[i+1]->out_port[0];
				rt[i]->in_val[2].write(rt[i+1]->out_val[0]);
				rt[i+1]->in_port[0] = rt[i]->out_port[2];
				rt[i+1]->in_val[0].write(rt[i]->out_val[2]);
			}else if(i == 0){
				rt[i]->in_port[2] = rt[i+1]->out_port[0];
				rt[i]->in_val[2].write(rt[i+1]->out_val[0]);
				rt[i+1]->in_port[0] = rt[i]->out_port[2];
				rt[i+1]->in_val[0].write(rt[i]->out_val[2]);
			}
		}


		rt[1]->in_port[0] = rt[0]->out_port[1];
		rt[1]->in_val[0].write(rt[0]->out_val[1]);
		rt[3]->in_port[1] = rt[2]->out_port[2];
		rt[3]->in_val[1].write(rt[2]->out_val[2]); 
	}

	
	void map_rede(){
		for (int i = 0; i < 4; i++){
			rt[i]->position = i;
			rt[i]->tabela = table[i];
		}
	}

	/*void packet_injection(){
		for(int i = 0; i < 2; i++){
			for(int j = 0; j < SIZE_FLIT_PACKET; j++){
				rt[0]->in_val[4].write(1);
				rt[0]->in_portL = pkt[0].flit_packet[j];
				rt[2]->in_val[4].write(1);
				rt[2]->in_portL = pkt[1].flit_packet[j];
				//wait(10, SC_NS);
				cout << rt[0]->out_por[3].destiny << endl;
			}
		}
	}*/


	SC_CTOR(NoC){
		
		for(int i = 0; i < 4; i++){
			rt[i] = new router("rt");
			rt[i]->clk(clk);
		}

		//SC_CTHREAD(packet_injection, clk.pos());
		SC_METHOD(chaveamento_externo);
		sensitive << clk.pos();
		SC_METHOD(map_rede);
		sensitive << clk.pos();

	}
};

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS);

	routing_table table[4];

	int coreNumbers = 4;

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

	packet pkt[2];

	NoC rede("NoC");
	rede.clk(clock);

	for(int i = 0; i < coreNumbers; i++){
		rede.table[i].push_back({rede.rt[i]->position, LOCAL, 0});
		for(int j = 0; j < ((coreNumbers - 1) * 2); j++){
			if(i == 0){
				rede.table[i].push_back({rede.rt[i+1]->position, SOUTH, 1});
			}else if((i % 2) == 0){
				rede.table[i].push_back({rede.rt[i+1]->position, SOUTH, 1});
			}			
		}	
	}
	

	/*for(int i = 0; i < 4; i++){
		rede.table[i] = table[i];
	}*/

	pkt[0].destiny = 4;
	pkt[1].destiny = 5;

	for (int i = 0; i < 2; i++){
		for(int j = 0; j < SIZE_FLIT_PACKET; j++){
			if(j == 0){
				pkt[i].flit_packet[j].type = BEGIN_PACKET;
			}else if(j == SIZE_FLIT_PACKET - 1){
				pkt[i].flit_packet[j].type = END_PACKET;
			}else{
				pkt[i].flit_packet[j].type = INSTRUCTION;
				pkt[i].flit_packet[j].payload = 10;
			}
			pkt[i].flit_packet[j].destiny = pkt[i].destiny;
		}
	}

	for(int i = 0; i < 2; i++){
		rede.pkt[i] = pkt[i];
	}
	
	for(int i = 0; i < 2; i++){
		for(int j = 0; j < SIZE_FLIT_PACKET; j++){
			rede.rt[0]->in_val[4].write(1);
			rede.rt[0]->in_port[4] = pkt[0].flit_packet[j];
			rede.rt[2]->in_val[4].write(1);
			rede.rt[2]->in_port[4] = pkt[1].flit_packet[j];
		}
	}
	
	sc_start(1000, SC_NS);

	cout << rede.rt[1]->out_port[3].type << endl;
	cout << rede.rt[3]->out_portN.type << endl;	;

}