//CONTROL SIGNALS
// 11 - Note On
// 10 - Note Off
// 01 - Velocity
// 00 - No Messages to Send

void setup()
{
    //Serial.begin(115200);
    DDRD = (B11000000 | DDRD) & B11010011;
    DDRB = (B00111111 | DDRB);
    
}

char ackMask = B00100000;
void loop()
{
    typedef struct
    {
        boolean noteOnMessage;
        char noteNumber;
        char velocity;
    }
    message;
    
    message messages[20];
    messages[0] =  (message){true,  0x00,0x01};  //Note 0 ON Velocity 1
    messages[1] =  (message){true,  0x01,0x01};  //Note 1 ON Velocity 2
    messages[2] =  (message){false, 0x00,0x00};  //Note 0 OFF
    messages[3] =  (message){true,  0x02,0x01};  //Note 2 ON Velocity 3
    messages[4] =  (message){false, 0x01,0x00};  //Note 1 OFF
    messages[5] =  (message){true,  0x04,0x01};  //Note 4 ON Velocity 5
    messages[6] =  (message){true,  0x05,0x01};  //Note 5 ON Velocity 6
    messages[7] =  (message){true,  0x06,0x01};  //Note 6 ON Velocity 7
    messages[8] =  (message){false, 0x05,0x00};  //Note 5 OFF
    messages[9] =  (message){false, 0x06,0x00};  //Note 6 OFF
    messages[10] = (message){false, 0x04,0x00};  //Note 4 OFF
    messages[11] = (message){true,  0x03,0x01};  //Note 3 ON Velociy 15
    messages[12] = (message){false, 0x02,0x00};  //Note 2 OFF
    messages[13] = (message){true,  0x00,0x01};  //Note 0 ON Velocity 32
    messages[14] = (message){true,  0x04,0x01};  //Note 1 ON Velocity 32
    messages[15] = (message){false, 0x00,0x00};  //Note 0 OFF
    messages[16] = (message){true,  0x00,0x01};  //Note 0 ON Velocity 33
    messages[17] = (message){false, 0x04,0x00};  //Note 1 OFF
    messages[18] = (message){false, 0x00,0x00};  //Note 0 OFF
    messages[19] = (message){false, 0x03,0x00};  //Note 3 OFF
     
    PORTB = 0x00;
    
    while (1)
    {
        for(int i=0; i < 20; i++)
        {
            //Serial.print("AckMask: ");
            //Serial.println(ackMask & B00100000);
            //Serial.print("PIND: ");
            //Serial.println(PIND&B00100000);
            while (((PIND & B00001100) >> 2) != 0x01) //if it is not slave number 1 select, wait
            {
                //Serial.println("A");
                //delayMicroseconds(3);
            }
              
            if (messages[i].noteOnMessage)
            {
                PORTB = B00111111 & messages[i].noteNumber;  //Push Note # into B[5:0]
                PORTD = B11000000 | PORTD;                   //Control Signal = 11
                //Serial.print("Sent note for message on ");
                //Serial.println(i);
                ackMask = PIND & B00100000;                  //Get new ACK state
                while (ackMask == (PIND & B00100000)) 
                {
                    //Serial.println("B");
                    //delayMicroseconds(3);                    //(ACK state != old ACK state)
                }       //(ACK state != old ACK state)
                ackMask = PIND & B00100000;                  //Get new ACK state
                PORTB = B00111111 & messages[i].velocity;    //Push velocity into B[5:0]
                PORTD = (B00111111 & PORTD) | B01000000;                   //Control Signal = 01
                //Serial.print("Sent velocity for message ");
                //Serial.println(i);
            }
            else
            {
                ackMask = PIND & B00100000;                  //Get new ACK state
                PORTB = B00111111 & messages[i].noteNumber;  //Push Note # into B[5:0]
                PORTD = (B10000000 | PORTD) & B10111111;     //Control Signal = 10
                //Serial.print("Sent note for message off ");
                //Serial.println(i);
                
                while (ackMask == (PIND & B00100000)) 
                {
                    //Serial.println("C");
                    //delayMicroseconds(3);                    //(ACK state != old ACK state)
                } 
            }
            
            PORTB = 0x00;
            PORTD = B00111111 & PORTD; //load control signal 00
            
            while (((PIND & B00001100) >> 2) == 0x01)
            {
              //Serial.println("D");
              //delayMicroseconds(3);
            }
        }
    }
}


