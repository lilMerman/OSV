#include <Enes100.h>
#include "DFRTank.h"

//tank
DFRTank tank;
//int distPin = A1;
int raw;

//ultrasonic
int trigPin = 7;
int echoPin = 10;
long t;
int dist;

//vision system
Enes100 enes("TGTWAPTT", BLACK_BOX, 12, 8, 9);    //enes("name", MISSION, image_num, RX, TX)
float x; 
float y;
float theta;

void turnCCW(int sp){
  tank.setRightMotorPWM(sp);
  tank.setLeftMotorPWM(-1*sp);
}

void turnCW(int sp){
  tank.setRightMotorPWM(-1*sp);
  tank.setLeftMotorPWM(sp);
}

void turnR(int sp){                   //face right; theta = 0
  if(enes.location.theta < (-1*0.3)){                 //facing down
    while(enes.location.theta < (-1*0.3)){
      turnCCW(sp);
      enes.print("theta: ");
          enes.println(enes.location.theta);
          enes.updateLocation();
      enes.updateLocation();
    }
    off();
  }
  else{                                         //facing up
    while(enes.location.theta > 0.3){
      turnCW(sp);
      enes.print("theta: ");
          enes.println(enes.location.theta);
          enes.updateLocation();
      enes.updateLocation();
    }
    off();
  }
}

void turnL(int sp){                   //face left; theta = PI
  if(enes.location.theta > PI){                 //facing up
    while(enes.location.theta < PI-0.2){
      turnCCW(sp);
      enes.updateLocation();
    }
    off();
  }
  else{                                         //facing down
    while(enes.location.theta < PI+0.2){
      turnCCW(sp);
      enes.updateLocation();
    }
    off();
  }
}

void turnU(int sp){                   //face up; theta = PI/2
  if(abs(enes.location.theta - (PI/2)) > 0.2){                                  //if theta significantly off from PI/2
      if(enes.location.theta < PI/2 && enes.location.theta > -1*PI/2){           //facing right
        //turn left
        while(enes.location.theta < PI/2-0.3){
          turnCCW(sp);
          enes.print("theta: ");
          enes.println(enes.location.theta);
          enes.updateLocation();
        }
        off();
      }
      else {        //facing left
        //turn right
        while(enes.location.theta < 0 || enes.location.theta > PI/2+0.3){
          turnCW(sp);
          enes.print("theta: ");
          enes.println(enes.location.theta);
          enes.updateLocation();
        }
        off();
      }
    }
}

void turnD(int sp){
  if(abs(enes.location.theta - (-1*PI/2)) > 0.1){                                  //if theta significantly off from -PI/2
      if(enes.location.theta < PI/2 && enes.location.theta > -1*PI/2){           //facing right
        //turn right
        while((enes.location.theta > 0 && enes.location.theta <= PI/2) || enes.location.theta > -1*PI/2+0.2){
          turnCW(sp);
          enes.updateLocation();
        }
        off();
      }
      else if(enes.location.theta > 0 || enes.location.theta < -1*PI/2-0.2){        //facing left
        //turn left
        while(enes.location.theta < -1*PI/2-0.2){
          turnCCW(sp);
          enes.updateLocation();
        }
        off();
      }
    }
}

void forward(int sp){
  tank.setRightMotorPWM(sp);
  tank.setLeftMotorPWM(sp);
}

/*
void acceleration(int init, int fin){
  // this function will accelerate motors at specific rate
  // motor 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  // motor 2
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  // Accelerate
  Serial.println("accelerate");
  for (int i = init; i < fin; i ++) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(accelerationTime / (fin-init));
  }
}

void deceleration(int init, int fin){
  // motor 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  // motor 2
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  // Decelerate
  Serial.println("decelerate");
  for (int i = init; i >= fin; i --) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(accelerationTime / (init-fin));
  }
 
}
*/

void off(){
  tank.setRightMotorPWM(0);
  tank.setLeftMotorPWM(0);
}

void ultrasonic(int sp, int dir){     //dir = original direction; 0 = right (theta = 0), 1 = up (theta = PI/2), 2 = left (theta = PI), 3 = down (theta = 3PI/2)
  //Clear ultrasonic
  digitalWrite(trigPin, LOW);
  delay(2);
  
  //Generate ultrasound, 10 us
  digitalWrite(trigPin, HIGH);
  delay(10);
  digitalWrite(trigPin, LOW);
  
  //Reading from echoPin
  t = pulseIn(echoPin, HIGH);
  Serial.println(t);
  
  //distance from time
  dist = t * 0.034 / 2;

  if(dist < 35){      //obstacle detected
    if(enes.location.y < 0.6 || (enes.location.y >= 1.0 && enes.location.y <= 1.4)){                  //within 60 cm of lower wall or b/w y=1 & y=1.4
      turnU(80);
      int tempY = enes.location.y + 0.3;
      while((tempY - enes.location.y) > 0.05){
        forward(80);
        enes.updateLocation();
      }
    }
    else{                                                                                             //within 60 cm of upper wall or b/w y=1 & y=0.6
      turnD(80);
      int tempY = enes.location.y - 0.3;
      while((enes.location.y = tempY) > 0.05){
        forward(80);
        enes.updateLocation();
      }
    }
    //turn to desired orientation after avoiding obstacle
    if(dir==0){
      turnR(180);
    }else if(dir==1){
      turnU(180);
    }else if(dir==2){
      turnL(180);
    }else{
      turnD(180);
    }
  }
}

void setup (){
  Serial.begin(9600);
  tank.init();
  enes.println("Connected");
  delay(6000);
      //turn to face y = 0
  enes.updateLocation();
  enes.println("turn towards y = 1");
  enes.print("(");
      enes.print(enes.location.x);
      enes.print(" ,");
      enes.print(enes.location.y);
      enes.println(")");
      enes.print("theta: ");
      enes.println(enes.location.theta);
  if(enes.location.y - 1.0 > 0){                //above y = 1; turn to theta = 3PI/2
    enes.println("above y = 1; turning to -PI/2");
    turnD(180);
    delay(1000);
  }
  else {                                        //below y = 1 (or at y = 1); turn to theta = PI/2
    enes.println("below y = 1; turning to PI/2");
    turnU(180);
    delay(1000);
  }
  enes.updateLocation();
  enes.print("(");
      enes.print(enes.location.x);
      enes.print(" ,");
      enes.print(enes.location.y);
      enes.println(")");
      enes.print("theta: ");
      enes.println(enes.location.theta);
  
  //move to y = 1.0
  enes.println("move to y = 1");
  while(abs(enes.location.y-1) > 0.05){
    forward(180);
    enes.updateLocation();
  }
  off();

  enes.updateLocation();
  
  //turn to theta = 0
  enes.println("turn to theta = 0");
  turnR(180);

  enes.println("moving to x = 2.25");
  while(enes.location.x < 2.1){
    forward(180);
    //ultrasonic(180, 0);
    enes.updateLocation();
    
  }
  off();
  if(abs(enes.location.y - 1.0) > 0.1){
    enes.println("turn towards y = 1");
    if(enes.location.y - 1.0 > 0){
      enes.println("above y = 1");
      turnD(180);
    }
    else{
      enes.println("below y = 1");
      turnU(180);
    }
    off();
    enes.println("move to y = 1");
    while(abs(enes.location.y - 1.0) > 0.1){
      forward(180);
    }
  }
  off();
  enes.println("Finished");
}

void loop() {
  // put your main code here, to run repeatedly:
 
}
