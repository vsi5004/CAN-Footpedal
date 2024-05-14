#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>
#include <RunningAverage.h>
#include <arduino-timer.h>

#define ANALOG_PIN A1
MCP_CAN CAN0(10); // Set CS to pin 10
byte canData[1];
constexpr unsigned long CAN_ID = 0x100;
RunningAverage readings(10);
auto timer = timer_create_default();
int pedalValue = 0;

bool updatePedalValue(void *)
{
  readings.addValue(analogRead(ANALOG_PIN));
  pedalValue = readings.getAverage();
  Serial.println(pedalValue);
  return true;
}

bool sendCanData(void *)
{
  canData[0] = pedalValue;
  byte sndStat = CAN0.sendMsgBuf(CAN_ID, 0, sizeof(canData), canData);
  if (sndStat == CAN_OK)
  {
    Serial.println("Message Sent Successfully!");
    return true;
  }
  else
  {
    Serial.print("Error Sending Message: ");
    Serial.println(sndStat);
    return false;
  }
}

void setup()
{
  Serial.begin(115200);

  readings.clear();

  // Initialize MCP2515 running at 16MHz with a baudrate of 500kb/s and the masks and filters disabled.
  if (CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_LOOPBACK); // Change to normal mode to allow messages to be transmitted

  timer.every(10, updatePedalValue);
  timer.every(100, sendCanData); // Change the argument type of sendCanData to void
}

void loop()
{
  timer.tick();
}
