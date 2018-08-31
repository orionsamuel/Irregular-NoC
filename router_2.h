#include "systemc.h"
#include "parameters.h"
#include "flow_control.h"
#include "buffer.h"
#include "routing.h"
#include "arbiter_vc.h"
#include <vector>
#include <iostream>

using namespace std;


SC_MODULE(router){
	//Posição em que o roteador se encontra
	sc_int<32> position;

	sc_int<32> count;
	sc_int<32> count_rd;

	sc_time latencia_rt;

	sc_in<bool> clk;

	//Portas do Roteador;
	flit in_port[5]; 

	flit out_port[5];

	flit out_portNN, out_portNN2, out_portNE, out_portNE2, out_portNS, out_portNS2, out_portNW, out_portNW2, out_portNL, out_portNL2;
	flit out_portEN, out_portEN2, out_portEE, out_portEE2, out_portES, out_portES2, out_portEW, out_portEW2, out_portEL, out_portEL2;
	flit out_portSN, out_portSN2, out_portSE, out_portSE2, out_portSS, out_portSS2, out_portSW, out_portSW2, out_portSL, out_portSL2;
	flit out_portWN, out_portWN2, out_portWE, out_portWE2, out_portWS, out_portWS2, out_portWW, out_portWW2, out_portWL, out_portWL2;
	flit out_portLN, out_portLN2, out_portLE, out_portLE2, out_portLS, out_portLS2, out_portLW, out_portLW2, out_portLL, out_portLL2;

	//Tabela de roteamento
	routing_table tabela;

	//Componentes do roteador
	flow_control *fcN, *fcN2, *fcE, *fcE2, *fcS, *fcS2, *fcW, *fcW2, *fcL, *fcL2;
	Buffer *bfN, *bfN2, *bfE, *bfE2, *bfS, *bfS2, *bfW, *bfW2, *bfL, *bfL2;
	routing rtgN, rtgN2, rtgE, rtgE2, rtgS, rtgS2, rtgW, rtgW2, rtgL, rtgL2;
	arbiter *arbN, *arbE, *arbS, *arbW, *arbL;


	//Portas de comunicação entre roteadores
	sc_signal<sc_int<32> > in_val[5];

	sc_signal<sc_int<32> > out_val[5];

	sc_signal<sc_int<32> > in_ack[10];

	sc_signal<sc_int<32> > out_ack[10];

	int portDestiny[10];

	//Sinais para verificação nos buffers
	sc_signal<sc_int<32> > wok[5];
	sc_signal<sc_int<32> > wr[5];
	
	//Sinais para o chaveamento dos buffers
	sc_signal<sc_int<32> > rd[5];
	

	void map_fc(){
		if((in_val[0].read() == 1) && (in_port[0].priority == 1)){
			fcN->in_val.write(1);
		}else if((in_val[0].read() == 1) && (in_port[0].priority == 2)){
			fcN2->in_val.write(1);
		}
		if((in_val[1].read() == 1) && (in_port[1].priority == 1)){
			fcE->in_val.write(1);
		}else if((in_val[1].read() == 1) && (in_port[1].priority == 2)){
			fcE2->in_val.write(1);
		}
		if((in_val[2].read() == 1) && (in_port[2].priority == 1)){
			fcS->in_val.write(1);
		}else if((in_val[2].read() == 1) && (in_port[2].priority == 2)){
			fcS2->in_val.write(1);
		}
		if((in_val[3].read() == 1) && (in_port[3].priority == 1)){
			fcW->in_val.write(1);
		}else if((in_val[3].read() == 1) && (in_port[3].priority == 2)){
			fcW2->in_val.write(1);
		}
		if((in_val[4].read() == 1) && (in_port[4].priority == 1)){
			fcL->in_val.write(1);
		}else if((in_val[4].read() == 1) && (in_port[4].priority == 2)){
			fcL2->in_val.write(1);
		}


		if(fcN->out_ack.read() == 1){
			out_ack[0].write(1);
		}
		if(fcN2->out_ack.read() == 1){
			out_ack[1].write(1);
		}
		if(fcE->out_ack.read() == 1){
			out_ack[2].write(1);
		}
		if(fcE2->out_ack.read() == 1){
			out_ack[3].write(1);
		}
		if(fcS->out_ack.read() == 1){
			out_ack[4].write(1);
		}
		if(fcS->out_ack.read() == 1){
			out_ack[5].write(1);
		}
		if(fcW->out_ack.read() == 1){
			out_ack[6].write(1);
		}
		if(fcW2->out_ack.read() == 1){
			out_ack[7].write(1);
		}
		if(fcL->out_ack.read() == 1){
			out_ack[8].write(1);
		}
		if(fcL2->out_ack.read() == 1){
			out_ack[9].write(1);
		}


		if(fcN->in_val.read() == 1){
			bfN->wr.write(1);
		}
		if(fcN2->in_val.read() == 1){
			bfN2->wr.write(1);
		}
		if(fcE->in_val.read() == 1){
			bfE->wr.write(1);
		}
		if(fcE2->in_val.read() == 1){
			bfE2->wr.write(1);
		}
		if(fcS->in_val.read() == 1){
			bfS->wr.write(1);
		}
		if(fcS2->in_val.read() == 1){
			bfS2->wr.write(1);
		}
		if(fcW->in_val.read() == 1){
			bfW->wr.write(1);
		}
		if(fcW2->in_val.read() == 1){
			bfW2->wr.write(1);
		}
		if(fcL->in_val.read() == 1){
			bfL->wr.write(1);
		}
		if(fcL2->in_val.read() == 1){
			bfL2->wr.write(1);
		}		


		if(bfN->wok.read() == 1){
			fcN->out_ack.write(1);
		}
		if(bfN2->wok.read() == 1){
			fcN2->out_ack.write(1);
		}
		if(bfE->wok.read() == 1){
			fcE->out_ack.write(1);
		}
		if(bfE2->wok.read() == 1){
			fcE2->out_ack.write(1);
		}
		if(bfS->wok.read() == 1){
			fcS->out_ack.write(1);
		}
		if(bfS2->wok.read() == 1){
			fcS2->out_ack.write(1);
		}
		if(bfW->wok.read() == 1){
			fcW->out_ack.write(1);
		}
		if(bfW2->wok.read() == 1){
			fcW2->out_ack.write(1);
		}
		if(bfL->wok.read() == 1){
			fcL->out_ack.write(1);
		}
		if(bfL2->wok.read() == 1){
			fcL2->out_ack.write(1);
		}


	}

	void map_bf(){
		if(in_port[0].priority == 1){
			bfN->din = in_port[0];
		}else if(in_port[0].priority == 2){
			bfN2->din = in_port[0];
		}if(in_port[1].priority == 1){
			bfE->din = in_port[1];
		}else if(in_port[1].priority == 2){
			bfE2->din = in_port[1];
		}
		if(in_port[2].priority == 1){
			bfS->din = in_port[2];
		}else if(in_port[2].priority == 2){
			bfS2->din = in_port[2];
		}
		if(in_port[3].priority == 1){
			bfW->din = in_port[3];
		}else if(in_port[3].priority == 2){
			bfW2->din = in_port[3];
		}
		if(in_port[4].priority == 1){
			bfL->din = in_port[4];
		}else if(in_port[4].priority == 2){
			bfL2->din = in_port[4];
		}
	}

	void map_rtg(){
		rtgN.tabela =  tabela;
		rtgN2.tabela =  tabela;
		rtgE.tabela =  tabela;
		rtgE2.tabela =  tabela;
		rtgS.tabela =  tabela;
		rtgS2.tabela =  tabela;
		rtgW.tabela =  tabela;
		rtgW2.tabela =  tabela;
		rtgL.tabela =  tabela;
		rtgL2.tabela =  tabela;


		rtgN.destiny = bfN->din.destiny;
		rtgN2.destiny = bfN2->din.destiny;
		rtgE.destiny = bfE->din.destiny;
		rtgE2.destiny = bfE2->din.destiny;
		rtgS.destiny = bfS->din.destiny;
		rtgS2.destiny = bfS2->din.destiny;
		rtgW.destiny = bfW->din.destiny;
		rtgW2.destiny = bfW2->din.destiny;
		rtgL.destiny = bfL->din.destiny;
		rtgL2.destiny = bfL2->din.destiny;

		portDestiny[0] = rtgN.tableAcess();
		portDestiny[1] = rtgN2.tableAcess();
		portDestiny[2] = rtgE.tableAcess();
		portDestiny[3] = rtgE2.tableAcess();
		portDestiny[4] = rtgS.tableAcess();
		portDestiny[5] = rtgS2.tableAcess();
		portDestiny[6] = rtgW.tableAcess();
		portDestiny[7] = rtgW2.tableAcess();
		portDestiny[8] = rtgL.tableAcess();
		portDestiny[9] = rtgL2.tableAcess();
	}

	void map_arb(){

		if(arbN->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbN->rd[1].read() == 1){
			bfN2->rd.write(1);;
		}
		if(arbN->rd[2].read() == 1){
			bfE->rd.write(1);
		}
		if(arbN->rd[3].read() == 1){
			bfE2->rd.write(1);
		}
		if(arbN->rd[4].read() == 1){
			bfS->rd.write(1);
		}
		if(arbN->rd[5].read() == 1){
			bfS2->rd.write(1);
		}
		if(arbN->rd[6].read() == 1){
			bfW->rd.write(1);;
		}
		if(arbN->rd[7].read() == 1){
			bfW2->rd.write(1);
		}
		if(arbN->rd[8].read() == 1){
			bfL->rd.write(1);
		}
		if(arbN->rd[9].read() == 1){
			bfL2->rd.write(1);
		}


		if(arbE->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbE->rd[1].read() == 1){
			bfN2->rd.write(1);;
		}
		if(arbE->rd[2].read() == 1){
			bfE->rd.write(1);
		}
		if(arbE->rd[3].read() == 1){
			bfE2->rd.write(1);
		}
		if(arbE->rd[4].read() == 1){
			bfS->rd.write(1);
		}
		if(arbE->rd[5].read() == 1){
			bfS2->rd.write(1);
		}
		if(arbE->rd[6].read() == 1){
			bfW->rd.write(1);;
		}
		if(arbE->rd[7].read() == 1){
			bfW2->rd.write(1);
		}
		if(arbE->rd[8].read() == 1){
			bfL->rd.write(1);
		}
		if(arbE->rd[9].read() == 1){
			bfL2->rd.write(1);
		}



		if(arbS->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbS->rd[1].read() == 1){
			bfN2->rd.write(1);;
		}
		if(arbS->rd[2].read() == 1){
			bfE->rd.write(1);
		}
		if(arbS->rd[3].read() == 1){
			bfE2->rd.write(1);
		}
		if(arbS->rd[4].read() == 1){
			bfS->rd.write(1);
		}
		if(arbS->rd[5].read() == 1){
			bfS2->rd.write(1);
		}
		if(arbS->rd[6].read() == 1){
			bfW->rd.write(1);;
		}
		if(arbS->rd[7].read() == 1){
			bfW2->rd.write(1);
		}
		if(arbS->rd[8].read() == 1){
			bfL->rd.write(1);
		}
		if(arbS->rd[9].read() == 1){
			bfL2->rd.write(1);
		}



		if(arbW->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbW->rd[1].read() == 1){
			bfN2->rd.write(1);;
		}
		if(arbW->rd[2].read() == 1){
			bfE->rd.write(1);
		}
		if(arbW->rd[3].read() == 1){
			bfE2->rd.write(1);
		}
		if(arbW->rd[4].read() == 1){
			bfS->rd.write(1);
		}
		if(arbW->rd[5].read() == 1){
			bfS2->rd.write(1);
		}
		if(arbW->rd[6].read() == 1){
			bfW->rd.write(1);;
		}
		if(arbW->rd[7].read() == 1){
			bfW2->rd.write(1);
		}
		if(arbW->rd[8].read() == 1){
			bfL->rd.write(1);
		}
		if(arbW->rd[9].read() == 1){
			bfL2->rd.write(1);
		}


		if(arbL->rd[0].read() == 1){
			bfN->rd.write(1);
		}
		if(arbL->rd[1].read() == 1){
			bfN2->rd.write(1);;
		}
		if(arbL->rd[2].read() == 1){
			bfE->rd.write(1);
		}
		if(arbL->rd[3].read() == 1){
			bfE2->rd.write(1);
		}
		if(arbL->rd[4].read() == 1){
			bfS->rd.write(1);
		}
		if(arbL->rd[5].read() == 1){
			bfS2->rd.write(1);
		}
		if(arbL->rd[6].read() == 1){
			bfW->rd.write(1);;
		}
		if(arbL->rd[7].read() == 1){
			bfW2->rd.write(1);
		}
		if(arbL->rd[8].read() == 1){
			bfL->rd.write(1);
		}
		if(arbL->rd[9].read() == 1){
			bfL2->rd.write(1);
		}
	}

	void request_arbiter(){
		if(portDestiny[0] == NORTH){
			arbN->bufferCircular[0] = 1;
		}else if(portDestiny[0] == EAST){
			arbE->bufferCircular[0] = 1;
		}else if(portDestiny[0] == SOUTH){
			arbS->bufferCircular[0] = 1;
		}else if(portDestiny[0] == WEST){
			arbW->bufferCircular[0] = 1;
		}else if(portDestiny[0] == LOCAL){
			arbL->bufferCircular[0] = 1;
		}


		if(portDestiny[1] == NORTH){
			arbN->bufferCircular[1] = 1;
		}else if(portDestiny[1] == EAST){
			arbE->bufferCircular[1] = 1;
		}else if(portDestiny[1] == SOUTH){
			arbS->bufferCircular[1] = 1;
		}else if(portDestiny[1] == WEST){
			arbW->bufferCircular[1] = 1;
		}else if(portDestiny[1] == LOCAL){
			arbL->bufferCircular[1] = 1;
		}


		if(portDestiny[2] == NORTH){
			arbN->bufferCircular[2] = 1;
		}else if(portDestiny[2] == EAST){
			arbE->bufferCircular[2] = 1;
		}else if(portDestiny[2] == SOUTH){
			arbS->bufferCircular[2] = 1;
		}else if(portDestiny[2] == WEST){
			arbW->bufferCircular[2] = 1;
		}else if(portDestiny[2] == LOCAL){
			arbL->bufferCircular[2] = 1;
		}


		if(portDestiny[3] == NORTH){
			arbN->bufferCircular[3] = 1;
		}else if(portDestiny[3] == EAST){
			arbE->bufferCircular[3] = 1;
		}else if(portDestiny[3] == SOUTH){
			arbS->bufferCircular[3] = 1;
		}else if(portDestiny[3] == WEST){
			arbW->bufferCircular[3] = 1;
		}else if(portDestiny[3] == LOCAL){
			arbL->bufferCircular[3] = 1;
		}


		if(portDestiny[4] == NORTH){
			arbN->bufferCircular[4] = 1;
		}else if(portDestiny[4] == EAST){
			arbE->bufferCircular[4] = 1;
		}else if(portDestiny[4] == SOUTH){
			arbS->bufferCircular[4] = 1;
		}else if(portDestiny[4] == WEST){
			arbW->bufferCircular[4] = 1;
		}else if(portDestiny[4] == LOCAL){
			arbL->bufferCircular[4] = 1;
		}

		if(portDestiny[5] == NORTH){
			arbN->bufferCircular[5] = 1;
		}else if(portDestiny[5] == EAST){
			arbE->bufferCircular[5] = 1;
		}else if(portDestiny[5] == SOUTH){
			arbS->bufferCircular[5] = 1;
		}else if(portDestiny[5] == WEST){
			arbW->bufferCircular[5] = 1;
		}else if(portDestiny[5] == LOCAL){
			arbL->bufferCircular[5] = 1;
		}

		if(portDestiny[6] == NORTH){
			arbN->bufferCircular[6] = 1;
		}else if(portDestiny[6] == EAST){
			arbE->bufferCircular[6] = 1;
		}else if(portDestiny[6] == SOUTH){
			arbS->bufferCircular[6] = 1;
		}else if(portDestiny[6] == WEST){
			arbW->bufferCircular[6] = 1;
		}else if(portDestiny[6] == LOCAL){
			arbL->bufferCircular[6] = 1;
		}

		if(portDestiny[7] == NORTH){
			arbN->bufferCircular[7] = 1;
		}else if(portDestiny[7] == EAST){
			arbE->bufferCircular[7] = 1;
		}else if(portDestiny[7] == SOUTH){
			arbS->bufferCircular[7] = 1;
		}else if(portDestiny[7] == WEST){
			arbW->bufferCircular[7] = 1;
		}else if(portDestiny[7] == LOCAL){
			arbL->bufferCircular[7] = 1;
		}

		if(portDestiny[8] == NORTH){
			arbN->bufferCircular[8] = 1;
		}else if(portDestiny[8] == EAST){
			arbE->bufferCircular[8] = 1;
		}else if(portDestiny[8] == SOUTH){
			arbS->bufferCircular[8] = 1;
		}else if(portDestiny[8] == WEST){
			arbW->bufferCircular[8] = 1;
		}else if(portDestiny[8] == LOCAL){
			arbL->bufferCircular[8] = 1;
		}

		if(portDestiny[9] == NORTH){
			arbN->bufferCircular[9] = 1;
		}else if(portDestiny[9] == EAST){
			arbE->bufferCircular[9] = 1;
		}else if(portDestiny[9] == SOUTH){
			arbS->bufferCircular[9] = 1;
		}else if(portDestiny[9] == WEST){
			arbW->bufferCircular[9] = 1;
		}else if(portDestiny[9] == LOCAL){
			arbL->bufferCircular[9] = 1;
		}

	}


	void chaveamento_interno(){
		if(portDestiny[0] == NORTH){
			out_portNN = bfN->dout;
			out_val[0].write(1);
			arbN->bufferCircular[0] = 0;
		}else if(portDestiny[0] == EAST){
			out_portNE = bfN->dout;
			out_val[1].write(1);
			arbE->bufferCircular[0] = 0;;
		}else if(portDestiny[0] == SOUTH){
			out_portNS = bfN->dout;
			out_val[2].write(1);
			arbS->bufferCircular[0] = 0;
		}else if(portDestiny[0] == WEST){
			out_portNW = bfN->dout;
			out_val[3].write(1);
			arbW->bufferCircular[0] = 0;
		}else if(portDestiny[0]  == LOCAL){
			out_portNL = bfN->dout;
			out_val[4].write(1);
			arbL->bufferCircular[0] = 0;
		}

		if(portDestiny[1] == NORTH){
			out_portNN2 = bfN2->dout;
			out_val[0].write(1);
			arbN->bufferCircular[1] = 0;
		}else if(portDestiny[1] == EAST){
			out_portNE = bfN->dout;
			out_val[1].write(1);
			arbE->bufferCircular[1] = 0;;
		}else if(portDestiny[1] == SOUTH){
			out_portNS = bfN->dout;
			out_val[2].write(1);
			arbS->bufferCircular[1] = 0;
		}else if(portDestiny[1] == WEST){
			out_portNW = bfN->dout;
			out_val[3].write(1);
			arbW->bufferCircular[1] = 0;
		}else if(portDestiny[1]  == LOCAL){
			out_portNL = bfN->dout;
			out_val[4].write(1);
			arbL->bufferCircular[1] = 0;
		}



		if(portDestiny[2] == NORTH){
			out_portEN = bfE->dout;
			out_val[0].write(1);
			arbN->bufferCircular[2] = 0;
		}else if(portDestiny[2] == EAST){
			out_portEE = bfE->dout;
			out_val[1].write(1);
			arbE->bufferCircular[2] = 0;
		}else if(portDestiny[2] == SOUTH){
			out_portES = bfE->dout;
			out_val[2].write(1);
			arbS->bufferCircular[2] = 0;
		}else if(portDestiny[2] == WEST){
			out_portEW = bfE->dout;
			out_val[3].write(1);
			arbW->bufferCircular[2] = 0;
		}else if(portDestiny[2] == LOCAL){
			out_portEL = bfE->dout;
			out_val[4].write(1);
			arbL->bufferCircular[2] = 0;
		}

		if(portDestiny[3] == NORTH){
			out_portEN2 = bfE2->dout;
			out_val[0].write(1);
			arbN->bufferCircular[3] = 0;
		}else if(portDestiny[3] == EAST){
			out_portEE2 = bfE2->dout;
			out_val[1].write(1);
			arbE->bufferCircular[3] = 0;
		}else if(portDestiny[3] == SOUTH){
			out_portES2 = bfE2->dout;
			out_val[2].write(1);
			arbS->bufferCircular[3] = 0;
		}else if(portDestiny[3] == WEST){
			out_portEW2 = bfE2->dout;
			out_val[3].write(1);
			arbW->bufferCircular[3] = 0;
		}else if(portDestiny[3] == LOCAL){
			out_portEL2 = bfE2->dout;
			out_val[4].write(1);
			arbL->bufferCircular[3] = 0;
		}



		if(portDestiny[4] == NORTH){
			out_portSN = bfS->dout;
			out_val[0].write(1);
			arbN->bufferCircular[4] = 0;
		}else if(portDestiny[4] == EAST){
			out_portSE = bfS->dout;		
			out_val[1].write(1);
			arbE->bufferCircular[4] = 0;
		}else if(portDestiny[4] == SOUTH){
			out_portSS = bfS->dout;
			out_val[2].write(1);
			arbS->bufferCircular[4] = 0;
		}else if(portDestiny[4] == WEST){
			out_portSW = bfS->dout;
			out_val[3].write(1);
			arbW->bufferCircular[4] = 0;
		}else if(portDestiny[4] == LOCAL){
			out_portSL = bfS->dout;
			out_val[4].write(1);
			arbL->bufferCircular[4] = 0;
		}



		if(portDestiny[5] == NORTH){
			out_portSN2 = bfS2->dout;
			out_val[0].write(1);
			arbN->bufferCircular[5] = 0;
		}else if(portDestiny[5] == EAST){
			out_portSE2 = bfS2->dout;
			out_val[1].write(1);
			arbE->bufferCircular[5] = 0;
		}else if(portDestiny[5] == SOUTH){
			out_portSS2 = bfS2->dout;
			out_val[2].write(1);
			arbS->bufferCircular[5] = 0;
		}else if(portDestiny[5] == WEST){
			out_portSW2 = bfS2->dout;
			out_val[3].write(1);
			arbW->bufferCircular[5] = 0;
		}else if(portDestiny[5] == LOCAL){
			out_portSL2 = bfS2->dout;
			out_val[4].write(1);
			arbL->bufferCircular[5] = 0;
		}

		if(portDestiny[6] == NORTH){
			out_portWN = bfW->dout;
			out_val[0].write(1);
			arbN->bufferCircular[6] = 0;
		}else if(portDestiny[6] == EAST){
			out_portWE = bfW->dout;
			out_val[1].write(1);
			arbE->bufferCircular[6] = 0;
		}else if(portDestiny[6] == SOUTH){
			out_portWS = bfW->dout;
			out_val[2].write(1);
			arbS->bufferCircular[6] = 0;
		}else if(portDestiny[6] == WEST){
			out_portWW = bfW->dout;
			out_val[3].write(1);
			arbW->bufferCircular[6] = 0;
		}else if(portDestiny[6] == LOCAL){
			out_portWL = bfW->dout;
			out_val[4].write(1);
			arbL->bufferCircular[6] = 0;
		}

		if(portDestiny[7] == NORTH){
			out_portWN2 = bfW2->dout;
			out_val[0].write(1);
			arbN->bufferCircular[7] = 0;
		}else if(portDestiny[7] == EAST){
			out_portWE2 = bfW2->dout;
			out_val[1].write(1);
			arbE->bufferCircular[7] = 0;
		}else if(portDestiny[7] == SOUTH){
			out_portWS2 = bfW2->dout;
			out_val[2].write(1);
			arbS->bufferCircular[7] = 0;
		}else if(portDestiny[7] == WEST){
			out_portWW2 = bfW2->dout;
			out_val[3].write(1);
			arbW->bufferCircular[7] = 0;
		}else if(portDestiny[7] == LOCAL){
			out_portWL2 = bfW2->dout;
			out_val[4].write(1);
			arbL->bufferCircular[7] = 0;
		}



		if(portDestiny[8] == NORTH){
			out_portLN = bfL->dout;
			out_val[0].write(1);
			arbN->bufferCircular[8] = 0;
		}else if(portDestiny[8] == EAST){
			out_portLE = bfL->dout;
			out_val[1].write(1);
			arbE->bufferCircular[8] = 0;
		}else if(portDestiny[8] == SOUTH){
			out_portLS = bfL->dout;
			out_val[2].write(1);
			arbS->bufferCircular[8] = 0;
		}else if(portDestiny[8] == WEST){
			out_portLW = bfL->dout;	
			out_val[3].write(1);
			arbW->bufferCircular[8] = 0;
		}else if(portDestiny[8] == LOCAL){
			out_portLL = bfL->dout;
			out_val[4].write(1);
			arbL->bufferCircular[8] = 0;
		}

		if(portDestiny[9] == NORTH){
			out_portLN2 = bfL2->dout;
			out_val[0].write(1);
			arbN->bufferCircular[9] = 0;
		}else if(portDestiny[9] == EAST){
			out_portLE2 = bfL2->dout;
			out_val[1].write(1);
			arbE->bufferCircular[9] = 0;
		}else if(portDestiny[9] == SOUTH){
			out_portLS2 = bfL2->dout;
			out_val[2].write(1);
			arbS->bufferCircular[9] = 0;
		}else if(portDestiny[9] == WEST){
			out_portLW2 = bfL2->dout;	
			out_val[3].write(1);
			arbW->bufferCircular[9] = 0;
		}else if(portDestiny[9] == LOCAL){
			out_portLL2 = bfL2->dout;
			out_val[4].write(1);
			arbL->bufferCircular[9] = 0;
		}

		

		if(out_portNN.type != 0){
			out_port[0] = out_portNN;
		}
		if(out_portNE.type != 0){
			out_port[1] = out_portNE;
		}
		if(out_portNS.type != 0){
			out_port[2] = out_portNS;
		}
		if(out_portNW.type != 0){
			out_port[3] = out_portNW;
		}
		if(out_portNL.type != 0){
			out_port[4] = out_portNL;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}



		if(out_portNN2.type != 0){
			out_port[0] = out_portNN2;
		}
		if(out_portNE2.type != 0){
			out_port[1] = out_portNE2;
		}
		if(out_portNS2.type != 0){
			out_port[2] = out_portNS2;
		}
		if(out_portNW2.type != 0){
			out_port[3] = out_portNW2;
		}
		if(out_portNL2.type != 0){
			out_port[4] = out_portNL2;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}




		if(out_portEN.type != 0){
			out_port[0] = out_portEN;
		}
		if(out_portEE.type != 0){
			out_port[1] = out_portEE;
		}
		if(out_portES.type != 0){
			out_port[2] = out_portES;
		}
		if(out_portEW.type != 0){
			out_port[3] = out_portEW;
		}
		if(out_portEL.type != 0){
			out_port[4] = out_portEL;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}



		if(out_portEN2.type != 0){
			out_port[0] = out_portEN;
		}
		if(out_portEE2.type != 0){
			out_port[1] = out_portEE;
		}
		if(out_portES2.type != 0){
			out_port[2] = out_portES;
		}
		if(out_portEW2.type != 0){
			out_port[3] = out_portEW;
		}
		if(out_portEL2.type != 0){
			out_port[4] = out_portEL;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}




		if(out_portSN.type != 0){
			out_port[0] = out_portSN;
		}
		if(out_portSE.type != 0){
			out_port[1] = out_portSE;
		}
		if(out_portSS.type != 0){
			out_port[2] = out_portSS;
		}
		if(out_portSW.type != 0){
			out_port[3] = out_portSW;
		}
		if(out_portSL.type != 0){
			out_port[4] = out_portSL;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}



		if(out_portSN2.type != 0){
			out_port[0] = out_portSN;
		}
		if(out_portSE2.type != 0){
			out_port[1] = out_portSE;
		}
		if(out_portSS2.type != 0){
			out_port[2] = out_portSS;
		}
		if(out_portSW2.type != 0){
			out_port[3] = out_portSW;
		}
		if(out_portSL2.type != 0){
			out_port[4] = out_portSL;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}




		if(out_portWN.type != 0){
			out_port[0] = out_portWN;
		}
		if(out_portWE.type != 0){
			out_port[1] = out_portWE;
		}if(out_portWS.type != 0){
			out_port[2] = out_portWS;
		}if(out_portWW.type != 0){
			out_port[3] = out_portWW;
		}if(out_portWL.type != 0){
			out_port[4] = out_portWL;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}



		if(out_portWN2.type != 0){
			out_port[0] = out_portWN;
		}
		if(out_portWE2.type != 0){
			out_port[1] = out_portWE;
		}if(out_portWS2.type != 0){
			out_port[2] = out_portWS;
		}if(out_portWW2.type != 0){
			out_port[3] = out_portWW;
		}if(out_portWL2.type != 0){
			out_port[4] = out_portWL;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}




		if(out_portLN.type != 0){
			out_port[0] = out_portLN;
		}if(out_portLE.type != 0){
			out_port[1] = out_portLE;
		}if(out_portLS.type != 0){
			out_port[2] = out_portLS;
		}if(out_portLW.type != 0){
			out_port[3] = out_portLW;
		}if(out_portLL.type != 0){
			out_port[4] = out_portLL;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}



		if(out_portLN2.type != 0){
			out_port[0] = out_portLN;
		}if(out_portLE2.type != 0){
			out_port[1] = out_portLE;
		}if(out_portLS2.type != 0){
			out_port[2] = out_portLS;
		}if(out_portLW2.type != 0){
			out_port[3] = out_portLW;
		}if(out_portLL2.type != 0){
			out_port[4] = out_portLL;
		}


		if((out_port[4].type > 0) && (out_port[4].type < 3)){
			count_rd = 1;
		}else if((out_port[4].type == 3) && (count_rd == 1)){
			if(out_port[4].priority == 1){
				count = count + 1;
			}
			latencia_rt = sc_time_stamp();
			count_rd = 0;
		}
		
	}

	void print(){
		//cout << bfE->din.payload << endl;
		//cout << rtgS->portDestiny << endl;
	}

	void deadline_count(){
		
	}


	SC_CTOR(router){
		//Instanciando o controle de fluxo
		fcN = new flow_control("fcN");
		fcE = new flow_control("fcE");
		fcS = new flow_control("fcS");
		fcW = new flow_control("fcW");
		fcL = new flow_control("fcL");
		fcN2 = new flow_control("fcN2");
		fcE2 = new flow_control("fcE2");
		fcS2 = new flow_control("fcS2");
		fcW2 = new flow_control("fcW2");
		fcL2 = new flow_control("fcL2");


		//Instanciando os buffers
		bfN = new Buffer("bfN");
		bfE = new Buffer("bfE");
		bfS = new Buffer("bfS");
		bfW = new Buffer("bfW");
		bfL = new Buffer("bfL");
		bfN2 = new Buffer("bfN2");
		bfE2 = new Buffer("bfE2");
		bfS2 = new Buffer("bfS2");
		bfW2 = new Buffer("bfW2");
		bfL2 = new Buffer("bfL");

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
		fcN2->clk(clk);
		fcE2->clk(clk);
		fcS2->clk(clk);
		fcW2->clk(clk);
		fcL2->clk(clk);


		//Buffers
		bfN->clk(clk);
		bfE->clk(clk);
		bfS->clk(clk);
		bfW->clk(clk);
		bfL->clk(clk);
		bfN2->clk(clk);
		bfE2->clk(clk);
		bfS2->clk(clk);
		bfW2->clk(clk);
		bfL2->clk(clk);


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