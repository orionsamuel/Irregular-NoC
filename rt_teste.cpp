#include "systemc.h"
#include <iostream>
#include "router.h"
#include "parameters.h"


SC_MODULE(NoC){

	sc_in<bool> clk;

	router *rt[4];

	int coreNumbers;

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


	SC_CTOR(NoC){
		for(int i = 0; i < 4; i++){
			rt[i] = new router("rt");
			rt[i]->clk(clk);
		}

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



	NoC *rede;
	rede = new NoC("rede");
	rede->clk(clock);

	int coreNumbers = 4;

	rede->coreNumbers = coreNumbers;

	//Seta as posições dos roteadores
	for(int i = 0; i < coreNumbers; i++){
		rede->rt[i]->position = i;
	}


	//Preenchimento das tabelas de roteamento
	for(int i = 0; i < coreNumbers; i++){
		rede->table[i].push_back({rede->rt[i]->position, LOCAL, 0});
		for(int j = 0; j < (coreNumbers - 1); j++){
			if(i == 0){
				rede->table[i].push_back({rede->rt[j+1]->position, NORTH, (j+1)});
			}else if(i == (coreNumbers - 1)){
				rede->table[i].push_back({rede->rt[j]->position, SOUTH, (coreNumbers - 1 - j)});
			}else if((i == 1) && (i != (coreNumbers - 1))){
				if(j == 0){
					rede->table[i].push_back({rede->rt[i-1]->position, SOUTH, 1});
				}else{
					rede->table[i].push_back({rede->rt[i+1]->position, EAST, j});
				}
			}else if(((i % 2) == 1) && (i != (coreNumbers - 1))){
				if(j < i){
					rede->table[i].push_back({rede->rt[j]->position, SOUTH, (i - j)});
				}else if((j > i) or (j == i)){
					rede->table[i].push_back({rede->rt[j+1]->position, EAST, (j - i + 1)});
				}
			}else if(((i % 2) == 0) && (i != (coreNumbers - 1))){
				if(j < i){
					rede->table[i].push_back({rede->rt[j]->position, WEST, (i - j)});
				}else if((j > i) or (j == i)){
					rede->table[i].push_back({rede->rt[j+1]->position, NORTH, (j - i + 1)});	
				}
			}			
		}	
	}


	rede->rt[0]->in_val[4].write(1);

	rede->rt[0]->in_port[4].type = 1;
	rede->rt[0]->in_port[4].payload = 8;
	rede->rt[0]->in_port[4].destiny = 2;

	/*rede.rt[2]->in_val[4].write(1);

	rede.rt[2]->in_port[4].type = 1;
	rede.rt[2]->in_port[4].payload = 10;
	rede.rt[2]->in_port[4].destiny = 3;*/
	
	sc_start(500, SC_NS);

	//cout << rede.rt[2]->out_port[4].destiny << endl;
	cout << rede->rt[1]->rtgS->portDestiny << endl;	

}