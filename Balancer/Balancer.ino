//variate inputNumber to increase or decrease number of inputs
//variate analog_inputs and digital_outputs to increase, decrease number of pins or to change pins
float inputNumber=4, temporary, average;
byte analog_inputs[]={A0,A1,A2,A3};
int digital_outputs[]={2,3,4,5};
float voltage_divs1[]={9.99/3.244, 9.83/3.253, 10.03/3.254, 9.91/3.255}; 
float voltage_divs2[]={10.15/3.232, 10.04/3.242, 9.94/3.235, 10.0/3.265};
float voltage_divs3[]={10.05/3.242, 9.94/3.245, 9.89/3.242, 10.0/3.275};
float voltage_divs4[]={9.93/3.245, 9.8/3.236, 9.87/3.253, 9.835/3.239};
float cell_ave_interval=0.01, temp;
float voltages[17]={};

void blinking(){
  //builtin led blinks
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    Serial.flush();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  
  for (int i=0; i<inputNumber; i++){
    //pinMode(analog_inputs[i], INPUT);
    pinMode(digital_outputs[i], OUTPUT);
    //voltages[i]=0;
  }
  
}

void loop() {
  // put your main code here, to run repeatedly:
  temporary=0;
  for (int i=0; i<inputNumber; i++){
    temp=0;
    for (int j=0; j<8;j++){
      temp+=((float)(analogRead(analog_inputs[i]))*voltage_divs4[i]*5/1023)-temporary;
      delay(1);
    }
    voltages[i] = temp/8;
    temporary += voltages[i];
    Serial.print(i+1);
    Serial.print(":");
    Serial.println(voltages[i]);
    Serial.println();
  }
  Serial.println("---------");
  average = temporary/inputNumber;
  Serial.println(average);

  for (int i=0; i<inputNumber; i++){
    if (voltages[i] - average > cell_ave_interval){
      digitalWrite(digital_outputs[i],HIGH); 
    }
    
    else{
      digitalWrite(digital_outputs[i],LOW);
    }
    
  }

  blinking();
}
