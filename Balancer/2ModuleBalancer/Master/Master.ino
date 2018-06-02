#include <Wire.h>
#include <OneWire.h>
//variate inputNumber to increase or decrease # of inputs
//variate analog_inputs and digital_outputs to increase, decrease # of pins or to change pins
float inputNumber = 8, temporary, temp, cell_ave_treshold = 0.1;
int led = 0;
byte analog_inputs[] = {A0, A1, A2, A3, A4, A5, A6, A7, A8, A9, A10, A11, A12, A13, A14, A15};
int digital_outputs[] = {22, 24, 26, 28, 30, 32, 34, 36, 38, 40, 42, 44, 47, 49, 51, 53};
float voltage_divs[] = {9.99 / 3.244, 9.83 / 3.253, 10.03 / 3.254, 9.91 / 3.255, 10.15 / 3.232, 10.04 / 3.242, 9.94 / 3.235, 10.0 / 3.265, 10.05 / 3.242, 9.94 / 3.245, 9.89 / 3.242, 10.0 / 3.275, 9.93 / 3.245, 9.8 / 3.236, 9.87 / 3.253, 9.835 / 3.239};
float voltage_divs1[] = {9.99 / 3.244, 9.83 / 3.253, 10.03 / 3.254, 9.91 / 3.255, 10.05 / 3.242, 9.94 / 3.245, 9.89 / 3.242, 10.0 / 3.275};
int module_outs[3] = {2, 3};
float temperature[17];
float voltages[17];
float average[5];
unsigned long initial_time;
unsigned long previous_time = 0;
unsigned long led_time = 0;
OneWire ds0(23);
OneWire ds1(25);
OneWire ds2(27);
OneWire ds3(29);
OneWire ds4(31);
OneWire ds5(33);
OneWire ds6(35);
OneWire ds7(37);

void blinking() {
  //builtin led blinks
  if (millis() - led_time > 200) {
    if (led == 0) {
      led = 1;
      digitalWrite(LED_BUILTIN, HIGH);
      led_time = millis();
    }
    else {
      led = 0;
      digitalWrite(LED_BUILTIN, LOW);
      led_time = millis();
    }
  }
}

void setup() {
  // put your setup code here, to run once:
  //Wire.begin();
  Serial.begin(115200);
  initial_time = millis();
  pinMode(LED_BUILTIN, OUTPUT);
  initial_time = millis();
  for (int i = 0; i < inputNumber; i++) {
    //pinMode(analog_inputs[i], INPUT);
    pinMode(digital_outputs[i], OUTPUT);

  }
  pinMode(2, OUTPUT);
  pinMode(3, OUTPUT);

}

void loop() {
  // put your main code here, to run repeatedly:
  initial_time = millis();
  if (initial_time - previous_time > 2000) {
    temperature[0] = getTemp(ds0); temperature[1] = getTemp(ds1); temperature[2] = getTemp(ds2); temperature[3] = getTemp(ds3);
    temperature[4] = getTemp(ds4); temperature[5] = getTemp(ds5); temperature[6] = getTemp(ds6); temperature[7] = getTemp(ds7);
    
    temporary = 0;
    for (int i = 0; i < inputNumber; i++) {
      temp = 0;
      if (temperature[i]<0){
        temperature[i]=-1;
      }
      for (int j = 0; j < 4; j++) {
        temp += ((float)(analogRead(analog_inputs[i])) * voltage_divs1[i] * 5 / 1023);
      }
      voltages[i] = temp / 4 - temporary;
      temporary += voltages[i];

      if ((i + 1) % 4 == 1) {
        Serial.println();
      }
      Serial.print(i + 1);
      Serial.print(":");
      Serial.print(voltages[i]);
      Serial.print("V ");
      
      Serial.print(i + 1);
      Serial.print(":");
      Serial.print(temperature[i]);
      Serial.print("C ");
      

      if ((i + 1) % 4 == 0) {
        if (i > 0) {
          average[(int)(i / 4) - 1] = temporary / 4;
          Serial.print("module "); Serial.print((int)(i / 4) + 1); Serial.print(":"); Serial.print(average[(int)(i / 4) - 1]);
          Serial.println();
        }
        temporary = 0;

      }
    }
    Serial.println("---------");


    for (int i = 0; i < inputNumber; i++) {

      if (voltages[i] - average[(int)(i / 4) - 1] > cell_ave_treshold) {
        digitalWrite(digital_outputs[i], HIGH);
        Serial.print("yaniyor: "); Serial.print(i + 1); Serial.print(" ");
        Serial.println(); 
      }

      else {
        digitalWrite(digital_outputs[i], LOW);
      }

    }

    if (average[0] * 4 - average[1] * 4 > 0.1) {
      digitalWrite(module_outs[0], HIGH);
      digitalWrite(module_outs[1], LOW);
      Serial.print("module 1 is active.");
    }
    else if (average[0] * 4 - average[1] * 4 > 0.1) {
      digitalWrite(module_outs[0], LOW);
      digitalWrite(module_outs[1], HIGH);
      Serial.print("module 2 is active.");
    }
    else {
      digitalWrite(module_outs[0], HIGH);
      digitalWrite(module_outs[1], HIGH);
      Serial.print("module 1 is active.");
    }
    /*
      for (int i=0; i<16;i++){
        if (i==0){Wire.beginTransmission(9);Wire.write(100);delay(100);Wire.endTransmission();}

        Wire.beginTransmission(9); // transmit to device #9
        Wire.write((int)(voltages[i]*10));              // sends voltages[i]*10
        delay(100);
        Wire.endTransmission();    // stop transmitting
      }
      previous_time=initial_time;
      }*/
  }
  blinking();
}

float getTemp(OneWire sensor) {
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
  sensor.write(0x44, 1); // start conversion, with parasite power on at the end

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

