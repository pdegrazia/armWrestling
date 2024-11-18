 
#include <FastLED.h>
#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

#define LED_PIN     3
#define NUM_LEDS    40
#define BRIGHTNESS  50
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define PLAYER_ONE_BUTTON 10
#define PLAYER_TWO_BUTTON 9

#define UPDATES_PER_SECOND 100

#define HARDWARE_TYPE MD_MAX72XX::FC16_HW

#define MAX_DEVICES 4

#define CS_PIN 5
#define CLK_PIN 6
#define DATA_PIN 4

MD_Parola P = MD_Parola(HARDWARE_TYPE, DATA_PIN, CLK_PIN, CS_PIN, MAX_DEVICES);

int playerOneCurrentIndex=19;
int playerTwoCurrentIndex=20;

bool gameStarted = false;


const char  *phrases[] =
{
  "Who's the strongest??? ",
  "Ready?",
  "Fight!!!",
};

int phrasesIndex = 0;

//button stuff
bool buttonOnePressed = 0;
int currentStatusButtonOne = 1;

bool buttonTwoPressed = 0;
int currentStatusButtonTwo = 1;

//score stuff
int redIndex = 20;

void setup() {
  delay( 3000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS);
  FastLED.setBrightness(  BRIGHTNESS );
  pinMode(PLAYER_ONE_BUTTON, INPUT);
  digitalWrite(PLAYER_ONE_BUTTON, HIGH);
  pinMode(PLAYER_TWO_BUTTON, INPUT);
  digitalWrite(PLAYER_TWO_BUTTON, HIGH);
  Serial.begin(9600);
  P.begin();
  P.setSpeed(100);
  for (int i=0; i<redIndex; i++){
      leds[i]=CRGB::Red;
  }
    
  for (int i=redIndex; i<40; i++){
      leds[i]=CRGB::Blue;
  }
  FastLED.show();
}

void loop() {

  if (gameStarted) {    
    //reads 0 when pressed, 1 when opened
    buttonOnePressed = digitalRead(PLAYER_ONE_BUTTON);

    if (buttonOnePressed == 0) {
      if (buttonOnePressed != currentStatusButtonOne){
        currentStatusButtonOne = 0;
        //increase score of player 1
        if(redIndex == 39) {
          P.print("RED");
          delay(10000);
          gameStarted = false;
        } else {
          redIndex++;
        }
      }
    } else {
      currentStatusButtonOne = 1;
    }

    //reads 0 when pressed, 1 when opened
    buttonTwoPressed = digitalRead(PLAYER_TWO_BUTTON);

    if (buttonTwoPressed == 0) {
      if (buttonTwoPressed != currentStatusButtonTwo){
        currentStatusButtonTwo = 0;
        //increase score of player 2
        if(redIndex == 0){
          P.print("BLUE");
          delay(10000);
          gameStarted = false;
        } else {
          redIndex--;
        }
      }
    } else {
      currentStatusButtonTwo = 1;
    }
  } else {
    // this code is before game started
    redIndex = 20;
    if (P.displayAnimate()){
      P.displayText(phrases[0], PA_CENTER, 100, 0, PA_SCROLL_LEFT, PA_NO_EFFECT);
    }
    
    buttonOnePressed = digitalRead(PLAYER_ONE_BUTTON);
    if (buttonOnePressed == 0) {
        P.print("ready?");
        delay(1000);
        P.print("3");
        delay(1000);
        P.print("2");
        delay(1000);
        P.print("1");
        delay(1000);
        P.print("fight!!!");
        gameStarted=true;
    }  
  }

  //update the led strip each cycle, regardless of whether game started or not               
  for (int i=0; i<redIndex; i++){
      leds[i]=CRGB::Red;
  }
  
  for (int j=redIndex; j<40; j++){
      leds[j]=CRGB::Blue;
  }

  FastLED.show();
  FastLED.delay(1000 / UPDATES_PER_SECOND);
}
