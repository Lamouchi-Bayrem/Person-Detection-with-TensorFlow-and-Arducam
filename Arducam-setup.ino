#include <ArduCam.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>

// ArduCam pins
#define CAM_CS    10 // CS pin
#define CAM_MOSI  11 // MOSI pin
#define CAM_MISO  12 // MISO pin
#define CAM_SCK   13 // SCK pin

ArduCAM myCAM(OV2640, CAM_CS);

void setup() {
  Serial.begin(115200);

  // Initialize ArduCam
  Wire.begin();
  myCAM.InitCAM();
  myCAM.OV2640_set_JPEG_size(OV2640_320x240);
}

void loop() {
  // Capture image
  myCAM.flush_fifo();
  myCAM.clear_fifo_flag();
  myCAM.start_capture();

  while (!myCAM.get_bit(ARDUCHIP_TRIG, CAP_DONE_MASK)) {}

  // Read image data
  uint32_t len = myCAM.read_fifo_length();
  Serial.write((uint8_t)(len >> 16));
  Serial.write((uint8_t)(len >> 8));
  Serial.write((uint8_t)len);

  // Send image data
  while (len--) {
    Serial.write(myCAM.read_fifo());
  }
}