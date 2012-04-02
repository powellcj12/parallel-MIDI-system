//CONTROL SIGNALS
// 11 - Note On
// 10 - Note Off
// 0X - Velocity

void setup()
{
    //Serial.begin(115200);
    DDRD = (B00111111 & DDRD) | B00101100;
    DDRB = (B1100000);
}


char noteNumber = 0x00;
char control = 0x03;
char data = 0x00;
boolean ack = false;
char state = 0x00;

void loop()
{
    delay(1000);
    PORTD = PORTD | (state << 2);
    while (1)
    {
        
        //delay(1000); //wait for signal from new slave
        //Serial.print("state: "); Serial.println(state);
        //Serial.print("control: ");Serial.print((int)control);Serial.print(" data: ");Serial.println((int)data);
	//make sure we don't re-read the same data
        if (!(PIND >> 6))
          delayMicroseconds(2);
        if (!(PIND >> 6)) //I don't have anything on the lines. Shift slave select.
        {
            //Serial.println("Chainging state");
            state = (state+1);
            PORTD = (PORTD & B11110011) | (B00001100 & (state << 2));
        }
        else
        {
            data = PINB & B00111111;          //Get data
            control = PIND >> 6;              //Get control signal
            PORTD = PORTD ^ B00100000;        //SEND ACK
            //Serial.println("Got first data");
            if (control == 0x03) //get velocity now
            {
                //Serial.println("Note On Message");
                while (PIND >> 6 == 0x03)
                {
                    //delayMicroseconds(3);
                }
                data = PINB & B00111111;
                control = PIND >> 6;
                PORTD = PORTD ^ B00100000;
                //Serial.println("Velocity Message");
            }
            else
            {
                //Serial.println("Note Off Message");
            }
            while (PIND >> 6);
                //delayMicroseconds(3);
            //Serial.println("Successfully got data for message");
            
        }
    }
}
