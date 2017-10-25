#include "systemc.h"
#include <iostream>
#include "router2.h"
#include "parameters.h"
#include <vector>

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS);
	sc_signal<sc_int<32> > in_val_N;
	sc_signal<sc_int<32> > in_val_E;
	sc_signal<sc_int<32> > in_val_S;
	sc_signal<sc_int<32> > in_val_W;
	sc_signal<sc_int<32> > in_val_L;

	sc_signal<sc_int<32> > out_val_N;
	sc_signal<sc_int<32> > out_val_E;
	sc_signal<sc_int<32> > out_val_S;
	sc_signal<sc_int<32> > out_val_W;
	sc_signal<sc_int<32> > out_val_L;

	sc_signal<sc_int<32> > in_ack_N;
	sc_signal<sc_int<32> > in_ack_E;
	sc_signal<sc_int<32> > in_ack_S;
	sc_signal<sc_int<32> > in_ack_W;
	sc_signal<sc_int<32> > in_ack_L;

	sc_signal<sc_int<32> > out_ack_N;
	sc_signal<sc_int<32> > out_ack_E;
	sc_signal<sc_int<32> > out_ack_S;
	sc_signal<sc_int<32> > out_ack_W;
	sc_signal<sc_int<32> > out_ack_L;

	routing_table table0;

	table0.push_back({3, WEST, 5});
	table0.push_back({4, EAST, 2});
	table0.push_back({3, NORTH, 8});
	table0.push_back({1, EAST, 1});

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

	in_portL.type = 1;
	in_portL.payload = 8;
	in_portL.destiny = 3;

	in_val_N.write(1);

	router rt("ROUTER");
	rt.clk(clock);

	rt.wok[4].write(1);

	rt.in_val[0].write(in_val_N);
	rt.in_val[1].write(in_val_E);
	rt.in_val[2].write(in_val_S);
	rt.in_val[3].write(in_val_W);
	rt.in_val[4].write(in_val_L);

	rt.out_val[0].write(out_val_N);
	rt.out_val[1].write(out_val_E);
	rt.out_val[2].write(out_val_S);
	rt.out_val[3].write(out_val_W);
	rt.out_val[4].write(out_val_L);

	rt.in_ack[0].write(in_ack_N);
	rt.in_ack[1].write(in_ack_E);
	rt.in_ack[2].write(in_ack_S);
	rt.in_ack[3].write(in_ack_W);
	rt.in_ack[4].write(in_ack_L);

	rt.out_ack[0].write(out_ack_N);
	rt.out_ack[1].write(out_ack_E);
	rt.out_ack[2].write(out_ack_S);
	rt.out_ack[3].write(out_ack_W);
	rt.out_ack[4].write(out_ack_L);

	rt.tabela = table0;

	rt.in_portN = in_portN;
	rt.in_portE = in_portE;
	rt.in_portS = in_portS;
	rt.in_portW = in_portW;
	rt.in_portL = in_portL;

	out_portN = rt.out_portN;
	out_portE = rt.out_portE;
	out_portS = rt.out_portS;
	out_portW = rt.out_portW;
	out_portL = rt.out_portL;


	sc_start(10, SC_NS);

	//cout << rt.out_portL.payload << endl;

}