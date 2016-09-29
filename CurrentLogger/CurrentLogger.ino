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
const static uint8_t ip[] = {192,168,137,100};
const static uint8_t gw[] = {192,168,137,1};
const static uint8_t dns[] = {192,168,137,1};
// called when the client request is complete


static void my_callback (byte status, word off, word len) {
  int status_index = -1;
  int ok_index = -1;
 // Ethernet::buffer[off+200] = 0;

/*  if ( strstr((const char*) Ethernet::buffer + off , ": \"OK\"") != NULL )
	  Serial.println("OK");
  else
	  Serial.println("Error");*/
}

#define rxPin 0 // Our Serial RX pin connected to RO- Receiver Output Pin on Max485- we receive on
#define txPin 1 // Our Serial TX pin connected to DI- Driver Output Pin on Max485 - we send on
#define switchPin 7 // Arduino's Pin 7 Connected to Pins 2 & 3 (RE/DE) on MAX485

#define BUFF_LEN (100)
#define DATE_LEN (35)

DS3231 clock;
RTCDateTime dt;


char logBuffer[BUFF_LEN];

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
      Serial.println(F("Failed to access Ethernet controller"));
  if(!ether.staticSetup(ip, gw, dns))
      Serial.println(F("static failed"));

  ether.parseIp(ether.hisip, "192.168.8.6");
  ether.hisport = 8080;

  ornoManager.Init(txPin , rxPin, switchPin);

  //No Serial.println from this line!!
  ornoManager.configureReadCommand( 0x01 , 0x00, 0x0010 );
  delay(1000);

}

void loop()
{
  static uint32_t cntr=0;

  ether.packetLoop(ether.packetReceive());
  if (millis() > timer) {
  timer = millis() + 5000;
  ornoManager.sendReadCommand();

  if (  ornoManager.receivedFrame() == true )
  {
	  readResponse = ornoManager.getResponse();
	  memset(logBuffer, 0 , BUFF_LEN);
	  sprintf(logBuffer,"/json.htm?type=command&param=udevice&idx=%d&nvalue=0&svalue=%d.%d",13,readResponse.getCurrent().value, readResponse.getCurrent().fraction);
	  ether.browseUrl(PSTR(""),logBuffer, website, my_callback);
  }

  }
}
