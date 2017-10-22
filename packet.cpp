#import "packet.h"

packet::packet(){

}

packet::~packet(){

}

packet::void set_flit_packet(flit flit_packet[SIZE_FLIT_PACKET]){
	for (int i = 0; i < SIZE_FLIT_PACKET; i++){
		if (i == 0){
			flit_packet[i].type = BEGIN_PACKET;
			flit_packet[i].payload = NULL;
			flit_packet[i].destiny = this->destiny;
		}else if(i == (SIZE_FLIT_PACKET - 1)){
			flit_packet[i].type = END_PACKET;
			flit_packet[i].payload = NULL;
			flit_packet[i].destiny = this->destiny;
		}else{
			flit_packet[i].type = INSTRUCION;
			flit_packet[i].payload = 0;
			flit_packet[i].destiny = this->destiny;
		}
	}
	
	this->flit_packet = flit_packet;
}
