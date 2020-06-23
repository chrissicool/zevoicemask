# Ze Voice Mask

This is an Arduino implemetation of a face mask with voice visuals.
It features a microphone and a 8x8 matrix LED screen.
In normal operation it shows a mouth on the screen that reproduces your voice.
Put it in the pocket of your face mask and show people your smile.

When you do not talk for a while, it plays some animations every now and then.

## Hardware

You will need the following hardware:

 *   ESP8266 NodeMCU DevBoard

 *   Microphone sensor (MAX4466) breakout board with a rotary potentiometer

     Connect the sensor's *OUT* pin to the *A0* pin on the NodeMCU board.
     Connect the sensor's *VCC* and *GND* pins to their respective *3.3V* and *GND* pins on the NodeMCU board.

     Adjust the potentiometer to almost minimum gain.
     Play a bit around to find a good value.

 *   8x8 matrix LED based on WS2812B (ideally with a flexible board)

     Connect the matrix's *DIN* (data in) pin with the *RX* pin (aka. *D9*) on the NodeMCU board.
     Connect the matrix's *5V*/*VCC* and *GND* pins to their respective *3.3V* and *GND* pins on the NodeMCU board.

     In your final setup, the matrix can have another layout than mine.
     You will need to adjust the NeoTopology in the code.

 *   9V block battery clip (optional)

     Connect the clip's positive (*+*) wire to the *VIN* pin on the NodeMCU board.
     Connect the clip's negative (*-*) wire to any of the *GND* pins on the NodeMCU board.

If you choose not to use the battery clip, you can use a regular USB powerbank instead.

## Installation

This is an Arduino project based on [Platform.IO](https://platformio.org/).
After you have set up the hardware, simply flash the firmware.

    make flash

## Hacking

Arduino has the drawback that it always runs the *loop()* function,
even when there is nothing to do.
The ESP family of microcontrollers has a mechanism based on timer-events to schedule execution jobs.
This allows for cleaner code than using *millis()* and put everything into the *loop()* function.

Timers are implemented with a real hardware timer interrupt on ESPs.
The hardware timer always fires at the same interval.
This also allows us to put the CPU to a full halt when there is nothing to do.
This is *not* deep-sleep, but an idle sleep.
One drawback to this is that you need to wake up periodically,
otherwise the hardware watchdog will reboot the device.
In practice, this does not seem to be a problem (especially when you need to poll sensors).

The code uses the aweseome [NeoPixelBus](https://github.com/Makuna/NeoPixelBus) library.

Feel free to file bug reports or pull requests.

Happy hacking!

## History

This project came to life during the SARS-CoV2 world-wide pandemic in 2020.
