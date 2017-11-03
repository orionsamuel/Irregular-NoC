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

	sc_int<32> count;

	sc_in<bool> clk;

	//Portas do Roteador;
	flit in_port[5]; 

	flit out_port[5];

	flit out_portNN, out_portNE, out_portNS, out_portNW, out_portNL;
	flit out_portEN, out_portEE, out_portES, out_portEW, out_portEL;
	flit out_portSN, out_portSE, out_portSS, out_portSW, out_portSL;
	flit out_portWN, out_portWE, out_portWS, out_portWW, out_portWL;
	flit out_portLN, out_portLE, out_portLS, out_portLW, out_portLL;

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

		bfN->din = in_port[0];
		bfE->din = in_port[1];
		bfS->din = in_port[2];
		bfW->din = in_port[3];
		bfL->din = in_port[4];

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
		if(rtgN->portDestiny.read() == NORTH){
			arbN->bufferCircular[0] = 1;
		}else if(rtgN->portDestiny.read() == EAST){
			arbE->bufferCircular[0] = 1;
		}else if(rtgN->portDestiny.read() == SOUTH){
			arbS->bufferCircular[0] = 1;
		}else if(rtgN->portDestiny.read() == WEST){
			arbW->bufferCircular[0] = 1;
		}else if(rtgN->portDestiny.read() == LOCAL){
			arbL->bufferCircular[0] = 1;
		}


		if(rtgE->portDestiny.read() == NORTH){
			arbN->bufferCircular[1] = 1;
		}else if(rtgE->portDestiny.read() == EAST){
			arbE->bufferCircular[1] = 1;
		}else if(rtgE->portDestiny.read() == SOUTH){
			arbS->bufferCircular[1] = 1;
		}else if(rtgE->portDestiny.read() == WEST){
			arbW->bufferCircular[1] = 1;
		}else if(rtgE->portDestiny.read() == LOCAL){
			arbL->bufferCircular[1] = 1;
		}


		if(rtgS->portDestiny.read() == NORTH){
			arbN->bufferCircular[2] = 1;
		}else if(rtgS->portDestiny.read() == EAST){
			arbE->bufferCircular[2] = 1;
		}else if(rtgS->portDestiny.read() == SOUTH){
			arbS->bufferCircular[2] = 1;
		}else if(rtgS->portDestiny.read() == WEST){
			arbW->bufferCircular[2] = 1;
		}else if(rtgS->portDestiny.read() == LOCAL){
			arbL->bufferCircular[2] = 1;
		}


		if(rtgW->portDestiny.read() == NORTH){
			arbN->bufferCircular[3] = 1;
		}else if(rtgW->portDestiny.read() == EAST){
			arbE->bufferCircular[3] = 1;
		}else if(rtgW->portDestiny.read() == SOUTH){
			arbS->bufferCircular[3] = 1;
		}else if(rtgW->portDestiny.read() == WEST){
			arbW->bufferCircular[3] = 1;
		}else if(rtgW->portDestiny.read() == LOCAL){
			arbL->bufferCircular[3] = 1;
		}


		if(rtgL->portDestiny.read() == NORTH){
			arbN->bufferCircular[4] = 1;
		}else if(rtgL->portDestiny.read() == EAST){
			arbE->bufferCircular[4] = 1;
		}else if(rtgL->portDestiny.read() == SOUTH){
			arbS->bufferCircular[4] = 1;
		}else if(rtgL->portDestiny.read() == WEST){
			arbW->bufferCircular[4] = 1;
		}else if(rtgL->portDestiny.read() == LOCAL){
			arbL->bufferCircular[4] = 1;
		}

	}


	void chaveamento_interno(){
		if(rtgN->portDestiny.read() == NORTH){
			out_portNN = bfN->dout;
			out_val[0].write(1);
			arbN->bufferCircular[NORTH] = 0;
		}else if(rtgN->portDestiny.read() == EAST){
			out_portNE = bfN->dout;
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}else if(rtgN->portDestiny.read() == SOUTH){
			out_portNS = bfN->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}else if(rtgN->portDestiny.read() == WEST){
			out_portNW = bfN->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}else if(rtgN->portDestiny.read() == LOCAL){
			out_portNL = bfN->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}



		if(rtgE->portDestiny.read() == NORTH){
			out_portEN = bfE->dout;
			out_val[0].write(1);
			arbN->bufferCircular[NORTH] = 0;
		}else if(rtgE->portDestiny.read() == EAST){
			out_portEE = bfE->dout;
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}else if(rtgE->portDestiny.read() == SOUTH){
			out_portES = bfE->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}else if(rtgE->portDestiny.read() == WEST){
			out_portEW = bfE->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}else if(rtgE->portDestiny.read() == LOCAL){
			out_portEL = bfE->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}



		if(rtgS->portDestiny.read() == NORTH){
			out_portSN = bfS->dout;
			out_val[0].write(1);
			arbN->bufferCircular[NORTH] = 0;
		}else if(rtgS->portDestiny.read() == EAST){
			out_portSE = bfS->dout;		
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}else if(rtgS->portDestiny.read() == SOUTH){
			out_portSS = bfS->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}else if(rtgS->portDestiny.read() == WEST){
			out_portSW = bfS->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}else if(rtgS->portDestiny.read() == LOCAL){
			out_portSL = bfS->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}



		if(rtgW->portDestiny.read() == NORTH){
			out_portWN = bfW->dout;
			out_val[0].write(1);
			arbN->bufferCircular[NORTH] = 0;
		}else if(rtgW->portDestiny.read() == EAST){
			out_portWE = bfW->dout;
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}else if(rtgW->portDestiny.read() == SOUTH){
			out_portWS = bfW->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}else if(rtgW->portDestiny.read() == WEST){
			out_portWW = bfW->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}else if(rtgW->portDestiny.read() == LOCAL){
			out_portWL = bfW->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}



		if(rtgL->portDestiny.read() == NORTH){
			out_portLN = bfL->dout;
			out_val[0].write(1);
			arbN->bufferCircular[NORTH] = 0;
		}else if(rtgL->portDestiny.read() == EAST){
			out_portLE = bfL->dout;
			out_val[1].write(1);
			arbE->bufferCircular[EAST] = 0;
		}else if(rtgL->portDestiny.read() == SOUTH){
			out_portLS = bfL->dout;
			out_val[2].write(1);
			arbS->bufferCircular[SOUTH] = 0;
		}else if(rtgL->portDestiny.read() == WEST){
			out_portLW = bfL->dout;
			out_val[3].write(1);
			arbW->bufferCircular[WEST] = 0;
		}else if(rtgL->portDestiny.read() == LOCAL){
			out_portLL = bfL->dout;
			out_val[4].write(1);
			arbL->bufferCircular[LOCAL] = 0;
		}

		

		if(out_portNN.type != 0){
			out_port[0] = out_portNN;
		}else if(out_portNE.type != 0){
			out_port[1] = out_portNE;
		}else if(out_portNS.type != 0){
			out_port[2] = out_portNS;
		}else if(out_portNW.type != 0){
			out_port[3] = out_portNW;
		}else if(out_portNL.type != 0){
			out_port[4] = out_portNL;
		}


		if(out_portEN.type != 0){
			out_port[0] = out_portEN;
		}else if(out_portEE.type != 0){
			out_port[1] = out_portEE;
		}else if(out_portES.type != 0){
			out_port[2] = out_portES;
		}else if(out_portEW.type != 0){
			out_port[3] = out_portEW;
		}else if(out_portEL.type != 0){
			out_port[4] = out_portEL;
		}


		if(out_portSN.type != 0){
			out_port[0] = out_portSN;
		}else if(out_portSE.type != 0){
			out_port[1] = out_portSE;
		}else if(out_portSS.type != 0){
			out_port[2] = out_portSS;
		}else if(out_portSW.type != 0){
			out_port[3] = out_portSW;
		}else if(out_portSL.type != 0){
			out_port[4] = out_portSL;
		}


		if(out_portWN.type != 0){
			out_port[0] = out_portWN;
		}else if(out_portWE.type != 0){
			out_port[1] = out_portWE;
		}else if(out_portWS.type != 0){
			out_port[2] = out_portWS;
		}else if(out_portWW.type != 0){
			out_port[3] = out_portWW;
		}else if(out_portWL.type != 0){
			out_port[4] = out_portWL;
		}


		if(out_portLN.type != 0){
			out_port[0] = out_portLN;
		}else if(out_portLE.type != 0){
			out_port[1] = out_portLE;
		}else if(out_portLS.type != 0){
			out_port[2] = out_portLS;
		}else if(out_portLW.type != 0){
			out_port[3] = out_portLW;
		}else if(out_portLL.type != 0){
			out_port[4] = out_portLL;
		}
		
	}

	void print(){
		//cout << bfS->dout.payload << endl;
		//cout << rtgS->portDestiny << endl;
	}

	void deadline_count(){
		if(out_port[4].type == 2){
			count = count + 1;
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
		SC_METHOD(deadline_count);
		sensitive << clk.pos();

	}
};