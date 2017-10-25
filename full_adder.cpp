#include "systemc.h"
#include "half_adder.cpp"
#include <iostream>

using namespace std;

SC_MODULE(fa){
	sc_in<bool> x, y, vem;
	sc_out<bool> soma, vai;

	sc_signal<bool> s1, s2, s3;

	ha_sc *h1, *h2;

	void proc(){
		vai = s2 | s3;
		cout << h1->v << endl;
	}

	SC_CTOR(fa){
		h1 = new ha_sc("h1");
		h2 = new ha_sc("h2");

		h1->x(x);
		h1->y(y);
		h1->s(s1);
		h1->v(s2);

		h2->x(s1);
		h2->y(vem);
		h2->s(soma);
		h2->v(s3);

		SC_METHOD(proc);
		sensitive << s2, s3;
	}
};