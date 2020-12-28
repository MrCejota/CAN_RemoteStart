#include <SPI.h>
#include <mcp2515.h>

struct can_frame wakeMsg;
struct can_frame initMsg;
struct can_frame startupMsg;
struct can_frame stopdownMsg;

const int spi_cs_pin = 10;
MCP2515 mcp2515(spi_cs_pin);

const int redLED = 2;
const int greenLED = 3;

const int startButtonPin = 5;
const int stopButtonPin = 6;

void startCar(void);
void shutdownCar(void);

void setup() {

  pinMode(redLED, OUTPUT);
  pinMode(greenLED, OUTPUT);
  pinMode(startButtonPin, INPUT);
  pinMode(stopButtonPin, INPUT);

  Serial.begin(115200);
  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS, MCP_16MHZ);
  mcp2515.setNormalMode();

  wakeMsg.can_id  = 0x100;
  wakeMsg.can_dlc = 8;
  wakeMsg.data[0] = 0x00;
  wakeMsg.data[1] = 0x00;
  wakeMsg.data[2] = 0x00;
  wakeMsg.data[3] = 0x00;
  wakeMsg.data[4] = 0x00;
  wakeMsg.data[5] = 0x00;
  wakeMsg.data[6] = 0x00;
  wakeMsg.data[7] = 0x00;

  initMsg.can_id  = 0x638;
  initMsg.can_dlc = 8;
  initMsg.data[0] = 0x01;
  initMsg.data[1] = 0x14;
  initMsg.data[2] = 0x00;
  initMsg.data[3] = 0x00;
  initMsg.data[4] = 0x23;
  initMsg.data[5] = 0x40;
  initMsg.data[6] = 0x05;
  initMsg.data[7] = 0x03;
  
  startupMsg.can_id  = 0x044097;
  startupMsg.can_dlc = 3;
  startupMsg.data[0] = 0x00;
  startupMsg.data[1] = 0xFF;
  startupMsg.data[2] = 0x0a;

  stopdownMsg.can_id  = 0x0080B0;
  stopdownMsg.can_dlc = 3;
  stopdownMsg.data[0] = 0x02;
  stopdownMsg.data[1] = 0x0C;

  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);  
  Serial.println("Example: Write to CAN");
}

void loop() 
{
  if (digitalRead(startButtonPin) == HIGH &&
      digitalRead(stopButtonPin != HIGH))
    startCar();
  if (digitalRead(stopButtonPin) == HIGH &&
      digitalRead(startButtonPin != HIGH))
    shutdownCar();

  delay(500);
}

void startCar(void)
{
  mcp2515.sendMessage(&wakeMsg);
  delay(500);
  mcp2515.sendMessage(&initMsg);
  delay(500);
  mcp2515.sendMessage(&startupMsg);
  Serial.println("Startup message sent");
  delay(500);
  digitalWrite(redLED, LOW);
  digitalWrite(greenLED, HIGH);  
}

void shutdownCar(void)
{
  mcp2515.sendMessage(&wakeMsg);
  delay(500);
  mcp2515.sendMessage(&initMsg);
  delay(500);
  mcp2515.sendMessage(&stopdownMsg);
  Serial.println("Shutdown message sent");
  delay(500);
  delay(500);
  digitalWrite(redLED, HIGH);
  digitalWrite(greenLED, LOW);  
}