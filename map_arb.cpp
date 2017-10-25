	void map_arb(){
		arbL->bufferCircular[EAST] = 1;

		if(arbN->rd[0].read() == 1){
			rd[0].write(1);
		}else{
			rd[0].write(0);
		}
		if(arbN->rd[1].read() == 1){
			rd[1].write(1);
		}else{
			rd[1].write(0);
		}
		if(arbN->rd[2].read() == 1){
			rd[2].write(1);
		}else{
			rd[2].write(0);
		}
		if(arbN->rd[3].read() == 1){
			rd[3].write(1);
		}else{
			rd[3].write(0);
		}
		if(arbN->rd[4].read() == 1){
			rd[4].write(1);
		}else{
			rd[4].write(0);
		}


		if(arbE->rd[0].read() == 1){
			rd[0].write(1);
		}else{
			rd[0].write(0);
		}
		if(arbE->rd[1].read() == 1){
			rd[1].write(1);
		}else{
			rd[1].write(0);
		}
		if(arbE->rd[2].read() == 1){
			rd[2].write(1);
		}else{
			rd[2].write(0);
		}
		if(arbE->rd[3].read() == 1){
			rd[3].write(1);
		}else{
			rd[3].write(0);
		}
		if(arbE->rd[4].read() == 1){
			rd[4].write(1);
		}else{
			rd[4].write(0);
		}


		if(arbS->rd[0].read() == 1){
			rd[0].write(1);
		}else{
			rd[0].write(0);
		}
		if(arbS->rd[1].read() == 1){
			rd[1].write(1);
		}else{
			rd[1].write(0);
		}
		if(arbS->rd[2].read() == 1){
			rd[2].write(1);
		}else{
			rd[2].write(0);
		}
		if(arbS->rd[3].read() == 1){
			rd[3].write(1);
		}else{
			rd[3].write(0);
		}
		if(arbS->rd[4].read() == 1){
			rd[4].write(1);
		}else{
			rd[4].write(0);
		}


		if(arbW->rd[0].read() == 1){
			rd[0].write(1);
		}else{
			rd[0].write(0);
		}
		if(arbW->rd[1].read() == 1){
			rd[1].write(1);
		}else{
			rd[1].write(0);
		}
		if(arbW->rd[2].read() == 1){
			rd[2].write(1);
		}else{
			rd[2].write(0);
		}
		if(arbW->rd[3].read() == 1){
			rd[3].write(1);
		}else{
			rd[3].write(0);
		}
		if(arbW->rd[4].read() == 1){
			rd[4].write(1);
		}else{
			rd[4].write(0);
		}


		if(arbL->rd[0].read() == 1){
			rd[0].write(1);
		}else{
			rd[0].write(0);
		}
		if(arbL->rd[1].read() == 1){
			rd[1].write(1);
		}else{
			rd[1].write(0);
		}
		if(arbL->rd[2].read() == 1){
			rd[2].write(1);
		}else{
			rd[2].write(0);
		}
		if(arbL->rd[3].read() == 1){
			rd[3].write(1);
		}else{
			rd[3].write(0);
		}
		if(arbL->rd[4].read() == 1){
			rd[4].write(1);
		}else{
			rd[4].write(0);
		}
	}