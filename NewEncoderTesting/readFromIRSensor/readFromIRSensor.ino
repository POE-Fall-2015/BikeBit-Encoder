int sensorPin = A0;
int REFLECTION_THRESHOLD = 200;
int INACTIVE_THRESHOLD = 5000; //After no positive edges are deteced for INACTIVE_THRESHOLD time, data is broadcasted

long lastUpdatedTime = millis();
long lastPosEdgeTime = 0;
boolean lastReflection;
boolean broadcasted = false;

//number of measured wheel rotations in a session. reset to zero when no new rotations are detected for a while.
int numberRotations = 0;
//number of total measured rotations while arduino is running.
long totalRotations = 0;

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
  totalRotations++;
  printRPM();
  lastPosEdgeTime = millis();
  lastUpdatedTime = millis();
  broadcasted = false;
}

void printRPM(){
   //uses last time positive edge was detected compared with current time to calculate rotations per second
   long timeBetweenEdges = millis() - lastPosEdgeTime;
   float RPM = 1.0/timeBetweenEdges * 60 * 1000;
   Serial.println(RPM);  
}

void broadcastNumberRotations(){
  if (millis() - lastUpdatedTime > INACTIVE_THRESHOLD && !broadcasted){
//    Serial.println(numberRotations); // we broadcast the data by printing to serial
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
