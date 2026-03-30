/*
  Arduino ADC Voltage-Range Indicator

  LEDs behavior:
  - Yellow ON when Vin < 2.5 V
  - Green  ON when 2.5 V <= Vin <= 3.5 V
  - Red    ON when Vin > 3.5 V
*/

const int POT_PIN = A0;
const int LED_YELLOW = 8;
const int LED_GREEN  = 9;
const int LED_RED    = 10;

const float VREF = 5.0;      // ADC reference voltage for Arduino Uno
const int ADC_MAX = 1023;    // 10-bit ADC max value

const float THRESH_LOW_V = 2.5;
const float THRESH_HIGH_V = 3.5;

int thresholdLowCount;
int thresholdHighCount;

void setup() {
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);

  // Convert voltage thresholds to ADC counts once at startup
  thresholdLowCount  = (int)((THRESH_LOW_V / VREF) * ADC_MAX + 0.5);
  thresholdHighCount = (int)((THRESH_HIGH_V / VREF) * ADC_MAX + 0.5);

  Serial.begin(9600);
}

void turnOnly(bool yellow, bool green, bool red) {
  digitalWrite(LED_YELLOW, yellow ? HIGH : LOW);
  digitalWrite(LED_GREEN,  green  ? HIGH : LOW);
  digitalWrite(LED_RED,    red    ? HIGH : LOW);
}

void loop() {
  int adcValue = analogRead(POT_PIN);
  float vin = (adcValue * VREF) / ADC_MAX;

  if (adcValue < thresholdLowCount) {
    turnOnly(true, false, false);   // Yellow
  } else if (adcValue <= thresholdHighCount) {
    turnOnly(false, true, false);   // Green
  } else {
    turnOnly(false, false, true);   // Red
  }

  // Optional serial monitor output for debugging/calibration
  Serial.print("ADC=");
  Serial.print(adcValue);
  Serial.print("  Vin=");
  Serial.println(vin, 3);

  delay(100);
}
