#include "systemc.h"
#include "parameters.h"

SC_MODULE(arbiter){
	sc_in<bool> clk;
	sc_in<sc_int<32> > portDestiny;
	sc_out<sc_int<32> > rd;
	sc_int<32> bufferCircular[5];
	sc_int<32> priority;
	sc_int<32> pointer;

	//Caso no buffer tenham pacotes passando a prioridade só passa para o próximo buffer quando o mesmo terminar de enviar um pacote.
	void setPriority(){
		if (this->bufferCircular[NORTH] == 1){
			this->priority = 1;
			rd.write(1);
		}else if (this->bufferCircular[EAST] == 1){
			this->priority = 1;
			rd.write(1);
		}else if (this->bufferCircular[SOUTH] == 1){
			this->priority = 1;
			rd.write(1);
		}else if (this->bufferCircular[WEST] == 1){
			this->priority = 1;
			rd.write(1);
		}else if (this->bufferCircular[LOCAL] == 1){
			this->priority = 1;
			rd.write(1);
		}
	}


	//Verifica em qual buffer se encontra a prioridade no momento
	void checkPriority(){
		if (bufferCircular[NORTH] == 1){
			this->pointer = NORTH;
		}else if (bufferCircular[EAST] == 1){
			this->pointer = EAST;
		}else if (bufferCircular[SOUTH] == 1){
			this->pointer = SOUTH;
		}else if (bufferCircular[WEST] == 1){
			this->pointer = WEST;
		}else if (bufferCircular[LOCAL] == 1){
			this->pointer = LOCAL;
		} 
	}

	//Verifica a disponibilidade do buffer que estão tentando enviar pacotes
	//Vai escrever na porta de saída work se o buffer ta enviando, esperando ou livre
	void checkAvailiability(){
		if (portDestiny.read() == NORTH){
			this->bufferCircular[NORTH] = 1;
		}
		if (portDestiny.read() == EAST){
			this->bufferCircular[EAST] = 1;
		}
		if (portDestiny.read() == SOUTH){
			this->bufferCircular[SOUTH] = 1;
		}
		if (portDestiny.read() == WEST){
			this->bufferCircular[WEST] = 1;
		}
		if (portDestiny.read() == LOCAL){
			this->bufferCircular[LOCAL] = 1;
		}
	}

	SC_CTOR(arbiter){

		priority = NORTH;

		for (int i = 0; i < 5; ++i){
			bufferCircular[i] = 0;
		}

		SC_METHOD(checkAvailiability);
		sensitive << portDestiny << clk;
	}
};