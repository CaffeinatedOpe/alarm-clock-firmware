/**
 * @file player-sd-audiokit.ino
 * @brief see https://github.com/pschatzmann/arduino-audio-tools/blob/main/examples/examples-audiokit/player-sd-audiokit/README.md
 * Make sure that the pins are set to off, on, on, off, off
 * @author Phil Schatzmann
 * @copyright GPLv3
 */
#include "AudioTools.h"
#include "AudioTools/AudioLibs/AudioBoardStream.h"
#include "AudioTools/Disk/AudioSourceSD.h" // or AudioSourceIdxSD.h
#include "AudioTools/AudioCodecs/CodecMP3Helix.h"
#include "audio.h"
#include <stdio.h>
#include "wifiCreator.h"

void setup(){
   //audioSetup(9000);
   wifiSetup();
}

void loop(){
   //audioLoop();
   wifiLoop();
}