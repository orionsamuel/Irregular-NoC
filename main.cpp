#include "router.h"
#include "parameters.h"

SC_MODULE(main){
	router *r0;

	SC_CTOR(main){
		r0 = new router("r0");

		routing_table table;
		table.push_back({10, 20, 30});

		r0->tabela = table;
	}

};


int sc_main (int argc, char* argv[]){
	

	//router *rout;
	//rout = new router("rout");
	//router rout("rout");
	//rout.position(position);
	//rout.teste2 = 3;

	return 0;
}
