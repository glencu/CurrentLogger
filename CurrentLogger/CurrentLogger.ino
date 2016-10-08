#include <SPI.h>
#include <Wire.h>
#include <DS3231.h>
#include <EtherCard.h>

#include "OrnoManager.h"
#include "OrnoReadHoldingRegistersResponse.h"


// ethernet interface mac address, must be unique on the LAN
static byte mymac[] = { 0x74,0x69,0x69,0x2D,0x30,0x31 };

byte Ethernet::buffer[700];
static uint32_t timer;

const char website[] PROGMEM = "192.168.8.6:8080";
const static uint8_t ip[] = {192,168,8,8};
const static uint8_t gw[] = {192,168,8,1};
const static uint8_t dns[] = {192,168,8,1};


const static uint8_t DOMOTICZ_RECOVERY_CURRENT_ID = 13;
const static uint8_t DOMOTICZ_RECOVERY_WATT_ID = 14;
const static uint8_t DOMOTICZ_LOFT_TEMPERATURE_ID = 16;

static void my_callback (byte status, word off, word len)
{
	//TODO: Error handling
}

#define rxPin 0 // Our Serial RX pin connected to RO- Receiver Output Pin on Max485- we receive on
#define txPin 1 // Our Serial TX pin connected to DI- Driver Output Pin on Max485 - we send on
#define switchPin 7 // Arduino's Pin 7 Connected to Pins 2 & 3 (RE/DE) on MAX485

#define BUFF_LEN (100)

void updateDomoticz( uint8_t, uint16_t, uint16_t );

enum State
{
	READING_ORNO,
	SENDING_CURRENT,
	SENDING_WATT,
	SENDING_TEMPERATURE,
	SENDING_NULL_CURRENT,
	SENDING_NULL_WATT
};

DS3231 clock;
RTCDateTime dt;

char logBuffer[BUFF_LEN];

State state = SENDING_TEMPERATURE;

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
  delay(500);
  Serial.println("Starting system");
  clock.begin();
  delay(500);
  printInitialDate();
  delay(1000);

  if (ether.begin(sizeof Ethernet::buffer, mymac) == 0)
      Serial.println( F("Failed to access Ethernet controller") );
  if(!ether.staticSetup(ip, gw, dns))
      Serial.println( F("static failed") );

  ether.parseIp( ether.hisip, "192.168.8.6" );
  ether.hisport = 8080;

  ornoManager.Init( txPin , rxPin, switchPin );

  //No Serial.println from this line!!
  ornoManager.configureReadCommand( 0x01 , 0x00, 0x0010 );
  delay(1000);

}

void loop()
{
  ether.packetLoop(ether.packetReceive());

  if (millis() > timer)
  {
	  timer = millis() + 1000;
	  if ( state == SENDING_TEMPERATURE )
	  {
	  		  clock.forceConversion();
	  		  float temperature = clock.readTemperature();
	  		  temperature *= 100;
	  		  uint16_t highTemp = (uint16_t)temperature / 100;
	  		  uint16_t lowTemp = (uint16_t)temperature % 100;

	  		  updateDomoticz( DOMOTICZ_LOFT_TEMPERATURE_ID ,highTemp ,lowTemp );
	  		  state = READING_ORNO;
	  }
	  else if ( state == READING_ORNO )
	  {
		  ornoManager.sendReadCommand();
		  if (  ornoManager.receivedFrame() == true )
		  {
			  readResponse = ornoManager.getResponse();
			  state = SENDING_CURRENT;
		  }
		  else
		  {
			  state = SENDING_NULL_CURRENT;
		  }
	  }
	  else  if ( state == SENDING_CURRENT )
	  {
		  updateDomoticz( DOMOTICZ_RECOVERY_CURRENT_ID ,readResponse.getCurrent().value ,readResponse.getCurrent().fraction );
		  state = SENDING_WATT;
	  }
	  else  if ( state == SENDING_WATT )
	  {
		  uint16_t watts = (readResponse.getActualPower().value * 1000) + readResponse.getActualPower().fraction;
		  updateDomoticz( DOMOTICZ_RECOVERY_WATT_ID ,watts, 0 );
		  state = SENDING_TEMPERATURE;
	  }
	  else  if ( state == SENDING_NULL_CURRENT )
	  {
		  updateDomoticz( DOMOTICZ_RECOVERY_CURRENT_ID ,0 ,0 );
		  state = SENDING_NULL_WATT;
	  }
	  else  if ( state == SENDING_NULL_WATT )
	  {
		  updateDomoticz( DOMOTICZ_RECOVERY_WATT_ID ,0 ,0 );
		  state = SENDING_TEMPERATURE;
	  }
  }
}

void updateDomoticz(uint8_t deviceId , uint16_t value , uint16_t fraction)
{
	memset(logBuffer, 0 , BUFF_LEN);
    sprintf(logBuffer,"/json.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%d.%d",deviceId,value,fraction);
	ether.browseUrl(PSTR(""),logBuffer, website, my_callback);
}
