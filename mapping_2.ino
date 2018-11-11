#include <Servo.h>

int trigPin1 = 9;
int echoPin1 = 10;

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

int object [10][2];
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
    digitalWrite(trigPin1, LOW);
    delay(2);
  
    //Generate ultrasound, 10 us
    digitalWrite(trigPin1, HIGH);
    delay(10);
    digitalWrite(trigPin1, LOW);
  
    //Reading from echoPin
    time = pulseIn(echoPin1, HIGH);
  
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
      //grid coordinates
      int grid_y = y/3;                             //y grid square
      int grid_x = x/3;                             //x grid square
      Serial.print("Object @ ");
      Serial.print("(");
      Serial.print(x);
      Serial.print(", ");
      Serial.print(y);
      Serial.println(")");
      Serial.print("Grid square: ");
      Serial.print("(");
      Serial.print(grid_x);
      Serial.print(", ");
      Serial.print(grid_y);
      Serial.println(")");
      if(object[counter-1][1] == grid_y){           //if previous grid square (y val) already read, update x val
        object[counter-1][0] = grid_x;
      }
      else{                                         //new grid square
        object[counter][0] = grid_x;
        object[counter][1] = grid_y;
        counter++;                                  //count number of obstacle readings
        avgDist += dist;                            //add to running sum of dist
      }
      
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

  int objectFinal[counter - (2*extra)][2];          //new object "map"; sized to get rid of excess detected space
  for(int i = 0; i<(counter - (2*extra)); i++){
    for(int j = 0; j<2; j++){
      objectFinal[i][j] = object[i+extra+1][j];     //store middle-ish (shifted towards higher y by 1) values
    }
  }

  Serial.println("objectFinal");
  for(int i = 0; i<counter - (2*extra); i++){
    for(int j = 0; j<2; j++){
      Serial.print(objectFinal[i][j]);
      Serial.print(" ");
    }
    Serial.println();
  }
  
  

  //Serial.println("delay 1s");
  delay(1000);
  //Serial.println("loop 2");
  //reset to 0
  for(int a = 180; a>0; a--){
    servo.write(a);
    delay(15);
  }
  

  //Serial.println("write");
  servo.write(0);
  //Serial.println("3s");
  delay(3000);
  Serial.println();
  Serial.println("New Scan:");
}
