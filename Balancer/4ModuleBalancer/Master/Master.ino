#include <Wire.h>

//variate inputNumber to increase or decrease # of inputs
//variate analog_inputs and digital_outputs to increase, decrease # of pins or to change pins
float inputNumber=16, temporary, temp, cell_ave_treshold=0;
int led=0;
byte analog_inputs[]={A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};
int digital_outputs[]={2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17};
float voltage_divs[]={9.99/3.244, 9.83/3.253, 10.03/3.254, 9.91/3.255, 10.15/3.232, 10.04/3.242, 9.94/3.235, 10.0/3.265, 10.05/3.242, 9.94/3.245, 9.89/3.242, 10.0/3.275, 9.93/3.245, 9.8/3.236, 9.87/3.253, 9.835/3.239};
float voltages[17];
float average[5];
unsigned long initial_time;
unsigned long previous_time=0;
unsigned long led_time=0;
void blinking(){
  //builtin led blinks
  if (millis()-led_time>200){
    if (led==0){
      led=1;
      digitalWrite(LED_BUILTIN, HIGH);
      led_time=millis();
    }
    else{
      led=0;
      digitalWrite(LED_BUILTIN, LOW);
      led_time=millis();
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  initial_time=millis();
  pinMode(LED_BUILTIN, OUTPUT);
  initial_time=millis();
  for (int i=0; i<inputNumber; i++){
    //pinMode(analog_inputs[i], INPUT);
    pinMode(i, OUTPUT);
    
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  initial_time=millis();
  if (initial_time-previous_time>2000){
  temporary=0;
  for (int i=0; i<inputNumber; i++){
    temp=0;
    for (int j=0; j<8; j++){
        temp = (analogRead(analog_inputs[i])*voltage_divs[i]*5/1023)-temporary;
      }
    voltages[i]=temp/8;
    temporary += voltages[i];
    if ((i+1)%4==0){
      average[(i+1)%4-1] = temporary/inputNumber;
      temporary=0;
    }
  }
   
  

  for (int i=0; i<inputNumber; i++){
    if (voltages[i] - average[(i+1)%4-1] > cell_ave_treshold){
      digitalWrite(digital_outputs[i],HIGH); 
    }
    
    else{
      digitalWrite(digital_outputs[i],LOW);
    }
    
  }

  for (int i=0; i<16;i++){
    if (i==0){Wire.beginTransmission(9);Wire.write(100);delay(100);Wire.endTransmission();}
    
    Wire.beginTransmission(9); // transmit to device #9
    Wire.write((int)(voltages[i]*10));              // sends voltages[i]*10 
    delay(100);
    Wire.endTransmission();    // stop transmitting
  } 
  previous_time=initial_time;
 }
 blinking();
}
