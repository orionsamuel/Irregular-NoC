#include "systemc.h"
#include "parameters.h"

class packet{
	public:
		flit flit_packet[SIZE_FLIT_PACKET];
		sc_int<32> destiny;
	packet();
	~packet();
	void set_flit_packet();
};