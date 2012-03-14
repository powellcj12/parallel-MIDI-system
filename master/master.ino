//CONTROL SIGNALS
// 11 - Note On
// 10 - Note Off
// 0X - Velocity

void setup()
{
    Serial.begin(115200);
    pinMode(5, OUTPUT);
    for(int i=6; i < 14; i++)
        pinMode(i,INPUT);
}

void loop()
{
    char noteNumber = 0x0;
    char control = 0x03;
    char data = 0x0;
    boolean ack = false;
    
    while (1)
    {
	//make sure we don't re-read the same data
        if(!(data == (PINB & B00111111) && control == (PIND >> 6)))
        {
            delayMicroseconds(3);
            data = PINB & B00111111;  //set new data
            control = PIND >> 6;      //set new control signal
            
            if(PIND & B10000000) //read note number (True for values B1XXXXXXX, NoteOn/NoteOff message)
            {
                noteNumber = data = PINB & B00111111;
                if(PIND & B01000000) //Branch for values B11XXXXXX  (Note On Messages)
                { 
                    Serial.print("Received note on message for note ");
                }
                else //Branch for values B10XXXXXX  (Note On Messages)
                {
                    Serial.print("Received note off message for note ");
                }
                Serial.println(data);
            }
            else //read velocity
            {
                Serial.print("Received velocity ");
                Serial.print(data);
                Serial.print(" for note ");
                Serial.println(noteNumber);
            }
            
            PORTD = PORTD ^ B00100000;
        }
        else
        {
            delayMicroseconds(3);
        }
    }
}
