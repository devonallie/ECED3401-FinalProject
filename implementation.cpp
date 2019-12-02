#include "definition.h"

int main(void)
{
	ifstream fin;
	ofstream fout;
	fin.open("input.txt");
	fout.open("output.txt");

	int currentPacket = 1; 													//Starting from the first packet

	packetData packet;														//packet is the object that hosts all the information on the packet stream

	packet.read(fin);														//reading in the parameters for the stream from the input file

	char packetStatus[packet.packetQuantity()];								//declaring a character array with the status of each packet
	int pInterference, idInterference, dataInterference; 					//declaring the variables to count how much interference occurs at each portion of the packet

	while(currentPacket <= packet.packetQuantity())							//checking the status of each packet
	{
	pInterference = 0;														//the occurences of the interference at
	idInterference = 0;														//each portion is reset
	dataInterference = 0;													//upon decoding a new packet


																///////// Validating the Preamble ////////////


	if((packet.timeOf(currentPacket) + packet.pLength() - 1 >= packet.timeOf(currentPacket + 1)) && (currentPacket != packet.packetQuantity())) //If the end of the current preamble overlaps with the preamble of the next packet, the packet is tossed immediately
	{
		packetStatus[currentPacket] = 'F';



	}else if(packet.timeOf(currentPacket) <= packet.timeOf(currentPacket - 1) + packet.pLength() - 1) //If the begining of the current preamble overlaps with the previous preamble, the packet is tossed	immediately
	{
		packetStatus[currentPacket] = 'F';



	}else //If the current preamble doesnt overlap with a past or future preamble the preamble must be compared with id and data portions of packet past and future
	{
		packetStatus[currentPacket] = 'd';
	}

	if(packetStatus[currentPacket] == 'd')
	{
		int i = 1; //how far forward or back in time
		while(i < packet.pTolerance() + 1) //going as far forward or back in time as required to find interference up to the tolerence
		{
			if((packet.timeOf((currentPacket - i) + packet.pLength() + packet.idLength() + packet.dataLength() - 1 >= packet.timeOf(currentPacket))) && (currentPacket - i > 0)) // && (current - 1 > 0 ) prevents going into negative time
			{

				pInterference++; //If any of the previous packet overlaps with the current packets preamble, we detect an interference
			}

			i++; //Future packets are not compared to the current packet because it is fundamentally impossible to have future id or data portions interfere with the current packets preamble which was already tested previously
		}

		if(pInterference > packet.pTolerance()- 1) //If the amount of interferences exceeds the tolerence then the packet is lost entirely just as before
		{
			packetStatus[currentPacket] = 'F';
		}else //Otherwise the packet is declared "D" for detected
		{
			packetStatus[currentPacket] = 'D';
		}
	}

			/////////Validating ID/////////////

	if(packetStatus[currentPacket] == 'D') //Only proceed if packet preamble has been decoded and packet is detected
	{
		int j = 1; //how far forward or back in time
		while(j < packet.idTolerance() + 1) //going as far forward or back in time as required to find interference up to the tolerence
		{
			if((packet.timeOf(currentPacket) + packet.pLength() <= packet.timeOf(currentPacket - j) + packet.pLength() + packet.idLength() + packet.dataLength() - 1) && (currentPacket - j > 0)) //&& (currentPacket - j > 0) prevents going into negative time
			{

				idInterference ++; //If the current packets ID portion overlaps with any part of the previous packet, count for interference
			}if((packet.timeOf(currentPacket) + packet.pLength() + packet.idLength() - 1 >= packet.timeOf(currentPacket + j)) && (currentPacket + j <= packet.packetQuantity())) //&& (currentPacket + j <= packet.packetQuantity()) prevents comparing with packets too far in the future
			{

				idInterference ++; //If the current packets ID portion overlaps with any portion of the next packet, count interference
			}
			j++;
		}


		if(idInterference > packet.idTolerance() - 1) //If id interference exceeds tolerence, ID is not decoded but status remains as detected
		{
			packetStatus[currentPacket] = 'D';
		}else //Otherwise the packet ID is decoded and the status is 'I'
		{

			packetStatus[currentPacket] = 'I';
		}
	}
			///////////Validating Data//////////////

	if(packetStatus[currentPacket] == 'I') //Proceed to decode data only if ID has already been decoded
	{
		int k = 1; //how far forward or back in time
		while(k < packet.dataTolerance() + 1) //going as far forward or back in time as required to find interference up to the tolerence
			{
				if((packet.timeOf(currentPacket) + packet.pLength() + packet.idLength() <= packet.timeOf(currentPacket - k) + packet.pLength() + packet.idLength() + packet.dataLength() - 1) && (currentPacket - k > 0)) //&& (currentPacket - k > 0) prevents negative time
				{

					dataInterference ++; //If the current packets data portion overlaps with any part of the previous packet, count for interference//
				}if((packet.timeOf(currentPacket) + packet.pLength() + packet.idLength()  + packet.dataLength() - 1 >= packet.timeOf(currentPacket + k)) && (currentPacket + k <= packet.packetQuantity())) // && (currentPacket + k <= packet.packetQuantity()) prevents going too far forward in time where no packets exist
				{
					dataInterference ++; //If the current packets data portion overlaps with any portion of the next packet, count interference//
				}
				k++;
			}

			if(idInterference > packet.dataTolerance() - 1 )//If data overlap tolerence is exceeded, data is not decoded but packet remains detected and ID remains decoded//
			{
				packetStatus[currentPacket] = 'I';
			}else
			{
				packetStatus[currentPacket] = 'S'; //Otherwise the packet decoding process is fully successful
			}
	}
		currentPacket++; //Next packet
	}

	for(int n = 1; n < currentPacket; n++) //printing out the status of each packet, line by line
	{
		fout << packetStatus[n] << endl;
	}

	fin.close();
	fout.close();
	return 0;
}
