#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DS3231.h>


#include "SDLogger.h"
#include "OrnoManager.h"
#include "OrnoReadHoldingRegistersResponse.h"

#define rxPin 0 // Our Serial RX pin connected to RO- Receiver Output Pin on Max485- we receive on
#define txPin 1 // Our Serial TX pin connected to DI- Driver Output Pin on Max485 - we send on
#define switchPin 7 // Arduino's Pin 7 Connected to Pins 2 & 3 (RE/DE) on MAX485

#define BUFF_LEN (100)
#define DATE_LEN (35)

DS3231 clock;
RTCDateTime dt;


char logBuffer[BUFF_LEN];
char dateBuf[DATE_LEN];

SDLogger *sdlogger;
OrnoManager ornoManager;
OrnoReadHoldingRegistersResponse readResponse;

void printInitialDate()
{
	dt = clock.getDateTime();
	Serial.print("Raw data: ");
	Serial.print(dt.year);
	Serial.print("-");
	Serial.print(dt.month);
	Serial.print("-");
	Serial.print(dt.day);
	Serial.print(" ");
	Serial.print(dt.hour);
	Serial.print(":");
	Serial.print(dt.minute);
	Serial.print(":");
	Serial.print(dt.second);
	Serial.println("");
}

void setup()
{

	 Serial.begin(9600);
	 delay(2000);
	 Serial.println("Starting system");
	 clock.begin();
	 delay(2000);
	 printInitialDate();
	 Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  sdlogger = new SDLogger();
  delay(1000);

  ornoManager.Init(txPin , rxPin, switchPin);

  //No Serial.println from this line!!
  ornoManager.configureReadCommand( 0x01 , 0x00, 0x0010 );
  delay(1000);

}

void loop()
{

  ornoManager.sendReadCommand();

  dt = clock.getDateTime();
  memset(dateBuf, 0 , DATE_LEN);
  sprintf(dateBuf,"%d-%d-%d %d:%d:%d ",dt.year,dt.month,dt.day,dt.hour,dt.minute,dt.second);

  memset(logBuffer, 0 , BUFF_LEN);

  if (  ornoManager.receivedFrame() == true )
  {
	  readResponse = ornoManager.getResponse();
	  sprintf(logBuffer,"%s Volts: %d Current %d.%d Actual Power %d.%2d Kwh %lu.%2lu\n" ,dateBuf,readResponse.getVolt() ,
	      			  readResponse.getCurrent().value, readResponse.getCurrent().fraction ,
	  				  readResponse.getActualPower().value,readResponse.getActualPower().fraction,
	  readResponse.getKWh().value,readResponse.getKWh().fraction);
	  sdlogger->LogToFile(logBuffer);

  }

  delay(1000); //for test
}








