// This #include statement was automatically added by the Particle IDE.

/*
//Commented out because the request library functionality was replaced with webhooks. Keep this around
//for future reference in case webhooks decide to stop working.
#include "HttpClient/HttpClient.h"

HttpClient http;

http_header_t headers[] = {
{"Accept" , "text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*\/*;q=0.8"},
{"Accept-Encoding" , "gzip, deflate, sdch"},
{"Accept-Language","en-US,en;q=0.8"},
//Specifying content type in the header is important, otherwise the server application will not know how to parse the request body.
{"Content-Type","application/x-www-form-urlencoded"},
{"Connection","keep-alive"},
//Host must be set to domain we are sending requests to otherwise requests are refused
{"Host","bikechill.herokuapp.com"},
{"Upgrade-Insecure-Requests","1"},
{"User-Agent","Mozilla/5.0 (X11; Linux x86_64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.80 Safari/537.36"}
};

http_request_t request;
http_response_t response;

request.hostname = "bikechill.herokuapp.com";
request.port = 80;
request.path = "/bikeSession";
*/

const int reedSwitchPin = D0;

long lastUpdatedTime = millis();
long lastPosEdgeTime = 0;
long DEBOUNCE_TIME = 5; //debounce time of 5 ms. Multiple positive edges happening within debounce time of each other are counted as one positive edge.
long INACTIVE_THRESHOLD_TIME = 5000;
long SLEEP_THRESHOLD_TIME = 20000;
boolean lastSwitchVal;
boolean broadcasted = true;

//number of measured wheel rotations in a session. reset to zero when no new rotations are detected for a while.
int numberRotations = 0;

long tempTime = millis();

void setup() {
    lastUpdatedTime = millis();
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

void postRotations(){
    //makes use of particle webhooks to do the post request for us
    //instead of the requeset library because the request library seems
    //to break easily and won't work when the device goes to sleep and
    //wakes up again
    Particle.publish("postRotations", String(numberRotations), PRIVATE);
}

void broadcastNumberRotations(){
  if (((millis() - lastUpdatedTime) > INACTIVE_THRESHOLD_TIME) && !broadcasted){
    postRotations();
    numberRotations = 0;
    lastUpdatedTime = millis();
    broadcasted = true; //this way we won't broadcast data until we see another positive edge
  }
}

void sleepIfIdle(){
    if(broadcasted){
        if((millis() - lastUpdatedTime) > SLEEP_THRESHOLD_TIME){
            System.sleep(reedSwitchPin, CHANGE);
        }
    }
}

void loop() {
  boolean IRVal = digitalRead(reedSwitchPin);
  detectPositiveEdge(IRVal);
  if (WiFi.ready()){
    broadcastNumberRotations();
    sleepIfIdle();
  }
}
