// constants won't change. They're used here to 
// set pin numbers:
const int buttonPin = 14;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
volatile char keyPress = 0x0;

void setup() {
  PCICR |= 1 << PCIE1; //enable interups for Port C
  
  //watch for change on all pins of Port C
  PCMSK1 |= 1 << PCINT8;
  PCMSK1 |= 1 << PCINT9;
  PCMSK1 |= 1 << PCINT10;
  PCMSK1 |= 1 << PCINT11;
  PCMSK1 |= 1 << PCINT12;
  PCMSK1 |= 1 << PCINT13;
  
  MCUCR = (1<<ISC01) | (1<<ISC01); //risign edge triggered
  
  DDRC = B00000000; //all pins are input (only use [5:0])
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  Serial.begin(115200);
  interrupts();
}

void loop(){
  // read the state of the pushbutton value:
  //buttonState = digitalRead(buttonPin);
  /*
  buttonState = keyPress & B00100000;
  
  for(int i = 0; i < 6; i++)
  {
      //int val = keyPress >> i;
      char pinTest = 1 << i;
      
      if(keyPress & pinTest)
      {
        Serial.print("Button ");
        Serial.println(i);
        //Serial.print(": ");
        //Serial.println(val);
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
  */
}

ISR(PCINT1_vect) {
  if(PINC & B00100000)
      Serial.println("Pin 5");
  if(PINC & B00010000)
      Serial.println("Pin 4");
  if(PINC & B00001000)
      Serial.println("Pin 3");
  if(PINC & B00000100)
      Serial.println("Pin 2");
  if(PINC & B00000010)
      Serial.println("Pin 1");
  if(PINC & B00000001)
      Serial.println("Pin 0");
    
  //keyPress = PINC;
}
