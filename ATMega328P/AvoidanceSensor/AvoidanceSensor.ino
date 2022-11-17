//Specifications
//    Working voltage: DC 3.3V-5V
//    Working current: ≥ 20mA
//    Operating temperature: -10 ℃ – +50 ℃
//    detection distance :2-40cm
//    IO Interface: 4-wire interfaces (- / + / S / EN)
//    Output signal: TTL level
//    Effective angle: 35°
//
//Sensor  Wemos Mini
//    G       GND
//    V+      5v
//    S       PIN 8
//    EN      Not connected

int detector = 8; // define the obstacle avoidance sensor interface
int val ;
 
void setup ()
{
  pinMode(13, OUTPUT);     // Built in Arduino LED
  digitalWrite (13, LOW);
  pinMode (detector, INPUT) ;// define the obstacle avoidance sensor output interface
}

void loop ()
{
  val = digitalRead (detector) ;
  if (val == HIGH) // When the obstacle avoidance sensor detects a signal, LED flashes
  {
    digitalWrite (13, HIGH);
    delay(100);
  }
  else
  {
    digitalWrite (13, LOW);
    delay(100);
  }
}
