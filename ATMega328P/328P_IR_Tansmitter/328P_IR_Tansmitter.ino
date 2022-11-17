#define FPU_CLK 16000000

#define Address_A 2
#define Address_B 3
#define Address_C 4
#define INHIBIT   5

#define PWM_PIN 9 //Pwm Output Pin

int Pwmpin=9;


void setup()
{
  // DDRB |= (1 << PB1) ; (OC1A/PCINT1)
  pinMode(PWM_PIN, OUTPUT);//Pwm pin as Output
  pinMode(Address_A, OUTPUT);
  pinMode(Address_B, OUTPUT);
  pinMode(Address_C, OUTPUT);
  pinMode(INHIBIT, OUTPUT);
  
  TCCR1A=_BV(COM1A1); // clear OC1A on Compare Match (Set output to low level)
  TCCR1B=_BV(WGM13)|_BV(CS11);// PWM phase and frequency correct,(CS11)Prescalar 8
  ICR1=26000;//60000;
  OCR1A=int(13000);
  Serial.begin(115200);
  Serial.println("Init done");
}

void setLedAddress(int address)
{
  int tmpAddress = 0;
  if(address > 8)
  {
    digitalWrite(INHIBIT, HIGH);
    return;
  }
  
  if (address >= 4) {
    Serial.print("C ");
    digitalWrite(Address_C, HIGH);
  } else {
    digitalWrite(Address_C, LOW); 
  }

  tmpAddress = address % 4;

  if (tmpAddress >= 2) {
    Serial.print("B ");
    digitalWrite(Address_B, HIGH);
  } else {
    digitalWrite(Address_B, LOW); 
  }

  tmpAddress = address % 2;

  if (tmpAddress == 1) {
    Serial.print("A ");
    digitalWrite(Address_A, HIGH);
  } else {
    digitalWrite(Address_A, LOW); 
  }
  
  digitalWrite(INHIBIT, LOW);
  //Serial.println();
}
void loop(){
//    Serial.print("Pwm Freq =");
//    Serial.println(0);
  //for (int i = 0; i < 10; i++)
  //{
 //   setLedAddress(i);
 //   delay(1000);
 // }
 //digitalWrite(PWM_PIN, HIGH);
  delay(50);
 //digitalWrite(PWM_PIN, LOW);
  delay(50);
  
}
