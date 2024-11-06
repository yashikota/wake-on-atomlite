#include <FastLED.h>
#include <HTTPClient.h>
#include <M5Unified.h>
#include <WiFi.h>

// LED
const int NUM_LEDS = 1;
const int LED_DATA_PIN = 27;

// WiFi
const char* ssid = "";
const char* pass = "";

const char* url = "";
const char* macAddress = "";

CRGB leds[NUM_LEDS];

void resetLED() {
    leds[0] = CRGB::Blue;
    FastLED.show();
}

void setup() {
    M5.begin();

    // LED initialize
    FastLED.addLeds<WS2811, LED_DATA_PIN, GRB>(leds, NUM_LEDS);
    FastLED.setBrightness(20);

    // WiFi initialize
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        leds[0] = CRGB::Red;
        FastLED.show();
        delay(1000);
        leds[0] = CRGB::Black;
        FastLED.show();
        delay(1000);
        Serial.println("Connecting...");
    }

    Serial.println("WiFi Connected");
    resetLED();
}

bool wakeOnLAN() {
    HTTPClient http;

    http.begin(url);
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    String postData = "macSelect=" + String(macAddress);
    int httpResponseCode = http.POST(postData);

    if (httpResponseCode > 0) {
        String response = http.getString();
        Serial.println("WOL Response: " + response);
        http.end();
        return true;
    } else {
        Serial.println("Error on sending POST: " + String(httpResponseCode));
        http.end();
        return false;
    }
}

void loop() {
    M5.update();

    if (M5.BtnA.isPressed()) {
        leds[0] = CRGB::Yellow;
        FastLED.show();

        // Wake On LAN
        if (wakeOnLAN()) {
            // Success
            leds[0] = CRGB::Green;
            FastLED.show();
            delay(2000);
        } else {
            // Fail
            leds[0] = CRGB::Red;
            FastLED.show();
            delay(2000);
        }
    }

    resetLED();
}
