#include "systemc.h"
#include <iostream>
#include "router.h"
#include "parameters.h"
#include <fstream>
#include <string>


using namespace std;

SC_MODULE(NoC){

	sc_clock clock;

	router *rt[25];

	int coreNumbers;

	routing_table table[25];
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

	packet pkt[10000][10000];

	
	trafego_rede trafego;

	void read_file(){
		ifstream arquivoTrafego;

		//Instanciamento do arquivo de trafego
		arquivoTrafego.open("teste01.txt", ios_base::in);
		if (arquivoTrafego.is_open()){
			arquivoTrafego.getline(linha, 100);
			this->coreNumbers = atoi(linha);
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
	}


	void chaveamento_externo(){
		for(int i = 0; i < 24; i++){
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
	}

	void table_full(){
		//Preenchimento das tabelas de roteamento
		for(int i = 0; i < this->coreNumbers; i++){
			table[i].push_back({rt[i]->position, LOCAL, 0});
			for(int j = 0; j < (this->coreNumbers - 1); j++){
				if(i == 0){
					table[i].push_back({rt[j+1]->position, SOUTH, (j+1)});
				}else if(i == (this->coreNumbers - 1)){
					table[i].push_back({rt[j]->position, NORTH, (coreNumbers - 1 - j)});
				}else if((i == 1) && (i != (this->coreNumbers - 1))){
					if(j == 0){
						table[i].push_back({rt[i-1]->position, NORTH, 1});
					}else{
						table[i].push_back({rt[i+1]->position, EAST, j});
					}
				}else if(((i % 2) == 1) && (i != (this->coreNumbers - 1))){
					if(j < i){
						table[i].push_back({rt[j]->position, NORTH, (i - j)});
					}else if((j > i) or (j == i)){
						table[i].push_back({rt[j+1]->position, EAST, (j - i + 1)});
					}
				}else if(((i % 2) == 0) && (i != (this->coreNumbers - 1))){
					if(j < i){
						table[i].push_back({rt[j]->position, WEST, (i - j)});
					}else if((j > i) or (j == i)){
						table[i].push_back({rt[j+1]->position, SOUTH, (j - i + 1)});	
					}
				}			
			}	
		}
	}

	
	void map_rede(){
		for (int i = 0; i < this->coreNumbers; i++){
			rt[i]->tabela = table[i];
		}
	}


	void set_destiny(){
		//Seta o destino dos pacotes
		for(int i = 0; i < trafego.size(); i++){
			for(int j = 0; j < trafego[0].pacotes; j++){
				pkt[i][j].destiny = trafego[i].destino;
			}
		}
	}

	void packet_create(){
		//Cria os pacotes
		for(int k = 0; k < trafego.size(); k++){
			for (int i = 0; i < trafego[0].pacotes; i++){
				for(int j = 0; j < SIZE_FLIT_PACKET; j++){
					if(j == 0){
						pkt[k][i].flit_packet[j].type = BEGIN_PACKET;
					}else if(j == SIZE_FLIT_PACKET - 1){
						pkt[k][i].flit_packet[j].type = END_PACKET;
					}else{
						pkt[k][i].flit_packet[j].type = INSTRUCTION;
						pkt[k][i].flit_packet[j].payload = 10;
					}
					pkt[k][i].flit_packet[j].destiny = pkt[k][i].destiny;
				}
			}
		}	
	}

	void set_packets(){
		//Seta os pacotes para o trafego
		for(int h = 0; h < this->coreNumbers; h++){
			for(int i = 0; i < trafego.size(); i++){
				for(int j = 0; j < trafego[0].pacotes; j++){
					for(int k = 0; k < SIZE_FLIT_PACKET; k++){
						if(rt[h]->position == trafego[i].origem){
							rt[h]->in_val[4].write(1);
							rt[h]->in_port[4] = pkt[i][j].flit_packet[k];
						}
					}
				}
			}	
		}
	}

	SC_CTOR(NoC): clock("Clock", 10, SC_NS){
		
		for(int i = 0; i < 25; i++){
			rt[i] = new router("rt");
			rt[i]->clk(clock);
		}

		//Seta as posições dos roteadores
		for(int i = 0; i < this->coreNumbers; i++){
			rt[i]->position = i;
		}

		SC_METHOD(chaveamento_externo);
		sensitive << clock;
		SC_METHOD(map_rede);
		sensitive << clock;
		SC_METHOD(read_file);
		SC_METHOD(table_full);
		SC_METHOD(set_destiny);
		SC_METHOD(packet_create);

	}
};

using namespace std;

int sc_main (int argc, char* argv[]){

	
	NoC* rede = new NoC( "rede" );		
	
	sc_start(rede->trafego[1].deadline, SC_NS);

	cout << rede->rt[1]->out_port[4].type << endl;
	cout << rede->rt[3]->out_port[4].type << endl;
}