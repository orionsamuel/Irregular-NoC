#include "router.h"
#include "parameters.h"

SC_MODULE(main){
	router *r0, *r1, *r2;

	

	SC_CTOR(main){
		r0 = new router("r0");
		r1 = new router("r1");
		r2 = new router("r2");

		//Mapeamento dos roteadores
		r0->position = 0;
		r1->position = 1;
		r2->position = 2;

		r0->coreNumbers = 3;
		r1->coreNumbers = 3;
		r2->coreNumbers = 3;

		r0->out_portW = r1->in_portN;
		r0->out_portE = r2->in_portN;

		r1->out_portN = r0->in_portW;
		r1->out_portE = r2->in_portW;

		r2->out_portN = r0->in_portE;
		r2->out_portW = r1->in_portE;

		routing_table table0;
		routing_table table1;
		routing_table table2;

		table0.push_back({10, 20, 30});
		table1.push_back({10, 20, 30});
		table2.push_back({10, 20, 30});

		r0->tabela = table0;
		r1->tabela = table1;
		r2->tabela = table2;
	}

};


int sc_main (int argc, char* argv[]){
	
	

	return 0;
}
