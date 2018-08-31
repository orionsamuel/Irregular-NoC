#include "systemc.h"
#include <iostream>
#include <cstdlib>
#include <ctime>
#include "router_2.h"
#include "caminho_min.h"
#include "parameters.h"


SC_MODULE(NoC){

	sc_in<bool> clk;

	int coreNumbers;
	int prioridade;

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
		//srand((unsigned)time(NULL));
		for(int i = 0; i < coreNumbers; i++){
			for(int j = 0; j < trafego.size(); j++){
				if(rt[i]->position == trafego[j].origem){
					rt[i]->in_val[4].write(1);
					for(int k = 0; k < trafego[j].pacotes; k++){
						prioridade = 1 + (rand() % 2);
						for(int l = 0; l < 5; l++){
							if(l == 0){
								rt[i]->in_port[4].type = BEGIN_PACKET;
								rt[i]->in_port[4].payload = (8 + i);
								rt[i]->in_port[4].destiny = trafego[j].destino;
								rt[i]->in_port[4].priority = prioridade;
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
		for(int i = 0; i < 25; i++){
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
	int dist[10][10];
	int caminho[10][10];
	caminho_min cam;

	trafego_rede trafego;

	float deadline_parcial, deadline;
	int total_packets;
	sc_time latencia_parcial, latencia_media;

	ifstream arquivoTrafego, leitura;
	ofstream saidaDados;


	//Instanciamento do arquivo de trafego
	arquivoTrafego.open("01.txt", ios_base::in);
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

	arquivoTrafego.close();



	rede.coreNumbers = coreNumbers;
	rede.trafego = trafego;


	for(int i = 0; i < coreNumbers; i++){
		rede.rt[i]->position = i + 1;
	}

	//Preenchimento da matriz de adjacência
	for(int i = 0; i < coreNumbers; i++){
		for(int j = 0; j < coreNumbers; j++){
			if (i == j){
				dist[i][j] = 0;
			}else if(i == 0){
				dist[i][i + 1] = 1;
			}else if(i == coreNumbers){
				dist[coreNumbers][coreNumbers - 1] = 1;
			}else if((i > 0) && (i < coreNumbers)){
				dist[i][i - 1] = 1;
				dist[i][i + 1] = 1;
			}
		}
	}

	for(int i = 0; i < coreNumbers; i++){
		for(int j = 0; j < coreNumbers; j++){
			if((i != j) && (dist[i][j] != 1)){
				dist[i][j] = 100;
			}
		}
	}

	//Passagem dos valores das matrizes pro caminho mínimo

	for(int i = 0; i < coreNumbers; i++){
		for(int j = 0; j < coreNumbers; j++){
			//cout << dist[i][j] << " ";
        }
        //cout << endl;
	}
	
	cam.floyd(dist, caminho);


	//Preenchimento das tabelas de roteamento
	for(int i = 0; i < coreNumbers; i++){
		for(int j = 0; j < coreNumbers; j++){
			if(i == j){
				rede.table[i].push_back({j, LOCAL, dist[i][j]});
			}else if(i == 0){
				rede.table[i].push_back({j, NORTH, dist[i][j]});
			}else if(i == coreNumbers){
				rede.table[i].push_back({j, SOUTH, dist[i][j]});
			}else if((i == 1) && (i != coreNumbers)){
				if(j == 0){
					rede.table[i].push_back({j, SOUTH, dist[i][j]});
				}else{
					rede.table[i].push_back({j, EAST, dist[i][j]});
				}
			}else if(((i % 2) == 1) && (i != coreNumbers)){
				if(j < i){
					rede.table[i].push_back({j, SOUTH, dist[i][j]});
				}else if((j > i) or (j == i)){
					rede.table[i].push_back({j, EAST, dist[i][j]});
				}
			}else if(((i % 2) == 0) && (i != coreNumbers)){
				if(j < i){
					rede.table[i].push_back({j, WEST, dist[i][j]});
				}else if((j > i) or (j == i)){
					rede.table[i].push_back({j, NORTH, dist[i][j]});	
				}
			}
		}
	}

	//Execução da simulação
	sc_start(trafego[0].deadline, SC_NS);

	//Calculo dos pacotes que chegaram ao destino
	for(int i = 0; i < trafego.size(); i++){
		total_packets = total_packets + trafego[i].pacotes;
	}

	for(int i = 0; i < coreNumbers; i++){
		deadline_parcial = deadline_parcial + rede.rt[i]-> count;
	}
	
	for(int i = 0; i < coreNumbers; i++){
		latencia_parcial = latencia_parcial + rede.rt[i]->latencia_rt;
	}

	deadline = (deadline_parcial * 100) / total_packets;
	latencia_media = (latencia_parcial / total_packets);

	saidaDados.open("saida_simulacao.txt");
	saidaDados << deadline << endl;
	saidaDados << latencia_media;
	saidaDados.close();

	/*for(int i = 0; i < coreNumbers; i++){
		cout << rede.rt[0]->tabela[i].destiny << " " << rede.rt[0]->tabela[i].port << " " << rede.rt[0]->tabela[i].hops << endl;
	}*/

	/*for (int i = 0; i < coreNumbers; i++){
        for (int j = 0; j < coreNumbers; j++){
            cout << dist[i][j] << " ";
        }
        cout << endl;
    }*/

	cout << rede.rt[0]->out_port[4].payload << endl;
	cout << rede.rt[1]->out_port[4].payload << endl;
	cout << rede.rt[2]->out_port[4].payload << endl;
	cout << rede.rt[3]->out_port[4].payload << endl;

}