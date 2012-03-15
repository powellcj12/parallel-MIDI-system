//CONTROL SIGNALS
// 11 - Note On
// 10 - Note Off
// 0X - Velocity

void setup()
{
    //Serial.begin(115200);
    DDRD = (B00111111 & DDRD) | B00100000;
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
void loop()
{
    delay(5000);
    PORTD = PORTD ^ B00100000;
    while (1)
    {
        //Serial.print("control: ");Serial.print((int)control);Serial.print(" data: ");Serial.println((int)data);
	//make sure we don't re-read the same data
        if(!(data == (PINB & B00111111) && control == (PIND >> 6)))
        {
            //Serial.print("control: ");Serial.print((int)control);Serial.print(" data: ");Serial.println((int)data);
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
            delayMicroseconds(3);
        }
    }
}
