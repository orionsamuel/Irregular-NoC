//#include "flow_control.h"

void flow_control::request(){
	if(in_val.read() == 1){
		wr.write(1);
	}
}

void flow_control::response(){
	if(wok.read() == 1){
		in_ack.write(1);
	}
}