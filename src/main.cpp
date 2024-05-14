#include <Arduino.h>
#include <mcp_can.h>
#include <SPI.h>
#include <RunningAverage.h>
#include <arduino-timer.h>

#define ANALOG_PIN A1
#define CAN_CS_PIN 10

constexpr unsigned long CAN_ID = 0x100u;
constexpr unsigned long CAN_SEND_DELAY_MS = 100;
constexpr unsigned long PEDAL_READ_DELAY_MS = 10;
constexpr unsigned long MAX_AVG_READINGS = 10;

MCP_CAN CAN0(CAN_CS_PIN);
byte canData[1];
RunningAverage readings(MAX_AVG_READINGS);
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

  if (CAN0.begin(MCP_ANY, CAN_1000KBPS, MCP_16MHZ) == CAN_OK)
    Serial.println("MCP2515 Initialized Successfully!");
  else
    Serial.println("Error Initializing MCP2515...");

  CAN0.setMode(MCP_LOOPBACK);

  timer.every(PEDAL_READ_DELAY_MS, updatePedalValue);
  timer.every(CAN_SEND_DELAY_MS, sendCanData);
}

void loop()
{
  timer.tick();
}
