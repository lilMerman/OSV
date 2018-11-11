// motor one
int enA = 10; /* PMW control speed motor 1*/
int in1 = 9; /*Output HIGH or LOW for direction */
int in2 = 8; /*Output LOW or HIGH for direction */
 
 
// motor two
int enB = 5; /* PMW control speed motor 1*/
int in3 = 7; /*Output HIGH or LOW for direction */
int in4 = 6; /*Output LOW or HIGH for direction */

int s = 255;

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

void off(){
  //motor 1 off
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);  

  //motor 2 off
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}

void setup() {
  pinMode(enA, OUTPUT);     //all pins to output
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  //test different PWMs; call forward with different sp values (from 0 to 255)
}
