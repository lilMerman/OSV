#include <IRremote.h>
#include <IRremoteInt.h>
#include <IRremoteTools.h>

#include "Enes100.h"
#include <Servo.h>

Enes100 enes("TGTWAPTT", BLACK_BOX, 5, 5, 13);    //enes("name", MISSION, image_num, connect to pin 5, connect to pin 4)

int trigL = 2;                                          //left ultrasonic
int echoL = 4;
int trigR = 7;                                          //right ultrasonic
int echoR = 8;

int distL;
int distR;

Servo IR_servo;                                         //IR sensor system
int IR_servo_pin = 9;
int RECV_PIN = 3;

Servo BB_servo;                                         //BB collection servo
int BB_servo_pin = 6;

int enA = 11; /* PMW control speed motor 1*/            // motor one
//in1 = A0; /*Output HIGH or LOW for direction */
//in2 = A1; /*Output LOW or HIGH for direction */
 
int enB = 10; /* PMW control speed motor 1*/            // motor two
//in3 = A2; /*Output HIGH or LOW for direction */
//in4 = A3; /*Output LOW or HIGH for direction */

int pwm_turn = 255;
int pwm_move = 180;

void forward(int sp){                                   //move forwards
  // motor 1
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  analogWrite(enA, sp);
 
  // motor 2
  digitalWrite(A2, LOW);
  digitalWrite(A3, HIGH);
  analogWrite(enB, sp-15);
}

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

/*
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
*/

void reverse(int sp){                                   //move in reverse
  // motor 1
  digitalWrite(A0, LOW);
  digitalWrite(A1, HIGH);
  analogWrite(enA, sp);
 
  // motor 2
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
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

bool is_right(float theta){                             //check if OSV is facing right (-PI/2 <= theta <= PI/2))
  if(abs(theta) <= PI/2){
    return true;
  }
  return false;
}

bool is_left(float theta){                              //check if OSV is facing left (PI/2 < theta <= PI OR -PI/2 > theta >= -PI)
  if(abs(theta) > PI/2){
    return true;
  }
  return false;
}

void turn_y_1(int sp){                                  //turn to face y = 1
  if(enes.location.y > 1){                              //above y = 1, turn to -PI/2
      enes.println("Above y = 1");
      while(enes.location.theta > -PI/2+0.4 || enes.location.theta < -PI/2-0.4){              //turn CCW until within 0.4 rad of -PI/2
        turnCCW(sp);
        enes.updateLocation();
      }
      off();
    }
    else{                                                 //below y = 1, turn to PI/2
      enes.println("Below y = 1");
      while(enes.location.theta < PI/2-0.4 || enes.location.theta > PI/2 + 0.4){              //turn CCW until within 0.4 rad of PI/2
        turnCCW(sp);
        enes.updateLocation();
      }
    }
}

void move_y_1(int sp){                                      //move to y = 1
  while(fabs(enes.location.y-1)>0.1){                        //forwards until within 5 cm of y = 1
    forward(sp);
    enes.updateLocation();
  }
  off();
}

void turn_0(int sp){                                        //turn to face theta = 0
  while(fabs(enes.location.theta) > 0.2){                              //turn CCW until within 0.4 rad of 0
    turnCCW(sp);
    delay(600);
    off();
    enes.updateLocation();
  }
  off();
}

void move_rocks(int sp){                                    //move over rocks (x = 1.25)
  while(enes.location.x < 1.15){                                                               //forwards until within 5 cm of x = 1.25
    forward(sp);
    enes.updateLocation();
  }
  off();
}

void turn_down(int sp){                                     //turn down (theta = -PI/2)
  while(enes.location.theta > (-PI/2)+0.4 || enes.location.theta < -PI/2-0.4){                //turn CCW until within 0.4 rad of 
    turnCCW(sp);
    enes.updateLocation();
  }
  off();
}

void turn_up(int sp){                                       //turn up (theta = PI/2
  while(enes.location.theta < PI/2-0.4 || enes.location.theta > (PI/2)+0.4){
    turnCCW(sp);
    enes.updateLocation();
  }
  off();
}

bool ultrasonic(){                                          //run ultrasonics
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
  if(distL > 0 || distR > 0){
    return true;
  }
  return false;
}

void ultrasonic_movement(){
  if(distL < 20 && distR < 20){       //if both sense, depends on y position
    off();
    delay(1000);
    if(enes.location.y < 1){              //if below y = 1, turn up, move 30 cm
      turn_up(pwm_turn);                            //turn up (PI/2)
      delay(500);
      float safe_y = enes.location.y + 0.3;
      move_up_y(pwm_move, safe_y);                  //move 30 cm up
      off();
    }
    else{                                 //else, turn down, move 30 cm
      turn_down(pwm_turn);                          //turn down (-PI/2)
      delay(500);
      float safe_y = enes.location.y - 0.3;
      move_down_y(pwm_move, safe_y);                //move 30 cm down
      off();
    }
    delay(500);
    turn_0(pwm_turn);                     //turn to theta = 0
    delay(500);
    off();
  }
  else if(distL < 20){                //if left senses, turn down, move 15 cm
    off();
    delay(500);
    turn_down(pwm_turn);                            //turn down (-PI/2)
    delay(500);
    float safe_y = enes.location.y - 0.15;
    move_down_y(pwm_move, safe_y);                  //move 15 cm down
    off();
    delay(500);
    turn_0(pwm_turn);                               //turn to theta = 0
    delay(500);
    off();
  }
  else if(distR < 20){                //if right senses, turn up, move 15 cm
    off();
    delay(500);
    turn_up(pwm_turn);                              //turn up (PI/2)
    delay(500);
    float safe_y = enes.location.y + 0.15;
    move_up_y(pwm_move, safe_y);                    //move 15 cm up
    off();
    delay(500);
    turn_0(pwm_turn);                               //turn to theta = 0
    delay(500);
    off();
  }
}

void move_up_y(int sp, float safe_y){               //move in positive y direction, until y = safe_y
  while(enes.location.y < safe_y - 0.05){
    forward(pwm_move);
    enes.updateLocation();
  }
  off();
}

void move_down_y(int sp, float safe_y){             //move in negative y direction, until y = safe_y
  while(enes.location.y > safe_y + 0.05){
    forward(pwm_move);
    enes.updateLocation();
  }
  off();
}

void turn_theta(int sp, float theta){               //turn to input theta
  while(fabs(enes.location.theta - theta) < 0.4){
    turnCCW(sp);
    enes.updateLocation();
  }
  off();
}

float IR(){
  float deg;
  for(int i = 0; i < 180; i++){   //writes servo from 1 to 180 deg
    IR_servo.write(i);
    delay(60);
    if (irrecv.decode(&results)){ //if IR signal received, stop servo, break loop
      deg = i;
      break;
    }
  }
  return (deg*PI/180)-(PI/2);     //return radians of IR signal
}

bool destination(){               //checks if limit switch triggered
  if(digitalRead(A5) == LOW){
    return true;                        //returns true if triggered
  }
  return false;                         //returns false if not triggered
}

void dest_cheat(){                //cheatsy BB destination code
  enes.navigated();
  enes.println("Destination location: ");
  float x, y;
  for(int i = 0; i <= 12; i++){
    x = 2.25 + (i*0.1);
    for(int j = 0; j <= 6; j++){
      y = 0.5 + (j*0.1);
    }
    enes.print("(");
    enes.print(x);
    enes.print(", ");
    enes.print(y);
    enes.print(") OR ");
  }
}

IRrecv irrecv(RECV_PIN);            //set IR module pin
 
decode_results results;

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A5, INPUT);               //Limit switch
  pinMode(trigL, OUTPUT);
  pinMode(trigR, OUTPUT);
  pinMode(echoL, INPUT);
  pinMode(echoR, INPUT);
  irrecv.enableIRIn();          
  IR_servo.attach(IR_servo_pin);    //IR servo
  IR_servo.write(0);
  BB_servo.attach(BB_servo_pin);    //BB servo
  BB_servo.write(270);
  enes.println("connected");
  delay(4000);
}

void loop() {
  while(enes.updateLocation());
  enes.println("connected");
  if(enes.location.x < 1.2){                  //before x = 1.25
    enes.println("Before x = 1.2. Turn to y = 1.");
    turn_y_1(pwm_turn);                               //turn to face y = 1
    delay(1000);
    enes.println("Move to y = 1");
    move_y_1(pwm_move);                               //move to y = 1
    delay(1000);
    enes.println("turn to theta = 0");
    turn_0(pwm_turn);                                 //turn to theta = 0
    delay(1000);
    enes.println("move over rocks");
    move_rocks(pwm_move);                             //move over rocks (to x = 1.25)
    delay(1000);
    turn_0(pwm_turn);                                 //readjust: turn to theta = 0
  }
  else if(enes.location.x < 2.2){            //before x = 2.25 --> obstacle avoidance
    if(ultrasonic()){                                 //if obstacle, object avoidance
      ultrasonic_movement();
      off();
    }else{
      forward(pwm_move);                              //no obstacle, move forward
    }
  }
  else{
    off();
    delay(1000);
    float BB_rad = IR();
    
    while(enes.location.x < 3.5 && !destination()){ //within BB area
      forward();
      if(ultrasonic()){                             //if obstacle, object avoidance
        ultrasonic_movement();
        off();
      }else{
        forward(pwm_move);                          //no obstacle, move forward
      }
    }
    off();
    while(!destination()){                  //while limit switch not triggered, move up a little
      turn_up(pwm_turn);                            //turn up (PI/2)
      off();
      delay(500);
      float y = enes.location.y;
      move_up_y(pwm_move, y + 0.25);                //move up 25 cm
      turn_theta(pwm_turn, PI);                     //turn left (PI)
    }
    while(enes.location.x > 2.5 && !destination()){ //while limit switch not triggered, moving back towards x = 2.5
      if(ultrasonic()){                             //obstacle avoidance
        ultrasonic_movement();
        off();
      }else{
        forward(pwm_move);                          //no obstacle --> continue moving forwards
      }
    }
    off();
    while(!destination()){                  //while limit switch not triggered, move halfway down BB area
      turn_down(pwm_turn);                          //turn down (-PI/2)
      off();
      delay(500);
      float y = enes.location.y;
      move_down_y(pwm_move, y - 0.5);               //move down 50 cm
      turn_theta(pwm_turn, PI);                     //turn left (PI);
    }
    while(enes.location.x < 3.25 && !destination()){  //while limit switch not triggered, moving towards x = 3.25
      if(ultrasonic()){                             //obstacle avoidance
        ultrasonic_movement();
        off();
      }else{
        forward(pwm_move);                          //no obstacle --> continue moving forwards
      }
    }
    off();
    if(destination()){                              //limit switch triggered, print message
      enes.navigated();
      enes.print("Black Box found at: (");
      enes.print(enes.location.x);
      enes.print(", ");
      enes.print(enes.location.y);
      enes.println(")");
      delay(1000);
      float reverse = enes.location.theta - PI;     //determine theta for rear to face BB
      if(reverse < -PI){
        reverse = reverse + (2*PI);
      }else if(reverse > PI){
        reverse = reverse - (2*PI);
      }
      while(fabs(enes.location.theta - reverse) < 0.2){ //turn to reverse angle
        turnCCW(pwm_turn);
        enes.updateLocation();
      }
      off();
      delay(500);
      reverse(pwm_move-20);                             //reverse, slower than normal
      delay(2000);
      off();
      delay(500);
      for(int i = 270; i >= 0; i--){                    //run BB collection servo
        BB_servo.write(i);
        delay(20);
      }
      while(fabs(enes.location.theta - PI) < 0.2){      //turn to face theta = PI
        turnCCW(pwm_turn);
        enes.updateLocation();
      }
      for(int i = 15; i <= pwm_move; i++){              //accelerate to pwm_move from 0
        forward(i);
      }
      while(enes.location.x > 0.65){                    //navigate back to landing zone
        if(ultrasonic()){                                     //obstacle avoidance
          ultrasonic_movement();
          off();
          for(int i = 15; i <= pwm_move; i++){                //accelerate
            forward(i);
          }
        }else{
          forward(pwm_move);                                  //no obstacle --> continue moving forwards
        }
      }
      off();                                            //DONE
      delay(10000);
    }
    else{                                               //limit switch not triggered
      off();
      dest_cheat();                                     //cheatsy code
      delay(10000);                                     //DONE
    }
  }
}
