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
boolean pwmComplete = false;
boolean setledComplete = false;
int i;

void setup() {
  // initialize serial:
  Serial.begin(9600);
  //initialize fastLED
  FastLED.addLeds<WS2812,LED_PIN, GRB>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.show();

}

void loop() {

  // Recieve data from Node and write it to a String
   while (Serial.available() && toggleComplete == false && pwmComplete == false) {
    char inChar = (char)Serial.read();
    if(inChar == 'E'){ // end character for toggle LED
     toggleComplete = true;
    }
    if(inChar == 'P'){// end character for dim LED
      pwmComplete = true;
    }
    if(inChar == 'O'){// end character for dim LED
      setledComplete = true;
    }
    else{
      inputString += inChar;
    }
  }
  // Toggle LED 13
  if(!Serial.available() && toggleComplete == true)
  {
    // convert String to int.
    int recievedVal = stringToInt();
    digitalWrite(LED_DEBUG,HIGH);
    delay(500);
    digitalWrite(LED_DEBUG,LOW);
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
    toggleComplete = false;
  }
  // Dim LED 3
  if(!Serial.available() && pwmComplete == true){
    // convert String to int
    int recievedVal = stringToInt();
    FastLED.setBrightness(constrain(recievedVal, MIN_BRIGHTNESS, MAX_BRIGHTNESS));
    pwmComplete = false;
  }
  if(!Serial.available() && setledComplete == true){
    // convert String to int
    var recievedValTab = strtok(inputString, '/');
    inputString = "";
    leds[recievedValTab[0]] = CRGB(recievedValTab[1],recievedValTab[2],recievedValTab[3]);
    FastLED.setBrightness(constrain(recievedValTab[4]);
    pwmComplete = false;
  }
}

int stringToInt()
{
    char charHolder[inputString.length()+1];
    inputString.toCharArray(charHolder,inputString.length()+1);
    inputString = "";
    int _recievedVal = atoi(charHolder);
    return _recievedVal;
}
