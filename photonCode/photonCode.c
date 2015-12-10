const int reedSwitchPin = D2;

long lastUpdatedTime = millis();
long DEBOUNCE_TIME = 10;
long SESSION_END_TIME = 10000; //After 10000 ms of idle time, session is considered to have ended.

//number of measured wheel rotations in a session. reset to zero when no new rotations are detected for a while.
int numberRotations = 0;
bool rotationsPosted = true;

void setup() {
    lastUpdatedTime = millis();
    attachInterrupt(reedSwitchPin,onReedSwitchHigh,RISING);
}

void onReedSwitchHigh(){
    long now = millis();
    if(now - lastUpdatedTime > DEBOUNCE_TIME){
        numberRotations++;
        lastUpdatedTime = now;
        rotationsPosted = false;
    }
}

void postRotations(){
    //makes use of particle webhooks to do the post request for us
    //instead of the requeset library because the request library seems
    //to break easily and won't work when the device goes to sleep and
    //wakes up again
    Particle.publish("postRotations", String(numberRotations), PRIVATE);
    rotationsPosted = true;
}

void resetRotations(){
    numberRotations = 0;
}

void loop() {
    bool sessionEnded = (millis() - lastUpdatedTime) > SESSION_END_TIME;
    if(Particle.connected() && sessionEnded && !rotationsPosted){
        postRotations();
        resetRotations();
    }
}
