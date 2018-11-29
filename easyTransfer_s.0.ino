#include <Wire.h>
#include <EasyTransferI2C.h>

//create object
EasyTransferI2C ET; 

struct SEND_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to send
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int distance;  
  int distance2;
};

//give a name to the group of data
SEND_DATA_STRUCTURE mydata;

//define slave i2c address
#define I2C_SLAVE_ADDRESS 9





const int trigPin = 9;
const int echoPin = 10;
const int trigPin2 = 11;
const int echoPin2 = 12;

long duration;
long duration2;
int distance;  
int distance2;





void setup(){
  Wire.begin();
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc.
  ET.begin(details(mydata), &Wire);


  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT); 
  pinMode(3,OUTPUT);
  Serial.begin(9600); 
}









void loop(){

  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin2, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  digitalWrite(trigPin2, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  digitalWrite(trigPin2, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  duration2 = pulseIn(echoPin2, HIGH);
  distance = duration*0.034/2;
  distance2 = duration2*0.034/2;




  if (distance < 10 || distance2 <  10){
      //send the data
      mydata.distance= distance;
      mydata.distance2= distance2;
      ET.sendData(I2C_SLAVE_ADDRESS);
      delay(1000);
  }
}
