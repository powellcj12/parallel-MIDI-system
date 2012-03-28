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
#define BUFFERSIZE 20
const int buttonPin = 14; // analog pins start at 14

void setup()
{
    //Serial.begin(115200);
    DDRD = (B11000011 | DDRD) & B11011111;
    DDRB = (B00111111 | DDRB);
    DDRC = (B00000000 | DDRC);
    
    PCICR |= 1 << PCIE1; //enable interups for Port C
  
    //watch for change on all pins of Port C
    PCMSK1 |= 1 << PCINT8;
    PCMSK1 |= 1 << PCINT9;
    PCMSK1 |= 1 << PCINT10;
    PCMSK1 |= 1 << PCINT11;
    PCMSK1 |= 1 << PCINT12;
    PCMSK1 |= 1 << PCINT13;
  
    MCUCR = (1<<ISC01) | (1<<ISC01); //risign edge triggered
    
    randomSeed(analogRead(0));
    interrupts();
}

char ackMask = B00100000;

typedef struct
{
    boolean noteOnMessage;
    char noteNumber;
    char velocity;
}
message;

message buffer[20];
buffer[0] =  (message){true,  0x00,0x01};  //Note 0 ON Velocity 1
buffer[1] =  (message){true,  0x01,0x01};  //Note 1 ON Velocity 2
buffer[2] =  (message){false, 0x00,0x00};  //Note 0 OFF
buffer[3] =  (message){true,  0x02,0x01};  //Note 2 ON Velocity 3
buffer[4] =  (message){false, 0x01,0x00};  //Note 1 OFF
buffer[5] =  (message){true,  0x04,0x01};  //Note 4 ON Velocity 5
buffer[6] =  (message){true,  0x05,0x01};  //Note 5 ON Velocity 6
buffer[7] =  (message){true,  0x06,0x01};  //Note 6 ON Velocity 7
buffer[8] =  (message){false, 0x05,0x00};  //Note 5 OFF
buffer[9] =  (message){false, 0x06,0x00};  //Note 6 OFF
buffer[10] = (message){false, 0x04,0x00};  //Note 4 OFF
buffer[11] = (message){true,  0x03,0x01};  //Note 3 ON Velociy 15
buffer[12] = (message){false, 0x02,0x00};  //Note 2 OFF
buffer[13] = (message){true,  0x00,0x01};  //Note 0 ON Velocity 32
buffer[14] = (message){true,  0x04,0x01};  //Note 1 ON Velocity 32
buffer[15] = (message){false, 0x00,0x00};  //Note 0 OFF
buffer[16] = (message){true,  0x00,0x01};  //Note 0 ON Velocity 33
buffer[17] = (message){false, 0x04,0x00};  //Note 1 OFF
buffer[18] = (message){false, 0x00,0x00};  //Note 0 OFF
buffer[19] = (message){false, 0x03,0x00};  //Note 3 OFF

int bufferIndex =0;
int messagesIndex=0;
int numMessages =0;
char state = 0x00;

void loop()
{
    PORTB = 0x00;
    
    while (1)
    {
        state = PORTD & B00000011;
        if (state == 0x01) //This is slave 1
        {
            //output 1 message for this slave
            //message is either a note on & velocity 
            // or note off message
            
            if (numMessages) //if we have messages in buffer
            {
                if (buffer[bufferIndex].noteOnMessage)
                {
                    ackMask = PIND & B00100000;                  //Get new ACK state
                    PORTB = B00111111 & buffer[bufferIndex].noteNumber;  //Push Note # into B[5:0]
                    PORTD = B11000000 | PORTD;                   //Control Signal = 11
                    while (ackMask == (PIND & B00100000)) 
                    {
                        delayMicroseconds(3);                    //(ACK state != old ACK state)
                    }       //(ACK state != old ACK state)
                    ackMask = PIND & B00100000;                  //Get new ACK state
                    PORTB = B00111111 & messages[bufferIndex].velocity;    //Push velocity into B[5:0]
                    PORTD = (B00111111 & PORTD) | B01000000;                   //Control Signal = 01
                }
                else
                {
                    ackMask = PIND & B00100000;                  //Get new ACK state
                    PORTB = B00111111 & buffer[bufferIndex].noteNumber;  //Push Note # into B[5:0]
                    PORTD = (B10000000 | PORTD) & B10111111;     //Control Signal = 10
                }
                
                while (ackMask == (PIND & B00100000)) //after it adknowledges the message it should also change the state
                {
                    delayMicroseconds(3);                    //(ACK state != old ACK state)
                }
                bufferIndex = (bufferIndex+1)%BUFFERSIZE;     //move buffer
                numMessages--;                                //defrease number of Message in buffer
                delayMicroseconds(6);
            }
            
        }
        else
        {
            PORTB = PORTB & B11000000;
            PORTD = PORTD & B00111111;
            if (random(1000) <= 2)
            {
                numMessages++;
            }
        }
    }
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
