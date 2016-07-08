/*
 * OrnoReadHoldingRegistersCommand.h
 *
 *  Created on: 28 gru 2015
 *      Author: Bobasek
 */

#ifndef ORNOREADHOLDINGREGISTERSCOMMAND_H_
#define ORNOREADHOLDINGREGISTERSCOMMAND_H_

class OrnoReadHoldingRegistersCommand {

	unsigned char slaveAddress;
	const unsigned char functionCode;
	unsigned short dataAdress;
	unsigned short numOfRegisters;
	unsigned short Crc;
	static const unsigned char COMMAND_LENGHT = 8;
	static const unsigned char COMMAND_CODE = 3;
	unsigned char modbusCommand[COMMAND_LENGHT];


public:
	OrnoReadHoldingRegistersCommand();
	virtual ~OrnoReadHoldingRegistersCommand();
	unsigned short getCrc() const;
	void setCrc(unsigned short crc);
	void setDataAdress(unsigned short dataAdress);
	void setNumOfRegisters(unsigned short numOfRegisters);
	void setSlaveAddress(unsigned char slaveAddress);

	unsigned char* getCommand();
};

#endif /* ORNOREADHOLDINGREGISTERSCOMMAND_H_ */
