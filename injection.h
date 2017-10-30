#include "systemc.h"
#include <iostream>
#include "router.h"
#include "parameters.h"

using namespace std;

SC_MODULE(injection){
	sc_in<bool> clk;
	
}