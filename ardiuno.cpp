#include <Wire.h>
#include <Adafruit_APDS9960.h>
#include <Adafruit_NeoPixel.h>

#define LED_PIN 6      // LED Strip Pin
#define LED_COUNT 10   // Number of LEDs

Adafruit_APDS9960 apds;
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

int brightness = 100;  // Initial brightness
int colorIndex = 0;
uint32_t colors[] = {strip.Color(255, 0, 0), strip.Color(0, 255, 0), strip.Color(0, 0, 255), strip.Color(255, 255, 0)};

void setup() {
    Serial.begin(115200);
    strip.begin();
    strip.show();
    
    if (!apds.begin()) {
        Serial.println("Failed to initialize APDS-9960 sensor!");
        while (1);
    }

    apds.enableGesture(true);
}

void loop() {
    if (apds.gestureAvailable()) {
        int gesture = apds.readGesture();
        
        switch (gesture) {
            case APDS9960_UP:
                brightness = min(255, brightness + 50);
                Serial.println("Brightness Up");
                break;

            case APDS9960_DOWN:
                brightness = max(50, brightness - 50);
                Serial.println("Brightness Down");
                break;

            case APDS9960_LEFT:
                colorIndex = (colorIndex - 1 + 4) % 4;
                Serial.println("Color Left");
                break;

            case APDS9960_RIGHT:
                colorIndex = (colorIndex + 1) % 4;
                Serial.println("Color Right");
                break;
        }
        
        updateLEDs();
    }
}

void updateLEDs() {
    for (int i = 0; i < LED_COUNT; i++) {
        strip.setPixelColor(i, colors[colorIndex]);
    }
    strip.setBrightness(brightness);
    strip.show();
}
