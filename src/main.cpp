#include <AudioTools.h>
#include <HardwareSerial.h>
#include <espeak.h>

//#define DEBUG

I2SStream out;
VolumeStream vol(out);
ESpeak espeak(vol);
#ifndef DEBUG
HardwareSerial TextSerial(1);
#else
HWCDC TextSerial = Serial;
#endif
auto vol_config = vol.defaultConfig();
int volume = 100;
int rate = 200;

void setup() {
	Serial.begin(115200);
	espeak.begin();
	//TextSerial.begin(115200, SERIAL_8N1, 6, 5);
	auto cfg = out.defaultConfig();
	auto espeak_info = espeak.audioInfo();
	cfg.sample_rate = espeak_info.sample_rate;
	cfg.channels = espeak_info.channels;
	cfg.bits_per_sample = espeak_info.bits_per_sample;
	cfg.i2s_format = I2S_LSB_FORMAT;
	cfg.pin_ws = 7;
	cfg.pin_data = 8;
	cfg.pin_bck = 9;
	vol.begin(vol_config);
	out.begin(cfg);
	#ifdef DEBUG
	Serial.println("Waiting for data...");
	espeak.say("Hello World");
	//HWCDC TalkSerial = Serial;
	#else
	TextSerial.begin(115200, SERIAL_8N1, 6, 5);
	#endif
}

void loop() {

	String sin = TextSerial.readStringUntil('\n');
	Serial.printf("Received: %s\n", sin);
	if (!sin.isEmpty()) {
		// Check if sin starts with #!Volume
		if (sin.startsWith("#!V")) {
			String volDelta = sin.substring(4, sin.length() - 1);
			volume += volDelta.toInt();
			if (volume > 100)
				volume = 100;
			if (volume < 0)
				volume = 0;
			vol.setVolume((volume) / 100.0);
			return;
		} else if (sin.startsWith("#!S")) {
			String voice = sin.substring(4, sin.length() - 1);
			rate += voice.toInt();
			if (rate > 450)
				rate = 450;
			if (rate < 80)
				rate = 80;
			espeak.setRate(rate);
			return;
		}
		espeak.say(sin.c_str());
	}
}
