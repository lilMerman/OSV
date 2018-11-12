#include <Servo.h>

int trigPin = 9;
int echoPin = 10;

long time;
int dist;
int prevDist = 0;

int servoPin = 7;
Servo servo;
int angle = 0;

int xo = 0;
int yo = 18;
int x;
int y;

float object [10][2];
int counter;

int avgDist;
int extra;

void setup() {
  servo.attach(servoPin);
  servo.write(0);
  
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(9600);
}



void loop() {
  //Serial.println("for loop");
  
  
  //Serial.println("loop 1");

  counter = 0;
  avgDist = 0;
  extra = 0;
  
  for(int a = 0; a<180; a+=3){              //Sweep 180 degrees, step = 3
    float angle = (a*PI/180)-(PI/2);        //Convert to radians, w/ 0 rad at y = 0
    //Serial.print("angle: ");
    //Serial.println(angle);
    servo.write(a);
    delay(15);

//ultrasonic 1
    //clear trig
    digitalWrite(trigPin, LOW);
    delay(2);
  
    //Generate ultrasound, 10 us
    digitalWrite(trigPin, HIGH);
    delay(10);
    digitalWrite(trigPin, LOW);
  
    //Reading from echoPin
    time = pulseIn(echoPin, HIGH);
  
    //distance from time
    dist = time * 0.034 / 2;
    //avgDist += dist;
    
    x = dist*cos(angle) + xo;
    y = dist*sin(angle) + yo;
    if(y < 0){
      y = 0;
    }

  //obstacle detected
    if(abs(prevDist - dist) < 5 && dist < 40){      //check for 5cm difference in readings, within 40 cm
      
      object[counter][0] = dist;
      object[counter][1] = angle;
      counter++;
      avgDist += dist;
    }
    
    prevDist = dist;
    //delay(5000);
    
  }           
  //end servo sweep & ultrasonic scan

  
  avgDist /= counter;                               //avg distance of obstacle
  Serial.print("counter = ");
  Serial.println(counter);
  Serial.print("Avg dist = ");
  Serial.println(avgDist);
  extra = (avgDist * tan(PI/12) / 3);               //excess sensed space
  Serial.print("extra = ");
  Serial.println(extra);

  float objectTrimmed[counter - (2*extra)][2];          //new object "map"; sized to get rid of excess detected space
  for(int i = 0; i<(counter - (2*extra)); i++){
    for(int j = 0; j<2; j++){
      objectTrimmed[i][j] = object[i+extra+1][j];     //store middle-ish (shifted towards higher y by 1) values
    }
  }

  float objectFinal[2][2];
  objectFinal[0][0] = objectTrimmed[0][0];
  objectFinal[0][1] = objectTrimmed[0][1];
  objectFinal[1][0] = objectTrimmed[counter - (2*extra)][0];
  objectFinal[1][1] = objectTrimmed[counter - (2*extra)][1];
  

}
