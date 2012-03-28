//CONTROL SIGNALS
// 11 - Note On
// 10 - Note Off
// 0X - Velocity

void setup()
{
    Serial.begin(115200);
    DDRD = (B00111111 & DDRD) | B00101100;
    DDRB = (B1100000);
    /*
    pinMode(5, OUTPUT);
    for(int i=6; i < 14; i++)
        pinMode(i,INPUT);*/
}


char noteNumber = 0x00;
char control = 0x03;
char data = 0x00;
boolean ack = false;
char state = 0x00;

void loop()
{
    delay(1000);
    PORTD = (PORTD ^ B00100000) | (state << 2);
    while (1)
    {
        delay(1000); //wait for signal from new slave
        Serial.print("state: "); Serial.println(state);
        //Serial.print("control: ");Serial.print((int)control);Serial.print(" data: ");Serial.println((int)data);
	//make sure we don't re-read the same data
        if (!(PIND >> 6)) //I don't have anything on the lines. Shift slave select.
        {
            Serial.println("Chainging state");
            state = (state+1);
            PORTD = (PORTD & B11110011) | (B00001100 & (state << 2));
        }
        else
        {
            data = PINB & B00111111;          //Get data
            control = PIND >> 6;              //Get control signal
            PORTD = PORTD ^ B00100000;        //SEND ACK
            if (control == 0x03) //get velocity now
            {
                while ((data == (PINB & B00111111) && control == (PIND >> 6)))
                {
                    delayMicroseconds(3);
                }
                data = PINB & B00111111;
                control = PIND >> 6;
                PORTD = PORTD ^ B00100000;
                while ((data == (PINB & B00111111) && control == (PIND >> 6))) //delay until next control signal sent
                {
                    delayMicroseconds(3);
                }
            }
            else
            {
                while ((PIND >> 6))
                  delayMicroseconds(3);
            } //we only needed to recieve one message
            
        }
        /*
        if( (PIND >> 6) && !(data == (PINB & B00111111) && control == (PIND >> 6)))
        {
            Serial.println("Recieved Data");
            if (control) //if we recieve a valid control signal
            {
                Serial.print("control: ");Serial.print((int)control);Serial.print(" data: ");Serial.println((int)data);
    	        //Serial.print("input: ");Serial.print((int)(PINB & B00111111));Serial.print( "datain: "); Serial.println((int)(PIND >> 6));
                delayMicroseconds(3);
                data = PINB & B00111111;  //set new data
                control = PIND >> 6;      //set new control signal
                
                if(PIND & B10000000) //read note number (True for values B1XXXXXXX, NoteOn/NoteOff message)
                {
                    noteNumber = data = PINB & B00111111;
                    if(PIND & B01000000) //Branch for values B11XXXXXX  (Note On Messages)
                    { 
                        //Serial.print("Received note on message for note ");
                    }
                    else //Branch for values B10XXXXXX  (Note On Messages)
                    {
                       // Serial.print("Received note off message for note ");
                    }
                    //Serial.println((int)data);
                }
                else //read velocity
                {
                    //Serial.print("Received velocity ");
                    //Serial.print((int)data);
                    //Serial.print(" for note ");
                    //Serial.println((int)noteNumber);
                }
                
                PORTD = PORTD ^ B00100000;
            }
            else
            {
              
                state = (state+1) % 3;
                PORTD = (PORTD ^ B00100000) & (B11110011 | (state << 2));
                delayMicroseconds(1000);
                
            }
        }
        else
        {
            Serial.println("Chainging state");
            state = (state+1);
            PORTD = (PORTD & B11110011) | (B00001100 & (state << 2));
        }*/
    }
}
