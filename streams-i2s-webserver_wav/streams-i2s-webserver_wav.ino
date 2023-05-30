/**
 * @file streams-i2s-webserver_wav.ino
 *
 *  This sketch reads sound data from I2S. The result is provided as WAV stream which can be listened to in a Web Browser
 *
 * @author Phil Schatzmann
 * @copyright GPLv3
 */

#include "AudioTools.h"
#include "base64.hpp"
#include "config.h"

//AudioEncodedServer server(new WAVEncoder(),"ssid","password");
AudioWAVServer server(wifiName, wifiPassword);  // the same a above

#define BUFSZ 200

uint8_t bytes[100];
unsigned char base64[BUFSZ];
char out[BUFSZ];

I2SStream i2sStream;                                     // Access I2S as stream
//ConverterFillLeftAndRight<int24_t> filler(RightIsEmpty);  // fill both channels - or change to RightIsEmpty

void setup() {
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  Serial.println("WiFi connected, go to:");
  Serial.println(WiFi.localIP());

  // start i2s input with default configuration
  Serial.println("starting I2S...");
  auto config = i2sStream.defaultConfig(RX_MODE);
  config.i2s_format = I2S_STD_FORMAT; // if quality is bad change to I2S_LSB_FORMAT https://github.com/pschatzmann/arduino-audio-tools/issues/23
  config.sample_rate = 22050;
  config.channels = 1;
  config.bits_per_sample = 24;
  config.pin_data = PIN_SD;
  config.pin_ws = PIN_WS;
  config.pin_bck = PIN_SCK;
  i2sStream.begin(config);
  Serial.println("I2S started");

  // start data sink
  server.begin(i2sStream, config);

  Serial.println("Sinking data");
}

// Arduino loop  
void loop() {
  // Handle new connections
  server.copy();
  /*
  if(i2sStream.available()) {
    size_t byteCount = i2sStream.readBytes(bytes, 50);
    size_t encoded_len = encode_base64(bytes, byteCount, base64);
    //Serial.println((const char*)base64);
    snprintf(out, 200, "Read %d bytes: %s\n", byteCount, base64);
    Serial.print(out);
  }
  */
}
