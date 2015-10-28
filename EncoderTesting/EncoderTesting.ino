int sensorPin = A0;    // select the input pin for IR sensor
int sensorValue = 0;  // variable to store the value coming from sensor
boolean oldStateTape = false;
boolean currentStateTape = false;
int counter = 0;
long lastUpdatedTime = millis();
boolean sentData = false;

void setup() {
  Serial.begin(9600);
}

void loop() {
  //Serial.print("count: ");
  //Serial.println(counter);
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
   Serial.println(sensorValue);
  if (sensorValue > 400){
    currentStateTape = true;
  }else{
    currentStateTape = false;
  }
  if (oldStateTape != currentStateTape && currentStateTape){
    counter+=1; 
    lastUpdatedTime = millis();
    sentData = false;
  } 
  if (millis() - lastUpdatedTime > 5000 && !sentData){
    Serial.println(counter); // printing count for one session
    counter = 0;
    lastUpdatedTime = millis();
    sentData = true;
  }
  oldStateTape = currentStateTape;
}
