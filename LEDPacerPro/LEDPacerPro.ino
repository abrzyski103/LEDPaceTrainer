// Simple strand test for Adafruit Dot Star RGB LED strip.
// This is a basic diagnostic tool, NOT a graphics demo...helps confirm
// correct wiring and tests each pixel's ability to display red, green
// and blue and to forward data down the line.  By limiting the number
// and color of LEDs, it's reasonably safe to power a couple meters off
// the Arduino's 5V pin.  DON'T try that with other code!

#include <Adafruit_DotStar.h>
// Because conditional #includes don't work w/Arduino sketches...
#include <SPI.h>         // COMMENT OUT THIS LINE FOR GEMMA OR TRINKET
//#include <avr/power.h> // ENABLE THIS LINE FOR GEMMA OR TRINKET

#define NUMPIXELS 288 // Number of LEDs in strip

// Here's how to control the LEDs from any two pins:
#define DATAPIN    23
#define CLOCKPIN   18
Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
//Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BRG);

// Runs 10 LEDs at a time along strip, cycling through red, green and blue.
// This requires about 200 mA for all the 'on' pixels + 1 mA per 'off' pixel.

int      head  = 0, tail = -10; // Index of first 'on' and 'off' pixels
uint32_t color = 0x0000FF;      // 'On' color (starts red)
int StartTime;
bool forward = true;
int targetTime=20;
int CurrentTime;
int targetDelay=10.0/288*1000000 - 1632;
int walltargetDelay = 1.0*targetDelay*0.4;
int wallLeds=115;
void setup() {
targetDelay=1.0*targetDelay*1.4;
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000L)
  clock_prescale_set(clock_div_1); // Enable 16 MHz on Trinket
#endif
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
  Serial.begin(57600);
  Serial.println(targetDelay);
  StartTime=micros();
}

void loop() {
          if(forward){
            CurrentTime=micros();
            strip.setPixelColor(head, color); // 'On' pixel at head
            strip.setPixelColor(tail, 0);     // 'Off' pixel at tail
            strip.show();                     // Refresh strip
            if (wallLeds>0)
            {
              while(micros()-CurrentTime<=walltargetDelay){}  // Pause 20 milliseconds (~50 FPS)
              wallLeds=--wallLeds;
            }
            else
              while(micros()-CurrentTime<=targetDelay){}
            head= ++head;
            tail = ++tail;
          }
          else
          {
            CurrentTime=micros();
            strip.setPixelColor(head, 0); // 'On' pixel at head
            strip.setPixelColor(tail, color);     // 'Off' pixel at tail
            strip.show();                     // Refresh strip
            if (wallLeds>0)
            {
              while(micros()-CurrentTime<=walltargetDelay){}  // Pause 20 milliseconds (~50 FPS)
              wallLeds=--wallLeds;
            }
            else
              while(micros()-CurrentTime<=targetDelay){} // Pause 20 milliseconds (~50 FPS)
            head = --head;
            tail = --tail;
          }
        
          if (head==0 && !forward)
          {
            int TotalTime = micros()-StartTime;
            Serial.println(TotalTime);
          }
          
         if(tail+1 > NUMPIXELS) 
         {
            forward = false; 
            tail = NUMPIXELS; // Increment, reset tail index
            wallLeds=115;
         }
         if(head-1 <0 && !forward)
         {
            forward = true;
            for(int i =0;i<5;i++)
            {
              strip.setPixelColor(0,color);
              strip.setPixelColor(1,color);
              strip.setPixelColor(2,color);
              strip.setPixelColor(3,color);
              strip.setPixelColor(4,color);
              strip.setPixelColor(5,color);
              strip.setPixelColor(6,color);
              strip.setPixelColor(7,color);
              strip.setPixelColor(8,color);
              strip.setPixelColor(9,color);
              strip.setPixelColor(10,color);
              strip.show();
              delay(500);
              strip.setPixelColor(0,0);
              strip.setPixelColor(1,0);
              strip.setPixelColor(2,0);
              strip.setPixelColor(3,0);
              strip.setPixelColor(4,0);
              strip.setPixelColor(5,0);
              strip.setPixelColor(6,0);
              strip.setPixelColor(7,0);
              strip.setPixelColor(8,0);
              strip.setPixelColor(9,0);
              strip.setPixelColor(10,0);
              strip.show();
              delay(500);
            }
            head =0;
            tail = -10;
            wallLeds = 115;
            StartTime=micros();
          }
}
