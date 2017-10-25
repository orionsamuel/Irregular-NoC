#include "systemc.h"
#include <iostream>

using namespace std;

SC_MODULE(flow_control){
	sc_in<bool> clk{"clk"};
	sc_in<sc_int<32> > in_val{"in_val"}; //Entrada pra ver se no buffer terá espaço 
	sc_out<sc_int<32> > out_ack{"out_ack"}; //Retorno da solicitação se terá espaço

	sc_in<sc_int<32> > wok{"wok"}; //Entrada que vem do buffer
	sc_out<sc_int<32> > wr{"wr"}; //Saída que vai para o buffer
	
	void request(){
		if(in_val.read() == 1){
		wr.write(1);
		}
		//cout << "in_val: " << in_val.read() << endl;
		//cout << "wr: " << wr << endl;
	}
	void response(){
		if(wok.read() == 1){
		out_ack.write(1);
		}
		//cout << "wok: " << wok.read() << endl;
		//cout << "out_ack: " << out_ack << endl;
	}

	SC_CTOR(flow_control){
		SC_METHOD(request);
		sensitive << in_val << clk.pos();
		SC_METHOD(response);
		sensitive << wok << clk.pos();
	}
	
};
