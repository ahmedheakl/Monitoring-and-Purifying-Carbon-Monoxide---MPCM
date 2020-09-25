
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>



/* anntenaaaaaaaaaaaaaaaaaaa */
RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00030","00031"}; 
bool role = 0;
int r=1;
int t=1;

/* MQ-7 Carbon Monoxide Sensor Circuit with Arduino */

const int AOUTpin=0;//the AOUT pin of the CO sensor goes into analog pin A0 of the arduino
const int DOUTpin=4;//the DOUT pin of the CO sensor goes into digital pin D8 of the arduino
const int ledPin=13;//the anode of the LED connects to digital pin D13 of the arduino
const int fanPin=10;//the anode of the fan connects to digital pin D13 of the arduino

int limit;
int value;

const int buzzer = 9; //buzzer to arduino pin 9

void setup() {
Serial.begin(115200);
 radio.begin();
radio.openWritingPipe(addresses[1]);
radio.openReadingPipe(1,addresses[0]);
  radio.setPALevel(RF24_PA_MIN);
 
 // Start the radio listening for data



pinMode(DOUTpin, INPUT);//sets the pin as an input to the arduino
pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino
pinMode(buzzer, OUTPUT); // Set buzzer - pin 9 as an output


}

void loop()
{
  

limit= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
value= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
Serial.print("CO value: ");
Serial.println(value);//prints the CO value
Serial.print("Limit: ");
Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
delay(100);

if (limit > 150){
  digitalWrite(ledPin, HIGH);
  }
  if(limit >150 && t==1){
tone(buzzer, 1000); // Send 1KHz sound signal...
  delay(1000);        // ...for 1 sec
      // Stop sound...
  delay(1000);  
  role =0;


if(role==0 && r==1){
  radio.startListening();
  char text[]="";
  if (radio.available()) {
    while (radio.available())  // While there is data ready
    {
  radio.read(&text,sizeof(text));}
  if(text=="buz"){
  t=0;
  noTone(buzzer);
  Serial.println(text);
  role=1;
  r=0;}
  }
  delay(1000);
}
  }
  if(limit <150 && t==0){
  if (role == 1 && r==0)  {
  radio.stopListening();                            
  Serial.println(("Now sending"));
  const char rec[] = "servo";
  radio.write(&rec, sizeof(rec));
  delay(1000);
  role=0;
  r=1;
}
  }
  if (limit > 150) {
digitalWrite (47,1);
digitalWrite (45,0);
  }
  if (limit < 95) {
digitalWrite (47,0);
digitalWrite (45,0);
  }




}

  















  
