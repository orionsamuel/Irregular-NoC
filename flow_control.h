#include "systemc.h"
#include <iostream>

using namespace std;

SC_MODULE(flow_control){
	sc_in<sc_int<32> > in_val; //Entrada pra ver se no buffer terá espaço 
	sc_out<sc_int<32> > in_ack; //Retorno da solicitação se terá espaço

	sc_in<sc_int<32> > wok; //Entrada que vem do buffer
	sc_out<sc_int<32> > wr; //Saída que vai para o buffer
	
	void request(){
		if(in_val.read() == 1){
		wr.write(1);
		}
	}
	void response(){
		if(wok.read() == 1){
		in_ack.write(1);
		}
	}

	SC_CTOR(flow_control){
		flow_control flow("flow");
		SC_METHOD(request);
		sensitive << in_val;
		SC_METHOD(response);
		sensitive << wok;
	}
	
};
