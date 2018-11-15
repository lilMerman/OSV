#include "Enes100.h"

Enes100 enes("TGTWAPTT", BLACK_BOX, 8, 12, 11);    //enes("name", MISSION, image_num, RX, TX)
float x; 
float y;
float theta;

// motor one
int enA = 10; /* PMW control speed motor 1*/
int in1 = 9; /*Output HIGH or LOW for direction */
int in2 = 8; /*Output LOW or HIGH for direction */
 
 
// motor two
int enB = 5; /* PMW control speed motor 1*/
int in3 = 7; /*Output HIGH or LOW for direction */
int in4 = 6; /*Output LOW or HIGH for direction */

int pwm_turn = 200;
int pwm_move = 230;

void turnCCW(int sp){
  // motor 1; turn right side forwards
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  analogWrite(enA, sp);
 
  // motor 2; turn left side backwards
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, sp);
}

void turnCW(int sp){
  // motor 1; turn left side backwards
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, sp);
 
  // motor 2; turn right side forwards
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW);
  analogWrite(enB, sp);
}

void off(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  //Serial.println("motor 1 off");
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  //Serial.println("motor 2 off");
}

bool is_up(float theta){
  if(theta >= 0){
    return true;
  }
  return false;
}

bool is_down(float theta){
  if(theta < 0){
    return true;
  }
  return false;
}

bool is_right(float theta){
  if(abs(theta) <= PI/2){
    return true;
  }
  return false;
}

bool is_left(float theta){
  if(abs(theta) > PI/2){
    return true;
  }
  return false;
}

void turn_y_1(int sp){
  if(enes.location.y > 1){                              //above y = 1, turn to -PI/2
      enes.println("Above y = 1");
      if(is_left(enes.location.theta)){
        enes.println("\tFacing left. Turning CCW");
        while(enes.location.theta > 0 || enes.location.theta < (-1*PI/2)-0.2){
          turnCCW(sp);
          enes.updateLocation();
        }
        off();
      }
      else{
        enes.println("\tFacing right. Turning CW");
        while(enes.location.theta > 0 || enes.location.theta > (-1*PI/2)+0.2){
          turnCW(sp);
          enes.updateLocation();
        }
        off();
        
      }
    }
    else{                                                 //below y = 1, turn to PI/2
      enes.println("Below y = 1");
      if(is_left(enes.location.theta)){
        enes.println("\tFacing left. Turning CW");
        while(enes.location.theta < 0 || enes.location.theta > (PI/2)+0.2){
          turnCW(sp);
          enes.updateLocation();
        }
        off();
      }
      else{
        enes.println("\tFacing right. Turning CCW");
        while(enes.location.theta < (PI/2)-0.2){
          turnCCW(sp);
          enes.updateLocation();
        }
        off();
      }
    }
}

void forward(int sp){
  // motor 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, sp);
 
  // motor 2
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, sp-15);
}

void move_y_1(int sp){
  while(fabs(enes.location.y-1)>0.05){
    forward(sp);
    enes.updateLocation();
  }
  off();
}

void turn_0(int sp){
  if(is_up(enes.location.theta)){
    while(enes.location.theta > 0.2){
      turnCW(sp);
      enes.updateLocation();
    }
    off();
  }
  else{
    while(enes.location.theta > 0.2){
      turnCCW(sp);
      enes.updateLocation();
    }
    off();
  }
}

void move_rocks(int sp){
  while(enes.location.x < 1.2){
    forward(sp);
    enes.updateLocation();
  }
  off();
}

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);

}

void loop() {
  delay(2000);  
  if(enes.updateLocation()){
    enes.println("location updated");
    turn_y_1(pwm_turn);
    delay(1000);
    move_y_1(pwm_move);
    delay(1000);
    turn_0(pwm_turn);
    delay(1000);
    move_rocks(pwm_move);
    delay(6000);
  }
  else{
    enes.println("update failed");
    Serial.println("update failed");
  }
}
