void setup()
{
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
        //make sure we don't re-read the same data
        if (!(PIND >> 6))
          delayMicroseconds(1);
        if (!(PIND >> 6)) //I don't have anything on the lines. Shift slave select.
        {
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
                while (PIND >> 6 == 0x03);
                data = PINB & B00111111;
                control = PIND >> 6;
                PORTD = PORTD ^ B00100000;
            }

            while (PIND >> 6);
        }
    }
}
