/*
 * All linear movements
 * forward, backward, accelerate, decelerate
 */
 
// motor one
int enA = 10; /* PMW control speed motor 1*/
int in1 = 9; /*Output HIGH or LOW for direction */
int in2 = 8; /*Output LOW or HIGH for direction */
 
 
// motor two
int enB = 5; /* PMW control speed motor 1*/
int in3 = 7; /*Output HIGH or LOW for direction */
int in4 = 6; /*Output LOW or HIGH for direction */
 
// int speed and time
int s = 255;          /*s from 0 to 255*/
int accelerationTime = 1000; /* how quickly the motor changes form vi to vf */
int runtime = 8000; /* change time for demo to run*/
 
 
 
 
void setup()
{
  // set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
}
 
void forward(int sp){
  // this function will run motors straight at a specific speed
  // motor 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, sp);
 
  // motor 2
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, sp);
 
}
 
void backward(int sp)
{
  // this function will run motors backwards at a specific speed
  // motor 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  analogWrite(enA, sp);
 
  // motor 2
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  analogWrite(enB, sp);
 
}
 
 
 
 
//accelerate from 0 to sp
void acelerate(int sp){
  // this function will accelerate motors at specific rate
  // motor 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  // motor 2
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  // Accelerate
  Serial.println("accelerate");
  for (int i = 0; i < sp; i ++) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(accelerationTime / sp);
  }
}

//decelerate from sp to 0
void decelerate(int sp){
  // motor 1
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  
  // motor 2
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  // Decelerate
  Serial.println("decelerate");
  for (int i = sp; i >= 0; i --) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(accelerationTime / sp);
  }
 
}
 
void off(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
 
void loop()
{
  delay(1000);
  accelerate(s);
  forward(s);
  delay(runtime);
  decelerate(s);
  off();
}
