#include "systemc.h"
#include <iostream>
#include <queue>
#include "buffer.h"
#include "parameters.h"

using namespace std;

int sc_main (int argc, char* argv[]){

sc_clock clock("Clock", 10, SC_NS, 0.5);

flit in, out;
in.type = 1;
in.payload = 0;
in.destiny = 8;

Buffer bf("Buffer");
bf.clk(clock);
bf.din = in;
bf.wr.write(1);
bf.rd.write(1);

sc_start();
cout << bf.flits.size() << endl;

}