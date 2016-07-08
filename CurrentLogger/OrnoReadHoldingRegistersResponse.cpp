/*
 * OrnoReadHoldingRegistersResponse.cpp
 *
 *  Created on: 29 gru 2015
 *      Author: Bobasek
 */

#include "OrnoReadHoldingRegistersResponse.h"

using Orno::Types::OrnoFloat;

OrnoReadHoldingRegistersResponse::OrnoReadHoldingRegistersResponse() {

	for(unsigned int i=0 ; i < RESPONSE_LENGHT ; i++)
    	this->responseBuffer[i] = 0;

}

const unsigned char* OrnoReadHoldingRegistersResponse::getResponseBuffer() const {
	return responseBuffer;
}


void OrnoReadHoldingRegistersResponse::setResponseBuffer(unsigned char* inBuffer){

	for(unsigned int i=0 ; i < RESPONSE_LENGHT ; i++)
		this->responseBuffer[i] = inBuffer[i];
}

OrnoReadHoldingRegistersResponse::~OrnoReadHoldingRegistersResponse() {
	// TODO Auto-generated destructor stub
}

unsigned char* OrnoReadHoldingRegistersResponse::getResponseData() {

	return this->responseStruct.data;
}

unsigned short OrnoReadHoldingRegistersResponse::getVolt() {

	  unsigned int volt = 0;
	  unsigned char temp1 = 0;

	  volt = this->responseStruct.data[0] << 8;
	  temp1 = this->responseStruct.data[1];
	  volt = volt | temp1;
	  volt = volt / 10;
	  return volt;
}

Orno::Types::OrnoFloat OrnoReadHoldingRegistersResponse::getCurrent() {

	  unsigned int curr = 0;
	  unsigned char temp1 = 0;

	  OrnoFloat result;

	  curr = this->responseStruct.data[2] << 8;
	  temp1 = this->responseStruct.data[3];
	  curr = curr | temp1;
      result.value = curr/10;
      result.fraction = curr % 10;

	  return result;
}

Orno::Types::OrnoFloat OrnoReadHoldingRegistersResponse::getActualPower() {

	  unsigned int act_pow = 0;
	  unsigned char temp1 = 0;
	  Orno::Types::OrnoFloat result;

	  act_pow = this->responseStruct.data[6] << 8;
	  temp1 = this->responseStruct.data[7];
	  act_pow = act_pow | temp1;

	  result.value = act_pow / 1000;
	  result.fraction = act_pow % 1000;

	  return result;
}

Orno::Types::OrnoLongFloat OrnoReadHoldingRegistersResponse::getKWh()
{
	Orno::Types::OrnoLongFloat result={0 , 0};

	unsigned int kWH_big = 0;
    unsigned char temp_big = 0;

    unsigned int kWH_little = 0;
    unsigned char temp_little = 0;

    unsigned long Kwh = 0;
    kWH_little = this->responseStruct.data[14] << 8;
    temp_little = this->responseStruct.data[15];

    kWH_little = kWH_little | temp_little;

    kWH_big = this->responseStruct.data[16] << 8;
    temp_big = this->responseStruct.data[17];

    kWH_big = kWH_big | temp_big;

    Kwh = ((unsigned long)kWH_little << 16) | kWH_big;

    result.value = Kwh / 1000;
    result.fraction = Kwh % 1000;

    return result;

}


unsigned short OrnoReadHoldingRegistersResponse::getCrc() {
   	  unsigned int crc = 0;
      unsigned char temp1 = 0;

	  crc = this->responseStruct.data[33] << 8;
	  temp1 = this->responseStruct.data[32];
	  crc = crc | temp1;
	  return crc;
}
