// ----------- BLYNK SETUP -----------
#define BLYNK_TEMPLATE_ID "TMPL3y7hDEJzn"
#define BLYNK_TEMPLATE_NAME "Nuclear Plant Monitoring"
#define BLYNK_AUTH_TOKEN "wz36uHoughOfF5rg5EJwO1TeUgAnHs67"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// WiFi credentials
const char* ssid = "arijit's M15";
const char* password = "aubr62218";

// ----------- DOPPLER RADAR SETUP -----------
#define HB100_PIN 15  // GPIO15 for Doppler HB100 radar
#define MOTION_SENSITIVITY 10

volatile unsigned long lastPulseTime = 0;
volatile unsigned long pulseInterval = 0;
bool dopplerMotionDetected = false;
int dopplerMotionDelay = 0;
int motionValue = 0;

void IRAM_ATTR onPulse() {
  unsigned long now = micros();
  if (lastPulseTime > 0) {
    pulseInterval = now - lastPulseTime;
  }
  lastPulseTime = now;
}

// ----------- OTHER SENSORS SETUP -----------
#define DO_PIN 13       // Flame sensor digital output
#define BUZZER_PIN 14   // Buzzer pin
#define DHTPIN 32       // DHT sensor pin
#define DHTTYPE DHT11
#define MQ2_PIN 34      // Smoke sensor analog input
#define LED_PIN 2       // LED pin

DHT dht(DHTPIN, DHTTYPE);

// Geiger counter variables
#define integratingTime 15000
unsigned long counts = 0;
unsigned long events = 0;
unsigned long cpm = 0;
unsigned int multiplier = 4;
unsigned long previousMillis = 0;
double avgCounts = 0;
double avgCPM = 0;
double avgUSV = 0;
double sumCPM = 0;
double sumUSV = 0;
double uSv = 0;
double dose = 0;

BlynkTimer timer;

void tube_impulse() {
  counts++;
  events++;
}

// ----------- SETUP -----------
void setup() {
  Serial.begin(115200);

  pinMode(HB100_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(HB100_PIN), onPulse, RISING);

  pinMode(DO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);

  dht.begin();
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, password);

  attachInterrupt(digitalPinToInterrupt(14), tube_impulse, FALLING);

  timer.setInterval(100L, updateMeasurementData);
  Serial.println("Connecting to WiFi...");
}

// ----------- MAIN LOOP -----------
void loop() {
  Blynk.run();
  timer.run();

  // Doppler motion sense and process
  senseAndDelay(250);
  updateMotionStatus();

  if (dopplerMotionDetected) {
    Serial.println("Doppler motion was detected!");
    Blynk.virtualWrite(V5, 255);  // Human detected → turn ON Blynk LED
  } else {
    Blynk.virtualWrite(V5, 0);    // No human detected → turn OFF Blynk LED
  }

  // Flame detection
  int flame_state = digitalRead(DO_PIN);
  if (flame_state == LOW) {
    digitalWrite(BUZZER_PIN, HIGH);
    delay(500);
    digitalWrite(BUZZER_PIN, LOW);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }

  // DHT11 reading
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int smokeLevel = analogRead(MQ2_PIN);

  if (!isnan(temperature) && !isnan(humidity)) {
    Serial.print("Temp: ");
    Serial.print(temperature);
    Serial.print(" C, Humidity: ");
    Serial.print(humidity);
    Serial.print(" %, Smoke: ");
    Serial.println(smokeLevel);

    Blynk.virtualWrite(V0, temperature);  // Temperature → V0
    Blynk.virtualWrite(V4, humidity);     // Humidity → V4
    Blynk.virtualWrite(V6, smokeLevel);   // Smoke → V6
  }
}

// ----------- GEIGER COUNTER DATA HANDLER -----------
void updateMeasurementData() {
  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis > integratingTime) {
    avgCounts++;
    previousMillis = currentMillis;
    cpm = counts * multiplier;
    uSv = (cpm / 151.0);

    sumCPM += cpm;
    sumUSV += uSv;

    avgCPM = sumCPM / avgCounts;
    avgUSV = sumUSV / avgCounts;

    Blynk.virtualWrite(V1, counts);   // Count per second → V1
    Blynk.virtualWrite(V2, cpm);      // CPM → V2
    Blynk.virtualWrite(V3, avgCPM);   // Hourly average CPM → V3

    Serial.print("CPM: ");
    Serial.print(cpm);
    Serial.print(", Avg CPM: ");
    Serial.println(avgCPM);

    counts = 0;
  }
}

// ----------- DOPPLER FUNCTIONS -----------
void updateMotionStatus() {
  if (motionValue > MOTION_SENSITIVITY) {
    dopplerMotionDetected = true;
    dopplerMotionDelay = 0;
  }

  if (dopplerMotionDetected) {
    if (dopplerMotionDelay < 5) {
      dopplerMotionDelay++;
    } else {
      dopplerMotionDetected = false;
    }
  }

  motionValue = 0;
}

void senseAndDelay(unsigned long duration) {
  unsigned long start = millis();

  while (millis() - start < duration) {
    int dopplerReturn = readDoppler();
    if (motionValue < dopplerReturn) {
      motionValue = dopplerReturn;
    }
  }
}

int readDoppler() {
  noInterrupts();
  unsigned long interval = pulseInterval;
  interrupts();

  if (interval > 0) {
    int frequency = 1000000 / interval;
    return frequency;
  }
  return 0;
}