#ifndef _BUFFER_H_
#define _BUFFER_H_ 

#include <iostream>
#include "systemc.h"
#include "parameters.h"

using namespace std;


SC_MODULE (Buffer){

	
	
	fila_flits flits;

	flit din; //Entrada data + bop + eop
	flit dout; //Saída que vai para o roteamento

	sc_in<bool> clk{"clk"};

	sc_in < sc_int<32> > wr{"wr"}; //Entrada que vem do controle de fluxo
	sc_out< sc_int<32> > wok{"wok"}; //Saída que volta para o controle de fluxo
	sc_in < sc_int<32> > rd{"rd"}; //Entrada que vem do chaveamento
	sc_out< sc_int<32> > rok{"rok"}; //Saída do chaveamento
	
	sc_int<32> length;

	sc_int<32> destiny_flit;


	void add(){
		if ((wr.read() == 1) && (flits.size() == length)){
			wok.write(0); // error, value not added
		}else if((wr.read() == 1) && (flits.size() < length)){
			flits.push(din);
			wok.write(1);
		}		
	}

	void remove(){
		if (flits.size() < 1){
			rok.write(0); // queue is empty. Nothing to remove
		} else if (rd.read() == 1){
			rok.write(1);
			dout = flits.front();
			flits.pop();
		}
	}

	//Se estiver Vazio retorna 1, senão retorna 0
	int isEmpty(){
		return (flits.empty() == 1) ? 1: 0;
	}

	SC_CTOR(Buffer) {
		length = 50;
		destiny_flit = dout.destiny;
        SC_METHOD(add);
        sensitive << wr << clk.pos();
        SC_METHOD(remove);
        sensitive << rd << clk.pos();
    }

	
};
#endif