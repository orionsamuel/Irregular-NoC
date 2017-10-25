#include "systemc.h"
#include "parameters.h"
#include "flow_control.h"
#include "buffer.h"
#include "routing.h"
#include "arbiter.h"
#include <vector>
#include <iostream>

using namespace std;


SC_MODULE(router){
	//Posição em que o roteador se encontra
	sc_int<32> position;

	sc_in<bool> clk{"clk"};

	//Portas do Roteador;
	flit in_portN; 
	flit in_portE;
	flit in_portS;
	flit in_portW;
	flit in_portL;

	flit out_portN;
	flit out_portE;
	flit out_portS;
	flit out_portW;
	flit out_portL;

	//Tabela de roteamento
	routing_table tabela;

	//Componentes do roteador
	flow_control *fcN, *fcE, *fcS, *fcW, *fcL;
	Buffer *bfN, *bfE, *bfS, *bfW, *bfL;
	routing *rtgN, *rtgE, *rtgS, *rtgW, *rtgL;
	arbiter *arbN, *arbE, *arbS, *arbW, *arbL;


	//Portas de comunicação entre roteadores
	sc_signal<sc_int<32> > in_val[5];

	sc_signal<sc_int<32> > out_val[5];

	sc_signal<sc_int<32> > in_ack[5];

	sc_signal<sc_int<32> > out_ack[5];

	//Sinais para verificação nos buffers
	sc_signal<sc_int<32> > wok[5];
	sc_signal<sc_int<32> > wr[5];
	
	//Sinais para o chaveamento dos buffers
	sc_signal<sc_int<32> > rd[5];
	sc_signal<sc_int<32> > rok[5];

	void execute(){
		if (fcN->in_val.read() == 1)
		{
		cout << "norte" << endl;
		fcN->wr.write(1);
		sc_start();
		if (bfN->wr.read() == 1)
			bfN->wok.write(buffer_leste->isEmpty());
		sc_start();
		if (cf_leste->in_cf_buffer.read() == 1)
			cf_leste->in_ack.write(1);	
		} 

	}


	SC_CTOR(router){
		//Instanciando o controle de fluxo
		fcN = new flow_control("fcN");
		fcE = new flow_control("fcE");
		fcS = new flow_control("fcS");
		fcW = new flow_control("fcW");
		fcL = new flow_control("fcL");

		//Instanciando os buffers
		bfN = new Buffer("bfN");
		bfE = new Buffer("bfE");
		bfS = new Buffer("bfS");
		bfW = new Buffer("bfW");
		bfL = new Buffer("bfL");

		//Instanciando o roteamento
		rtgN = new routing("rtgN");
		rtgE = new routing("rtgE");
		rtgS = new routing("rtgS");
		rtgW = new routing("rtgW");
		rtgL = new routing("rtgL");

		//Instanciando os árbitros
		arbN = new arbiter("arbN");
		arbE = new arbiter("arbE");
		arbS = new arbiter("arbS");
		arbW = new arbiter("arbW");
		arbL = new arbiter("arbL");



		SC_METHOD(execute);
		sensitive << clk.pos();

	}
};