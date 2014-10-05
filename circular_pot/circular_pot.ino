int sample = 0;
int lastSample = 0;
int samples[6];
int movingAvg = 0;
int lastMovingAvg = -1;
int val = 0;
int tooHigh = 100;
void loop() {
  int threshold = 20;
  sample = Toy.getInput(1);
  for (int i = 0; i < 5; i++) {
    samples[i] = samples[i+1];
  }
  samples[5] = sample; 
  for (int j = 0; j < 5; j++) {
    movingAvg += samples[j];
  }
  Serial.print(samples[0]);
  Serial.print(", ");
  Serial.print(samples[1]);
  Serial.print(", ");
  Serial.print(samples[2]);
  Serial.print(", ");
  Serial.print(samples[3]);
  Serial.print(", ");
  Serial.print(samples[4]);
  Serial.print(", ");
  Serial.print(samples[5]);
  
  movingAvg /= 5;
  Serial.print(", ");
  Serial.println(movingAvg);
  if (lastMovingAvg < 0) {
    lastMovingAvg = movingAvg;
  }
  
  if (movingAvg - lastMovingAvg > tooHigh) {
    int highAvg = (samples[0] + samples[1] + samples[2]) / 3;
    int lowAvg = (samples[4] + samples[5]) / 2;
    
    if (highAvg > 900 && lowAvg < 100) {
      val+=10;
      if (!clicked) {
        Toy.setOutput(1, val);
      }
    }
  }
  if (movingAvg - lastMovingAvg >= threshold && movingAvg - lastMovingAvg < tooHigh) { // increasing
    Serial.print("increasing, ");
    Serial.println(movingAvg - lastMovingAvg);
    val+=10;
    if (!clicked) {
      Toy.setOutput(0, val);
    }
  } else if (movingAvg - lastMovingAvg < -threshold && movingAvg - lastMovingAvg > -tooHigh) { // decreasing
      Serial.print("decreasing, ");
      Serial.println(movingAvg - lastMovingAvg);
    val-=10;
    if (!clicked) {
      Toy.setOutput(0, val);
    }
  }
  
  delay(50);
  lastMovingAvg = movingAvg;
  movingAvg = 0;
  
  
}
