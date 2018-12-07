// motor one
int enA = 11; /* PMW control speed motor 1*/
//in1 = A0; /*Output HIGH or LOW for direction */
//in2 = A1; /*Output LOW or HIGH for direction */
 
 
// motor two
int enB = 10; /* PMW control speed motor 1*/
//in3 = A2; /*Output HIGH or LOW for direction */
//in4 = A3; /*Output LOW or HIGH for direction */

int pwm_turn = 230;
int pwm_move = 200;

void turnCCW(int sp){                                   //turn counter clockwise
  // motor 1; turn right side forwards
  digitalWrite(A0, HIGH);
  digitalWrite(A1, LOW);
  analogWrite(enA, sp);
 
  // motor 2; turn left side backwards
  digitalWrite(A2, HIGH);
  digitalWrite(A3, LOW);
  digitalWrite(enB, 255);
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
  digitalWrite(enB, 255);
  
}

void off(){                                             //motors off
  digitalWrite(A0, LOW);
  digitalWrite(A1, LOW);  
  
  digitalWrite(A2, LOW);
  digitalWrite(A3, LOW);
}

void setup() {
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(A0, OUTPUT);
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
}

void loop() {
  delay(1000);
  turnCW(250);
  delay(2000);
  off();
  delay(1000);
  turnCCW(250);
  delay(2000);
  off();
}
