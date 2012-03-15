// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 14;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  DDRC = B00000000; //all pins are input (only use [5:0])
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  for(int i = 0; i < 6; i++)
  {
      if(digitalRead(buttonPin+i))
      {
      Serial.print("Button ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(digitalRead(buttonPin+i));
      }
  }

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED on:    
    digitalWrite(ledPin, HIGH); 
  } 
  else {
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  }
}
