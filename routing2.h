#include "systemc.h"
#include "parameters.h"
#include <iostream>

using namespace std;

SC_MODULE(routing){
	sc_int<32> counter;
	routing_table tabela;
	sc_signal<sc_int<32> > destiny; //Entra o endereço do roteador destino
	int portDestiny; //Retorna a porta por onde o pacote deve passar
	
	

	int tableAcess(){
		for(int i = 0; i < tabela.size(); i++){
			if(tabela[i].destiny == destiny.read()){
				portDestiny = tabela[i].port;
			}
		}
		return portDestiny;
	}


	SC_CTOR(routing){

	}	
};