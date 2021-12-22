#include <Adafruit_NeoPixel.h>

// Button Input Pins
// The data pin used for detecting the START button being pressed
#define START_PIN 2
// The data pin used for detecting the SELECT button being pressed
#define SELECT_PIN 3
// The data pin used for detecting the L button being pressed
#define L_PIN 5
// The data pin used for detecting the R button being pressed
#define R_PIN 7
// The data pin used for detecting the A button being pressed
#define A_PIN 10
// The data pin used for detecting the B button being pressed
#define B_PIN 11
// The data pin used for detecting the X button being pressed
#define X_PIN 12

//Analog Pins - slightly different implementation
// The data pin used for detecting the Y button being pressed
#define Y_PIN A6
// The data pin used for detecting the LEFT button being pressed
#define LEFT_PIN A0
// The data pin used for detecting the RIGHT button being pressed
#define RIGHT_PIN A1
// The data pin used for detecting the DOWN button being pressed
#define DOWN_PIN A2
// The data pin used for detecting the UP button being pressed
#define UP_PIN A7

//Button Output Pins
// The data pin used for triggering the CAPTURE button
#define CAPTURE_PIN 4
// The data pin used for triggering the HOME button
#define HOME_PIN 6

//LED Variables
// Data pin used for LEDs
#define LED_PIN 9
// How many NeoPixels are attached to the Arduino?
#define LED_COUNT 27 //Total number of LEDs attached 
// Define the number of LEDs ACTUALLY used
#define LEDS_USED 14 //HAVE GOTTEN AWAY WITH USING 18 LEDS BEFORE ON WHITE BREATHING
// NeoPixel brightness, 0 (min) to 255 (max)
#define BRIGHTNESS 150
// Number of LED modes
#define LED_MODES 7
// Initial LED mode (range of 0 to LED_MODES - 1)
#define INIT_MODE 1

// The time before the controller determines it's idle.
#define AFK_TIME 30000 //Problems if exceeding 30000
//~45 seconds at 30000 @ increment of 1.
//Right now, timeout is ~5 mins and increments are in 0.15.

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)

void setup() {
  // intialize the buttons being detected
  pinMode( START_PIN, INPUT );
  pinMode( SELECT_PIN, INPUT );
  pinMode( L_PIN, INPUT );
  pinMode( R_PIN, INPUT );
  pinMode( A_PIN, INPUT );
  pinMode( B_PIN, INPUT );
  pinMode( X_PIN, INPUT );
  // initialize the analog input buttons (default are D-pad buttons)
  pinMode( Y_PIN, INPUT );
  pinMode( LEFT_PIN, INPUT );
  pinMode( RIGHT_PIN, INPUT );
  pinMode( DOWN_PIN, INPUT );
  pinMode( UP_PIN, INPUT );
  // initialize the button outputs
  pinMode( HOME_PIN, OUTPUT );
  pinMode( CAPTURE_PIN, OUTPUT );
  // set the pin modes to default to high
  digitalWrite(HOME_PIN, HIGH);
  digitalWrite(CAPTURE_PIN, HIGH);

  // initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);
  // initialize the LED strip
  strip.begin();           // INITIALIZE NeoPixel strip object
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(BRIGHTNESS); // Set the brightness of the LEDs
}

void loop() {
  // This is the default mode number for the LEDs
  int mode = INIT_MODE;
  // This is the flag used for telling if the mode is switched
  boolean modeSwitched = false;
  // This is the number that will be incremented each time to update all LEDs
  int ledNum = -1;
  // This is the number that will be kept for the iteration
  int iteration = -16;
  if(mode == 1) {
    iteration = -16;
  } else {
    iteration = -1;
  }
  // This makes the LEDs reverse their color direction change
  int flip = 0;
  // This is for the fade for LEDs
  int fadeVal=0, fadeMax=100;
  // Smoothness of the transition in the dragon color transitions
  int smoothness = 500;
  // affects the width of peak (more or less darkness)
  float gamma = 0.21;//0.14;
  // shifts the gaussian to be symmetric
  float beta = 0.5;
  // Create the offset variable
  int offset = 0;
  // Variables for Orange/Blue/White switching
  int lowTime = 100;
  int midTime = 1000;
  int highTime = 1400;
  // AFK Variables
  bool isAFK = false;
  float afkTiming = 0.0;
  int previousMode = -1;
  while(true) {
    // Test for input and if something is input, use the led on board to show an input is pressed
    if(digitalRead(START_PIN) == LOW || digitalRead(SELECT_PIN) == LOW || digitalRead(L_PIN) == LOW || digitalRead(R_PIN) == LOW 
    || digitalRead(A_PIN) == LOW || digitalRead(B_PIN) == LOW || digitalRead(X_PIN) == LOW || (analogRead(Y_PIN) < 300)
    || (analogRead(LEFT_PIN) < 300) || (analogRead(RIGHT_PIN) < 300) || (analogRead(DOWN_PIN) < 300)  || (analogRead(UP_PIN) < 300)){
      //Detect if button is pressed
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      //Reset AFK timing
      afkTiming = 0.0;
    } else {
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      if(!isAFK) {
        //Start AFK counter
        afkTiming += 0.15;
      }
    }
    
    // Check for the START and SELECT buttons being pressed. If they are, tigger the home button
    if(digitalRead(START_PIN) == LOW && digitalRead(SELECT_PIN) == LOW) { //Detect if buttons are pressed
      // Check for if changing for LS/RS/DP emulation
      if( !((analogRead(LEFT_PIN) < 300) || (analogRead(RIGHT_PIN) < 300) || (analogRead(UP_PIN) < 300)) ) {
        digitalWrite(HOME_PIN, LOW);   // trigger the home button by shorting to ground
      }
    } else {
      digitalWrite(HOME_PIN, HIGH);    // stop the home button trigger
    }
    // Check for the SELECT and L buttons being pressed. If they are, tigger the capture button
    if(digitalRead(SELECT_PIN) == LOW && digitalRead(L_PIN) == LOW) { //Detect if buttons are pressed
      digitalWrite(CAPTURE_PIN, LOW);   // trigger the capture button by shorting to ground
    } else {
      digitalWrite(CAPTURE_PIN, HIGH);    // stop the capture button trigger
    }
    // Reset flag for switching LED modes
    if((digitalRead(START_PIN) == HIGH || digitalRead(L_PIN) == HIGH) && modeSwitched) {
      modeSwitched = false;
    }

    // Controller goes into AFK mode
    if(afkTiming > AFK_TIME && !isAFK) {
      isAFK = true;
      previousMode = mode;
      mode = 0;
    }

    // Controller exits AFK mode
    if(afkTiming < AFK_TIME && isAFK) {
      isAFK = false;
      mode = previousMode;
      previousMode = -1;
    }
    
    // Check for the START and L buttons being pressed. If they are, change the LED mode. If holding down, go backwards
    if(digitalRead(START_PIN) == LOW && digitalRead(L_PIN) == LOW && !modeSwitched) {
      // Detect if button is pressed
      // Set all values back to initial
      ledNum = -1;
      if(mode == 1) {
        iteration = -16;
      } else {
        iteration = -1;
      }
      flip = 0;
      fadeVal=0;
      fadeMax=100;
      // If going backwards
      if(analogRead(DOWN_PIN) < 300) {
        //Decrement mode
        mode -= 1;
        // Set mode switch flag
        modeSwitched = true;
        // reset if mode number is not less than number of modes
        if(mode < 0) {
          mode = LED_MODES - 1;
        }
      } else {
        // Increment mode
        mode += 1;
        // Set mode switch flag
        modeSwitched = true;
        // reset if mode number is not less than number of modes
        if(mode >= LED_MODES) {
          mode = 0;
        }
      }
    }
  
  // Manipulate LEDs
    // increment LED num
    if(ledNum == 0 || ledNum == 2 || ledNum == 4 || ledNum == 6 || ledNum == 8 || ledNum == 10 || ledNum == 12 || ledNum == 14 || ledNum == 16 || ledNum == 18 || ledNum == 20 || ledNum == 22 || ledNum == 24) {
      ledNum += 2;
    //} else if(ledNum == 4 || ledNum == 18) {
     // ledNum += 4;
    } else {
      ledNum += 1;
    }
    //Check valid LED num
    if(ledNum >= LED_COUNT) {
      ledNum = 0;
    }
    
    ////////////////////////////////////
    //  LEDS OFF                      //
    ////////////////////////////////////
    if(mode == 0) {
      strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
    }
    
    ////////////////////////////////////
    //  WHITE BREATHING               //
    ////////////////////////////////////
    else if(mode == 6) {
      // increment loop iteration if at the start of the loop
      if(ledNum == 0) {
        if(flip == 0) {
          iteration += 1;
        } else {
          iteration -= 1;
        }
      }
      // Set the flip if at either end of the spectrum
      if(iteration > 255) {
        flip = 1;
        iteration = 255;
      } else if(iteration < 0) {
        flip = 0;
        iteration = 0;
      }
      strip.setPixelColor(ledNum, strip.Color(iteration, iteration, iteration));
    }
    
    ////////////////////////////////////
    //  RANDOM RAINBOW LIGHTS         //
    ////////////////////////////////////
    else if(mode == 2) {
      // increment loop iteration if at the start of the loop
      //if(ledNum == 0) {
        if(iteration > 65536) {
          iteration = 0;
        } else {
          iteration += 16;
        }
      //}
      // Hue of first pixel runs 'rainbowLoops' complete loops through the color
      // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
      // just count from 0 to rainbowLoops*65536, using steps of 256 so we
      // advance around the wheel at a decent clip.
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      //uint32_t pixelHue = iteration + (ledNum * 65536L / strip.numPixels());
      uint32_t pixelHue = iteration + (ledNum * 65536L / LED_COUNT);
    
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(ledNum, strip.gamma32(strip.ColorHSV(pixelHue, 255, 255 * fadeVal / fadeMax)));
      if(iteration < 65536) {                              // First loop,
        if(fadeVal < fadeMax) fadeVal++;                   // fade in
      } else {
        fadeVal = fadeMax; // Interim loop, make sure fade is at max
      }
    }
    
    ////////////////////////////////////
    //  INPUT DISPLAY                 //
    ////////////////////////////////////
    else if(mode == 3) {
      // Do something different depending on the LED
      switch(ledNum) {
        default:
          // Base case -> make LEDs white 255, 255, 255
          strip.setPixelColor(ledNum, strip.Color(255, 255, 255));
          break;
        case 22:
          // case for LED 4 -> A button 245, 33, 55
          if(digitalRead(A_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(255, 0, 0));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
        case 20:
          // case for LED 4 -> B button 232, 166, 22
          if(digitalRead(B_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(255, 255, 0));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
        case 18:
          // case for LED 4 -> X button 38, 102, 217
          if(digitalRead(X_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 255));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
        case 16:
          // case for LED 4 -> Y button 55, 186, 70
          if(analogRead(Y_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(0, 255, 0));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
        case 14:
          // case for LED 4 -> R button 187, 255, 254
          if(digitalRead(R_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(0, 255, 255));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
        case 12:
          // case for LED 4 -> L button 187, 255, 254
          if(digitalRead(L_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(0, 255, 255));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
        case 10:
          // case for LED 4 -> RIGHT button 55, 186, 70
          if(analogRead(RIGHT_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(0, 255, 0));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
        case 8:
          // case for LED 4 -> UP button 38, 102, 217
          if(analogRead(UP_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 255));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
        case 6:
          // case for LED 4 -> DOWN button 232, 166, 22
          if(analogRead(DOWN_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(255, 255, 0));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
        case 4:
          // case for LED 4 -> LEFT button 245, 33, 55
          if(analogRead(LEFT_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(255, 0, 0));
          } else {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 0));
          }
          break;
      }
    }
    
    ////////////////////////////////////
    //  Solid Color Rainbow Fade      //
    ////////////////////////////////////
    else if(mode == 4) {
      if(iteration > 65536) {
        iteration = 0;
      } else {
        iteration += 16;
      }
      // Hue of first pixel runs 'rainbowLoops' complete loops through the color
      // wheel. Color wheel has a range of 65536 but it's OK if we roll over, so
      // just count from 0 to rainbowLoops*65536, using steps of 256 so we
      // advance around the wheel at a decent clip.
      // Offset pixel hue by an amount to make one full revolution of the
      // color wheel (range of 65536) along the length of the strip
      // (strip.numPixels() steps):
      //uint32_t pixelHue = iteration + (ledNum * 65536L / strip.numPixels());
      uint32_t pixelHue = iteration + (ledNum * 65536L / 2);
    
      // strip.ColorHSV() can take 1 or 3 arguments: a hue (0 to 65535) or
      // optionally add saturation and value (brightness) (each 0 to 255).
      // Here we're using just the three-argument variant, though the
      // second value (saturation) is a constant 255.
      strip.setPixelColor(ledNum, strip.gamma32(strip.ColorHSV(pixelHue, 255, 255 * fadeVal / fadeMax)));
      if(iteration < 65536) {                              // First loop,
        if(fadeVal < fadeMax) fadeVal++;                   // fade in
      } else {
        fadeVal = fadeMax; // Interim loop, make sure fade is at max
      }
    }
    
    ////////////////////////////////////
    //  Charizard Lights              //
    ////////////////////////////////////
    else if(mode == 5) {
      //Increment the iterator
      if(ledNum == 0) {
        //if(iteration > smoothness) {
        if(iteration >highTime) {
          iteration = 0;
        } else {
          iteration += 1;
        }
      }
      
      // Cycle through colors smoothly
      if(iteration >= 0 && iteration < lowTime) { //white -> blue
        if(ledNum > LED_COUNT / 2) {
          strip.setPixelColor(ledNum, strip.Color(255-255*((iteration)/float(lowTime)), 255-255*(iteration)/(lowTime), 255));
        } else {
          strip.setPixelColor(ledNum, strip.Color(255, 255-255*((iteration)/float(lowTime)), 255-255*(iteration)/(lowTime)));
        }
      } else if(iteration >= lowTime && iteration < midTime) { //blue -> cyan
        if(ledNum > LED_COUNT / 2) {
          strip.setPixelColor(ledNum, strip.Color(0, 255*((iteration-lowTime)/float(midTime-lowTime)), 255));
        } else {
          strip.setPixelColor(ledNum, strip.Color(255, 128*((iteration-lowTime)/float(midTime-lowTime)), 0));
        }
      } else if(iteration >= midTime && iteration < highTime) { //cyan -> white
        if(ledNum > LED_COUNT / 2) {
          strip.setPixelColor(ledNum, strip.Color(255*((iteration-midTime)/float(highTime-midTime)), 255, 255));
        } else {
          strip.setPixelColor(ledNum, strip.Color(255, 128 + 128*((iteration-midTime)/float(highTime-midTime)), 255*((iteration-midTime)/float(highTime-midTime))));
        }
      }
      
      // Do something different depending on the LED
     /* switch(ledNum) {
        default:
          // Base case -> make LEDs white 255, 255, 255
          strip.setPixelColor(ledNum, strip.Color(255, 255, 255));
          break;
        case 0:
          //strip.setPixelColor(ledNum, strip.Color(0, 1, 1));
          //offset = 426;
          offset = 400;
          strip.setPixelColor(ledNum, strip.Color(0, int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0)))));
          break;
        case 2:
          //strip.setPixelColor(ledNum, strip.Color(0, 22, 22));
          //offset = 355;
          offset = 325;
          strip.setPixelColor(ledNum, strip.Color(0, int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0)))));
          break;
        case 4:
          //strip.setPixelColor(ledNum, strip.Color(0, 43, 43));
          //offset = 284;
          offset = 275;
          strip.setPixelColor(ledNum, strip.Color(0, int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0)))));
          break;
        case 6:
          //strip.setPixelColor(ledNum, strip.Color(0, 65, 65));
          //offset = 213;
          offset = 250;
          strip.setPixelColor(ledNum, strip.Color(0, int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0)))));
          break;
        case 8:
          //strip.setPixelColor(ledNum, strip.Color(0, 86, 86));
          //offset = 142;
          offset = 225;
          strip.setPixelColor(ledNum, strip.Color(0, int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0)))));
          break;
        case 10:
          //strip.setPixelColor(ledNum, strip.Color(0, 107, 107));
          //offset = 71;
          offset = 175;
          strip.setPixelColor(ledNum, strip.Color(0, int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0)))));
          break;
        case 12:
          //strip.setPixelColor(ledNum, strip.Color(0, 128, 128));
          //offset = 0;
          offset = 100;
          strip.setPixelColor(ledNum, strip.Color(0, int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(128*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0)))));
          break;
        case 14:
          //strip.setPixelColor(ledNum, strip.Color(255, 85, 0));
          offset = 0;
          strip.setPixelColor(ledNum, strip.Color(int(255*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(85*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), 0));
          break;
        case 16:
          //strip.setPixelColor(ledNum, strip.Color(213, 71, 0));
          offset = 71;
          strip.setPixelColor(ledNum, strip.Color(int(255*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(85*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), 0));
          break;
        case 18:
          //strip.setPixelColor(ledNum, strip.Color(171, 57, 0));
          offset = 142;
          strip.setPixelColor(ledNum, strip.Color(int(255*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(85*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), 0));
          break;
        case 20:
          //strip.setPixelColor(ledNum, strip.Color(129, 43, 0));
          offset = 213;
          strip.setPixelColor(ledNum, strip.Color(int(255*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(85*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), 0));
          break;
        case 22:
          //strip.setPixelColor(ledNum, strip.Color(87, 29, 0));
          offset = 284;
          strip.setPixelColor(ledNum, strip.Color(int(255*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(85*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), 0));
          break;
        case 24:
          //strip.setPixelColor(ledNum, strip.Color(45, 15, 0));
          offset = 355;
          strip.setPixelColor(ledNum, strip.Color(int(255*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(85*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), 0));
          break;
        case 26:
          //strip.setPixelColor(ledNum, strip.Color(3, 1, 0));
          offset = 426;
          strip.setPixelColor(ledNum, strip.Color(int(255*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), int(85*(exp(-(pow(((((iteration + offset) % smoothness)/float(smoothness))-beta)/gamma,2.0))/2.0))), 0));
          break;
      }*/
    }
    
    ////////////////////////////////////
    // Charizard Lights Input Display //
    ////////////////////////////////////
    else if(mode == 1) {
      //Increment the iterator
      if(ledNum == 0) {
        //if(iteration > smoothness) {
        if(iteration > highTime) {
          iteration = 0;
        } else {
          iteration += 1;
        }
      }
      
      // Cycle through colors smoothly
      if(iteration >= 0 && iteration < lowTime) { //white -> blue
        if(ledNum > LED_COUNT / 2) {
          strip.setPixelColor(ledNum, strip.Color(255-255*((iteration)/float(lowTime)), 255-255*(iteration)/(lowTime), 255));
        } else {
          strip.setPixelColor(ledNum, strip.Color(255, 255-255*((iteration)/float(lowTime)), 255-255*(iteration)/(lowTime)));
        }
      } else if(iteration >= lowTime && iteration < midTime) { //blue -> cyan
        if(ledNum > LED_COUNT / 2) {
          strip.setPixelColor(ledNum, strip.Color(0, 255*((iteration-lowTime)/float(midTime-lowTime)), 255));
        } else {
          strip.setPixelColor(ledNum, strip.Color(255, 128*((iteration-lowTime)/float(midTime-lowTime)), 0));
        }
      } else if(iteration >= midTime && iteration < highTime) { //cyan -> white
        if(ledNum > LED_COUNT / 2) {
          strip.setPixelColor(ledNum, strip.Color(255*((iteration-midTime)/float(highTime-midTime)), 255, 255));
        } else {
          strip.setPixelColor(ledNum, strip.Color(255, 128 + 128*((iteration-midTime)/float(highTime-midTime)), 255*((iteration-midTime)/float(highTime-midTime))));
        }
      }

      // Do something different depending on the LED
      switch(ledNum) {
        default:
          // Base case -> Do nothing
          break;
        case 22:
          // case for LED 4 -> A button 245, 33, 55
          if(digitalRead(A_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(255, 0, 0));
          }
          break;
        case 20:
          // case for LED 4 -> B button 232, 166, 22
          if(digitalRead(B_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(255, 255, 0));
          }
          break;
        case 18:
          // case for LED 4 -> X button 38, 102, 217
          if(digitalRead(X_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 255));
          }
          break;
        case 16:
          // case for LED 4 -> Y button 55, 186, 70
          if(analogRead(Y_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(0, 255, 0));
          }
          break;
        case 14:
          // case for LED 4 -> R button 187, 255, 254
          if(digitalRead(R_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(0, 255, 255));
          }
          break;
        case 12:
          // case for LED 4 -> L button 187, 255, 254
          if(digitalRead(L_PIN) == LOW) {
            strip.setPixelColor(ledNum, strip.Color(0, 255, 255));
          }
          break;
        case 10:
          // case for LED 4 -> RIGHT button 55, 186, 70
          if(analogRead(RIGHT_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(0, 255, 0));
          }
          break;
        case 8:
          // case for LED 4 -> UP button 38, 102, 217
          if(analogRead(UP_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(0, 0, 255));
          }
          break;
        case 6:
          // case for LED 4 -> DOWN button 232, 166, 22
          if(analogRead(DOWN_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(255, 255, 0));
          }
          break;
        case 4:
          // case for LED 4 -> LEFT button 245, 33, 55
          if(analogRead(LEFT_PIN) < 300) {
            strip.setPixelColor(ledNum, strip.Color(255, 0, 0));
          }
          break;
      }
    } //END
    strip.show();
  }
}
