//variate inputNumber to increase or decrease number of inputs
//variate analog_inputs and digital_outputs to increase, decrease number of pins or to change pins
float voltage_div=3,inputNumber=4, temporary, average;
byte analog_inputs[]={A0,A1,A2,A3};
int digital_outputs[]={2,3,4,5};
float voltages[]={};

void blinking(){
  //builtin led blinks
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
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
  //Serial.print("hello");
  temporary=0;
  for (int i=0; i<inputNumber; i++){
    voltages[i] = (analogRead(analog_inputs[i])*voltage_div*5/1023)-temporary;
    temporary += voltages[i];
    Serial.println(analog_inputs[i]);    
    Serial.print("pin: ");
    Serial.println(voltages[i]);
    Serial.print("\n"); 
  }
  average = temporary/inputNumber;
  //Serial.println(average);

  for (int i=0; i<inputNumber; i++){
    if (voltages[i] > average){
      digitalWrite(digital_outputs[i],HIGH); 
    }
    
    else{
      digitalWrite(digital_outputs[i],LOW);
    }
    
  }

  blinking();
}
