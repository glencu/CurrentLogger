/*
 * OrnoHelper.cpp
 *
 *  Created on: 28 gru 2015
 *      Author: Bobasek
 */

#include "OrnoHelper.h"

OrnoHelper::OrnoHelper() {
	// TODO Auto-generated constructor stub

}

OrnoHelper::~OrnoHelper() {
	// TODO Auto-generated destructor stub
}


unsigned short OrnoHelper::ModRTU_CRC(const unsigned char buf[], int len)
{
	unsigned short crc = 0xFFFF;

	  for (int pos = 0; pos < len; pos++) {
	    crc ^= (unsigned short)buf[pos];          // XOR byte into least sig. byte of crc

	    for (int i = 8; i != 0; i--) {    // Loop over each bit
	      if ((crc & 0x0001) != 0) {      // If the LSB is set
	        crc >>= 1;                    // Shift right and XOR 0xA001
	        crc ^= 0xA001;
	      }
	      else                            // Else LSB is not set
	        crc >>= 1;                    // Just shift right
	    }
	  }
	  return crc;
}
