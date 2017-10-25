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
	sc_in<sc_int<32> > in_val_N{"in_val_N"};
	sc_in<sc_int<32> > in_val_E{"in_val_E"};
	sc_in<sc_int<32> > in_val_S{"in_val_S"};
	sc_in<sc_int<32> > in_val_W{"in_val_W"};
	sc_in<sc_int<32> > in_val_L{"in_val_L"};

	sc_out<sc_int<32> > out_val_N{"out_val_N"};
	sc_out<sc_int<32> > out_val_E{"out_val_E"};
	sc_out<sc_int<32> > out_val_S{"out_val_S"};
	sc_out<sc_int<32> > out_val_W{"out_val_W"};
	sc_out<sc_int<32> > out_val_L{"out_val_L"};

	sc_in<sc_int<32> > in_ack_N{"in_ack_N"};
	sc_in<sc_int<32> > in_ack_E{"in_ack_E"};
	sc_in<sc_int<32> > in_ack_S{"in_ack_S"};
	sc_in<sc_int<32> > in_ack_W{"in_ack_W"};
	sc_in<sc_int<32> > in_ack_L{"in_ack_L"};

	sc_out<sc_int<32> > out_ack_N{"out_ack_N"};
	sc_out<sc_int<32> > out_ack_E{"out_ack_E"};
	sc_out<sc_int<32> > out_ack_S{"out_ack_S"};
	sc_out<sc_int<32> > out_ack_W{"out_ack_W"};
	sc_out<sc_int<32> > out_ack_L{"out_ack_L"};
	
	//Sinais de comunicação com os buffers
	sc_signal<sc_int<32> > wok_N;
	sc_signal<sc_int<32> > wok_E;
	sc_signal<sc_int<32> > wok_S;
	sc_signal<sc_int<32> > wok_W;
	sc_signal<sc_int<32> > wok_L;

	sc_signal<sc_int<32> > wr_N;
	sc_signal<sc_int<32> > wr_E;
	sc_signal<sc_int<32> > wr_S;
	sc_signal<sc_int<32> > wr_W;
	sc_signal<sc_int<32> > wr_L;

	//Sinais para controle de chaveamento
	sc_signal<sc_int<32>, SC_MANY_WRITERS> rd_N;
	sc_signal<sc_int<32>, SC_MANY_WRITERS> rd_E;
	sc_signal<sc_int<32>, SC_MANY_WRITERS> rd_S;
	sc_signal<sc_int<32>, SC_MANY_WRITERS> rd_W;
	sc_signal<sc_int<32>, SC_MANY_WRITERS> rd_L;

	sc_signal<sc_int<32> > rok_N;
	sc_signal<sc_int<32> > rok_E;
	sc_signal<sc_int<32> > rok_S;
	sc_signal<sc_int<32> > rok_W;
	sc_signal<sc_int<32> > rok_L;

	//Sinais de destino do pacote e saída do roteamento
	sc_signal<sc_int<32> > destiny_N;
	sc_signal<sc_int<32> > destiny_E;
	sc_signal<sc_int<32> > destiny_S;
	sc_signal<sc_int<32> > destiny_W;
	sc_signal<sc_int<32> > destiny_L;

	sc_signal<sc_int<32> > portDestiny_N;
	sc_signal<sc_int<32> > portDestiny_E;
	sc_signal<sc_int<32> > portDestiny_S;
	sc_signal<sc_int<32> > portDestiny_W;
	sc_signal<sc_int<32> > portDestiny_L;
	

	sc_int<32> portExitN, portExitE, portExitS, portExitW, portExitL;
	sc_int<32> rokN, rokE, rokS, rokW, rokL;

	void request_arbiter(){
		if(portExitN == NORTH){
			arbN->bufferCircular[NORTH] = 1;
		}else if(portExitN == EAST){
			arbE->bufferCircular[NORTH] = 1;
		}else if(portExitN == SOUTH){
			arbS->bufferCircular[NORTH] = 1;
		}else if(portExitN == WEST){
			arbW->bufferCircular[NORTH] = 1;
		}else if(portExitN == LOCAL){
			arbL->bufferCircular[NORTH] = 1;
		}

		if(portExitE == NORTH){
			arbN->bufferCircular[EAST] = 1;
		}else if(portExitE == EAST){
			arbE->bufferCircular[EAST] = 1;
		}else if(portExitE == SOUTH){
			arbS->bufferCircular[EAST] = 1;
		}else if(portExitE == WEST){
			arbW->bufferCircular[EAST] = 1;
		}else if(portExitE == LOCAL){
			arbL->bufferCircular[EAST] = 1;
		}

		if(portExitS == NORTH){
			arbN->bufferCircular[SOUTH] = 1;
		}else if(portExitS == EAST){
			arbE->bufferCircular[SOUTH] = 1;
		}else if(portExitS == SOUTH){
			arbS->bufferCircular[SOUTH] = 1;
		}else if(portExitS == WEST){
			arbW->bufferCircular[SOUTH] = 1;
		}else if(portExitS == LOCAL){
			arbL->bufferCircular[SOUTH] = 1;
		}

		if(portExitW == NORTH){
			arbN->bufferCircular[WEST] = 1;
		}else if(portExitW == EAST){
			arbE->bufferCircular[WEST] = 1;
		}else if(portExitW == SOUTH){
			arbS->bufferCircular[WEST] = 1;
		}else if(portExitW == WEST){
			arbW->bufferCircular[WEST] = 1;
		}else if(portExitW == LOCAL){
			arbL->bufferCircular[WEST] = 1;
		}

		if(portExitL == NORTH){
			arbN->bufferCircular[LOCAL] = 1;
		}else if(portExitL == EAST){
			arbE->bufferCircular[LOCAL] = 1;
		}else if(portExitL == SOUTH){
			arbS->bufferCircular[LOCAL] = 1;
		}else if(portExitL == WEST){
			arbW->bufferCircular[LOCAL] = 1;
		}else if(portExitL == LOCAL){
			arbL->bufferCircular[LOCAL] = 1;
		}

	}

	void execute(){
		//Chaveamento do buffer norte
		if((portExitN == NORTH) && (arbN->priority == NORTH)){
			out_val_N.write(1);
			if(in_ack_N.read() == 1){
				if(rokN == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portN = bfN->dout;
					arbN->bufferCircular[NORTH] = 0;
				}
			}
		}else if((portExitN == EAST) && (arbE->priority == NORTH)){
			out_val_E.write(1);
			if(in_ack_E.read() == 1){
				if(rokN == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portE = bfN->dout;
					arbE->bufferCircular[NORTH] = 0;
				}
			}
		}else if((portExitN == SOUTH) && (arbS->priority == NORTH)){
			out_val_S.write(1);
			if(in_ack_S.read() == 1){
				if(rokN == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portS = bfN->dout;
					arbS->bufferCircular[NORTH] = 0;
				}				
			}
		}else if((portExitN == WEST) && (arbW->priority == NORTH)){
			out_val_W.write(1);
			if(in_ack_W.read() == 1){
				if(rokN == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portW = bfN->dout;
					arbW->bufferCircular[NORTH] = 0;
				}
			}
		}else if((portExitN == LOCAL) && (arbL->priority == NORTH)){
			out_val_L.write(1);
			if(in_ack_L.read() == 1){
				if(rokN == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portL = bfN->dout;
					arbL->bufferCircular[NORTH] = 0;
				}
			}
		}


		//Chaveamento do buffer leste
		if((portExitE == NORTH) && (arbN->priority == EAST)){
			out_val_N.write(1);
			if(in_ack_E.read() == 1){
				if(rokE == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portN = bfE->dout;
					arbN->bufferCircular[EAST] = 0;
				}
			}
		}else if((portExitE == EAST) && (arbE->priority == EAST)){
			out_val_E.write(1);
			if(in_ack_E.read() == 1){
				if(rokE == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portE = bfE->dout;
					arbE->bufferCircular[EAST] = 0;
				}				
			}
		}else if((portExitE == SOUTH) && (arbS->priority == EAST)){
			out_val_S.write(1);
			if(in_ack_S.read() == 1){
				if(rokE == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portS = bfE->dout;
					arbS->bufferCircular[EAST] = 0;	
				}				
			}
		}else if((portExitE == WEST) && (arbW->priority == EAST)){
			out_val_W.write(1);
			if(in_ack_W.read() == 1){
				if(rokE == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portW = bfE->dout;
					arbW->bufferCircular[EAST] = 0;
				}
			}
		}else if((portExitE == LOCAL) && (arbL->priority == EAST)){
			out_val_L.write(1);
			if(in_ack_L.read() == 1){
				if(rokE == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portL = bfE->dout;
					arbL->bufferCircular[EAST] = 0;
				}				
			}
		}


		//Chaveamento buffer sul
		if((portExitS == NORTH) && (arbN->priority == SOUTH)){
			out_val_N.write(1);
			if(in_ack_N.read() == 1){
				if(rokS == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portN = bfS->dout;
					arbN->bufferCircular[SOUTH] = 0;
				}				
			}
		}else if((portExitS == EAST) && (arbE->priority == SOUTH)){
			out_val_E.write(1);
			if(in_ack_E.read() == 1){
				if(rokS == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portE = bfS->dout;
					arbE->bufferCircular[SOUTH] = 0;
				}				
			}
		}else if((portExitS == SOUTH) && (arbS->priority == SOUTH)){
			out_val_S.write(1);
			if(in_ack_S.read() == 1){
				if(rokS == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portS = bfS->dout;
					arbS->bufferCircular[SOUTH] = 0;
				}				
			}
		}else if((portExitS == WEST) && (arbW->priority == SOUTH)){
			out_val_W.write(1);
			if(in_ack_W.read() == 1){
				if(rokS == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portW = bfS->dout;
					arbW->bufferCircular[SOUTH] = 0;
				}				
			}
		}else if((portExitS == LOCAL) && (arbL->priority == SOUTH)){
			out_val_L.write(1);
			if(in_ack_L.read() == 1){
				if(rokS == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portL = bfS->dout;
					arbL->bufferCircular[SOUTH] = 0;
				}				
			}
		}


		//Chaveamento do buffer oeste
		if((portExitW == NORTH) && (arbN->priority == WEST)){
			out_val_N.write(1);
			if(in_ack_N.read() == 1){
				if(rokW == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portN = bfW->dout;
					arbN->bufferCircular[WEST] = 0;
				}				
			}
		}else if((portExitW == EAST) && (arbE->priority == WEST)){
			out_val_E.write(1);
			if(in_ack_E.read() == 1){
				if(rokW == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portE = bfW->dout;
					arbE->bufferCircular[WEST] = 0;
				}				
			}
		}else if((portExitW == SOUTH) && (arbS->priority == WEST)){
			out_val_S.write(1);
			if(in_ack_S.read() == 1){
				if(rokW == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portS = bfW->dout;
					arbS->bufferCircular[WEST] = 0;
				}				
			}
		}else if((portExitW == WEST) && (arbW->priority == WEST)){
			out_val_W.write(1);
			if(in_ack_W.read() == 1){
				if(rokW == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portW = bfW->dout;
					arbW->bufferCircular[WEST] = 0;
				}				
			}
		}else if((portExitW == LOCAL) && (arbL->priority == WEST)){
			out_val_L.write(1);
			if(in_ack_L.read() == 1){
				if(rokW == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portL = bfW->dout;
					arbL->bufferCircular[WEST] = 0;
				}				
			}
		}


		//Chaveamento para o buffer local
		if((portExitL == NORTH) && (arbN->priority == LOCAL)){
			out_val_N.write(1);
			if(in_ack_N.read() == 1){
				if(rokL == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portN = bfL->dout;
					arbN->bufferCircular[LOCAL] = 0;
				}				
			}
		}else if((portExitL == EAST) && (arbE->priority == LOCAL)){
			out_val_E.write(1);
			if(in_ack_E.read() == 1){
				if(rokL == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portE = bfL->dout;
					arbE->bufferCircular[LOCAL] = 0;
				}				
			}
		}else if((portExitL == SOUTH) && (arbS->priority == LOCAL)){
			out_val_S.write(1);
			if(in_ack_S.read() == 1){
				if(rokL == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portS = bfL->dout;
					arbS->bufferCircular[LOCAL] = 0;
				}				
			}
		}else if((portExitL == WEST) && (arbW->priority == LOCAL)){
			out_val_W.write(1);
			if(in_ack_W.read() == 1){
				if(rokL == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portW = bfL->dout;
					arbW->bufferCircular[LOCAL] = 0;
				}				
			}
		}else if((portExitL == LOCAL) && (arbL->priority == LOCAL)){
			out_val_L.write(1);
			if(in_ack_L.read() == 1){
				if(rokL == 0){
					cout << "Buffer Vazio" << endl;
				}else{
					out_portL = bfL->dout;
					arbL->bufferCircular[LOCAL] = 0;
				}
			}
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

		//Ligação dos entradas do controle de fluxo para as portas
		fcN->in_val(in_val_N);
		fcE->in_val(in_val_E);
		fcS->in_val(in_val_S);
		fcW->in_val(in_val_W);
		fcL->in_val(in_val_L);

		//Ligação da saída do controle de fluxo
		fcN->out_ack(out_ack_N);
		fcE->out_ack(out_ack_E);
		fcS->out_ack(out_ack_S);
		fcW->out_ack(out_ack_W);
		fcL->out_ack(out_ack_L);

		//Ligação do controle de fluxo e os buffers
		fcN->wok(wok_N);
		fcN->wr(wr_N);
		fcE->wok(wok_E);
		fcE->wr(wr_E);
		fcS->wok(wok_S);
		fcS->wr(wr_S);
		fcW->wok(wok_W);
		fcW->wr(wr_W);
		fcL->wok(wok_L);
		fcL->wr(wr_L);
	
		//Ligação do clock do controle de fluxo
		fcN->clk(clk);
		fcE->clk(clk);
		fcS->clk(clk);
		fcW->clk(clk);
		fcL->clk(clk);


		//Ligação entre os buffers e controle de fluxo
		bfN->wr(wr_N);
		bfN->wok(wok_N);
		bfE->wr(wr_E);
		bfE->wok(wok_E);
		bfS->wr(wr_S);
		bfS->wok(wok_S);
		bfW->wr(wr_W);
		bfW->wok(wok_W);
		bfL->wr(wr_L);
		bfL->wok(wok_L);

		//Controle do chaveamento
		bfN->rd(rd_N);
		bfE->rd(rd_E);
		bfS->rd(rd_S);
		bfW->rd(rd_W);
		bfL->rd(rd_L);

		bfN->rok(rok_N);
		bfE->rok(rok_E);
		bfS->rok(rok_S);
		bfW->rok(rok_W);
		bfL->rok(rok_L);

		//Ligação das entradas dos buffers
		bfN->din = in_portN;
		bfE->din = in_portE;
		bfS->din = in_portS;
		bfW->din = in_portW;
		bfL->din = in_portL;

		//Ligação do clock dos buffers
		bfN->clk(clk);
		bfE->clk(clk);
		bfS->clk(clk);
		bfW->clk(clk);
		bfL->clk(clk);

		//Destino dos flits que estão nos buffers;
		destiny_N = bfN->destiny_flit;
		destiny_E = bfE->destiny_flit;
		destiny_S = bfS->destiny_flit;
		destiny_W = bfW->destiny_flit;
		destiny_L = bfL->destiny_flit;


		//Ligação dos roteamentos
		rtgN->clk(clk);
		rtgE->clk(clk);
		rtgS->clk(clk);
		rtgW->clk(clk);
		rtgL->clk(clk);

		rtgN->tabela = tabela;
		rtgE->tabela = tabela;
		rtgS->tabela = tabela;
		rtgW->tabela = tabela;
		rtgL->tabela = tabela;

		rtgN->destiny(destiny_N);
		rtgE->destiny(destiny_E);
		rtgS->destiny(destiny_S);
		rtgW->destiny(destiny_W);
		rtgL->destiny(destiny_L);

		rtgN->portDestiny(portDestiny_N);
		rtgE->portDestiny(portDestiny_E);
		rtgS->portDestiny(portDestiny_S);
		rtgW->portDestiny(portDestiny_W);
		rtgL->portDestiny(portDestiny_L);


		//Ligação dos componentes dos árbitros
		arbN->clk(clk);
		arbE->clk(clk);
		arbS->clk(clk);
		arbW->clk(clk);
		arbL->clk(clk);

		arbN->rdN(rd_N);
		arbN->rdE(rd_E);
		arbN->rdS(rd_S);
		arbN->rdW(rd_W);
		arbN->rdL(rd_L);

		arbE->rdN(rd_N);
		arbE->rdE(rd_E);
		arbE->rdS(rd_S);
		arbE->rdW(rd_W);
		arbE->rdL(rd_L);
		
		arbS->rdN(rd_N);
		arbS->rdE(rd_E);
		arbS->rdS(rd_S);
		arbS->rdW(rd_W);
		arbS->rdL(rd_L);
		
		arbW->rdN(rd_N);
		arbW->rdE(rd_E);
		arbW->rdS(rd_S);
		arbW->rdW(rd_W);
		arbW->rdL(rd_L);
		
		arbL->rdN(rd_N);
		arbL->rdE(rd_E);
		arbL->rdS(rd_S);
		arbL->rdW(rd_W);
		arbL->rdL(rd_L);
		
		//Seta os destino dos flits
		portExitN = portDestiny_N;
		portExitE = portDestiny_E;
		portExitS = portDestiny_S;
		portExitW = portDestiny_W;
		portExitL = portDestiny_L;

		//Seta os as saídas rok dos buffers
		rok_N = rokN;
		rok_E = rokE;
		rok_S = rokS;
		rok_W = rokW;
		rok_L = rokL;


		SC_METHOD(execute);
		sensitive << clk.pos();
		SC_METHOD(request_arbiter);
		sensitive << clk.pos();
		//SC_METHOD(conexao_fc);
		//sensitive << in_val_N << in_val_E << in_val_S << in_val_W << in_val_L << clk;
	}
};