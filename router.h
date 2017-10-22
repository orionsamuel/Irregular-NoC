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
	sc_int<32> position;
	sc_int<32> coreNumbers;

	sc_in<bool> clk;
	
	sc_in<sc_int<32> > in_val_N, in_val_E, in_val_S, in_val_W, in_val_L; //Entrada poro controle de fluxo (falta saber como vai ser a entrada para in_val)
	sc_out<sc_int<32> > in_ack_N, in_ack_E, in_ack_S, in_ack_W, in_ack_L;
	sc_signal<sc_int<32> > ack_N, ack_E, ack_S, ack_W, ack_L; //Controle de fluxo
	sc_signal<sc_int<32> > wokN, wokE, wokS, wokW, wokL, wrN, wrE, wrS, wrW, wrL; //Controle de fluxo
	sc_signal<sc_int<32> > rokN, rokE, rokS, rokW, rokL, rdN, rdE, rdS, rdW, rdL; //Chaveamento //Entrada para o chaveamento no buffer (falta achar a entrada para rd)
	sc_signal<sc_int<32> > portExit_N, portExit_E, portExit_S, portExit_W, portExit_L, routerDestiny_N, routerDestiny_E, routerDestiny_S, routerDestiny_W, routerDestiny_L; //Entrada e saída do roteamento


	sc_int<32> aux_rokN, aux_rokE, aux_rokS, aux_rokW, aux_rokL;
	sc_int<32> aux_rdN, aux_rdE, aux_rdS, aux_rdW,aux_rdL;
	sc_int<32> portExitN, portExitE, portExitS, portExitW, portExitL;
	sc_out<sc_int<32> > out_val_N, out_val_E, out_val_S, out_val_W, out_val_L;

	routing_table tabela;
	
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

	flow_control *fcN, *fcE, *fcS, *fcW, *fcL;
	Buffer *bfN, *bfE, *bfS, *bfW, *bfL;
	routing *rtgN, *rtgE, *rtgS, *rtgW, *rtgL;
	arbiter *arbN, *arbE, *arbS, *arbW, *arbL;

	

	
	void set_ack(){
		in_ack_N.write(ack_N);
		in_ack_E.write(ack_E);
		in_ack_S.write(ack_S);
		in_ack_W.write(ack_W);
		in_ack_L.write(ack_L);
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

		//Ligando as entradas dos árbitros
		arbN->portDestiny(portExit_N);
		arbN->rd(rdN);
		arbN->clk(clk);
		arbE->portDestiny(portExit_E);
		arbE->rd(rdE);
		arbE->clk(clk);
		arbS->portDestiny(portExit_S);
		arbS->rd(rdS);
		arbS->clk(clk);
		arbW->portDestiny(portExit_W);
		arbW->rd(rdW);
		arbW->clk(clk);
		arbL->portDestiny(portExit_L);
		arbL->rd(rdL);
		arbL->clk(clk);

		//Ligação do roteamento nos buffers
		rtgN->tabela = tabela;
		rtgE->tabela = tabela;
		rtgS->tabela = tabela;
		rtgW->tabela = tabela;
		rtgL->tabela = tabela;

		rtgN->coreNumbers = coreNumbers;
		rtgE->coreNumbers = coreNumbers;
		rtgS->coreNumbers = coreNumbers;
		rtgW->coreNumbers = coreNumbers;
		rtgL->coreNumbers = coreNumbers;

		rtgN->portDestiny(portExit_N);
		rtgE->portDestiny(portExit_E);
		rtgS->portDestiny(portExit_S);
		rtgW->portDestiny(portExit_W);
		rtgL->portDestiny(portExit_L);

		routerDestiny_N = bfN->dout.destiny;
		routerDestiny_E = bfE->dout.destiny;
		routerDestiny_S = bfS->dout.destiny;
		routerDestiny_W = bfW->dout.destiny;
		routerDestiny_L = bfL->dout.destiny;

		rtgN->destiny(routerDestiny_N);
		rtgE->destiny(routerDestiny_E);
		rtgS->destiny(routerDestiny_S);
		rtgW->destiny(routerDestiny_W);
		rtgL->destiny(routerDestiny_L);

		rtgN->clk(clk);
		rtgE->clk(clk);
		rtgS->clk(clk);
		rtgW->clk(clk);
		rtgL->clk(clk);



		//Ligação entre os buffers e controle de fluxo
		bfN->wr(wrN);
		fcN->wr(wrN);
		bfN->wok(wokN);
		fcN->wok(wokN);

		bfE->wr(wrE);
		fcE->wr(wrE);
		bfE->wok(wokE);
		fcE->wok(wokE);

		bfS->wr(wrS);
		fcS->wr(wrS);
		bfS->wok(wokS);
		fcS->wok(wokS);

		bfW->wr(wrW);
		fcW->wr(wrW);
		bfW->wok(wokW);
		fcW->wok(wokW);

		bfL->wr(wrL);
		fcL->wr(wrL);
		bfL->wok(wokL);
		fcL->wok(wokL);

		//Controle do chaveamento
		bfN->rok(rokN);
		bfE->rok(rokE);
		bfS->rok(rokS);
		bfW->rok(rokW);
		bfL->rok(rokL);

		bfN->rd(rdN);
		bfE->rd(rdE);
		bfS->rd(rdS);
		bfW->rd(rdW);
		bfL->rd(rdL);

		//Verifica se pode fazer a remoção da fila, caso sim retira o flit do buffer e passa para a porta de saída

		aux_rokN = rokN;
		aux_rokE = rokE;
		aux_rokS = rokS;
		aux_rokW = rokW;
		aux_rokL = rokL;

		aux_rdN = rdN;
		aux_rdE = rdE;
		aux_rdS = rdS;
		aux_rdW = rdW;
		aux_rdL = rdL;

		portExitN = portExit_N;
		portExitE = portExit_E;
		portExitS = portExit_S;
		portExitW = portExit_W;
		portExitL = portExit_L;

		
		//Realização do chaveamento das portas
		if((portExitN == NORTH) && (aux_rdN == 1)){
			out_portN = bfN->dout;
			out_val_N.write(1);
		}else if((portExitN == EAST) && (aux_rdN == 1)){
			out_portE = bfN->dout;
			out_val_E.write(1);
		}else if((portExitN == SOUTH) && (aux_rdN == 1)){
			out_portS = bfN->dout;
			out_val_S.write(1);
		}else if((portExitN == WEST) && (aux_rdN == 1)){
			out_portW = bfN->dout;
			out_val_W.write(1);
		}else if((portExitN == LOCAL) && (aux_rdN == 1)){
			out_portL = bfN->dout;
			out_val_L.write(1);
		}

		if((portExitE == NORTH) && (aux_rdN == 1)){
			out_portN = bfE->dout;
			out_val_N.write(1);
		}else if((portExitE == EAST) && (aux_rdN == 1)){
			out_portE = bfE->dout;
			out_val_E.write(1);
		}else if((portExitE == SOUTH) && (aux_rdN == 1)){
			out_portS = bfE->dout;
			out_val_S.write(1);
		}else if((portExitE == WEST) && (aux_rdN == 1)){
			out_portW = bfE->dout;
			out_val_W.write(1);
		}else if((portExitE == LOCAL) && (aux_rdN == 1)){
			out_portL = bfE->dout;
			out_val_L.write(1);
		}

		if((portExitS == NORTH) && (aux_rdN == 1)){
			out_portN = bfS->dout;
			out_val_N.write(1);
		}else if((portExitS == EAST) && (aux_rdN == 1)){
			out_portE = bfS->dout;
			out_val_E.write(1);
		}else if((portExitS == SOUTH) && (aux_rdN == 1)){
			out_portS = bfS->dout;
			out_val_S.write(1);
		}else if((portExitS == WEST) && (aux_rdN == 1)){
			out_portW = bfS->dout;
			out_val_W.write(1);
		}else if((portExitS == LOCAL) && (aux_rdN == 1)){
			out_portL = bfS->dout;
			out_val_L.write(1);
		}

		if((portExitW == NORTH) && (aux_rdN == 1)){
			out_portN = bfW->dout;
			out_val_N.write(1);
		}else if((portExitW == EAST) && (aux_rdN == 1)){
			out_portE = bfW->dout;
			out_val_E.write(1);
		}else if((portExitW == SOUTH) && (aux_rdN == 1)){
			out_portS = bfW->dout;
			out_val_S.write(1);
		}else if((portExitW == WEST) && (aux_rdN == 1)){
			out_portW = bfW->dout;
			out_val_W.write(1);
		}else if((portExitW == LOCAL) && (aux_rdN == 1)){
			out_portL = bfW->dout;
			out_val_L.write(1);
		}

		if((portExitL == NORTH) && (aux_rdN == 1)){
			out_portN = bfL->dout;
			out_val_N.write(1);
		}else if((portExitL == EAST) && (aux_rdN == 1)){
			out_portE = bfL->dout;
			out_val_E.write(1);
		}else if((portExitL == SOUTH) && (aux_rdN == 1)){
			out_portS = bfL->dout;
			out_val_S.write(1);
		}else if((portExitL == WEST) && (aux_rdN == 1)){
			out_portW = bfL->dout;
			out_val_W.write(1);
		}else if((portExitL == LOCAL) && (aux_rdN == 1)){
			out_portL = bfL->dout;
			out_val_L.write(1);
		}

		bfN->din = in_portN;
		bfN->clk(clk);
		bfE->din = in_portE;
		bfE->clk(clk);
		bfS->din = in_portS;
		bfS->clk(clk);
		bfW->din = in_portW;
		bfW->clk(clk);
		bfL->din = in_portL;
		bfL->clk(clk);

		//Ligação dos entradas do controle de fluxo para as portas
		fcN->in_val(in_val_N);
		fcN->in_ack(ack_N);
		fcN->clk(clk);
		fcE->in_val(in_val_E);
		fcE->in_ack(ack_E);
		fcE->clk(clk);
		fcS->in_val(in_val_S);
		fcS->in_ack(ack_S);
		fcS->clk(clk);
		fcW->in_val(in_val_W);
		fcW->in_ack(ack_W);
		fcW->clk(clk);
		fcL->in_val(in_val_L);
		fcL->in_ack(ack_L);
		fcL->clk(clk);

		SC_METHOD(set_ack);
		sensitive << in_val_N, in_val_E, in_val_S, in_val_W, in_val_L;
	}
};