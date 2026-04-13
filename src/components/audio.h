#include <SPI.h>
#include <SD.h>
#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioBoardStream.h"
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"


const int chipSelect=PIN_AUDIO_KIT_SD_CARD_CS;
AudioBoardStream i2s(AudioKitEs8388V1); // final output of decoded stream
EncodedAudioStream decoder(&i2s, new MP3DecoderHelix()); // Decoding stream
StreamCopy copier; 
File audioFile;


typedef enum{
	PLAYING,
	SILENT
} AUDIOSTATUS;

AUDIOSTATUS audioStatus = SILENT;
float audioVolume;

void audioSetup() {
  auto config = i2s.defaultConfig(TX_MODE);
  config.sd_active = true;
  i2s.begin(config);

  // setup file
  SD.begin(chipSelect);
  audioFile = SD.open("/sounds/alarm.mp3");

  // setup I2S based on sampling rate provided by decoder
  decoder.begin();
	i2s.setVolume(0.5);

  // begin copy
  copier.begin(decoder, audioFile);
}

void audioPeriodic() {
  if (!copier.copy()) {
    stop();
  }
}

void playAudioLoop() {
	if (!copier.copy()) {
		audioFile = SD.open("/sounds/alarm.mp3");
    copier.begin(decoder, audioFile);
		Serial.println("looping");
  }
}

void startAudio() {
	Serial.println("starting playback");
	audioStatus = PLAYING;
	audioFile = SD.open("/sounds/alarm.mp3");
	copier.begin(decoder, audioFile);
}

void stopAudio() {
	Serial.println("stopped playback");
	copier.end();
	audioStatus = SILENT;
}

void toggleAudioState() {
	if (audioStatus == SILENT) {
		startAudio();
	}
	else {
		stopAudio();
	}
}