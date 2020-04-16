/*Example sketch to control a stepper motor with A4988 stepper motor driver and Arduino without a library. More info: https://www.makerguides.com */
// Define stepper motor connections and steps per revolution:
#define limitSwitch 2
#define dirPin 3
#define stepPin 4
#define stopSw 5
#define ledG 6
#define ledR 7
#define stepsPerRevolution 200

void setup() {
  Serial.begin(115200);

  pinMode(limitSwitch, INPUT_PULLUP);
  pinMode(stopSw, INPUT_PULLUP);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(ledG, OUTPUT);
  pinMode(ledR, OUTPUT);

  startup();
}

int pot1Reading = 0;
int pot2Reading = 0;
int motorSpeed  = 0;
int upperLimit  = 0;
int stepCount   = 0;

void loop() {
  while(!digitalRead(stopSw)) {
    delay(100);
  }

  if (digitalRead(limitSwitch)) {
    startup();
  }

  stepCount   = 0;
  pot1Reading = analogRead(A0);
  motorSpeed  = map(pot1Reading, 0, 1023, 900, 450);
  pot2Reading = analogRead(A1);
  upperLimit  = map(pot2Reading, 0, 1023, 1000, 1700);

  digitalWrite(ledG, HIGH);
  digitalWrite(ledR, LOW);
  dir_counterclockwise();
  for (stepCount = 0; stepCount < upperLimit; stepCount++) {
    if (stepCount%100 == 0) {
      pot1Reading = analogRead(A0);
      motorSpeed  = map(pot1Reading, 0, 1023, 800, 450);
      pot2Reading = analogRead(A1);
      upperLimit  = map(pot2Reading, 0, 1023, 1000, 1700);
      Serial.print("S");
      Serial.println(motorSpeed);
      Serial.print("L");
      Serial.println(upperLimit);
      Serial.print("C");
      Serial.println(stepCount);
      if (!digitalRead(stopSw)) {
        break;
      }
    }
    one_step(motorSpeed);
  }

  digitalWrite(ledG, LOW);
  digitalWrite(ledR, LOW);
  delay(300);

  digitalWrite(ledG, LOW);
  digitalWrite(ledR, HIGH);
  dir_clockwise();
  while(digitalRead(limitSwitch)) {
    if (stepCount%100 == 0) {
      pot1Reading = analogRead(A0);
      motorSpeed  = map(pot1Reading, 0, 1023, 800, 450);
      pot2Reading = analogRead(A1);
      upperLimit  = map(pot2Reading, 0, 1023, 1000, 1700);
      Serial.print("S");
      Serial.println(motorSpeed);
      Serial.print("L");
      Serial.println(upperLimit);
      Serial.print("C");
      Serial.println(stepCount);
      if (!digitalRead(stopSw)) {
        break;
      }
    }
    one_step(motorSpeed);
    stepCount--;
  }

  digitalWrite(ledG, LOW);
  digitalWrite(ledR, LOW);
  Serial.print("S");
  Serial.println(motorSpeed);
  Serial.print("L");
  Serial.println(upperLimit);
  Serial.print("C");
  Serial.println(stepCount);
  delay(1000);
}

void one_step(int deltime) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(deltime);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(deltime);
}

void dir_clockwise() {
  digitalWrite(dirPin, HIGH);
}

void dir_counterclockwise() {
  digitalWrite(dirPin, LOW);
}

void startup() {
  digitalWrite(dirPin, HIGH);
  delay(100);
  while(digitalRead(limitSwitch)) {
    one_step(500);
  }

  delay(1000);
}
