float voltage_div, min = 1000, temporary, previous_voltage, inputNumber=4; //change inputNumber to variate number of cells. 
float inputList[]={} ;                      // {0, 0, 0, 0};      //Change here to variate cell number. change input list size and add zero according to the array size.
String str1 = "";


void setup() {
  // put your setup code here, to run once:
  voltage_div = 6;  //input must be inp<5 Volts. all values divided by voltage divider. Change here to variate divider proportion.
                                                                                                                 //inputNumber=6;//(int)(sizeof(inputList)/ sizeof(inputList[0]));
  Serial.begin(9600);              //  setup serial
  for (int i=0; i<inputNumber; i++){  pinMode(i, OUTPUT); }

}


void loop() {
  // put your main code here, to run repeatedly:
  str1="";
  previous_voltage=0;
  temporary=0;
  for (int i=0; i<inputNumber; i++) {
    temporary = analogRead(i)*(5/1023)*voltage_div;    //analogRead() returns btw (0 to 1023)
    inputList[i]=temporary - previous_voltage;
    previous_voltage=temporary;
    str1+=("A"+String(i)+" = "+String(inputList[i])+"   ");
  }
  
  Serial.print(str1);

  for (int i=0; i<inputNumber; i++) {
    if (inputList[i]<min){min=inputList[i]; }
  }


  for (int i=0; i<inputNumber; i++) {          //check voltage values and close switches
    
    if (inputList[i]-min>0.1){              //allowed voltage diff. 0.1 Volts
      if (digitalRead(i)==LOW){digitalWrite(i, HIGH); Serial.println("switch #"+String(i)+" is closed.\n"); }
    }
  
    else {
      if (digitalRead(i)==HIGH) {digitalWrite(i, LOW);  Serial.println("switch #"+String(i)+" is opened.\n");}
    }
  }
  delay(1000);
  
}
