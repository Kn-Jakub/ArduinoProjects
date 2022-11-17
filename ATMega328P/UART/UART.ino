void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

}
String recv = "";
void loop() {
  // put your main code here, to run repeatedly:
  //Serial.write(15);
  if (Serial.available() > 0) {
    recv = Serial.readString();
    Serial.println(recv+1);
  }

}
