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

	sc_in < sc_int<32> > wr; //Entrada que vem do controle de fluxo
	sc_out< sc_int<32> > wok; //Saída que volta para o buffer
	sc_in < sc_int<32> > rd; //Entrada que vem do chaveamento
	sc_out< sc_int<32> > rok; //Saída ?
	
	sc_int<32> length;


	void add(){
		if (wr.read() == 1){
		if (flits.size() == length){
			wok.write(0); // error, value not added
		}else {
			flits.push(din);
			wok.write(1);
		}	
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
        SC_THREAD(add);
        SC_THREAD(remove);
        sensitive << wr << rd;
    }

	
};
#endif