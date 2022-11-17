#define CLK 2
#define SDI 4
#define LE  3
#define OE  9
#define SENSOR_1 10
#define SENSOR_2 11


const int ledCount = 10;
const int sensorCount = 2;
int rawData[sensorCount][ledCount];


void nextLED(bool isFirst)
{
    if(isFirst)
    {
      digitalWrite(SDI, HIGH);
    }
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
    if(isFirst)
    {
      digitalWrite(SDI, LOW);
    }

  digitalWrite(LE, HIGH);
  digitalWrite(LE, LOW);
}

void initLED()
{
  digitalWrite(OE, HIGH);
  for (int i = 0; i < 32; ++i)
  {
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }

  digitalWrite(LE, HIGH);
  delayMicroseconds(10);
  digitalWrite(LE, LOW);
  digitalWrite(OE, LOW);
}

void startTimer()
{
  TCCR1A=_BV(COM1A1); // clear OC1A on Compare Match (Set output to low level)
  TCCR1B=_BV(WGM13)|_BV(CS11);// PWM phase and frequency correct,(CS11)Prescalar 8
  ICR1=30;//60000;
  OCR1A=int(15);
}

void setup() {
  pinMode(CLK,OUTPUT);
  pinMode(SDI,OUTPUT);
  pinMode(LE,OUTPUT);
  pinMode(OE,OUTPUT);
  
  initLED();
  startTimer();
  Serial.begin(115200);
  Serial.println("Init..");

//  nextLED(true);
//  for(int i = 0; i < 3 ; i++)
//  {
//    nextLED(false);
//  }
  
}

void readData()
{
  unsigned long StartTime = millis();
  for(int i = 0; i < ledCount; i ++)
  {
    if(i == 0)
      nextLED(true);
    else
      nextLED(false);
    delayMicroseconds(300);
    rawData[0][i] = digitalRead(SENSOR_1);
    rawData[1][i] = digitalRead(SENSOR_2);
  }
  nextLED(false);
  unsigned long CurrentTime = millis();
  unsigned long ElapsedTime = CurrentTime - StartTime;
  Serial.print("Measurement took: ");
  Serial.print(ElapsedTime);
  Serial.println("ms");
}

void printData()
{
  for(int i = 0; i < ledCount; i ++)
  {
    Serial.print(rawData[0][i]);
    Serial.print(" ");
  }
  Serial.println("");
  for(int i = 0; i < ledCount; i ++)
  {
    Serial.print(rawData[1][i]);
    Serial.print(" ");
  }
  Serial.println("");
}

void loop() {

  readData();
  printData();
  delay(50);
  
}


void loop_2() {
  for(int i = 0; i < ledCount; i ++)
  {
    if(i == 0)
      nextLED(true);
    else
      nextLED(false);
      
    
  }

}
