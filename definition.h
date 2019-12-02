#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>

using namespace std;

class packetData
{
	private:
		int L;
		int I;	
		int N;
		int M;
		int Q;
		int P;
		int n;
		int t[100000];
		
	
	public:
		int pLength(); //returns L
		int idLength();//returns I
		int dataLength();//returns N
		int pTolerance();//returns M
		int idTolerance();//returns Q
		int dataTolerance();//returns P
		int packetQuantity();//returns n
		int timeOf(int time);//returns the start time of each packet
		void read(ifstream& fin);
};

#endif
