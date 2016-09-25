/*
 * OrnoManager.cpp
 *
 *  Created on: 25 wrz 2016
 *      Author: Bobasek
 */

#include "OrnoManager.h"

OrnoManager::OrnoManager():rs485(NULL) {
}

OrnoManager::~OrnoManager() {
}

void OrnoManager::Init(const UINT8 txPin ,const UINT8 rxPin,const UINT8 switchPin)
{
	rs485 = new RS485( txPin, rxPin, switchPin );
	rs485->Init();
}

void OrnoManager::configureReadCommand(unsigned char slaveAddress, unsigned short dataAddress, unsigned short numberOfRegisters )
{
	readCommand.setSlaveAddress(slaveAddress);
    readCommand.setDataAdress(dataAddress);
	readCommand.setNumOfRegisters(numberOfRegisters);
}

void OrnoManager::sendReadCommand()
{
	unsigned char* command = readCommand.getCommand();
    rs485->Send(command);
	delete command;
}

bool OrnoManager::receivedFrame()
{
    bool result = false;

	if (true == rs485->checkIfNumOfBytesAvailable(OrnoReadHoldingRegistersResponse::RESPONSE_LENGHT))
	{
		  rs485->ReceiveResponse(OrnoReadHoldingRegistersResponse::RESPONSE_LENGHT,&responseBuffer[0]);
		  readResponse.setResponseBuffer(responseBuffer);
		  unsigned short receivedFrameCrc = readResponse.getCrc();
	      unsigned short evaluatedCrc = OrnoHelper::ModRTU_CRC( readResponse.getResponseBuffer(), 35 );


	      if ( evaluatedCrc == receivedFrameCrc )
	      {
	    	  result =  true;
	      }
	      else
	      {
	    	  result = false;
	      }
	}
	else
	{
		result =  false;
	}

	return result;
}

OrnoReadHoldingRegistersResponse OrnoManager::getResponse()
{
    return 	readResponse;
}
