//#include "router.h"

void router::setPosicion(){
	posicionOut.write(this->posicion);
}

void router::setTabela(){
	tabelaOut.write(tabela);
}