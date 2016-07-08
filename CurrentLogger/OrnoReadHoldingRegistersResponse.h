/*
 * OrnoReadHoldingRegistersResponse.h
 *
 *  Created on: 29 gru 2015
 *      Author: Bobasek
 */

#ifndef ORNOREADHOLDINGREGISTERSRESPONSE_H_
#define ORNOREADHOLDINGREGISTERSRESPONSE_H_

#include "OrnoTypes.h"


struct OrnoReadHoldingRegistersResponseStruct
{
  unsigned char slaveAddress;
  unsigned char functionCode;
  unsigned char dataLength;
  unsigned char data[];
};

class OrnoReadHoldingRegistersResponse {


public:
	static const unsigned short RESPONSE_LENGHT = 37;
	OrnoReadHoldingRegistersResponse();
	virtual ~OrnoReadHoldingRegistersResponse();
	const unsigned char* getResponseBuffer() const;
	void setResponseBuffer(unsigned char*);
	unsigned char * getResponseData();

	unsigned short getCrc();

	unsigned short getVolt();
	Orno::Types::OrnoFloat getActualPower();
	Orno::Types::OrnoLongFloat getKWh();
	Orno::Types::OrnoFloat getCurrent();

private:
	union
	{
	 unsigned char responseBuffer[RESPONSE_LENGHT];
	 OrnoReadHoldingRegistersResponseStruct responseStruct;
	};
};

#endif /* ORNOREADHOLDINGREGISTERSRESPONSE_H_ */
