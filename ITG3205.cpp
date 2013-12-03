#include <Wire.h>
#include "ITG3205.h"


void ITG3205::initGyro() {
    WriteByte(ITG3200_Address, PWR_MGM, 0x00);
    WriteByte(ITG3200_Address, SMPLRT_DIV, 0x07); //Fsample = 1kHz / (7 + 1) = 125Hz, or 8ms per sample
    //WriteByte(ITG3200_Address, DLPF_FS, 0x1E); // +/- 2000 dgrs/sec, 1KHz, Low Pass Filter Bandwidth: 5Hz
    WriteByte(ITG3200_Address, DLPF_FS, 0x1B); // +/- 2000 dgrs/sec, 1KHz, Low Pass Filter Bandwidth: 42Hzh
    WriteByte(ITG3200_Address, INT_CFG, 0x00);
}

void ITG3205::GyroRead() {
    Wire.beginTransmission(ITG3200_Address);
    Wire.write(0x1B);
    Wire.endTransmission();

    Wire.beginTransmission(ITG3200_Address);
    Wire.requestFrom(ITG3200_Address, 8);    // request 8 bytes from ITG3200

    int i = 0;
    byte buff[8];
    while(Wire.available())
    {
        buff[i] = Wire.read();
        i++;
    }

    Wire.endTransmission();

    g.t = (buff[0] << 8) | buff[1]; // temperature
    g.x = ((buff[2] << 8) | buff[3]);
    g.y = ((buff[4] << 8) | buff[5]);
    g.z = ((buff[6] << 8) | buff[7]);
}


byte ITG3205::WriteByte(byte i2c_address, byte address, byte data) {
    Wire.beginTransmission(i2c_address);
    Wire.write(address);
    Wire.write(data);
    byte result = Wire.endTransmission();

    //do some error checking
    if (result > 0) {
        Serial.print("I2C Write PROBLEM..... Result code is ");
        Serial.println(result);
    }
    return result;
}
