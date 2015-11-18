const int reedSwitchPin = 2;

long lastUpdatedTime = millis();
long lastPosEdgeTime = 0;
long INACTIVE_THRESHOLD = 5000; //in ms, time that arduino will wait for new rotations to occur before broadcasting a final total
long DEBOUNCE_TIME = 10; //debounce time of 10 ms. Multiple positive edges happening within deboucne time of each other are counted as one positive edge.
boolean lastSwitchVal;
boolean broadcasted = false;

//number of measured wheel rotations in a session. reset to zero when no new rotations are detected for a while.
int numberRotations = 0;

void setup() {
  Serial.begin(9600);
}

void detectPositiveEdge(boolean switchVal){
  if(lastSwitchVal != switchVal && switchVal == HIGH){
    if(millis() - lastPosEdgeTime > DEBOUNCE_TIME){
      onPositiveEdgeDetected();
    }
  }
  lastSwitchVal = switchVal;
}

void onPositiveEdgeDetected(){
  numberRotations++;
  //printRPM();
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
    Serial.println(numberRotations); // we broadcast the data by printing to serial
    numberRotations = 0;
    lastUpdatedTime = millis();
    broadcasted = true; //this way we won't broadcast data until we see another positive edge
  }
}

void loop() {
  boolean IRVal = digitalRead(reedSwitchPin);
  detectPositiveEdge(IRVal);
  broadcastNumberRotations();
}
