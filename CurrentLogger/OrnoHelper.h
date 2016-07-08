/*
 * OrnoHelper.h
 *
 *  Created on: 28 gru 2015
 *      Author: Bobasek
 */

#ifndef ORNOHELPER_H_
#define ORNOHELPER_H_

class OrnoHelper {
public:
	OrnoHelper();
	virtual ~OrnoHelper();
	static unsigned short ModRTU_CRC(const unsigned char buf[], int len);

};

#endif /* ORNOHELPER_H_ */
