#include "systemc.h"
#include "parameters.h"
#include "flit.h"

class packet{
	public:
		flit flit_packet[SIZE_FLIT_PACKET];
	packet();
	~packet();
	void set_flit_packet(flit flit_packet[SIZE_FLIT_PACKET]);
	void set_destino(Coordinated destiny);
}