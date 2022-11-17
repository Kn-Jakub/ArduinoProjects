/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  http://www.arduino.cc/en/Tutorial/AnalogReadSerial

  regulacia od 155 - 255
*/
int Red = 20;
int green = 155;
int blue = 255;

int valueY = 0;
int valueX = 0;

bool changedX = false;
bool changedY = false;
/*enum color{
  RED = 1,
  GREEN,
  BLUE
  }selectColor;
*/
int selectColor = 0;  
void setColor(bool increment);
// the setup routine runs once when you press reset:
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
  
  pinMode(3, OUTPUT);
  pinMode(5, OUTPUT);
  pinMode(6, OUTPUT);
  pinMode(A6, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  //Red = Red + 5;
 // green = green - 5;
 control();
  Serial.print("Red = ");
  Serial.print(Red);
  Serial.print(", Blue = ");
  Serial.print(blue);
  Serial.print(", Green = ");
  Serial.println(green);
  delay(400);
  changeColor();
}

void changeColor(){
  analogWrite(3,Red);
  analogWrite(5,green);
  analogWrite(6,blue);
}

void control(){
  
 valueY = analogRead(A6);
  valueX = analogRead(A5);
  if(valueY > 1000 ) {
    if(!changedY) 
      if(selectColor == 2){
        selectColor = 0;
      }else {
        selectColor = selectColor + 1;  
      }
      changedY = true;
  } else if(valueY < 100 ) {
    if(!changedY) 
      if(selectColor == 0){
        selectColor = 2;
      }else {
        selectColor = selectColor - 1;  
      }
      changedY = true;
  } else {
    changedY= false;
    }

  if(valueX > 1000) {
    if(!changedX)
      setColor(true);
     // changedX = true;
  } else if(valueX < 100  ) {
    if(!changedX)
      setColor(false);
      //changedX = true;
  } else {
    changedX= false;
    }

  
  }
void setColor(bool increment){
  switch(selectColor){
    case 0:
      if(increment){
        Red = countValue(Red, true);
      } else {
        Red = countValue(Red, false);
      }
      break;
    case 1:
    if(increment){
        green = countValue(green, true);
      } else {
        green = countValue(green, false);
      }
      break;
    case 2:
    if(increment ){
        blue = countValue(blue, true);
      } else {
        blue = countValue(blue, false);
      }
    break;
    default:
    break;    
    }
}

int countValue(int value, bool increment){
    if(increment){
      if (value >= 255) return 155;
      if(value > 244){
        return value+1;
      }else if ((value > 215)) {
        return value + 5;  
      }else {
        return value + 10;  
      }
    } else {
      if (value <= 165) return 255;
      if(value > 244){
        return value-1;
      }else if ((value > 215)) {
        return value - 5;  
      }else {
        return value - 10;  
      }
      
    }
}
