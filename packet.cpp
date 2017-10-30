#import "packet.h"

packet::packet(){

}

packet::~packet(){

}

packet::void set_flit_packet(){
	for (int i = 0; i < SIZE_FLIT_PACKET; i++){
		if (i == 0){
			this->flit_packet[i].type = BEGIN_PACKET;
			this->flit_packet[i].payload = NULL;
			this->flit_packet[i].destiny = this->destiny;
		}else if(i == (SIZE_FLIT_PACKET - 1)){
			this->flit_packet[i].type = END_PACKET;
			this->flit_packet[i].payload = NULL;
			this->flit_packet[i].destiny = this->destiny;
		}else{
			this->flit_packet[i].type = INSTRUCTION;
			this->flit_packet[i].payload = 0;
			this->flit_packet[i].destiny = this->destiny;
		}
	}
}
