void setup() {
  // put your setup code here, to run once:

}

int oldSample, sample;
int beatThreshold = 50;
int heartBeat[][3] = {
  {1, 100, 100},
  {1, 0, 50},
  {2, 50, 100},
  {2, 0, 1},
};
unsigned long newTime, oldTime = millis();
void loop() {
 int bpm;
  sample = Toy.getInput(1);
  Serial.println(sample);
  if(sample > beatThreshold && sample > oldSample+beatThreshold) {
    newTime = millis();
    bpm = 60000 / abs(newTime - oldTime);
    Serial.print("BPM: ");
    Serial.println(bpm);
    Toy.runShortPattern(*heartBeat, sizeof(heartBeat) / sizeof(int) / 3);
    oldTime = newTime;
  }
 
  oldSample = sample; 
}

