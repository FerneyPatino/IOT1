#define BLYNK_TEMPLATE_ID "TMPL23d3r-AUu"
#define BLYNK_TEMPLATE_NAME "IOT"
#define BLYNK_AUTH_TOKEN "QfsDOBY5wwuv70_3ENEYPQOjhXhyUrlh"

#include "ROBLEX.h"
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <HTTPClient.h>

char ssid[] = "iPhone de Fer";
char pass[] = "87654321";

#define BUZZER_PIN pin3A
#define VIBRADOR_PIN 26
#define LED_PIN 19

#define BUZZER_FREQ 2000
#define BUZZER_RES 8

ROBLEX ROBLEX;
bool alarmaActiva = false;

BLYNK_WRITE(V0) {
  int estado = param.asInt();
  alarmaActiva = (estado == 1);
  if (!alarmaActiva) {
    digitalWrite(LED_PIN, LOW);
    ledcWriteTone(BUZZER_PIN, 0);
    Blynk.virtualWrite(V1, 0);
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(VIBRADOR_PIN, INPUT);
  pinMode(LED_PIN, OUTPUT);
  ledcAttach(BUZZER_PIN, BUZZER_FREQ, BUZZER_RES);
  ledcWriteTone(BUZZER_PIN, 0);
  digitalWrite(LED_PIN, LOW);
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);
}

void loop() {
  Blynk.run();

  if (alarmaActiva) {
    if (digitalRead(VIBRADOR_PIN) == HIGH) {
      digitalWrite(LED_PIN, HIGH);
      Blynk.virtualWrite(V1, 255);
      ledcWriteTone(BUZZER_PIN, 2000);
      Blynk.logEvent("vib_det", "🚨 Vibración detectada en el sistema");
      Serial.println("💥 Vibración detectada - Alarma SONANDO");
      delay(1000);
      digitalWrite(LED_PIN, LOW);
      Blynk.virtualWrite(V1, 0);
      ledcWriteTone(BUZZER_PIN, 0);
      HTTPClient http;
      http.begin("https://script.google.com/macros/s/AKfycbwP8p5qDid4zmRlydOh7qiqIofaEhgV7qB9o32f7zN8huH-CK48-4fgXtfFf-6tW7I/exec?sensor=1");
      http.GET();
      http.end();
    } else {
      digitalWrite(LED_PIN, LOW);
      Blynk.virtualWrite(V1, 0);
      ledcWriteTone(BUZZER_PIN, 0);
    }
  } else {
    digitalWrite(LED_PIN, LOW);
    Blynk.virtualWrite(V1, 0);
    ledcWriteTone(BUZZER_PIN, 0);
  }

  delay(10);
}
