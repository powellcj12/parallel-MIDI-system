//CONTROL SIGNALS
// 11 - Note On
// 10 - Note Off
// 01 - Velocity
// 00 - No Messages to Send

//PIN Setup
//B[5:0] Data
//C[5:0] Buttons
//D[7:6] Control
//D[5] ACK
//D[1:0] Select Lines

typedef struct
{
    boolean noteOnMessage;
    char noteNumber;
    char velocity;
}
message;

const int BUFFERSIZE = 20;
volatile int dataStorageIndex = 0; // index to store data in buffer
int bufferAccessIndex = 0; // index to access data from buffer
message messages[BUFFERSIZE];
char ackMask = B00100000;

void setup()
{
    DDRD = (B11000000 | DDRD) & B11010011;
    DDRB = (B11111111 | DDRB);
    DDRC = (B00000000 | DDRC);
    
    PCICR |= 1 << PCIE1; //enable interups for Port C
  
    //watch for change on all pins of Port C
    PCMSK1 |= 1 << PCINT8;
    PCMSK1 |= 1 << PCINT9;
    PCMSK1 |= 1 << PCINT10;
    PCMSK1 |= 1 << PCINT11;
    PCMSK1 |= 1 << PCINT12;
    PCMSK1 |= 1 << PCINT13;
  
    MCUCR = (1<<ISC01) | (1<<ISC01); //rising edge triggered
    
    randomSeed(analogRead(0));
    interrupts();
}

void loop()
{ 
    PORTB = 0x00;
    
    while (1)
    {
        while(bufferAccessIndex < dataStorageIndex)
        {
            while (((PIND & B00001100) >> 2) != 0x01); //if it is not slave number 1 select, wait
              
            if (messages[bufferAccessIndex % BUFFERSIZE].noteOnMessage)
            {
                PORTB = B00111111 & messages[bufferAccessIndex % BUFFERSIZE].noteNumber;  //Push Note # into B[5:0]
                PORTD = B11000000 | PORTD;  //Control Signal = 11
                ackMask = PIND & B00100000; //Get new ACK state
                while (ackMask == (PIND & B00100000));
                //(ACK state != old ACK state)
                ackMask = PIND & B00100000; //Get new ACK state
                PORTB = B00111111 & messages[bufferAccessIndex % BUFFERSIZE].velocity;    //Push velocity into B[5:0]
                PORTD = (B00111111 & PORTD) | B01000000; //Control Signal = 01
            }
            else
            {
                ackMask = PIND & B00100000; //Get new ACK state
                PORTB = B00111111 & messages[bufferAccessIndex % BUFFERSIZE].noteNumber;  //Push Note # into B[5:0]
                PORTD = (B10000000 | PORTD) & B10111111; //Control Signal = 10
                
                while (ackMask == (PIND & B00100000));
                //(ACK state != old ACK state)
            }

            bufferAccessIndex++;
            
            PORTB = 0x00;
            PORTD = B00111111 & PORTD; //load control signal 00
            
            while (((PIND & B00001100) >> 2) == 0x01);
        }
    }
}

ISR(PCINT1_vect) {
    char note;

    if(PINC & B00100000)
        note = 0x05; //pin 5
    else if(PINC & B00010000)
        note = 0x04; //pin 4
    else if(PINC & B00001000)
        note = 0x03; //pin 3
    else if(PINC & B00000100)
        note = 0x02; //pin 2
    else if(PINC & B00000010)
        note = 0x01; //pin 1
    else if(PINC & B00000001)
        note = 0x00; //pin 0  

    messages[dataStorageIndex % BUFFERSIZE] = (message){true, note, random(128)};
    dataStorageIndex++;
}
