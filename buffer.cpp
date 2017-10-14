//#include "buffer.h"



//Funcionando
void Buffer::add() {
	if (wr.read() == 1)
	{
		if (flits.size() == this->length){
			wok.write(0); // error, value not added
		} else {
			this->flits.push(din);
			wok.write(1);
		}	
	}
	
}
//Funcionando
void Buffer::remove() {
	if (flits.size() < 1){
		rok.write(0); // queue is empty. Nothing to remove
	} else if (rd.read() == 1){
		flits.pop();
		rok.write(1);
	}
}

//Se estiver Vazio retorna 1, senão retorna 0
int Buffer::isEmpty() {
	return (this->flits.empty() == 1) ? 1: 0;
}

