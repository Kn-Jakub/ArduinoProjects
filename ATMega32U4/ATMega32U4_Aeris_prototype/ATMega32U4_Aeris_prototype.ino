#define LED_CLK 4
#define LED_OE  5
#define LED_SDI 2
#define LED_LE  3

#define SENSR_1 9
#define SENSR_2 8

const int ledCount = 16;
const int sensorCount = 3;

int sensors[sensorCount] = {SENSR_1, SENSR_2};
int rawData[ledCount][sensorCount];

//------------------------------------------------------------------------------
// Methode for STP16 LED Driver
//------------------------------------------------------------------------------
void nextLED(bool isFirst)
{
  digitalWrite(LED_OE, HIGH);
    if(isFirst)
    {
      digitalWrite(LED_SDI, HIGH);
    }
    digitalWrite(LED_CLK, HIGH);
    digitalWrite(LED_CLK, LOW);
    if(isFirst)
    {
      digitalWrite(LED_SDI, LOW);
    }

  digitalWrite(LED_LE, HIGH);
  digitalWrite(LED_LE, LOW);

  digitalWrite(LED_OE, LOW);
}
//------------------------------------------------------------------------------
void initLED()
{
  digitalWrite(LED_OE, HIGH);
  for (int i = 0; i < 32; ++i)
  {
    digitalWrite(LED_CLK, HIGH);
    digitalWrite(LED_CLK, LOW);
  }

  digitalWrite(LED_LE, HIGH);
  delayMicroseconds(10);
  digitalWrite(LED_LE, LOW);
  digitalWrite(LED_OE, LOW);
}
//------------------------------------------------------------------------------
void startTimer()
{
  pinMode(10, OUTPUT); // DDRB |= (1 << 6);

  //     15|     14|     13|     12|     11|     10|      9|      8|
  //   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |
  // 32768   16384    8192    4096    2048    1024    512     256
  // 0x80     0x40    0x20    0x10    0x08    0x04    0x02    0x01

  //      7|      6|      5|      4|      3|      2|      1|      0|
  //   0   |   0   |   0   |   0   |   0   |   0   |   0   |   0   |
  //  128      64      32      16      8       4       2       1
  //  0x80    0x40    0x20    0x10    0x08    0x04    0x02    0x01

  // 16 000 000 / 8 = 2 000 000 / 1024 = 1953 tikov/sec
  // 16 000 000 / 8 = 2 000 000 / 52 = 38 461 Hz
  // 16 000 000 / 256 = 62 500 / 128 = 488 Hz        {CS12 |}
  TCCR1A =_BV(COM1B1); // toggle OC1B when counter compare to OCR1B 
  TCCR1B =_BV(WGM12) | _BV(WGM13) | _BV(CS11);// WGM12 count up to OCR1B {(CS11)Prescalar 8|(CS12)Prescalar 256}
  TCCR1A |=_BV(WGM11);
  ICR1=52;//60000;
  OCR1B = 26;
}
//------------------------------------------------------------------------------
// Read data fom sensors
//------------------------------------------------------------------------------
void readData()
{
  for(int i = 0; i < ledCount; i ++)
  {
    if(i == 0)
      nextLED(true);
    else
      nextLED(false);

      for (int s = 0; s < sensorCount; s++)
      {
        rawData[i][s] = sensors[s];
      }
  }
}
//------------------------------------------------------------------------------
// Debuging functions
//------------------------------------------------------------------------------
void printRawData()
{
  for (int s = 0; s < sensorCount; s++)
  {
      Serial.print("Sensor ");
      Serial.print(s);
      Serial.print(": ");
      for(int i = 0; i < ledCount; i ++)
      {
          if(rawData[i][s])
          {
          Serial.print("X ");
          }
          else
          {
          Serial.print("-");
          }
      }
  }
}



//------------------------------------------------------------------------------
// Arduino main FUNCTIONS
//------------------------------------------------------------------------------
void setup() {
  // put your setup code here, to run once:
  pinMode(LED_CLK, OUTPUT);
  pinMode(LED_OE, OUTPUT);
  pinMode(LED_SDI, OUTPUT);
  pinMode(LED_LE, OUTPUT);

  pinMode(SENSR_1, INPUT);
  pinMode(SENSR_2, INPUT);
  initLED();
  startTimer();

  Serial.begin(115200);
  Serial.println("Init..");
  nextLED(true);
  nextLED(false);
  nextLED(false);
  nextLED(false);
  nextLED(false);
}
//----------------------------------MAIN----------------------------------------
void loop1() {
  // put your main code here, to run repeatedly:
      
  Serial.print(digitalRead(SENSR_1));
  Serial.print(" ");
  delay(500);
  Serial.println();
}

// testing loop for LEDS STP16
void loop() {
  // put your main code here, to run repeatedly:
  for(int i = 0; i < ledCount; i ++)
  {
    if(i == 0)
      nextLED(true);
    else
      nextLED(false);
      
    Serial.print(i);
    
    Serial.print(" ");
    
    delay(500);
  }
  Serial.println();
}
