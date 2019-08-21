#define FASTLED_ALLOW_INTERRUPTS 0
#include <FastLED.h>

#define LED_PIN   9
#define LED_DEBUG 13
#define NUM_LEDS  24

CRGB leds[NUM_LEDS];


// LED read vars
String inputString = "";         // a string to hold incoming data
boolean toggleComplete = false;  // whether the string is complete
boolean pwmComplete = false;  
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
    leds[recievedVal] = CRGB(0,255,0);
    FastLED.show();
    leds[recievedVal] = CRGB::Black;
    pwmComplete = false;
  }
  delay(50);
  Serial.print("B"); // begin character 
  Serial.print(50);  
  Serial.print("E"); // end character
  
}

int stringToInt()
{
    char charHolder[inputString.length()+1];
    inputString.toCharArray(charHolder,inputString.length()+1);
    inputString = "";
    int _recievedVal = atoi(charHolder);
    return _recievedVal;
}
  
