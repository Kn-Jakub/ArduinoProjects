
#define SP          PA9 //biely
#define CLK         PA8 //sedy

/*     Definition of Led module*/
#define LED_SDI     PC6   //Hnedo biela
#define LED_CLK     PC8   //Zeleno biela
#define LED_LE      PC9   //modra

#define LED_OE_A    PC10  //ModroBiela 

#define LED_DEBUG   PA5
#define BTN_DEBUG   PC13


#define NB_PIXELS_LAST 20//0 ? 200  :40
#define NB_PIXELS      30//0 ? 300  :60

#define NB_CMOS                     1404
#define NB_CMOS_LAST_ELEMENT        936
#define NB_ZONES                  3

#define ARRAY_SIZE ((NB_PIXELS * NB_ZONES) + NB_PIXELS_LAST)

#define LEDS_COUNT 10

#define LIMIT  740
#define MAX_ZONES 10


HardwareSerial Serial2(PA3,PA2);

uint16_t rawData[LEDS_COUNT][ARRAY_SIZE];
uint16_t blackZones[LEDS_COUNT][MAX_ZONES][2];
uint8_t touchesPerLed[LEDS_COUNT];


uint16_t arrayLimits[LEDS_COUNT][2];

uint8_t currentLed = 0;

void setup() {
  pinMode(SP, OUTPUT); 
  pinMode(CLK, OUTPUT);
  pinMode(LED_DEBUG, OUTPUT);
  pinMode(BTN_DEBUG, INPUT);

  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  pinMode(A2,INPUT);
  pinMode(A3,INPUT);

  ledInit();
  
  Serial2.begin(115200);
  Calibrate();
  Serial2.println("SetUp succesful");
}
//******************************************************************************************** #LOOP# 
void loop() {


 //infraTouchMethode();
//  while(digitalRead(BTN_DEBUG) == LOW){
//    
//  }
//  delay(70);
  //setLed(5);
  //ledDemo();
  readCIS_TEST();
 //readCIS(0);
 //printInVisual(0);
 //printValues(0);
 digitalWrite(LED_DEBUG, !digitalRead(LED_DEBUG));
 delay(300);

}

void infraTouchMethode()
{
  setLed(0);
  readCIS(0);
  for(int ledIndex = 1; ledIndex <= LEDS_COUNT; ledIndex++)
  {
    setLed(ledIndex);
    readCIS(ledIndex-1);
    
    
  }

  checkTouch();
  delayMicroseconds(1);
  sendTouchDataToPython();
  //sendTouch();
}


void readCIS(int ledIndex)
{
  digitalWrite(SP, HIGH);
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW); 
  digitalWrite(SP, LOW);

  for(int i=0; i<82; i++){
      digitalWrite(CLK, HIGH);
      digitalWrite(CLK, LOW);
  }

  
  for(int i = 0; i<NB_PIXELS; i++){
    
    rawData[ledIndex][i] = analogRead(A0);
    rawData[ledIndex][i+NB_PIXELS] = analogRead(A1);
    rawData[ledIndex][i+NB_PIXELS*2] = analogRead(A2);
    
    if(i < NB_PIXELS_LAST){
      rawData[ledIndex][i+NB_PIXELS*3] = analogRead(A3);
    }
    
    for(int j=0; j<NB_CMOS/NB_PIXELS; j++){
      digitalWrite(CLK, HIGH);
      delayMicroseconds(2);
      digitalWrite(CLK, LOW);
      delayMicroseconds(2);
    }
    
  }
}

void readCIS_TEST()
{
  const int countOfValues = 60;
  uint16_t testData[countOfValues];
  
  digitalWrite(SP, HIGH);
  digitalWrite(CLK, HIGH);
  digitalWrite(CLK, LOW); 
  digitalWrite(SP, LOW);
  

  for(int i = 0; i < 5; i++){ //dummy 82
      digitalWrite(CLK, HIGH);
      digitalWrite(CLK, LOW);
  }

  for(int i = 0; i < countOfValues; i++)
  {
    testData[i] = analogRead(A0);
    digitalWrite(CLK, HIGH);
    digitalWrite(CLK, LOW);
  }
  
  // Print the data to terminal
  for(int k = 0; k < countOfValues; k++)
  {
    Serial2.print(testData[k]);
    Serial2.print(" ");
  }
  
  Serial2.println();
}

void ledInit()
{
  pinMode(LED_SDI, OUTPUT);
  pinMode(LED_CLK, OUTPUT);
  pinMode(LED_LE, OUTPUT);
  pinMode(LED_OE_A, OUTPUT);
  digitalWrite(LED_OE_A, 1); 
  digitalWrite(LED_LE, 0);
  digitalWrite(LED_SDI, 0);
  digitalWrite(LED_CLK, 0);
}

void setLed(int ledNbr)
{
  digitalWrite(LED_OE_A, 1);
  for (int i = 0; i < 16; i++)
  {
    if(16 - ledNbr == i)
    {
      digitalWrite(LED_SDI, 1);
    }
    digitalWrite(LED_CLK, 1);

    if(16 - ledNbr == i)
    {
      digitalWrite(LED_SDI, 0);
    }
    digitalWrite(LED_CLK, 0);

  }

  digitalWrite(LED_LE, 1);
  digitalWrite(LED_LE, 0);
  digitalWrite(LED_OE_A, 0); 
}

void ledDemo()
{
  for (int i = 0; i < LEDS_COUNT; i++)
  {
    setLed(i);
    delay(500);
  }
}

void printInVisual(int ledIndex)
{
  

  for(int k = 0; k < ARRAY_SIZE; k++)
  {
    if(rawData[ledIndex][k] < LIMIT)
    {
      Serial2.print("X");
    }
    else
    {
      Serial2.print("-");
    }
  }
  
  Serial2.println();
}

void printValues(int ledIndex)
{
  for(int k = 0; k < ARRAY_SIZE; k++)
  {
    Serial2.print(rawData[ledIndex][k]);
    Serial2.print(" ");
  }
  
  Serial2.println();
  Serial2.println();
}

void DEBUG_printValues(int ledIndex)
{
  Serial2.print("LED:");
  Serial2.print(ledIndex);
  Serial2.print(" ");
  for(int k = 0; k < ARRAY_SIZE; k++)
  {
    Serial2.print(rawData[ledIndex][k]);
    Serial2.print(" ");
  }
  
  Serial2.println();
  Serial2.println();
}


void checkTouch()
{
  
  bool touchDetected = false;
  int bufferForShadov = 0;
  int countOfTouches = 0;

  /*  LED 0*/
  for(int ledIndex = 0; ledIndex < LEDS_COUNT; ledIndex++)
  {
    for(int i = arrayLimits[ledIndex][0]; i < arrayLimits[ledIndex][1]; i++)
    {
      if(rawData[ledIndex][i] < LIMIT)
      {
        if(bufferForShadov < 7)
          bufferForShadov++;
        else
        {
          if(countOfTouches < MAX_ZONES)
          {
            touchDetected = true;
            blackZones[ledIndex][countOfTouches][0] = i-5;
          }
          else
          {
            bufferForShadov = 0;
          }
        }
      }
      else
      {
        if(touchDetected)
        {
          blackZones[ledIndex][countOfTouches][1] = i-1;
          countOfTouches++;
        }
        touchDetected = false;
        bufferForShadov = 0;
      }
    }
    if(touchDetected)
    {
      blackZones[ledIndex][countOfTouches][1] = arrayLimits[ledIndex][1];
      countOfTouches++;
    }
    touchesPerLed[ledIndex] = countOfTouches;
    touchDetected = false;
    bufferForShadov = 0;
    countOfTouches = 0;
 }
  /*  LED 1*/
//  for(int i = 63; i < 483; i++)
//  {
//    if(rawData[1][i] < LIMIT)
//    {
//      if(bufferForShadov < 5)
//        bufferForShadov++;
//      else
//      {
//        if(countOfTouches < MAX_ZONES)
//        {
//          touchDetected = true;
//          blackZones[1][countOfTouches][0] = i-5;
//        }
//        else
//        {
//          bufferForShadov = 0;
//        }
//      }
//    }
//    else
//    {
//      if(touchDetected)
//      {
//        blackZones[1][countOfTouches][1] = i-1;
//        countOfTouches++;
//      }
//      touchDetected = false;
//      bufferForShadov = 0;
//    }
//  }
//  if(touchDetected)
//  {
//    blackZones[1][countOfTouches][1] = 210;
//    countOfTouches++;
//  }
//  touchesPerLed[1] = countOfTouches;
//  touchDetected = false;
//  bufferForShadov = 0;
//  countOfTouches = 0;

  // TODO treba dorobit pre dalsie ledky + spravit korekciu pre vela dotykov podobne ako korekcia malej black zony
}

void sendTouch()
{
  for(int i = 0; i < LEDS_COUNT; i++)
  {
    if(touchesPerLed[i] > 0 )
    {
      for(int k = 0; k < touchesPerLed[i]; k++)
      {
       Serial2.print("Touch LED");
       Serial2.print(i);
       Serial2.print(" ");
       Serial2.print(k);
       Serial2.print(", ");
       Serial2.print(blackZones[i][k][0]);
       Serial2.print(", ");
       Serial2.print(blackZones[i][k][1]);
       Serial2.println("]");
      }
    }
  }
}

void sendTouchDataToPython()
{
  for(int i = 0; i < LEDS_COUNT; i++)
  {
    if(touchesPerLed[i] > 0 )
    {
      for(int k = 0; k < touchesPerLed[i]; k++)
      {
         Serial2.print(i);
         Serial2.print(":");
         Serial2.print(blackZones[i][k][0]);
         Serial2.print(":");
         Serial2.print(blackZones[i][k][1]);
         Serial2.println("");
      }
    }
  }
}
/********************************************************************************************************** Calibration/ Initialization functions ********************************************/

void Calibrate()
{
  loadLimits();

  for(int i = 0; i < LEDS_COUNT; i++)
  {
    Serial2.print(i);
    Serial2.print(":");
    Serial2.print(arrayLimits[i][0]);
    Serial2.print(":");
    Serial2.print(arrayLimits[i][1]);
    Serial2.println("");
  }
}

void loadLimits()
{
    uint8_t maxSizeErrorZone = 3;
    
    // First need to load all data from senzor for all leds
    setLed(0);
    readCIS(0);
    
    for(int i = 1; i <= LEDS_COUNT; i++)
    {
        setLed(i);
        readCIS(i-1);
    }

    uint16_t lowLimit;
    uint16_t highLimit;
    uint16_t lightCounter;
    uint16_t darkCounter;
    bool isInZone = false;
    
    for(int i = 0; i < LEDS_COUNT; i++)
    {
        lowLimit = 0;
        highLimit = 0;
        lightCounter = 0;
        darkCounter = 0;
        for(int j = 0; j < ARRAY_SIZE; j++)
        {
            if(rawData[i][j] > LIMIT)
            {
                if(!isInZone)
                {
                  isInZone = true;
                  lowLimit = j;
                }
                
                if(darkCounter > 0)
                {
                  darkCounter = 0;
                }
                
                lightCounter++;
            }
            else
            {
                if(isInZone)
                {
                  
                  if(darkCounter == 0)
                    highLimit = j - 3;

                  darkCounter++;
                  if(darkCounter > maxSizeErrorZone)
                  {
                      if(lightCounter < 5)
                      {
                          isInZone = false;
                          lowLimit = 0;
                          highLimit = 0;
                          lightCounter = 0;
                          darkCounter = 0; 
                      }
                      else
                      {
                        break;
                      }
                  }
                }
            }

            if(j == ARRAY_SIZE - 1 && isInZone)
            {
                highLimit = j;
            }
        }

        if(highLimit != 0)
        {
          arrayLimits[i][0] = lowLimit;
          arrayLimits[i][1] = highLimit;
        }
    }
  
}

/*
 // Nacitane dat do pola
//  for(int ledIndex = 0; ledIndex < LEDS_COUNT; ledIndex++)
//  {
//    digitalWrite(ledIndex + LED_OFFSET, HIGH);
//    if(ledIndex == 0)
//    {
//      readCIS(LEDS_COUNT-1);
//    }
//    else
//    {
//      readCIS(ledIndex-1);
//    }
//    
//    digitalWrite(ledIndex + LED_OFFSET, LOW);
//  }

//  checkTouch();
//  sendTouchDataToPython();
  //sendTouch();




******************************************************************** testovanie pre jednotlive ledky
  readCIS(0);
  for(int ledIndex = 0; ledIndex <= LEDS_COUNT; ledIndex++)
  {
    setLed(ledIndex);
    delay(150);
    if(ledIndex == 0)
    {
      readCIS(0);
    }
    else
    {
      readCIS(ledIndex-1);
    }
    
    
  }
  for(int ledIndex = 0; ledIndex < LEDS_COUNT; ledIndex++)
  {
    
    DEBUG_printValues(ledIndex);
  }
  

  while(digitalRead(BTN_DEBUG) == LOW){
    
  }

*/
