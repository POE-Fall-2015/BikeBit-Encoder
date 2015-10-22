int sensorPin = A0;    // select the input pin for IR sensor
int sensorValue = 0;  // variable to store the value coming from sensor
boolean oldStateTape = false;
boolean currentStateTape = false;
int counter = 0;
void setup() {
  Serial.begin(9600);
}

void loop() {
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
   // Serial.println(sensorValue);
  if (sensorValue > 400){
    currentStateTape = true;
  }else{
    currentStateTape = false;
  }
  if (oldStateTape != currentStateTape && currentStateTape){
    counter+=1;  
  }
  Serial.println(counter);
  oldStateTape = currentStateTape;
}
