#include "systemc.h"
#include <iostream>
#include "router3.h"
#include "parameters.h"


SC_MODULE(NoC){

	sc_in<bool> clk;

	int coreNumbers;

	router *rt[4];
	routing_table table[4];

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
				rt[i]->in_port[0] = rt[i+1]->out_port[2];
				rt[i]->in_val[0].write(rt[i+1]->out_val[2]);
				rt[i+1]->in_port[2] = rt[i]->out_port[0];
				rt[i+1]->in_val[2].write(rt[i]->out_val[0]);
			}else if(i == 0){
				rt[i]->in_port[0] = rt[i+1]->out_port[2];
				rt[i]->in_val[0].write(rt[i+1]->out_val[2]);
				rt[i+1]->in_port[2] = rt[i]->out_port[0];
				rt[i+1]->in_val[2].write(rt[i]->out_val[0]);
			}
		}
	}

	void map_rede(){
		for (int i = 0; i < coreNumbers; i++){
			rt[i]->tabela = table[i];
		}
	}

	void set_packets(){
		for(int i = 0; i < 4; i++){
			rt[i]->in_val[4].write(1);
			rt[i]->in_port[4].type = 1;
			rt[i]->in_port[4].payload = (8 + i);
			rt[i]->in_port[4].destiny = (i + 2);
			wait(3);
			rt[i]->in_port[4].type = 0;
		}

	}


	SC_CTOR(NoC){
		for(int i = 0; i < 4; i++){
			rt[i] = new router("rt");
			rt[i]->clk(clk);
		}

		SC_METHOD(chaveamento_externo);
		sensitive << clk.pos();
		SC_METHOD(map_rede);
		sensitive << clk.pos();
		SC_CTHREAD(set_packets, clk.pos());
	}
};

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS);

	routing_table table[4];




	NoC rede("NoC");
	rede.clk(clock);

	rede.rt[0]->position = 1;
	rede.rt[1]->position = 2;
	rede.rt[2]->position = 3;
	rede.rt[3]->position = 4;

	int coreNumbers = 4;

	rede.coreNumbers = coreNumbers;


	//Preenchimento das tabelas de roteamento
	for(int i = 0; i < coreNumbers; i++){
		rede.table[i].push_back({rede.rt[i]->position, LOCAL, 0});
		for(int j = 0; j < (coreNumbers - 1); j++){
			if(i == 0){
				rede.table[i].push_back({rede.rt[j+1]->position, NORTH, (j+1)});
			}else if(i == (coreNumbers - 1)){
				rede.table[i].push_back({rede.rt[j]->position, SOUTH, (coreNumbers - 1 - j)});
			}else if((i == 1) && (i != (coreNumbers - 1))){
				if(j == 0){
					rede.table[i].push_back({rede.rt[i-1]->position, SOUTH, 1});
				}else{
					rede.table[i].push_back({rede.rt[i+1]->position, EAST, j});
				}
			}else if(((i % 2) == 1) && (i != (coreNumbers - 1))){
				if(j < i){
					rede.table[i].push_back({rede.rt[j]->position, SOUTH, (i - j)});
				}else if((j > i) or (j == i)){
					rede.table[i].push_back({rede.rt[j+1]->position, EAST, (j - i + 1)});
				}
			}else if(((i % 2) == 0) && (i != (coreNumbers - 1))){
				if(j < i){
					rede.table[i].push_back({rede.rt[j]->position, WEST, (i - j)});
				}else if((j > i) or (j == i)){
					rede.table[i].push_back({rede.rt[j+1]->position, NORTH, (j - i + 1)});	
				}
			}			
		}	
	}

	

	sc_start(500, SC_NS);


	cout << rede.rt[0]->out_port[4].payload << endl;
	cout << rede.rt[1]->out_port[4].payload << endl;
	cout << rede.rt[2]->out_port[4].payload << endl;
	cout << rede.rt[3]->out_port[4].payload << endl;			

}