#include <SPI.h>
#include <SD.h>
#include <Wire.h>
#include <DS3231.h>
#include <EtherCard.h>


#include "RS485.h"
#include "OrnoReadHoldingRegistersCommand.h"
#include "OrnoReadHoldingRegistersResponse.h"
#include "OrnoHelper.h"
#include "SDLogger.h"

#define POWER_METER_IDX (13)

// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];

const char website[] PROGMEM = "192.168.8.6:8080";
const static uint8_t ip[] = {192,168,137,100};
const static uint8_t gw[] = {192,168,137,1};
const static uint8_t dns[] = {192,168,137,1};

static void my_callback (byte status, word off, word len) {
  int status_index = -1;
  int ok_index = -1;
  Serial.println(">>>");
  Ethernet::buffer[off+300] = 0;


  Serial.print((const char*) Ethernet::buffer + off);

  if ( strstr((const char*) Ethernet::buffer + off , ": \"OK\"") != NULL )
	  Serial.println("OK");
  else
	  Serial.println("Error");
}



#define rxPin 0 // Our Serial RX pin connected to RO- Receiver Output Pin on Max485- we receive on
#define txPin 1 // Our Serial TX pin connected to DI- Driver Output Pin on Max485 - we send on
#define switchPin 7 // Arduino's Pin 7 Connected to Pins 2 & 3 (RE/DE) on MAX485

#define BUFF_LEN (100)
#define DATE_LEN (35)

DS3231 clock;
RTCDateTime dt;
RS485 rs485(txPin,rxPin,switchPin);
OrnoReadHoldingRegistersCommand readCommand;
OrnoReadHoldingRegistersResponse readResponse;
char logBuffer[BUFF_LEN];
char dateBuf[DATE_LEN];
unsigned char responseBuffer[OrnoReadHoldingRegistersResponse::RESPONSE_LENGHT];
SDLogger *sdPowerLogger;
SDLogger *sdErrorLogger;

void setup() {

	 Serial.begin(9600);
	 delay(2000);
	 Serial.println("Starting system");
	 clock.begin();
	 delay(2000);
	 dt = clock.getDateTime();

	 Serial.print("Raw data: ");
	 Serial.print(dt.year);   Serial.print("-");
	 Serial.print(dt.month);  Serial.print("-");
	 Serial.print(dt.day);    Serial.print(" ");
	 Serial.print(dt.hour);   Serial.print(":");
	 Serial.print(dt.minute); Serial.print(":");
	 Serial.print(dt.second); Serial.println("");

  Serial.print("Initializing SD card...");

  if (!SD.begin(4)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  sdPowerLogger = new SDLogger();
  sdErrorLogger = new SDLogger("Error.txt");
  delay(1000);
  rs485.Init();
  //No Serial.println from this line!!
  readCommand.setSlaveAddress(0x01);
  readCommand.setDataAdress(0x00);
  readCommand.setNumOfRegisters(0x0010);


  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
     Serial.println(F("Failed to access Ethernet controller"));
  if(!ether.staticSetup(ip, gw, dns))
     Serial.println(F("static failed"));

  ether.printIp("IP:  ", ether.myip);
  ether.printIp("GW:  ", ether.gwip);
  ether.printIp("DNS: ", ether.dnsip);

  ether.parseIp(ether.hisip, "192.168.8.6");
  ether.printIp("SRV: ", ether.hisip);
  Serial.print("Port: ");
  ether.hisport = 8080;
  Serial.println(ether.hisport);

  delay(1000);

}

void loop()
{
  ether.packetLoop(ether.packetReceive());

  unsigned char* command = readCommand.getCommand();
  rs485.Send(command);
  delete command;

  bool goodFrameReceived = false;
  dt = clock.getDateTime();
  memset(dateBuf, 0 , DATE_LEN);
  sprintf(dateBuf,"%d-%d-%d %d:%d:%d ",dt.year,dt.month,dt.day,dt.hour,dt.minute,dt.second);

  memset(logBuffer, 0 , BUFF_LEN);

  if (true == rs485.checkIfNumOfBytesAvailable(OrnoReadHoldingRegistersResponse::RESPONSE_LENGHT))
  {
	  rs485.ReceiveResponse(OrnoReadHoldingRegistersResponse::RESPONSE_LENGHT,&responseBuffer[0]);
	  readResponse.setResponseBuffer(responseBuffer);
	  unsigned short receivedFrameCrc = readResponse.getCrc();
      unsigned short evaluatedCrc = OrnoHelper::ModRTU_CRC( readResponse.getResponseBuffer(), 35 );


      if ( evaluatedCrc == receivedFrameCrc )
      {
    	  goodFrameReceived = true;
      }
      else
      {
    	  sprintf(logBuffer," %s Received frame bad CRC\n", dateBuf );
      }
  }
  else
  {
	  sprintf(logBuffer,"%s Error when receiving data from module\n" ,dateBuf);
  }

  if ( goodFrameReceived == true )
  {
	  sprintf(logBuffer,"%s Volts: %d Current %d.%d Actual Power %d.%2d Kwh %lu.%2lu\n" ,dateBuf,readResponse.getVolt() ,
	    			  readResponse.getCurrent().value, readResponse.getCurrent().fraction ,
					  readResponse.getActualPower().value,readResponse.getActualPower().fraction,
					  readResponse.getKWh().value,readResponse.getKWh().fraction);
	  sdPowerLogger->LogToFile(logBuffer);

	  char buffer[100];
	  memset(buffer, 0 ,100);
	  sprintf(buffer,"/json.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%d",POWER_METER_IDX, readResponse.getKWh().value);
	  ether.browseUrl("",buffer, website, my_callback);

  }
  else
  {
	  sdErrorLogger->LogToFile(logBuffer);
  }


  delay(1000); //for test

}








