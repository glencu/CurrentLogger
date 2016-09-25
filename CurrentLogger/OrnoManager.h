/*
 * OrnoManager.h
 *
 *  Created on: 25 wrz 2016
 *      Author: Bobasek
 */

#ifndef ORNOMANAGER_H_
#define ORNOMANAGER_H_

#include <stdint.h>
#include <stddef.h>
#include "RS485.h"
#include "OrnoReadHoldingRegistersCommand.h"
#include "OrnoReadHoldingRegistersResponse.h"
#include "OrnoHelper.h"


class OrnoManager {

	OrnoReadHoldingRegistersCommand readCommand;
	OrnoReadHoldingRegistersResponse readResponse;

	RS485 *rs485;
	unsigned char responseBuffer[OrnoReadHoldingRegistersResponse::RESPONSE_LENGHT];

public:
	void Init(const UINT8 txPin ,const UINT8 rxPin,const UINT8 switchPin);
	void configureReadCommand(unsigned char , unsigned short , unsigned short);
	void sendReadCommand();
	bool receivedFrame();
	OrnoReadHoldingRegistersResponse getResponse();
	OrnoManager();
	virtual ~OrnoManager();
};

#endif /* ORNOMANAGER_H_ */
