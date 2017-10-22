//#include "routing.h"

void routing::tableAcess(){
	for(int i = 0; i < tabela.size(); i++){
			if(tabela[i].destiny == destiny.read()){
				if(tabela[i].hops < this->counter){
					this->counter = tabela[i].hops;	
				}
			}
		}
}

void routing::setDestiny(){
	for(int i = 0; i < tabela.size(); i++){
		if(tabela[i].hops == this->counter){
				portDestiny.write(tabela[i].port);
		}
	}
}
