#include "Enes100.h"

Enes100 enes("TGTWAPTT", BLACK_BOX, 14, 5, 1);    //enes("name", MISSION, image_num, connect to pin 5, connect to pin 4)

int trigL = 2;
int echoL = 4;
int trigR = 7;
int echoR = 8;

int distL;
int distR;

// motor one
int enA = 11; /* PMW control speed motor 1*/
//in1 = A0; /*Output HIGH or LOW for direction */
//in2 = A1; /*Output LOW or HIGH for direction */
 
 
// motor two
int enB = 10; /* PMW control speed motor 1*/
//in3 = A2; /*Output HIGH or LOW for direction */
//in4 = A3; /*Output LOW or HIGH for direction */

int pwm_turn = 150;
int pwm_move = 180;



void turnCCW(int sp){                                   //turn counter clockwise
  // motor 1; turn right side forwards
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  analogWrite(enA, sp);
 
  // motor 2; turn left side backwards
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
  analogWrite(enB, sp-15);
}

void turnCW(int sp){                                    //turn clockwise
  // motor 1; turn right side backwards
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);
  analogWrite(enA, sp);
 
  // motor 2; turn left side forwards
  // motor 2
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
  analogWrite(enB, sp-15);
  
}

void off(){                                             //motors off
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);  
  
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

bool is_up(float theta){                                //check if OSV facing up (theta >= 0)
  if(theta >= 0){
    return true;
  }
  return false;
}

bool is_down(float theta){                              //check if OSV facing down (theta < 0)
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

void turn_y_1(int sp){                                  //turn to face y = 1
  if(enes.location.y > 1){                              //above y = 1, turn to -PI/2
      enes.println("Above y = 1");
      if(is_left(enes.location.theta)){
        enes.println("\tFacing left. Turning CCW");
        while(enes.location.theta > 0 || enes.location.theta < (-1*PI/2)-0.4){
          turnCCW(sp);
          enes.updateLocation();
        }
        off();
      }
      else{
        enes.println("\tFacing right. Turning CW");
        while(enes.location.theta > 0 || enes.location.theta > (-1*PI/2)+0.4){
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
        while(enes.location.theta < 0 || enes.location.theta > (PI/2)+0.4){
          turnCW(sp);
          enes.updateLocation();
        }
        off();
      }
      else{
        enes.println("\tFacing right. Turning CCW");
        while(enes.location.theta < (PI/2)-0.4){
          turnCCW(sp);
          enes.updateLocation();
        }
        off();
      }
    }
}

void forward(int sp){                                      //move forwards
  // this function will run motors straight at a specific speed
  // motor 1
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  analogWrite(enA, sp);
 
  // motor 2
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
  analogWrite(enB, sp-15);
}

void move_y_1(int sp){                                      //move to y = 1
  while(fabs(enes.location.y-1)>0.05){
    forward(sp);
    enes.updateLocation();
  }
  off();
}

void turn_0(int sp){                                        //turn to face theta = 0
  if(is_up(enes.location.theta)){
    while(enes.location.theta > 0.4){
      turnCW(sp);
      enes.updateLocation();
    }
    off();
  }
  else{
    while(enes.location.theta > 0.4){
      turnCCW(sp);
      enes.updateLocation();
    }
    off();
  }
}

void move_rocks(int sp){                                    //move over rocks (x = 1.25)
  while(enes.location.x < 1.2){
    forward(sp);
    enes.updateLocation();
  }
  off();
}

void turn_down(int sp){                                     //turn down (theta = -PI/2)
  while(enes.location.theta > 0 || enes.location.theta > (-1*PI/2)+0.4){
    turnCW(sp);
    enes.updateLocation();
  }
  off();
}

void turn_up(int sp){                                       //turn up (theta = PI/2
  while(enes.location.theta > 0 || enes.location.theta > (-1*PI/2)+0.4){
    turnCW(sp);
    enes.updateLocation();
  }
  off();
}

void ultrasonic(){                                          //run ultrasonics
  int rDistTot = 0;
  int lDistTot = 0;
  for(int i = 0; i < 3; i++){
    //left ultrasonic
    digitalWrite(trigL, LOW);       //clear trig
    delayMicroseconds(2);
    digitalWrite(trigL, HIGH);      //left US on
    delayMicroseconds(10);
    digitalWrite(trigL, LOW);       //left US off
    long tL = pulseIn(echoL, HIGH);   //time to receive signal
    int d = tL * 0.034 / 2;         //dist to object
    lDistTot += d;
    
    //right ultrasonic
    digitalWrite(trigR, LOW);       //clear trig
    delayMicroseconds(2);
    digitalWrite(trigR, HIGH);      //right US on
    delayMicroseconds(10);
    digitalWrite(trigR, LOW);       //right US off
    long tR = pulseIn(echoR, HIGH);   //time to receive signal
    d = tR * 0.034 / 2;             //dist to object
    rDistTot += d;
  }
  distL = lDistTot/3;
  distR = rDistTot/3;
}

void move_up_y(int sp, float safe_y){
  while(enes.location.y < safe_y - 0.05){
    forward(pwm_move);
    enes.updateLocation();
  }
  off();
}

void move_down_y(int sp, float safe_y){
  while(enes.location.y > safe_y + 0.05){
    forward(pwm_move);
    enes.updateLocation();
  }
  off();
}

void setup() {
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(trigL, OUTPUT);
  pinMode(trigR, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(echoR, INPUT);
  Serial.begin(9600);
  enes.println("c");
  Serial.println("connected");
}

void loop() {
  if(enes.updateLocation()){
    if(enes.location.x < 1.2){
      turn_y_1(pwm_turn);
      delay(500);
      move_y_1(pwm_move);
      delay(500);
      turn_0(pwm_turn);
      delay(500);
      move_rocks(pwm_move);
      delay(500);
    }
    if(enes.location.x < 2.2){            //before x = 2.25 --> obstacle avoidance
      forward(pwm_move);  
      //Serial.println("forward");
      ultrasonic();
      if(distL < 20 && distR < 20){       //if both sense, depends on y position
        off();
        delay(1000);
        if(enes.location.y < 1){              //if below y = 1, turn up, move 30 cm
          turn_up(pwm_turn);                  
          delay(500);
          float safe_y = enes.location.y + 0.3;
          move_up_y(pwm_move, safe_y);
          off();
        }
        else{                                 //else, turn down, move 30 cm
          turn_down(pwm_turn);
          delay(500);
          float safe_y = enes.location.y - 0.3;
          move_down_y(pwm_move, safe_y);
          off();
        }
      }
      else if(distL < 20){                //if left senses, turn down, move 15 cm
        off();
        delay(500);
        turn_down(pwm_turn);
        delay(500);
        float safe_y = enes.location.y - 0.15;
        move_down_y(pwm_move, safe_y);
        off();
      }
      else if(distR < 20){                //if right sense, turn up, move 15 cm
        off();
        delay(500);
        turn_up(pwm_turn);
        delay(500);
        float safe_y = enes.location.y + 0.15;
        move_up_y(pwm_move, safe_y);
        off();
      }
      delay(500);
      turn_0(pwm_turn);
      delay(500);
      off();
    }
  }
}
