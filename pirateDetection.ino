void pirateDetection(void *pirateDetectionStruct) {

  Serial.println("entered pirate detection");

  pirateDetectionData *mydetectData = (pirateDetectionData*)pirateDetectionStruct;
  bool *pirateDetect = mydetectData->pirateDetect;
  unsigned int *pirateDistance = mydetectData->pirateDistance;

  float duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
 
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
  distance = (duration / 2) * 0.0680;
  *pirateDistance = (int) distance;
  
  if (distance >= 400 || distance <= 2){
    Serial.print("Distance = ");
    Serial.println("Out of range");
  }
  else {
    Serial.print("Distance = ");
    Serial.print(distance);
    Serial.println(" m");
    delay(500);
  }
  delay(500);

  
  if (*pirateDistance < 100) {
    *pirateDetect = true;
  } else {
    *pirateDetect = false;
  }

  
}

