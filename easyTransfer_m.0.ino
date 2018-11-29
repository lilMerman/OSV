#include <Wire.h>
#include <EasyTransferI2C.h>

//create object
EasyTransferI2C ET; 

struct RECEIVE_DATA_STRUCTURE{
  //put your variable definitions here for the data you want to receive
  //THIS MUST BE EXACTLY THE SAME ON THE OTHER ARDUINO
  int distance;  
  int distance2;
};

//give a name to the group of data
RECEIVE_DATA_STRUCTURE mydata;

//define slave i2c address
#define I2C_SLAVE_ADDRESS 9

int distance;  
int distance2;

int leftLED = 3;
int rightLED = 4;

void setup(){
  Wire.begin(I2C_SLAVE_ADDRESS);
  //start the library, pass in the data details and the name of the serial port. Can be Serial, Serial1, Serial2, etc. 
  ET.begin(details(mydata), &Wire);
  //define handler function on receiving data
  Wire.onReceive(receive);

  pinMode(leftLED, OUTPUT);
  pinMode(rightLED, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  //check and see if a data packet has come in. 
  if(ET.receiveData()){
    //this is how you access the variables. [name of the group].[variable name]

    distance = mydata.distance;
    distance2 = mydata.distance;

    if (distance < 10 && distance2 < 10){       //both sides sensed
      //Serial.println("both sensed");
      digitalWrite(leftLED, HIGH);
      digitalWrite(rightLED, HIGH);
    }
    else if (distance < 10 && distance2 > 10){       //left side sensed
      //Serial.println("1st sensed 2nd not");
      digitalWrite(leftLED, HIGH);
    }
    else if (distance > 10 && distance2 < 10){       //right side sensed
      //Serial.println("2nd sensed 1st not");
      digitalWrite(rightLED, HIGH);
    }
  }
  delay(2000);
  digitalWrite(leftLED, LOW);
  digitalWrite(rightLED, LOW);
}

void receive(int numBytes) {}
