#include <Arduino.h>
#include <NeoPixelBus.h>

// Set to the number of LEDs in your LED strip
#define NUM_LEDS 8
// Maximum number of packets to hold in the buffer. Don't change this.
#define BUFFER_LEN 1024
// Toggles FPS output (1 = print FPS over serial, 0 = disable output)
#define PRINT_FPS 0

//NeoPixelBus settings
const uint8_t PixelPin = 6;  // make sure to set this to the correct pin, ignored for Esp8266(set to 3 by default for DMA)

char packetBuffer[BUFFER_LEN];

// LED strip
NeoPixelBus<NeoBrgFeature, Neo400KbpsMethod> ledstrip(NUM_LEDS, PixelPin);

void setup() {
    Serial.begin(115200);
    Serial.println("Ready");
    Serial.setTimeout(2);
    ledstrip.Begin();//Begin output
    ledstrip.Show();//Clear the strip for use
}

uint8_t N = 0;
#if PRINT_FPS
    uint16_t fpsCounter = 0;
    uint32_t secondTimer = 0;
#endif

void loop() {
    
    // If packets have been received, interpret the command
    if (Serial.available()) {
        int len = Serial.readBytes(packetBuffer,BUFFER_LEN);
        //Serial.println(inByte);
        //delay(100);
        for(int i = 0; i < len; i+=4) {
            packetBuffer[len] = 0;
            N = packetBuffer[i];
            RgbColor pixel((uint8_t)packetBuffer[i+1], (uint8_t)packetBuffer[i+2], (uint8_t)packetBuffer[i+3]);
            ledstrip.SetPixelColor(N, pixel);
            Serial.println("OK");
        }
        ledstrip.Show();
        //delay(10);
        #if PRINT_FPS
            fpsCounter++;
            Serial.print("/");//Monitors connection(shows jumps/jitters in packets)
        #endif
    }
    #if PRINT_FPS
        if (millis() - secondTimer >= 1000U) {
            secondTimer = millis();
            //Serial.printf("FPS: %d\n", fpsCounter);
            fpsCounter = 0;
        }   
    #endif
}
