#import "packet.h"

packet::packet(){

}

packet::~packet(){

}

packet::void set_flit_packet(flit flit_packet[SIZE_FLIT_PACKET]){
	for (int i = 0; i < SIZE_FLIT_PACKET; i++){
		if (i == 0){
			flit_packet.type = BEGIN_PACKET;
			flit_packet.payload = NULL;
		}else if(i == (SIZE_FLIT_PACKET - 1)){
			flit_packet.type = END_PACKET;
			flit_packet.payload = NULL;
		}else{
			flit_packet.type = INSTRUCION;
			flit_packet.payload = 0;
		}
	}
	
	this->flit_packet = flit_packet;
}

packet::void set_destino(Coordinated destiny){
	this->flit_packet.destiny = destiny;
}
