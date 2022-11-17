

#define ANALOG_PIN    A0
#define SENSOR_LE     7
#define SENSOR_CLK    6
#define LED_DEBUG     13
#define TIME          80

/**
 * 
 * LEDky su zapojene od D4 D12
**/
#define NB_LEDS 9
#define LED_OFFSET 4

#define NB_PIXELS 20

#define NB_CMOS 5156


int tab_FULL[NB_PIXELS];

void setup() {
  pinMode(SENSOR_LE, OUTPUT); 
  pinMode(SENSOR_CLK, OUTPUT);
  pinMode(LED_DEBUG, OUTPUT);
  
  LED_setup();
  
  pinMode(ANALOG_PIN,INPUT);
  Serial.begin(115200);
  Serial.println("Setup done");
  turnOffLeds();
  digitalWrite(9, LOW);

}

void loop() {
//  turnOnLeds();
digitalWrite(8, LOW);
  
  for(int i = 0; i < NB_PIXELS; i++)
  {
    tab_FULL[i] = 0;
  }
  /*  READ CCD */
  digitalWrite(SENSOR_LE, HIGH);
  digitalWrite(SENSOR_CLK, HIGH);
  delayMicroseconds(2);            
  digitalWrite(SENSOR_CLK, LOW); 
  digitalWrite(SENSOR_LE, LOW);
  delayMicroseconds(2);

  for(int i=0; i<162; i++){ //Dummy 162
    
    digitalWrite(SENSOR_CLK, HIGH);
    delayMicroseconds(2);
    digitalWrite(SENSOR_CLK, LOW);
    delayMicroseconds(2);
  }
  
  //from 0 to 5156 CMOS
  
//   for(int i=0; i<1; i++){ //move i x 0,982 mm on CIS module
//     for(int i=0; i<23; i++){ //move 0,982 mm on CIS module 23x
//      
//      digitalWrite(SENSOR_CLK, HIGH); 
//      digitalWrite(SENSOR_CLK, LOW); 
//    }
//
//  }
  for(int i=0; i<NB_PIXELS; i++){
    
    for(int k = 0; k < NB_CMOS/NB_PIXELS; k++) //move 0,982 mm on CIS module 23x
    {
      digitalWrite(SENSOR_CLK, HIGH); 
      delayMicroseconds(2);
      digitalWrite(SENSOR_CLK, LOW); 
      delayMicroseconds(2);
    }
  //  delayMicroseconds(500);
    tab_FULL[i] = analogRead(ANALOG_PIN);
  }

  send_data_serial(tab_FULL);
  delay(TIME);
  
  
}

void send_data_serial(int tab[]){
    for(int i= 0; i < NB_PIXELS; i++){
//      if(tab[i] > 700){
//        Serial.print(" X ");
//      } else {
//        Serial.print(" - ");
//      }
      Serial.print(tab[i]);
      Serial.print(" ");
    }
    Serial.println();
  
}

void read_CCD(int tab[]){

  digitalWrite(SENSOR_LE, HIGH);
  digitalWrite(SENSOR_CLK, HIGH);            
  digitalWrite(SENSOR_CLK, LOW); 
  digitalWrite(SENSOR_LE, LOW);

  for(int i=0; i<NB_PIXELS; i++){
    for(int j=0; j<(NB_CMOS/NB_PIXELS);j++){
      digitalWrite(SENSOR_CLK, HIGH); 
      digitalWrite(SENSOR_CLK, LOW); 
    }
    tab[i] = analogRead(ANALOG_PIN);
  }
}


void LED_setup()
{
  for(int i = 0; i < NB_LEDS; i++)
  {
    pinMode(i+LED_OFFSET, OUTPUT);
  }
}

void turnOnLeds()
{
   for(int i = 0; i < NB_LEDS; i++)
  {
    digitalWrite(i+LED_OFFSET, LOW);
  }
}

void turnOffLeds()
{
   for(int i = 0; i < NB_LEDS; i++)
  {
    digitalWrite(i+LED_OFFSET, HIGH);
  }
}
