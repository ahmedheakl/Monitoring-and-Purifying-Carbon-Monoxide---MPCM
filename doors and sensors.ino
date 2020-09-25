


#include <Servo.h> 
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>
/* anntenaaaaaaaaaaaaaaaaaaa */
RF24 radio(7, 8); // CE, CSN
byte addresses[][6] = {"00030","00031"};
bool role = 0;
int r=1;
int t=1;

Servo servo_test;        //initialize a servo object for the connected servo  
                
int angle = 0; 





const int AOUTpin=0;//the AOUT pin of the nh4 sensor goes into analog pin A0 of the arduino
const int DOUTpin=4;//the DOUT pin of the nh4 sensor goes into digital pin D8 of the arduino
const int ledPin=13;//the anode of the LED connects to digital pin D13 of the arduino

int limit;
int value;


void setup() {
 servo_test.attach(9);      // attach the signal pin of servo to pin9 of arduino
servo_test.write(65);
Serial.begin(115200);
 radio.begin();
 radio.openWritingPipe(addresses[0]);
 radio.openReadingPipe(1,addresses[1]);
  radio.setPALevel(RF24_PA_MIN);
  pinMode(DOUTpin, OUTPUT);//sets the pin as an input to the arduino
pinMode(ledPin, OUTPUT);//sets the pin as an output of the arduino


}
void loop()
{
  
limit= analogRead(AOUTpin);//reads the analaog value from the CO sensor's AOUT pin
value= digitalRead(DOUTpin);//reads the digital value from the CO sensor's DOUT pin
Serial.print("ch4 value: ");
Serial.println(value);//prints the CO value
Serial.print("Limit: ");
Serial.print(limit);//prints the limit reached as either LOW or HIGH (above or underneath)
delay(100);

if (limit >145){
digitalWrite(ledPin, HIGH);//if limit has been reached, LED turns on as status indicator
}
if (limit >145 &&t==1){
for(angle = 65; angle > 0; angle -= 1)    // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);                 //command to rotate the servo to the specified angle
    delay(15);                       
   
 }
  delay(5000);
  
  for(angle =0; angle<=65; angle +=5)     // command to move from 180 degrees to 0 degrees 
  {                                
    servo_test.write(angle);              //command to rotate the servo to the specified angle
    delay(5);  
    role=1;
    r=0;                     
  } 
}
    delay(1000);

if (role == 1 && r==0)  {
  radio.stopListening();                            
  Serial.println(("Now sending"));
  const char text[] = "buz";
  radio.write(&text, sizeof(text));
  delay(1000);
  role=0;
  r=1;
  t=0;
}




if (limit <145 &&t==0){
radio.startListening();
  char rec[]="";
  if (radio.available()) {
    while (radio.available())  // While there is data ready
    {
  radio.read(&rec,sizeof(rec));}
  if(rec=="servo"){
  Serial.println(rec);
  role=1;
  r=0;
  for(angle = 65; angle > 0; angle -= 1)    // command to move from 0 degrees to 180 degrees 
  {                                  
    servo_test.write(angle);                 //command to rotate the servo to the specified angle
    delay(15);    }                   
  } 
  }
  delay(1000);
}

else{
digitalWrite(ledPin, LOW);//if threshold not reached, LED remains off
}
  
}
