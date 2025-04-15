int LDR = 34;                            // LDR pin
int LedPins[] = {18, 19, 21, 22, 23};    // LED pins
int IRSensors[] = {5, 17, 16, 4, 27};    // IR sensor pins

int dimBrightness = 64;
int fullBrightness = 255;
int buzzer = 26; 

const int pwmFreq = 5000;
const int pwmResolution = 8;

bool nightModePrev = false;  // to detect transition

void setup() {
  Serial.begin(115200);

  pinMode(buzzer, OUTPUT);
  for (int i = 0; i < 5; i++) {
    pinMode(LedPins[i], OUTPUT);
    pinMode(IRSensors[i], INPUT);
    ledcSetup(i, pwmFreq, pwmResolution);
    ledcAttachPin(LedPins[i], i);
  }
}

void loop() {
  int ldrValue = analogRead(LDR);
  bool isNight = (ldrValue < 1000);

  if (!isNight) {
    // DAY MODE: Turn off all LEDs

    Serial.println("Day Mode: All LEDs OFF");
    for (int i = 0; i < 5; i++) {
      ledcWrite(i, 0);
    }
    nightModePrev = false;  // reset flag
  } 
  else {
    // NIGHT MODE

    if (!nightModePrev) {
      Serial.println("Night Mode ON → Buzzing once...");
      digitalWrite(buzzer, HIGH);
      delay(800);  // Buzz for 1.5s
      digitalWrite(buzzer, LOW);
      nightModePrev = true; 
       // now we won't buzz again until next day
    }

    for (int i = 0; i < 5; i++) {
      int sensor_status = digitalRead(IRSensors[i]);

      if (sensor_status == HIGH) {
        // No obstacle for this sensor, keep LED dim
        ledcWrite(i, dimBrightness);
        Serial.print("LED "); Serial.print(i); Serial.println(" Dim (No Obstacle)");
      } 
      else {
        // Obstacle detected → instantly brighten corresponding LED
        Serial.print("Obstacle at IR "); Serial.print(i); Serial.println(" → Bright LED");
        ledcWrite(i, fullBrightness);
      }
    }
  }

  delay(100);  // Prevent overwhelming the loop
}
