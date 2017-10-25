#include "systemc.h"
#include "parameters.h"
#include <iostream>

using namespace std;

SC_MODULE(routing){
	sc_int<32> counter;
	routing_table tabela;
	sc_in<bool> clk{"clk"};
	sc_in<sc_int<32> > destiny{"destiny"}; //Entra o endereço do roteador destino
	sc_out<sc_int<32> > portDestiny{"portDestiny"}; //Retorna a porta por onde o pacote deve passar
	
	

	void tableAcess(){
		for(int i = 0; i < tabela.size(); i++){
			if(tabela[i].destiny == destiny.read()){
				if(tabela[i].hops < this->counter){
					this->counter = tabela[i].hops;	
				}
			}
		}
	}

	void setDestiny(){
		for(int i = 0; i < tabela.size(); i++){
			if(tabela[i].hops == this->counter){
				portDestiny.write(tabela[i].port);
			}
		}
		
	}

	SC_CTOR(routing){
		counter = 50;
		SC_METHOD(tableAcess);
		sensitive << destiny << clk.pos();
		SC_METHOD(setDestiny);
		sensitive << clk.pos();
	}
	
};