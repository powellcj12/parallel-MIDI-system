void setup()
{
    Serial.begin(115200);
    
    DDRB = B11111111;
    DDRD = B00111111;
    
    PORTB = B00000000;
    PORTD = B00000000;
}

void loop()
{
    PORTB = ~PORTB;
    PORTD = ~PORTD;
}
