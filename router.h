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

	flit out_port[5];
	flit out_portN;

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


	sc_signal<sc_int<32> > portDestiny[5];

	

	void map_fc(){
		if(in_val[0].read() == 1){
			fcN->in_val.write(1);
		}
		if(in_val[1].read() == 1){
			fcE->in_val.write(1);
		}
		if(in_val[2].read() == 1){
			fcS->in_val.write(1);
		}
		if(in_val[3].read() == 1){
			fcW->in_val.write(1);
		}
		if(in_val[4].read() == 1){
			fcL->in_val.write(1);
		}


		if(fcN->out_ack.read() == 1){
			out_ack[0].write(1);
		}
		if(fcE->out_ack.read() == 1){
			out_ack[1].write(1);
		}
		if(fcS->out_ack.read() == 1){
			out_ack[2].write(1);
		}
		if(fcW->out_ack.read() == 1){
			out_ack[3].write(1);
		}
		if(fcL->out_ack.read() == 1){
			out_ack[4].write(1);
		}


		if(fcN->in_val.read() == 1){
			bfN->wr.write(1);
		}
		if(fcE->in_val.read() == 1){
			bfE->wr.write(1);
		}
		if(fcS->in_val.read() == 1){
			bfS->wr.write(1);
		}
		if(fcW->in_val.read() == 1){
			bfW->wr.write(1);
		}
		if(fcL->in_val.read() == 1){
			bfL->wr.write(1);
		}		


		if(bfN->wok.read() == 1){
			fcN->out_ack.write(1);
		}
		if(bfE->wok.read() == 1){
			fcE->out_ack.write(1);
		}
		if(bfS->wok.read() == 1){
			fcS->out_ack.write(1);
		}
		if(bfW->wok.read() == 1){
			fcW->out_ack.write(1);
		}
		if(bfL->wok.read() == 1){
			fcL->out_ack.write(1);
		}
		
	}

	void map_bf(){

		bfN->din = in_portN;
		bfE->din = in_portE;
		bfS->din = in_portS;
		bfW->din = in_portW;
		bfL->din = in_portL;

	}

	void map_rtg(){
		rtgN->tabela =  tabela;
		rtgE->tabela =  tabela;
		rtgS->tabela =  tabela;
		rtgW->tabela =  tabela;
		rtgL->tabela =  tabela;


		rtgN->destiny.write(bfN->din.destiny);
		rtgE->destiny.write(bfE->din.destiny);
		rtgS->destiny.write(bfS->din.destiny);
		rtgW->destiny.write(bfW->din.destiny);
		rtgL->destiny.write(bfL->din.destiny);

	
		if(rtgN->portDestiny.read() == NORTH){
			portDestiny[0].write(NORTH);
		}
		if(rtgN->portDestiny.read() == EAST){
			portDestiny[0].write(EAST);
		}
		if(rtgN->portDestiny.read() == SOUTH){
			portDestiny[0].write(SOUTH);
		}
		if(rtgN->portDestiny.read() == WEST){
			portDestiny[0].write(WEST);
		}
		if(rtgN->portDestiny.read() == LOCAL){
			portDestiny[0].write(LOCAL);
		}



		if(rtgE->portDestiny.read() == NORTH){
			portDestiny[1].write(NORTH);
		}
		if(rtgE->portDestiny.read() == EAST){
			portDestiny[1].write(EAST);
		}
		if(rtgE->portDestiny.read() == SOUTH){
			portDestiny[1].write(SOUTH);
		}
		if(rtgE->portDestiny.read() == WEST){
			portDestiny[1].write(WEST);
		}
		if(rtgE->portDestiny.read() == LOCAL){
			portDestiny[1].write(LOCAL);
		}



		if(rtgS->portDestiny.read() == NORTH){
			portDestiny[2].write(NORTH);
		}
		if(rtgS->portDestiny.read() == EAST){
			portDestiny[2].write(EAST);
		}
		if(rtgS->portDestiny.read() == SOUTH){
			portDestiny[2].write(SOUTH);
		}
		if(rtgS->portDestiny.read() == WEST){
			portDestiny[2].write(WEST);
		}
		if(rtgS->portDestiny.read() == LOCAL){
			portDestiny[2].write(LOCAL);
		}



		if(rtgW->portDestiny.read() == NORTH){
			portDestiny[3].write(NORTH);
		}
		if(rtgW->portDestiny.read() == EAST){
			portDestiny[3].write(EAST);
		}
		if(rtgW->portDestiny.read() == SOUTH){
			portDestiny[3].write(SOUTH);
		}
		if(rtgW->portDestiny.read() == WEST){
			portDestiny[3].write(WEST);
		}
		if(rtgW->portDestiny.read() == LOCAL){
			portDestiny[3].write(LOCAL);
		}



		if(rtgL->portDestiny.read() == NORTH){
			portDestiny[4].write(NORTH);
		}
		if(rtgL->portDestiny.read() == EAST){
			portDestiny[4].write(EAST);
		}
		if(rtgL->portDestiny.read() == SOUTH){
			portDestiny[4].write(SOUTH);
		}
		if(rtgL->portDestiny.read() == WEST){
			portDestiny[4].write(WEST);
		}
		if(rtgL->portDestiny.read() == LOCAL){
			portDestiny[4].write(LOCAL);
		}

	}

	void map_arb(){

		if(arbN->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbN->rd[1].read() == 1){
			bfE->rd.write(1);;
		}
		if(arbN->rd[2].read() == 1){
			bfS->rd.write(1);
		}
		if(arbN->rd[3].read() == 1){
			bfW->rd.write(1);
		}
		if(arbN->rd[4].read() == 1){
			bfL->rd.write(1);
		}


		if(arbE->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbE->rd[1].read() == 1){
			bfE->rd.write(1);;
		}
		if(arbE->rd[2].read() == 1){
			bfS->rd.write(1);
		}
		if(arbE->rd[3].read() == 1){
			bfW->rd.write(1);
		}
		if(arbE->rd[4].read() == 1){
			bfL->rd.write(1);
		}


		if(arbS->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbS->rd[1].read() == 1){
			bfE->rd.write(1);;
		}
		if(arbS->rd[2].read() == 1){
			bfS->rd.write(1);
		}
		if(arbS->rd[3].read() == 1){
			bfW->rd.write(1);
		}
		if(arbS->rd[4].read() == 1){
			bfL->rd.write(1);
		}


		if(arbW->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbW->rd[1].read() == 1){
			bfE->rd.write(1);;
		}
		if(arbW->rd[2].read() == 1){
			bfS->rd.write(1);
		}
		if(arbW->rd[3].read() == 1){
			bfW->rd.write(1);
		}
		if(arbW->rd[4].read() == 1){
			bfL->rd.write(1);
		}


		if(arbL->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbL->rd[1].read() == 1){
			bfE->rd.write(1);;
		}
		if(arbL->rd[2].read() == 1){
			bfS->rd.write(1);
		}
		if(arbL->rd[3].read() == 1){
			bfW->rd.write(1);
		}
		if(arbL->rd[4].read() == 1){
			bfL->rd.write(1);
		}
	}

	void request_arbiter(){
		if(portDestiny[0].read() == NORTH){
			arbN->bufferCircular[NORTH] = 1;
		}else if(portDestiny[0].read() == EAST){
			arbE->bufferCircular[NORTH] = 1;
		}else if(portDestiny[0].read() == SOUTH){
			arbS->bufferCircular[NORTH] = 1;
		}else if(portDestiny[0].read() == WEST){
			arbW->bufferCircular[NORTH] = 1;
		}else if(portDestiny[0].read() == LOCAL){
			arbL->bufferCircular[NORTH] = 1;
		}


		if(portDestiny[1].read() == NORTH){
			arbN->bufferCircular[EAST] = 1;
		}else if(portDestiny[1].read() == EAST){
			arbE->bufferCircular[EAST] = 1;
		}else if(portDestiny[1].read() == SOUTH){
			arbS->bufferCircular[EAST] = 1;
		}else if(portDestiny[1].read() == WEST){
			arbW->bufferCircular[EAST] = 1;
		}else if(portDestiny[1].read() == LOCAL){
			arbL->bufferCircular[EAST] = 1;
		}


		if(portDestiny[2].read() == NORTH){
			arbN->bufferCircular[SOUTH] = 1;
		}else if(portDestiny[2].read() == EAST){
			arbE->bufferCircular[SOUTH] = 1;
		}else if(portDestiny[2].read() == SOUTH){
			arbS->bufferCircular[SOUTH] = 1;
		}else if(portDestiny[2].read() == WEST){
			arbW->bufferCircular[SOUTH] = 1;
		}else if(portDestiny[2].read() == LOCAL){
			arbL->bufferCircular[SOUTH] = 1;
		}


		if(portDestiny[3].read() == NORTH){
			arbN->bufferCircular[WEST] = 1;
		}else if(portDestiny[3].read() == EAST){
			arbE->bufferCircular[WEST] = 1;
		}else if(portDestiny[3].read() == SOUTH){
			arbS->bufferCircular[WEST] = 1;
		}else if(portDestiny[3].read() == WEST){
			arbW->bufferCircular[WEST] = 1;
		}else if(portDestiny[3].read() == LOCAL){
			arbL->bufferCircular[WEST] = 1;
		}


		if(portDestiny[4].read() == NORTH){
			arbN->bufferCircular[LOCAL] = 1;
		}else if(portDestiny[4].read() == EAST){
			arbE->bufferCircular[LOCAL] = 1;
		}else if(portDestiny[4].read() == SOUTH){
			arbS->bufferCircular[LOCAL] = 1;
		}else if(portDestiny[4].read() == WEST){
			arbW->bufferCircular[LOCAL] = 1;
		}else if(portDestiny[4].read() == LOCAL){
			arbL->bufferCircular[LOCAL] = 1;
		}
	}


	void chaveamento_interno(){
		if((portDestiny[0].read() == NORTH) && (arbN->priority == NORTH)){
			out_port[0] = bfN->dout;
			out_val[0].write(1);
			arbN->bufferCircular[NORTH] = 0;
		}
		if((portDestiny[0].read() == EAST) && (arbE->priority == NORTH)){
			out_port[1] = bfN->dout;
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}
		if((portDestiny[0].read() == SOUTH) && (arbS->priority == NORTH)){
			out_port[2] = bfN->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}
		if((portDestiny[0].read() == WEST) && (arbW->priority == NORTH)){
			out_port[3] = bfN->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}
		if((portDestiny[0].read() == LOCAL) && (arbL->priority == NORTH)){
			out_port[4] = bfN->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}


		if((portDestiny[1].read() == NORTH) && (arbN->priority == EAST)){
			out_portN = bfE->dout;
			out_val[0].write(1);
			//cout << out_port[0].payload << endl;
			arbN->bufferCircular[NORTH] = 0;
		}
		if((portDestiny[1].read() == EAST) && (arbE->priority == EAST)){
			out_port[1] = bfE->dout;
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}
		if((portDestiny[1].read() == SOUTH) && (arbS->priority == EAST)){
			out_port[2] = bfE->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}
		if((portDestiny[1].read() == WEST) && (arbW->priority == EAST)){
			out_port[3] = bfE->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}
		if((portDestiny[1].read() == LOCAL) && (arbL->priority == EAST)){
			out_port[4] = bfE->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}


		if((portDestiny[2].read() == NORTH) && (arbN->priority == SOUTH)){
			out_port[0] = bfS->dout;
			out_val[0].write(1);
			arbN->bufferCircular[NORTH] = 0;
		}
		if((portDestiny[2].read() == EAST) && (arbE->priority == SOUTH)){
			out_port[1] = bfS->dout;
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}
		if((portDestiny[2].read() == SOUTH) && (arbS->priority == SOUTH)){
			out_port[2] = bfS->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}
		if((portDestiny[2].read() == WEST) && (arbW->priority == SOUTH)){
			out_port[3] = bfS->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}
		if((portDestiny[2].read() == LOCAL) && (arbL->priority == SOUTH)){
			out_port[4] = bfS->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}


		if((portDestiny[3].read() == NORTH) && (arbN->priority == WEST)){
			out_port[0] = bfW->dout;
			out_val[0].write(1);
			arbN->bufferCircular[NORTH] = 0;
		}
		if((portDestiny[3].read() == EAST) && (arbE->priority == WEST)){
			out_port[1] = bfW->dout;
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}
		if((portDestiny[3].read() == SOUTH) && (arbS->priority == WEST)){
			out_port[2] = bfW->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}
		if((portDestiny[3].read() == WEST) && (arbW->priority == WEST)){
			out_port[3] = bfW->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}
		if((portDestiny[3].read() == LOCAL) && (arbL->priority == WEST)){
			out_port[4] = bfW->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}


		if((portDestiny[4].read() == NORTH) && (arbN->priority == LOCAL)){
			out_port[0] = bfL->dout;
			out_val[0].write(1);
			arbN->bufferCircular[NORTH] = 0;
		}
		if((portDestiny[4].read() == EAST) && (arbE->priority == LOCAL)){
			out_port[1] = bfL->dout;
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}
		if((portDestiny[4].read() == SOUTH) && (arbS->priority == LOCAL)){
			out_port[2] = bfL->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}
		if((portDestiny[4].read() == WEST) && (arbW->priority == LOCAL)){
			out_port[3] = bfL->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}
		if((portDestiny[4].read() == LOCAL) && (arbL->priority == LOCAL)){
			out_port[4] = bfL->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}
	}

	void print(){
		//cout << portDestiny[4] << endl;
		//cout << out_val[1] << endl;
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


		//Controle de fluxo
		fcN->clk(clk);
		fcE->clk(clk);
		fcS->clk(clk);
		fcW->clk(clk);
		fcL->clk(clk);


		//Buffers
		bfN->clk(clk);
		bfE->clk(clk);
		bfS->clk(clk);
		bfW->clk(clk);
		bfL->clk(clk);

		
		//Roteamento
		rtgN->clk(clk);
		rtgE->clk(clk);
		rtgS->clk(clk);
		rtgW->clk(clk);
		rtgL->clk(clk);


		//Arbitragem
		arbN->clk(clk);
		arbE->clk(clk);
		arbS->clk(clk);
		arbW->clk(clk);
		arbL->clk(clk);


		SC_METHOD(print);
		sensitive << clk.pos();
		SC_METHOD(map_fc);
		sensitive << clk.pos();
		SC_METHOD(map_bf);
		sensitive << clk.pos();
		SC_METHOD(map_rtg);
		sensitive << clk.pos();
		SC_METHOD(map_arb);
		sensitive << clk.pos();
		SC_METHOD(chaveamento_interno);
		sensitive << clk.pos();
		SC_METHOD(request_arbiter);
		sensitive << clk.pos();

	}
};