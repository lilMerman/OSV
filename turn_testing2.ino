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

int pwm = 200;

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

void turn_BB(int sp, float theta){
  if(theta < 0){
    while(fabs(enes.location.theta - theta) > 0.2){
      turnCW(sp);
      enes.updateLocation();
    }
    off();
  }
  else{
    while(fabs(enes.location.theta - theta) > 0.2){
      turnCCW(sp);
      enes.updateLocation();
    }
    off();
  }
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
    
  }
  else{
    enes.println("update failed");
    Serial.println("update failed");
  }
}
