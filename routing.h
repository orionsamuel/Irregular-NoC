#include "systemc.h"
#include "parameters.h"

using namespace std;

SC_MODULE(routing){
	sc_int<32> coreNumbers;
	sc_int<32> size;
	sc_int<32> counter;
	routing_table tabela;
	sc_in<bool> clk;
	sc_in<sc_int<32> > destiny; //Entra o endereço do roteador destino
	sc_out<sc_int<32> > portDestiny; //Retorna a porta por onde o pacote deve passar
	
	

	
	void returnSize(){
		this->size = tabela.size();
	}
	void tableAcess(){
		for(int i = 0; i < this->size; i++){
			if(tabela[i].destiny == destiny.read()){
			if(tabela[i].hops < this->counter){
				this->counter = tabela[i].hops;	
			}
			}
		}
	}
	void setDestiny(){
		for(int i = 0; i < this->size; i++){
			if(tabela[i].hops == this->counter){
				portDestiny.write(tabela[i].port);
			}
		}
	}

	SC_CTOR(routing){
		counter = 50;
		SC_METHOD(tableAcess);
		sensitive << destiny << clk;
	}
	
};