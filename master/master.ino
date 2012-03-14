static char currentInput = 0x0;
static bit state = 0; //0: Read Note Next; 1:Read Velocity Next

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
    char velocity = 0x0;
    char control = 0x03;
    char data = 0x0;
    boolean ack = false;
    
    while (1)
    {
	//make sure we don't re-read the same data
        if(!(data == (PINB & B00111111) && control == (PIND >> 6)))
        {
            data = PINB & B00111111;
            control = PIND >> 6;
            
            if(PIND & B10000000) //read note number
            {
                noteNumber = PINB & B00111111;
                    
                if(PIND & B01000000) //note on
                {
                    Serial.print("Received note on message for note ");
                }
                else //note off
                {
                    Serial.print("Received note off message for note ");
                } 
                
                Serial.println(noteNumber);
            }
            else //read velocity
            {
                velocity = PINB & B00111111;
                Serial.print("Received velocity ");
                Serial.print(velocity);
                Serial.print(" for note ");
                Serial.println(noteNumber);
            }
            
            ack = !ack;
            PORTD = (ack) ? PORTD & B11011111 : PORTD | B00100000;
        }
        else
        {
            delayMicroseconds(20);
        }
    }
}
