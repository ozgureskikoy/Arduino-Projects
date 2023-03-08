
int latchpin = 17;// connect to pin 12 on the '595
int clockpin = 16; // connect to pin 11 on the '595
int datapin = 5; // connect to pin 14 on the '595

int segment[10] = {63,6,91,79,102,109,125,7,127,111 }; // for common cathode

void setup()
{
pinMode(latchpin, OUTPUT);
pinMode(clockpin, OUTPUT);
pinMode(datapin, OUTPUT);
}
void loop()
{

digitalWrite(latchpin, LOW);
shiftOut(datapin, clockpin, MSBFIRST, 0); // clears the display
digitalWrite(latchpin, HIGH);
delay(500);

for (int lus=0; lus<10; lus++)

{
digitalWrite(latchpin, LOW);
shiftOut(datapin, clockpin, MSBFIRST, segment[lus]+128);
digitalWrite(latchpin, HIGH);
delay(500);
}

digitalWrite(latchpin, LOW);
shiftOut(datapin, clockpin, MSBFIRST, 0); // clear the display
digitalWrite(latchpin, HIGH);
delay(500);

digitalWrite(latchpin, LOW);
shiftOut(datapin, clockpin, MSBFIRST, 128); // light the decimal point
digitalWrite(latchpin, HIGH);
delay(500);

}
