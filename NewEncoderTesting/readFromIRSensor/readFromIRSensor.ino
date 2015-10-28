int sensorPin = A0;
int REFLECTION_THRESHOLD = 200;
int INACTIVE_THRESHOLD = 5000; //After no positive edges are deteced for INACTIVE_THRESHOLD time, data is broadcasted

long lastUpdatedTime = millis();
boolean lastReflection;
boolean broadcasted = false;

//number of measured wheel rotations
int numberRotations;

void setup() {
  Serial.begin(9600);
}

void detectPositiveEdge(int IRVal){
  boolean newReflection = IRVal > REFLECTION_THRESHOLD;
  if(lastReflection != newReflection && newReflection == HIGH){
    onPositiveEdgeDetected();
  }
  lastReflection = newReflection;
}

void onPositiveEdgeDetected(){
  numberRotations++;
  lastUpdatedTime = millis();
  broadcasted = false;
}

void broadcastNumberRotations(){
  if (millis() - lastUpdatedTime > INACTIVE_THRESHOLD && !broadcasted){
    Serial.println(numberRotations); // we broadcast the data by printing to serial
    numberRotations = 0;
    lastUpdatedTime = millis();
    broadcasted = true; //this way we won't broadcast data until we see another positive edge
  }
}

void loop() {
  int IRVal = analogRead(sensorPin);
  detectPositiveEdge(IRVal);
  broadcastNumberRotations();
}
