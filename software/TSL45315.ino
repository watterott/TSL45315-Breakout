/*
  Example sketch for TSL45315-Breakout (Digital Ambient Light Sensor).

  Board           I2C/TWI Pins
                  SDA, SCL
  ----------------------------
  Uno, Ethernet    A4, A5
  Mega             20, 21
  Leonardo          2,  3
  Due              20, 21

  Example source code free to use.
  Further information: https://learn.watterott.com/license/
*/

#include <Wire.h>

#define I2C_ADDR     0x29

#define REG_CONTROL  0x00
#define REG_CONFIG   0x01
#define REG_DATALOW  0x04
#define REG_DATAHIGH 0x05
#define REG_ID       0x0A

void setup()
{
  Serial.begin(9600);
  while(!Serial); //wait for serial port to connect (needed for Leonardo only)

  Wire.begin();

  Serial.print("ID: ");
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x80|REG_ID);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDR, 1); //request 1 byte
  while(Wire.available())
  {
    unsigned char c = Wire.read();
    Serial.print(c&0xF0, HEX);
  }
  Serial.println("");

  Serial.println("Power on...");
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x80|REG_CONTROL);
  Wire.write(0x03); //power on
  Wire.endTransmission();

  Serial.println("Config...");
  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x80|REG_CONFIG);
  Wire.write(0x00); //M=1 T=400ms
  // Wire.write(0x01); //M=2 T=200ms
  // Wire.write(0x02); //M=4 T=100ms
  Wire.endTransmission();
}

void loop()
{
  uint16_t l, h;
  uint32_t lux;

  Wire.beginTransmission(I2C_ADDR);
  Wire.write(0x80|REG_DATALOW);
  Wire.endTransmission();
  Wire.requestFrom(I2C_ADDR, 2); //request 2 bytes
  l = Wire.read();
  h = Wire.read();
  lux  = (h<<8) | (l<<0);
  lux *= 1; //M=1
  // lux *= 2; //M=2
  // lux *= 4; //M=4
  Serial.print("Lux: ");
  Serial.println(lux, DEC);

  delay(1000);
}
