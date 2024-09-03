#include <Wire.h>
#include <BlynkSimpleEsp32.h>
#include <RBDdimmer.h>

#define outputPin  14  // PWM pin for dimmer on ESP32
#define outputPin2 4   // PWM pin for dimmer on ESP32
#define outputPin3 5   // PWM pin for dimmer on ESP32
#define outputPin4 18  // PWM pin for dimmer on ESP32
#define zerocross  15  // for boards with CHANGEABLE input pins

dimmerLamp dimmer(outputPin, zerocross); // Initialize port for dimmer for ESP32
dimmerLamp dimmer2(outputPin2, zerocross); // Initialize port for dimmer for ESP32
dimmerLamp dimmer3(outputPin3, zerocross); // Initialize port for dimmer for ESP32
dimmerLamp dimmer4(outputPin4, zerocross); // Initialize port for dimmer for ESP32

int outVal = 0;  // Initialization value for dimmer
int outVal2 = 0; // Initialization value for dimmer
int outVal3 = 0; // Initialization value for dimmer
int outVal4 = 0; // Initialization value for dimmer
int dim_val, dim_val2, dim_val3, dim_val4; // Dimmer value to dim the appliance

#define LDR_PIN 34
#define BUTTON_MANUAL_PIN 19
#define BUTTON_AUTO_PIN 37
#define MANUAL_CONTROL_PIN 12 // Pin untuk tombol manual tambahan di aplikasi Blynk
#define RELAY_PIN1 16
#define RELAY_PIN2 17
#define RELAY_PIN3 13
#define RELAY_PIN4 27
const int PIRPin1 = 25; // the number of the pushbutton pin
const int PIRPin2 = 26; // the number of the pushbutton pin

char auth[] = "GA371t8Fyyvi7iW0F3swefRn35m9gNHb";  // Blynk auth key 

char ssid[] = "Memey";   // Wifi Name
char pass[] = "apaya69.";  // Wifi Password

BlynkTimer timer;

bool manualMode = false;
bool manualControl = false; // Variabel untuk mengontrol manual dari aplikasi Blynk
int PIRState1 = 0;
int PIRState2 = 0;

void setup() {
  Serial.begin(115200);
  
  pinMode(PIRPin1, INPUT);
  pinMode(PIRPin2, INPUT);
  dimmer.begin(NORMAL_MODE, ON); dimmer2.begin(NORMAL_MODE, ON); dimmer3.begin(NORMAL_MODE, ON); dimmer4.begin(NORMAL_MODE, ON); // Dimmer initialization: name.begin(MODE, STATE)
  Blynk.begin(auth, ssid, pass, "iot.serangkota.go.id", 8080);

  pinMode(LDR_PIN, INPUT);
  pinMode(BUTTON_MANUAL_PIN, INPUT_PULLUP);
  pinMode(BUTTON_AUTO_PIN, INPUT_PULLUP);
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(RELAY_PIN4, OUTPUT);
  
  timer.setInterval(1000L, checkButtons);
}

void loop() {
  Blynk.run();
  timer.run();

}
void checkButtons() {
  int ldrValue = analogRead(LDR_PIN);
  Blynk.virtualWrite(V0, ldrValue); // Sending to Blynk
  Serial.print(ldrValue);
  Serial.println(" ldr=");

  if (digitalRead(BUTTON_MANUAL_PIN) == LOW && digitalRead(MANUAL_CONTROL_PIN) == HIGH) {
    Serial.println("SUKSES OTOMATIS");

  }

  if (digitalRead(BUTTON_MANUAL_PIN) == HIGH && digitalRead(MANUAL_CONTROL_PIN) == LOW) {
    if (ldrValue > 2000) {
      digitalWrite(RELAY_PIN3, LOW); // Turn off the light during daytime 
      digitalWrite(RELAY_PIN4, LOW); // Turn off the light during daytime 
    } else {
      digitalWrite(RELAY_PIN3, HIGH); // Turn on the light at night
      digitalWrite(RELAY_PIN4, HIGH); // Turn on the light at night
    }

    PIRState1 = digitalRead(PIRPin1);
    PIRState2 = digitalRead(PIRPin2);

    if (PIRState1 == HIGH) {
      digitalWrite(RELAY_PIN1, HIGH); // Turn on the light if PIR detected motion
    } else {
      digitalWrite(RELAY_PIN1, LOW); // Turn off the light if no motion
    }

    if (PIRState2 == HIGH) {
      digitalWrite(RELAY_PIN2, HIGH); // Turn on the light if PIR detected motion
    } else {
      digitalWrite(RELAY_PIN2, LOW); // Turn off the light if no motion
    }
  }

  if (digitalRead(MANUAL_CONTROL_PIN) == LOW) {
    // Additional manual control logic can be added here
  }
  
}

BLYNK_WRITE(V1) {
  outVal = param.asInt();
  dim_val = map(outVal, 0, 1023, 0, 100);
  dimmer.setPower(dim_val);
}

BLYNK_WRITE(V2) {
  outVal2 = param.asInt();
  dim_val2 = map(outVal2, 0, 1023, 0, 100);
  dimmer2.setPower(dim_val2);
}

BLYNK_WRITE(V3) {
  outVal3 = param.asInt();
  dim_val3 = map(outVal3, 0, 1023, 0, 100);
  dimmer3.setPower(dim_val3);
}

BLYNK_WRITE(V4) {
  outVal4 = param.asInt();
  dim_val4 = map(outVal4, 0, 1023, 0, 100);
  dimmer4.setPower(dim_val4);
}
BLYNK_WRITE(V5) {
  int timerValue = param.asInt(); // 1 if the timer is ON, 0 if the timer is OFF

  if (timerValue == 1 && digitalRead(BUTTON_MANUAL_PIN) == LOW && digitalRead(MANUAL_CONTROL_PIN) == LOW) {
    // Turn on relays based on your logic
    digitalWrite(RELAY_PIN1, LOW);
    digitalWrite(RELAY_PIN2, LOW);
    digitalWrite(RELAY_PIN3, LOW);
    digitalWrite(RELAY_PIN4, LOW);

    Serial.println("Relays turned ON by timer");
  } else if (timerValue == 0 && digitalRead(BUTTON_MANUAL_PIN) == LOW && digitalRead(MANUAL_CONTROL_PIN) == LOW) {
    // Turn off relays
     digitalWrite(RELAY_PIN1, HIGH);
    digitalWrite(RELAY_PIN2, HIGH);
    digitalWrite(RELAY_PIN3, HIGH);
    digitalWrite(RELAY_PIN4, HIGH);
    Serial.println("Relays turned OFF by timer");
  }
}
