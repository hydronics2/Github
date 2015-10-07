// 7/29/2015 - added sound. added pin x for Sparkfun WAV trigger: https://www.sparkfun.com/products/13660
// 7/29/2015 - increased the solenoid ON time from 50 to 100ms to push out the plastic pieces

#include <Adafruit_NeoPixel.h>

#define PIN 6 //timer data line (neopixel onewire)

#include <Button.h>

#include "Timer.h" 
Timer t;

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
Adafruit_NeoPixel strip = Adafruit_NeoPixel(144, PIN, NEO_GRB + NEO_KHZ800);

const int soundTrigger1 = 8; // T1 on WAV Trigger 

int a = 1; //first row starts at pixel 0 and counts up
int b = 94; //second row starts at pixel 95 and counts down
int c = 97; // 3rd rwo starts at pixel 96 and counts up

int i = 0;
int solenoidOnTime = 100; //relay pops the DNA pieces out

int brightness = 0;    // how bright the button is
int fadeAmount = 5;    // how many points to fade the button
unsigned long currentTime; //for button fading
unsigned long loopTime;  //for button fading

Button button = Button(10, BUTTON_PULLUP_INTERNAL);

int gameOn = 0;  //this tells the countdown timer to start

void setup() {
  pinMode(13,OUTPUT); //this is the relay to popPieces
  digitalWrite(13, LOW);
  
  pinMode(3, OUTPUT);  //button LED
  currentTime = millis();  //this is for the button LED fading
  loopTime = currentTime;   //this is for the button LED fading
  
 Serial.begin(9600);
  pinMode(soundTrigger1, OUTPUT);
  digitalWrite(soundTrigger1, HIGH); 
  
  // Start up the LED strip
  strip.begin();

  // Update the strip, to start they are all 'off'
  strip.show();
  
    t.every(1000, moveCounter);  //this move the countdown timer forward
}


void loop() {
  t.update();
  fadeButton();
  if(button.isPressed()){
    gameOn=1;  //starts the game
    digitalWrite(soundTrigger1, LOW); // triggers Sparkfun WAV Trigger to start audio file
  //Serial.print("game ON =: "); //debugging
  //Serial.println(gameOn);
 delay(500); }
  


  
}

void clearLeds(){
  for(i=0; i<144; i++) {
    strip.setPixelColor(i, strip.Color(0, 0, 0));
    strip.show();
a = 1;
b = 94;
c = 97;
  }
}

void popPieces(){
digitalWrite(13,HIGH);
delay(solenoidOnTime);
digitalWrite(13,LOW);
}

void fadeButton(){  //this if for the button LED fading
   
  currentTime = millis();
  if(currentTime >= (loopTime + 20)){  
    // set the brightness of pin 9:
    analogWrite(3, brightness);    

    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade: 
    if (brightness == 0 || brightness == 255) {
      fadeAmount = -fadeAmount ; 
    }     
    loopTime = currentTime;  // Updates loopTime
  }
  // Other processing can be done here
                           
}

void moveCounter()  //this is the coundown timer function
{
  if(gameOn>0){
    Serial.println("start blinking");
      for(i=0; i<a; i++) {
    strip.setPixelColor(i,strip.Color(255, 255, 255)); //R,B,G
  }
 a++;
  for(i=95; i>b; i--) {
    strip.setPixelColor(i,strip.Color(255, 255, 255)); //R,B,G
  }
    b--;
   for(i=96; i<c; i++) {
    strip.setPixelColor(i,strip.Color(255, 255, 255)); //R,B,G
  }
  c++;
  strip.show();   
//  
  //delay(1000);
  
  if(a>48) {  //this indicates the countdown timer complete
    gameOn=0;  //turns the game off
    digitalWrite(soundTrigger1, HIGH); //turns sound trigger OFF
    for(i=0; i<144; i++) {  //turns the leds red 
    strip.setPixelColor(i, strip.Color(255, 0, 0));
    strip.show();
    a = 1;
    b = 94;
    c = 97;
    }
    
    popPieces();  //this function initiates the relay to pop the pieces out
    clearLeds();  //this clears the countdown timer
    }
  }}
    
  
  
