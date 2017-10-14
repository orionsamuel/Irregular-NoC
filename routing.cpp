//#include "routing.h"

void routing::returnSize(){
	this->size = tabela.size();
}

void routing::tableAcess(){
	this->counter;
	for(int i; i < this->size; i++){
		if(tabela[i].destiny.read() == destiny){
			if(tabela[i].hops.read() < this->counter){
				this->counter = tabela[i].hops.read();	
			}
		}
	}
}

void routing::setDestiny(){
	for(int i; i < this->size; i++){
		if(tabela[i].hops.read() == this->counter){
			portDestiny.write(tabela[i].port.read());
		}
	}
}
