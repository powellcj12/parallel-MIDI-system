const int buttonPin = 14;     // analog pins start at 14
const int ledPin =  13;      // the number of the LED pin

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
  Serial.begin(115200);
  interrupts();
}

void loop(){
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
}
