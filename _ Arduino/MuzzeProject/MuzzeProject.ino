#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define LED_PIN   9
#define LED_DEBUG 13
#define NUM_LEDS  24

CRGB leds[NUM_LEDS];

#define MAX_BRIGHTNESS 164      // Thats full on, watch the power!
#define MIN_BRIGHTNESS 32       // set to a minimum of 25%

// LED read vars
String inputString = "";         // a string to hold incoming data
boolean toggleComplete = false;  // whether the string is complete
boolean setledComplete = false;
int i;
char *recievedValTab;

int num_led = 0;
int red_val = 0;
int green_val = 0;
int blue_val = 0;
int brightness_val = 0;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  //initialize fastLED
  FastLED.addLeds<WS2812,LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.show();

}

void loop() {

  // Recieve data from Node and write it to a String
   while (Serial.available() > 0) {
    char inChar = (char)Serial.read();
    if(toggleComplete == false && setledComplete == false){
        if(inChar == 'A'){ // end character for toggle LED
         toggleComplete = true;
        }
        if(inChar == 'B'){// end character for dim LED
          setledComplete = true;
        }
    }else{
        if(toggleComplete == true){
          if(inChar != 'X'){
            inputString += inChar;
          }else{
            toggleLED();
            toggleComplete = false;
          }
        }
        if(setledComplete == true){
          if(inChar != 'X'){
            num_led = Serial.parseInt();
            red_val = Serial.parseInt();
            green_val = Serial.parseInt();
            blue_val = Serial.parseInt();
            brightness_val = Serial.parseInt();
          }else{
            leds[num_led] = CRGB(red_val,green_val,blue_val);
            FastLED.setBrightness(brightness_val);
            FastLED.show();
            inputString = "";
            setledComplete = false;
          }
        }
    }
  }



  /*if(!Serial.available() && setledComplete == true){
    // convert String to int
    String num_led = getValue(inputString, '-', 0);
    String red_val = getValue(inputString, '-', 1);
    String green_val = getValue(inputString, '-', 2);
    String blue_val = getValue(inputString, '-', 3);
    String brightness_val = getValue(inputString, '-', 4);
    leds[num_led.toInt()] = CRGB(red_val.toInt(),green_val.toInt(),blue_val.toInt());
    FastLED.setBrightness(brightness_val.toInt());
    inputString = "";
    pwmComplete = false;
  }
  */
}


void toggleLED(){
  int recievedVal = stringToInt();
  if(recievedVal == 0)
  {
    for(i=0;i<24;i++){
      if(i%2 == 0){
        leds[i] = CRGB(0,255,0);
      }else{
        leds[i] = CRGB::Black;
      }
    }
    FastLED.show();
    //digitalWrite(ledPin,recievedVal);
  }
  else if(recievedVal == 1)
  {
    for(i=0;i<24;i++){
      //if(i%2 != 0){
        leds[i] = CRGB(0,255,0);
      /*}else{
        leds[i] = CRGB::Black;
      }*/
    }
  //analogWrite(pwmPin,recievedVal);
  FastLED.show();
  }
}
/*
String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }
  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}*/
int stringToInt()
{
    char charHolder[inputString.length()+1];
    inputString.toCharArray(charHolder,inputString.length()+1);
    inputString = "";
    int _recievedVal = atoi(charHolder);
    return _recievedVal;
}
