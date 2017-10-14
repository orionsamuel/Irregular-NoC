#include "systemc.h"
#include "parameters.h"
#include "flow_control.h"
#include "buffer.h"
#include "routing.h"
#include <vector>
#include <iostream>

using namespace std;

SC_MODULE(router){
	sc_int<32> position;
	sc_int<32> coreNumbers;
	
	sc_in<sc_int<32> > in_val_N, in_val_E, in_val_S, in_val_W, in_val_L; //Entrada poro controle de fluxo (falta saber como vai ser a entrada para in_val)
	sc_out<sc_int<32> > in_ack_N, in_ack_E, in_ack_S, in_ack_W, in_ack_L;
	sc_signal<sc_int<32> > ack_N, ack_E, ack_S, ack_W, ack_L; //Controle de fluxo
	sc_signal<sc_int<32> > wokN, wokE, wokS, wokW, wokL, wrN, wrE, wrS, wrW, wrL; //Controle de fluxo
	sc_signal<sc_int<32> > rokN, rokE, rokS, rokW, rokL, rdN, rdE, rdS, rdW, rdL; //Chaveamento //Entrada para o chaveamento no buffer (falta achar a entrada para rd)
	sc_signal<sc_int<32> > portExit_N, portExit_E, portExit_S, portExit_W, portExit_L, routerDestiny_N, routerDestiny_E, routerDestiny_S, routerDestiny_W, routerDestiny_L; //Entrada e saída do roteamento

	sc_int<32> aux_rokN, aux_rokE, aux_rokS, aux_rokW, aux_rokL;

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

		//Ligação do roteamento nos buffers
		rtgN->tabela = tabela;
		rtgE->tabela = tabela;
		rtgS->tabela = tabela;
		rtgW->tabela = tabela;
		rtgL->tabela = tabela;

		rtgN->position = position;
		rtgE->position = position;
		rtgS->position = position;
		rtgW->position = position;
		rtgL->position = position;

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

		if(aux_rokN == 1){
			out_portN = bfN->dout;
		}

		if(aux_rokE == 1){
			out_portE = bfE->dout;
		}

		if(aux_rokS == 1){
			out_portS = bfS->dout;
		}

		if(aux_rokW == 1){
			out_portW = bfW->dout;
		}

		if(aux_rokL == 1){
			out_portL = bfL->dout;
		}

		bfN->din = in_portN;
		bfE->din = in_portE;
		bfS->din = in_portS;
		bfW->din = in_portW;
		bfL->din = in_portL;

		//Ligação dos entradas do controle de fluxo para as portas
		fcN->in_val(in_val_N);
		fcN->in_ack(ack_N);
		fcE->in_val(in_val_E);
		fcE->in_ack(ack_E);
		fcS->in_val(in_val_S);
		fcS->in_ack(ack_S);
		fcW->in_val(in_val_W);
		fcW->in_ack(ack_W);
		fcL->in_val(in_val_L);
		fcL->in_ack(ack_L);

		SC_METHOD(set_ack);
		sensitive << in_val_N, in_val_E, in_val_S, in_val_W, in_val_L;
	}
};