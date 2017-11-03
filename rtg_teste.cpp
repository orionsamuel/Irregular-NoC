#include "systemc.h"
#include <iostream>
#include <vector>
#include "routing3.h"
#include "parameters.h"

using namespace std;

int sc_main (int argc, char* argv[]){

	sc_clock clock("Clock", 10, SC_NS, 0.5);
	routing_table table[4];

	table[0].push_back({1, LOCAL, 0});
	table[0].push_back({2, NORTH, 1});
	table[0].push_back({3, NORTH, 2});
	table[0].push_back({4, NORTH, 3});

	table[1].push_back({2, LOCAL, 0});
	table[1].push_back({1, SOUTH, 1});
	table[1].push_back({3, EAST, 1});
	table[1].push_back({4, EAST, 2});

	table[2].push_back({3, LOCAL, 0});
	table[2].push_back({1, WEST, 2});
	table[2].push_back({2, WEST, 1});
	table[2].push_back({4, NORTH, 1});

	table[3].push_back({4, LOCAL, 0});
	table[3].push_back({1, SOUTH, 3});
	table[3].push_back({2, SOUTH, 2});
	table[3].push_back({3, SOUTH, 1});

	routing rtg;
	//rtg.clk(clock);
	rtg.destiny = 1;
	rtg.tabela = table[0];


	sc_start(50, SC_NS);

	int destino = rtg.tableAcess();

	cout << rtg.tabela.size() << endl;
	cout << destino << endl;

}