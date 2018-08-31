#ifndef CAMINHO_MIN_H_INCLUDED
#define CAMINHO_MIN_H_INCLUDED
#include "parameters.h"
#include <iostream>

using namespace std;

class caminho_min{
public: 
	//int dist[25][25];
	//int caminho[25][25];

	void floyd(int dist[10][10], int caminho[10][10]){
		for (int k = 0; k < 10; k++){
        	for (int i = 0; i < 10; i++){
            	for (int j = 0; j < 10; j++){
            		//Calculo do menor caminho
                	 if ( dist[i][k] + dist[k][j] < dist[i][j] ){
                    	dist[i][j] = dist[i][k] + dist[k][j];
                    	//Preenchimento da tabela de caminho
                    	caminho[i][j] = k;
                    	if(caminho[i][k] <  caminho[i][j]){
                    	    caminho[i][j] = caminho[i][k];
                    	}

                	} else if(dist[i][j] == 1){
                	    caminho[i][j] = j;
                	}else if(dist[i][j] == 0){
                	    caminho[i][j] = i;
                	}
            	}
        	}
    	}
	}

	caminho_min(){

	}
};

#endif