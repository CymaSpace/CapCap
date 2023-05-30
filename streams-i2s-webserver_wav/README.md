# I2S to Webserver

This sketch is based on the example from `arduino_audio_tools`, and reads sound data from an I2S microphone (below).

The result is provided as WAV stream with the following format:
Big-endian
24-bit PCM
Sample rate 22050 Hz

The WAV stream currently doesn't seem to have proper format information for
browsers or VLC to play it correctly, it plays but just as static.
Additionally, the microphone sends two channels, and the library is passing
that on in our output, despite the configuration specifying one channel.

But, if the output is saved to a file, it can be opened as raw data in audacity
with the above settings, and has one channel of the sound as desired.

### Running

Copy `config.example.h` to `config.h` and fill in the wifi password, and update
pins as needed, then compile with the Arudino IDE. If you have `arduino-cli`
installed, you can also run `make` and `make upload` to compile as well. Update
the makefile with your board ID as needed (run `arduino-cli board search <your
board>` to find your board ID (FQBN))

### TODO
[ ] 2023-05-29 Create python script to convert Audio Tools Library Audio from ESP32
[ ] 2023-05-29 Test/adapt to LilyGo

## Dev Log

2023-05-30
- Switched to ESP32-WROOM for now
- Adjusted example to use 24-bit audio instead of 16-bit and removed filler converter for now, which might require 16-bit and wasn't working anyway
- Set up pins for ESP32-WROOM
- Output audio data as base64 for debug (now commented out)
- Saved audio to file and opened with Audacity, extracted sound from left channel
- Verified I2S data with oscilloscope

![Picture of an oscilloscope screen showing several rectangles across the
middle in yellow and blue. The blue alternates between high and low, and the
yellow alternates between being a rectangle with several vertical lines in the right
half, and being a slow concave curve from the top-left to the lower-right.][scopetestannotated.jpg]

## Appendix

### Microphone
![inmp441](https://pschatzmann.github.io/resources/img/inmp441.jpeg)

the inmp441 is a high-performance, low power, digital-output, omnidirectional mems microphone with a bottom port. the complete inmp441 solution consists of a mems sensor, signal conditioning, an analog-to-digital converter, anti-aliasing filters, power management, and an industry-standard 24-bit i²s interface. the i²s interface allows the inmp441 to connect directly to digital processors, such as dsps and microcontrollers, without the need for an audio codec in the system.

## pins
 
| inmp441 |  esp32
| --------| ---------------
| vdd     |  3.3
| gnd     |  gnd
| sd      |  in (PIN_DAT)
| l/r     |  gnd
| ws      |  ws
| sck     |  bck

- sck: serial data clock for i²s interface
- ws: select serial data words for the i²s interface
- l/r: left / right channel selection
        when set to low, the microphone emits signals on the left channel of the i²s frame.
        when the high level is set, the microphone will send signals on the right channel.
- exsd: serial data output of the i²s interface
- vcc: input power 1.8v to 3.3v
- gnd: power groundhigh psr: -75 dbfs.


