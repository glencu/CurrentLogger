/*
 * OrnoReadHoldingRegistersCommand.cpp
 *
 *  Created on: 28 gru 2015
 *      Author: Bobasek
 */

#include "OrnoReadHoldingRegistersCommand.h"
#include "OrnoHelper.h"

OrnoReadHoldingRegistersCommand::OrnoReadHoldingRegistersCommand():functionCode(COMMAND_CODE),Crc(0),dataAdress(0),numOfRegisters(0),slaveAddress(0) {
	// TODO Auto-generated constructor stub

}

unsigned short OrnoReadHoldingRegistersCommand::getCrc() const {
	return Crc;
}

void OrnoReadHoldingRegistersCommand::setCrc(unsigned short crc) {
	Crc = crc;
}

void OrnoReadHoldingRegistersCommand::setDataAdress(unsigned short dataAdress) {
	this->dataAdress = dataAdress;
}

void OrnoReadHoldingRegistersCommand::setNumOfRegisters(
		unsigned short numOfRegisters) {
	this->numOfRegisters = numOfRegisters;
}

void OrnoReadHoldingRegistersCommand::setSlaveAddress(
		unsigned char slaveAddress) {
	this->slaveAddress = slaveAddress;
}

OrnoReadHoldingRegistersCommand::~OrnoReadHoldingRegistersCommand() {

}

unsigned char* OrnoReadHoldingRegistersCommand::getCommand() {

	unsigned char* command = new unsigned char[COMMAND_LENGHT];
	for(int i = 0 ; i < COMMAND_LENGHT; i++)
		command[i] = 0;
    command[0] = this->slaveAddress;
    command[1] = this->functionCode;
    command[2] = this->dataAdress;
    command[4] = (this->numOfRegisters >> 8) & 0x00FF;
    command[5] = this->numOfRegisters  & 0x00FF;
    unsigned short crc =  OrnoHelper::ModRTU_CRC(&command[0], 6);
    unsigned char tmp = crc >> 8;
    command[7] = tmp;
    tmp = crc & 0x00FF;
    command[6] = tmp;
    this->Crc = crc;
	return command;

}
