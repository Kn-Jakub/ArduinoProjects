
#define SDI PC6 //2   //  hnedo-biely
#define CLK PC8 //3   //  zeleno-biely
#define LE  PC9 //4   //  modry
#define OE  PC10 //5   //  modro-biely

/* Zapojenie:
 * Zelena     - PWR 5V  [Modra]
 * ModroBiela - OE
 * Modra      - LE  [Zelena]
 * ZelenoBiela- CLK
 * Hnedá      - GND
 * HnedoBiela - SDI
 */

#define LedPin 13


void setup() {
  pinMode(LedPin, OUTPUT);
  pinMode(SDI, OUTPUT);
  pinMode(CLK, OUTPUT);
  pinMode(LE, OUTPUT);
  pinMode(OE, OUTPUT);
  digitalWrite(OE, 1);
  digitalWrite(LE, 0);
  digitalWrite(SDI, 0);
  digitalWrite(CLK, 0);
 // Serial.begin(115200);
  //Serial.println("Begin");
}

void loop() {
  for(int i = 0 ; i < 10 ; i++)
  {
//    setLed(i);
//    digitalWrite(LedPin, i%2);
//    delay(1000);
  //  Serial.println(i);
  
    if(i%2 == 0)
    {
      turnOn();
    }
    else
    {
      turnOff();
    }
  }
}

void setLed(int ledNbr)
{
  digitalWrite(OE, 1);
  for(int i = 0; i <= 16; i++)
  {
    if(16 - ledNbr == i)
      digitalWrite(SDI, 1);
      
    digitalWrite(CLK, 1);
    //delay(1);

    if(16 - ledNbr == i)
      digitalWrite(SDI, 0);
    digitalWrite(CLK, 0);
  }
  digitalWrite(LE, 1);

  digitalWrite(LE, 0);
  
  digitalWrite(OE, 0);
}

void turnOff()
{
  digitalWrite(OE, 0);
  digitalWrite(LE, 1);
  for(int i = 0; i < 17; i++)
  {
      
    digitalWrite(CLK, 1);
    delay(500);
    //Serial.print("Turn off: ");
    //Serial.println(i);
    digitalWrite(CLK, 0);
  }
  
  digitalWrite(LE, 0);
}

void turnOn()
{
  digitalWrite(OE, 0);
  digitalWrite(LE, 1);
  for(int i = 0; i < 17; i++)
  {
    digitalWrite(SDI, 1);  
    digitalWrite(CLK, 1);
    
    delay(500);
    //Serial.print("Turn on: ");
    //Serial.println(i);

    digitalWrite(SDI, 0);
    digitalWrite(CLK, 0);
  }
  
  
  digitalWrite(LE, 0);
}
