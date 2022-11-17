int blue = 6;
int green = 5;
int red = 3;
void setup() {
  // put your setup code here, to run once:
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
}

void loop() {
  analogWrite(red,255-255);
 analogWrite(green,255-140);
 analogWrite(blue,255-5);
 

}
