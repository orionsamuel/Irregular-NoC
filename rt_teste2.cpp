#include "systemc.h"
#include <iostream>
#include "router3.h"
#include "parameters.h"


SC_MODULE(NoC){

	sc_in<bool> clk;

	int coreNumbers;

	trafego_rede trafego;

	router *rt[25];
	routing_table table[25];

	void chaveamento_externo(){
		for(int i = 0; i < (coreNumbers - 1); i++){
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
		for(int i = 0; i < coreNumbers; i++){
			for(int j = 0; j < trafego.size(); j++){
				if(rt[i]->position == trafego[j].origem){
					rt[i]->in_val[4].write(1);
					for(int k = 0; k < trafego[j].pacotes; k++){
						for(int l = 0; l < 5; l++){
							if(l == 0){
								rt[i]->in_port[4].type = BEGIN_PACKET;
								rt[i]->in_port[4].payload = (8 + i);
								rt[i]->in_port[4].destiny = trafego[j].destino;
								wait(3);
								rt[i]->in_port[4].type = 0;
							}else if(l == 4){
								rt[i]->in_port[4].type = END_PACKET;
								rt[i]->in_port[4].payload = (8 + i);
								rt[i]->in_port[4].destiny = trafego[j].destino;
								wait(3);
								rt[i]->in_port[4].type = 0;
							}else{
								rt[i]->in_port[4].type = INSTRUCTION;
								rt[i]->in_port[4].payload = (8 + i);
								rt[i]->in_port[4].destiny = trafego[j].destino;
								wait(3);
								rt[i]->in_port[4].type = 0;
							}
						}
					}
				}
			}
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

	NoC rede("NoC");
	rede.clk(clock);

	int coreNumbers;
	char linha[100];
	string temp0;
	string temp1;
	string temp2;
	string temp3;
	int temp00;
	int temp01;
	int temp02;
	int temp03;
	int count;

	trafego_rede trafego;

	ifstream arquivoTrafego;


	//Instanciamento do arquivo de trafego
	arquivoTrafego.open("teste01.txt", ios_base::in);
	if (arquivoTrafego.is_open()){
		arquivoTrafego.getline(linha, 100);
		coreNumbers = atoi(linha);
		while(arquivoTrafego.getline(linha, 100)){
			temp0 = "";
			temp1 = "";
			temp2 = "";
			temp3 = "";
			for(count = 0; count < 100; count++){
				if(linha[count] != ' '){
					temp0 = temp0 + linha[count];
				}else{
					break;
				}
			}
			temp00 = atoi(temp0.c_str());
			count = count + 1;

			for(; count < 100; count ++){
				if(linha[count] != ' '){
					temp1 = temp1 + linha[count];
				}else{
					break;
				}
			}
			temp01 = atoi(temp1.c_str());
			count = count + 1;

			for(; count < 100; count ++){
				if(linha[count] != ' '){
					temp2 = temp2 + linha[count];
				}else{
					break;
				}
			}
			temp02 = atoi(temp2.c_str());
			count = count + 1;

			for(; count < 100; count ++){
				if(linha[count] != ' '){
					temp3 = temp3 + linha[count];
				}else{
					break;
				}
			}
			temp03 = atoi(temp3.c_str());

			trafego.push_back({temp00, temp01, temp02, temp03});
		}
	}



	rede.coreNumbers = coreNumbers;
	rede.trafego = trafego;


	for(int i = 0; i < coreNumbers; i++){
		rede.rt[i]->position = i + 1;
	}


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
					rede.table[i].push_back({rede.rt[j+1]->position, EAST, j});
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

	

	sc_start(trafego[1].deadline, SC_NS);


	cout << rede.rt[0]->out_port[4].payload << endl;
	cout << rede.rt[1]->out_port[4].payload << endl;
	cout << rede.rt[2]->out_port[4].payload << endl;
	cout << rede.rt[3]->out_port[4].payload << endl;

}