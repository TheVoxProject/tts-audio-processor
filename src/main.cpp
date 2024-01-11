#include "flite_arduino.h" // install https://github.com/pschatzmann/arduino-flite
#include "AudioTools.h" // install https://github.com/pschatzmann/arduino-audio-tools
//#include "AudioLibs/AudioKit.h"

I2SStream out; // Replace with desired class e.g. AudioKitStream, AnalogAudioStream etc.
Flite flite(out);


void setup(){
  Serial.begin(115200);
  AudioLogger::instance().begin(Serial, AudioLogger::Info);

  // start data sink
  auto cfg = out.defaultConfig();
  cfg.sample_rate = 8000;
  cfg.channels = 1;
  cfg.bits_per_sample = 16;
  cfg.i2s_format = I2S_LSB_FORMAT;
  cfg.pin_ws = 25;
  cfg.pin_bck = 2;
  cfg.pin_data = 26;
  out.begin(cfg);
}

// Arduino loop  
void loop() {  
    String sin = Serial.readStringUntil('\n');
    //Serial.print("Received: ");
    //Serial.println(sin);
    if (!sin.isEmpty()) {
        flite.say(sin.c_str());
    }
}