#include <HardwareSerial.h>

#define RXD2 47  // Sensor TX → ESP RX
#define TXD2 48  // Sensor RX → ESP TX

HardwareSerial SensorSerial(2);  // UART2

unsigned char data[4] = {};
float distance = 0;

void setup() {
  Serial.begin(115200);
  SensorSerial.begin(9600, SERIAL_8N1, RXD2, TXD2);
  Serial.println("A01NYUB Sensor Ready...");
}

void loop() {
  // Wait for at least 4 bytes in the buffer
  while (SensorSerial.available() >= 4) {
    // Find header
    if (SensorSerial.read() == 0xFF) {
      data[0] = 0xFF;
      data[1] = SensorSerial.read();
      data[2] = SensorSerial.read();
      data[3] = SensorSerial.read();

      int checksum = (data[0] + data[1] + data[2]) & 0xFF;
      if (checksum == data[3]) {
        int raw_distance = (data[1] << 8) + data[2];  // in mm
        float dist_cm = raw_distance / 10.0;

        if (dist_cm > 28.0) {
          Serial.print("Distance: ");
          Serial.print(dist_cm, 1);
          Serial.println(" cm");
        } else {
          Serial.println("Below minimum range");
        }
      } else {
        Serial.println("Checksum ERROR");
      }
    } else {
      // If not header, discard one byte and keep going
      continue;
    }
  }

  delay(100);  // Short delay is enough
}
