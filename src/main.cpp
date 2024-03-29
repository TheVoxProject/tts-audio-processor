#include <AudioTools.h>
#include <HardwareSerial.h>
#include <flite_arduino.h>

I2SStream out;
VolumeStream vol(out);
Flite flite(vol);
HardwareSerial TextSerial(1);
auto vol_config = vol.defaultConfig();
int volume = 100;

void setup() {
	Serial.begin(115200);
	TextSerial.begin(115200, SERIAL_8N1, 6, 5);
	auto cfg = out.defaultConfig();
	cfg.sample_rate = 8000;
	cfg.channels = 1;
	cfg.bits_per_sample = 16;
	cfg.i2s_format = I2S_LSB_FORMAT;
	cfg.pin_ws = 7;
	cfg.pin_data = 8;
	cfg.pin_bck = 9;
	vol.begin(vol_config);
	out.begin(cfg);
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
		}
		flite.say(sin.c_str());
	}
}
