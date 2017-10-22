#include "systemc.h"
#include <iostream>
#include <queue>
#include "buffer.h"
#include "parameters.h"

using namespace std;

int sc_main (int argc, char* argv[]){

sc_clock clock("Clock", 10, SC_NS, 0.5);
sc_signal<sc_int<32> > wr, wok, rd, rok;

wr = 1;
rd = 1;

flit in, out;
in.type = 1;
in.payload = 0;
in.destiny = 8;

Buffer bf("Buffer");
bf.clk(clock);
bf.din = in;
bf.dout;
bf.wr(wr);
bf.wok(wok);
bf.rd(rd);
bf.rok(rok);

sc_start(10, SC_NS);
cout << bf.dout.destiny << endl;

}