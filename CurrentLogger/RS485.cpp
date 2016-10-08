#include <Arduino.h>
#include "RS485.h"

RS485::RS485(UINT8 txPin , UINT8 rxPin, UINT8 switchPin):txPin(txPin),rxPin(rxPin),switchPin(switchPin),isInit(false)
{

}


void RS485::Init()
{
	 Serial.begin(9600, SERIAL_8N1);
	 pinMode(this->txPin, OUTPUT); //sets our Arduino's serial TX pin to send data
	 pinMode(this->rxPin, INPUT); //sets our Arduino's serial RX pin to receive data
	 pinMode(this->switchPin, OUTPUT); //sterowanie DE i RE MAX485, podlaczone razem
	 while (!Serial) {
	    ; // wait for serial port to connect. Needed for Leonardo only
	  }

	 while (Serial.available() > 0) //clear serial.read buffer
	     Serial.read();

	 digitalWrite(this->switchPin, HIGH); //transmit to slave ready (pin MAX485)


	 isInit = true;
}


void RS485::Send(unsigned char* buffer)
{
	 digitalWrite(switchPin, HIGH); //transmit to slave ready (pin MAX485)
	 for (byte j = 0; j < 8; j++)
	 {
	    Serial.write(buffer[j]);
	 } //transmit time 9ms (9600) (buffor)

	 while (!(UCSR0A & (1 << TXC0)));   // Wait for the transmission to complete

	 digitalWrite(switchPin, LOW);

	 while (Serial.available() > 0) //clear serial.read buffer
	 {
	    Serial.read();
	 }

	 delay(170);


}

bool RS485::checkIfNumOfBytesAvailable(int32_t numOfBytes)
{
	if (Serial.available() == numOfBytes)
		return true;
	else
		return false;
}

uint32_t RS485::ReceiveResponse(uint32_t numOfBytes ,unsigned char* buffer)
{
	    uint32_t bytes_received = 0;

	      while (Serial.available() > 0)
	      {
	       //digitalWrite(stat_Pin, HIGH); TODO:Stat pin tasks
	       buffer[bytes_received] = Serial.read();
	       bytes_received++;
	       //digitalWrite(stat_Pin, LOW);
	      }
	      return bytes_received;

}
