#include "definition.h"


int packetData::pLength()
{
	return L;
}

int packetData::idLength()
{
	return I;
}

int packetData::dataLength()
{
	return N;
}

int packetData::pTolerance()
{
	return M;
}

int packetData::idTolerance()
{
	return Q;
}

int packetData::dataTolerance()
{
	return P;
}

int packetData::packetQuantity()
{
	return n;
}

int packetData::timeOf(int time)
{
	return t[time];
}

void packetData::read(ifstream& fin)
{
	
	fin >> L >> I >> N >> M >> Q >> P >> n;
	
	for(int i=1; i<=n; i++)
	{
		fin >> t[i];
	}

	
}

