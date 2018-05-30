#include <Wire.h>
#include <OneWire.h>

OneWire ds0(2);
OneWire ds1(3);
OneWire ds2(4);
OneWire ds3(5);
OneWire ds4(6);
OneWire ds5(7);
OneWire ds6(8);
OneWire ds7(9);
OneWire ds8(10);
OneWire ds9(11);
OneWire ds10(12);
OneWire ds11(13);
OneWire ds12(14);
OneWire ds13(15);
OneWire ds14(16);
OneWire ds15(17);

int inputNumber=16, x, counter=0, led=0;
float temp=0,average=0;
float temperature[17];
float voltages[17];
float module_voltages[5];
float module_outs[5]={22, 23, 24, 25};

unsigned long initial_time;
unsigned long previous_time=0;
unsigned long led_time=0;

void setup(void)
{
  Serial.begin(9600);
  Wire.begin(9);
  Wire.onReceive(receiveEvent);
  initial_time=millis();

  for (int i=0; i<4; i++){
    pinMode(module_outs[i],OUTPUT);
    }
  
  for (int i=0; i<16; i++){
    voltages[i]=3.7;
    }
}
 
 
void loop(void)
{
  
  initial_time=millis();
  if (initial_time-previous_time>2000){
    temperature[0] = getTemp(ds0); temperature[1] = getTemp(ds1); temperature[2] = getTemp(ds2); temperature[3] = getTemp(ds3); 
    temperature[4] = getTemp(ds4); temperature[5] = getTemp(ds5); temperature[6] = getTemp(ds6); temperature[7] = getTemp(ds7);
    temperature[8] = getTemp(ds8); temperature[9] = getTemp(ds9); temperature[10] = getTemp(ds10); temperature[11] = getTemp(ds11);
    temperature[12] = getTemp(ds12); temperature[13] = getTemp(ds13); temperature[14] = getTemp(ds14); temperature[15] = getTemp(ds15);
  
  
    for (int i=0; i<inputNumber; i++){
      temp+=voltages[i];
      if ((i+1)%4==0){
      module_voltages[(i+1)%4+1]=temp;
      average+=temp;
      temp=0;
      } 
    }
    average/=4;
    for (int i=0; i<4; i++){
      if (average-module_voltages[i]>0.1){
        digitalWrite(module_outs[i],HIGH);
        }
      else{
        digitalWrite(module_outs[i],LOW);
        }
      }
      serialprint();
      previous_time=initial_time;
    }
}


void receiveEvent(int bytes) {
    if (counter==16){counter=0;}
    x=Wire.read();
    voltages[counter]=0;
    voltages[counter]+=((float)x/10);
 
    counter++;
    if (x==100){counter=0;}
    delay(100); 
    
  }



float getTemp(OneWire sensor){
  //returns the temperature from one DS18S20 in DEG Celsius
  byte data[12];
  byte addr[8];

  if ( !sensor.search(addr)) {
     //no more sensors on chain, reset search
     sensor.reset_search();
    return -1000;
  }

  if ( OneWire::crc8( addr, 7) != addr[7]) {
     Serial.println("CRC is not valid!");
     return -1000;
  }
  
  if ( addr[0] != 0x28) {
     Serial.print("Device is not recognized");
     return -1000;
  }
  
  sensor.reset();
  sensor.select(addr);
  sensor.write(0x44,1); // start conversion, with parasite power on at the end
  
  byte present = sensor.reset();
  sensor.select(addr);  
  sensor.write(0xBE); // Read Scratchpad
  
  for (int i = 0; i < 9; i++) { // we need 9 bytes
    data[i] = sensor.read();
  }
  
  sensor.reset_search();
  
  byte MSB = data[1];
  byte LSB = data[0];

  float tempRead = ((MSB << 8) | LSB); //using two's compliment
  float TemperatureSum = tempRead / 16;

  return TemperatureSum;
}

void serialprint(){
      Serial.println();
      Serial.print("      \t Module1 \t Module2 \t Module3 \t Module4");
      Serial.println("-------------------------------------------------------------------------");
      Serial.println("Ave   "); Serial.print("\t "); Serial.print(module_voltages[0]); Serial.print("\t "); Serial.print(module_voltages[1]); 
      Serial.print("\t "); Serial.print(module_voltages[2]);  Serial.print("\t "); Serial.print(module_voltages[3]);
      for (int i=0; i<16; i+=4){
        if (i==0){Serial.println("Cell 1");}
        Serial.print("\t ");
        Serial.print(voltages[i]);
      }
      for (int i=0; i<16; i+=4){
        if (i==0){Serial.println("Cell 1");}
        Serial.print("\t ");
        Serial.print(temperature[i]);
      }
      for (int i=1; i<16; i+=4){
        if (i==1){Serial.println("Cell 2");}
        Serial.print("\t ");
        Serial.print(voltages[i]);
      }
      for (int i=1; i<16; i+=4){
        if (i==1){Serial.println("Cell 2");}
        Serial.print("\t ");
        Serial.print(temperature[i]);
      }
      for (int i=2; i<16; i+=4){
        if (i==2){Serial.println("Cell 3");}
        Serial.print("\t ");
        Serial.print(voltages[i]);
      }
      for (int i=2; i<16; i+=4){
        if (i==2){Serial.println("Cell 3");}
        Serial.print("\t ");
        Serial.print(temperature[i]);
      }
      for (int i=3; i<16; i+=4){
        if (i==3){Serial.println("Cell 4");}
        Serial.print("\t");
        Serial.print(voltages[i]);
      }
      for (int i=3; i<16; i+=4){
        if (i==3){Serial.println("Cell 4");}
        Serial.print("\t ");
        Serial.print(temperature[i]);
      }
}

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

