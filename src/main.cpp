/******************************************
 * Arcade Table RGB Presets Arduino Code
 * Author: Cody Carter
 * Credit: For inspiration for unique RGB effects,
 *         https://www.tweaking4all.com/hardware/arduino/adruino-led-strip-effects/#LEDStripEffectCylon
 * Date: July 2025
 * Version: 1.0
 * 
 * This modular firmware handles the control of individually addressable 
 * LED strips on an arcade table. Can be modified for any addition of 
 * RGB presets.
 ******************************************/

#include <Adafruit_NeoPixel.h>
#include <WebServer.h>
#include <SPIFFS.h>
#include <WiFi.h>
#include <Bounce2.h>
#include <Preferences.h>

// LED and momentary switch pin declaration
#define LED_PIN 13
#define switch1 16
#define switch2 17
#define switch3 18
#define switch4 19
#define switch5 21
#define switch6 22
#define switch7 23
#define switch8 25

// Switch debouncer declaration
Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
Bounce debouncer4 = Bounce();
Bounce debouncer5 = Bounce();
Bounce debouncer6 = Bounce();
Bounce debouncer7 = Bounce();
Bounce debouncer8 = Bounce();

// Solenoid number dial declaration
#define dial1 26
#define dial2 27
#define dial3 32

// Default case for switch presets, off in this case
int button1Preset = 0;
int button2Preset = 0;
int button3Preset = 0;
int button4Preset = 0;
int button5Preset = 0;
int button6Preset = 0;
int button7Preset = 0;
int button8Preset = 0;

// Object of Preferences library, allows for flash storage of presets
// that load saved presets
Preferences preferences;

// Amount of LEDs on strip
#define LED_COUNT 300

//Credentials for access point
const char *ap_ssid = "ArcadeTableRGB";
const char *ap_password = "rgbpassword";

// Web server initialization
WebServer server(80);
volatile int currentPreset = 0;
volatile bool interruptFlag = false;

uint8_t activeEffect = 0; // 0 = none, 1 = rainbow, 2 = theater chase, etc.

int currentEffect = 0;          // current effect index
int effectStep = 0;             // current step within the effect
unsigned long lastUpdate = 0;   // last time step was updated


// Handle preset selection
void handlePreset() {
  if (server.hasArg("preset")) {
    int preset = server.arg("preset").toInt();
    currentPreset = preset;
    interruptFlag = true;
    Serial.printf("Preset %d selected\n", preset);
    server.send(200, "text/plain", "Preset " + String(preset) + " selected");
  } else {
    server.send(400, "text/plain", "Missing preset parameter");
  }
}

// Opens html with SPIFFS
void handleRoot() {
  File file = SPIFFS.open("/index.html", "r");
  if (!file) {
    server.send(500, "text/plain", "Failed to open index");
    return;
  }
  server.streamFile(file, "text/html");
  file.close();
}

// Declare our NeoPixel strip object:
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
// Argument 1 = Number of pixels in NeoPixel strip
// Argument 2 = Arduino pin number (most are valid)
// Argument 3 = Pixel type flags, add together as needed:

// Handles user setting presets on HTML webpage to RGB functions
void handleAssignPreset() {
  if (server.hasArg("button") && server.hasArg("preset")) {
      int button = server.arg("button").toInt();
      int preset = server.arg("preset").toInt();
      String key = "btn" + String(button);

      preferences.putInt(key.c_str(), preset);

      switch (button) {
          case 1: button1Preset = preset; break;
          case 2: button2Preset = preset; break;
          case 3: button3Preset = preset; break;
          case 4: button4Preset = preset; break;
          case 5: button5Preset = preset; break;
          case 6: button6Preset = preset; break;
          case 7: button7Preset = preset; break;
          case 8: button8Preset = preset; break;
          default: break;
      }

      Serial.printf("Button %d preset updated to %d\n", button, preset);
      server.send(200, "text/plain", "Button " + String(button) + " updated to preset " + String(preset));
  } else {
      server.send(400, "text/plain", "Missing button or preset parameter");
  }
}

// Non blocking delay, handles interrupt caused by pressing another button
// Allows webpage and RGB functions to function smoothly without stalling
bool nonBlockingDelay(unsigned long ms) {
  unsigned long start = millis();
  while (millis() - start < ms) {
      server.handleClient();       // allow web commands
      debouncer1.update();
      debouncer2.update();
      debouncer3.update();
      debouncer4.update();
      debouncer5.update();
      debouncer6.update();
      debouncer7.update();
      debouncer8.update();

      if (interruptFlag) {
          return false; // interrupt occurred
      }
      delay(1); // yield briefly
  }
  return true; // completed without interrupt
}

void setup() {
  preferences.begin("arcade", false); // Initializes reading and writing of preset flash data in namespace 'arcade'

  Serial.begin(115200);

  // Retrieve saved presets from flash or use default 0
  button1Preset = preferences.getInt("btn1", 0);
  button2Preset = preferences.getInt("btn2", 0);
  button3Preset = preferences.getInt("btn3", 0);
  button4Preset = preferences.getInt("btn4", 0);
  button5Preset = preferences.getInt("btn5", 0);
  button6Preset = preferences.getInt("btn6", 0);
  button7Preset = preferences.getInt("btn7", 0);
  button8Preset = preferences.getInt("btn8", 0);

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch3, INPUT_PULLUP);
  pinMode(switch4, INPUT_PULLUP);
  pinMode(switch5, INPUT_PULLUP);
  pinMode(switch6, INPUT_PULLUP);
  pinMode(switch7, INPUT_PULLUP);
  pinMode(switch8, INPUT_PULLUP);

  pinMode(dial1, OUTPUT);
  pinMode(dial2, OUTPUT);
  pinMode(dial3, OUTPUT);

  // Counter for how many times the arcade machine is booted up
  digitalWrite(dial1, HIGH);
  delay(200);
  digitalWrite(dial1, LOW);

  // Accounts for button spikes
  debouncer1.attach(switch1);
  debouncer1.interval(25); // Debounce time

  debouncer2.attach(switch2);
  debouncer2.interval(25); // Debounce time

  debouncer3.attach(switch3);
  debouncer3.interval(25); // Debounce time

  debouncer4.attach(switch4);
  debouncer4.interval(25); // Debounce time

  debouncer5.attach(switch5);
  debouncer5.interval(25); // Debounce time

  debouncer6.attach(switch6);
  debouncer6.interval(25); // Debounce time

  debouncer7.attach(switch7);
  debouncer7.interval(25); // Debounce time

  debouncer8.attach(switch8);
  debouncer8.interval(25); // Debounce time
  
  // Sets up ESP32 as an access point, no external network required
  WiFi.softAP(ap_ssid, ap_password);
  Serial.println("ESP32 AP started");
  Serial.print("IP address: "); // To view, type in google, http://IP-Address while connected to hostname
  Serial.println(WiFi.softAPIP());

  // Mounts SPIFFS for application of html, css, and font files in data folder
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }
 
  server.on("/", handleRoot); // Listens for HTTP GET request
  server.on("/setPreset", handlePreset); // Listens for HTTP GET requests to /setPreset
  server.on("/assignPreset", handleAssignPreset); // Listens for HTTP GET requests to /assignPreset

// Serve pico.min.css
server.on("/pico.min.css", HTTP_GET, []() {
    File file = SPIFFS.open("/pico.min.css", "r");
    if (!file) {
        server.send(404, "text/plain", "File not found");
        return;
    }
    server.streamFile(file, "text/css");
    file.close();
});

// Serve pico.colors.min.css
server.on("/pico.colors.min.css", HTTP_GET, []() {
    File file = SPIFFS.open("/pico.colors.min.css", "r");
    if (!file) {
        server.send(404, "text/plain", "File not found");
        return;
    }
    server.streamFile(file, "text/css");
    file.close();
});

// Serve the SfAtarianSystem font
server.on("/SfAtarianSystem-d5LR.ttf", HTTP_GET, []() {
    File file = SPIFFS.open("/SfAtarianSystem-d5LR.ttf", "r");
    if (!file) {
        server.send(404, "text/plain", "File not found");
        return;
    }
    server.streamFile(file, "font/ttf");
    file.close();
});

// Serve the MrDafoe-Regular font
server.on("/MrDafoe-Regular.ttf", HTTP_GET, []() {
  File file = SPIFFS.open("/MrDafoe-Regular.ttf", "r");
  if (!file) {
      server.send(404, "text/plain", "File not found");
      return;
  }
  server.streamFile(file, "font/ttf");
  file.close();
});

// Serve the PressStart2P-Regular.ttf font
server.on("/PressStart2P-Regular.ttf", HTTP_GET, []() {
  File file = SPIFFS.open("/PressStart2P-Regular.ttf", "r");
  if (!file) {
      server.send(404, "text/plain", "File not found");
      return;
  }
  server.streamFile(file, "font/ttf");
  file.close();
});

server.begin(); // Begin HTTP server
Serial.println("HTTP server started");

// Fetches saved presets from flash and sends it to server buttons
server.on("/getPresets", HTTP_GET, []() {
  String json = "{";
  json += "\"btn1\":" + String(button1Preset) + ",";
  json += "\"btn2\":" + String(button2Preset) + ",";
  json += "\"btn3\":" + String(button3Preset) + ",";
  json += "\"btn4\":" + String(button4Preset) + ",";
  json += "\"btn5\":" + String(button5Preset) + ",";
  json += "\"btn6\":" + String(button6Preset) + ",";
  json += "\"btn7\":" + String(button7Preset) + ",";
  json += "\"btn8\":" + String(button8Preset);
  json += "}";
  server.send(200, "application/json", json);
});
}

// Shows effect assigned onto strip
void showStrip() {
  strip.show();
}

// Sets color for specific pixel on strip
void setPixel(int Pixel, byte red, byte green, byte blue) {
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

// Sets all pixels to same color set by byte
void setAll(byte red, byte green, byte blue) {
  for(int i = 0; i < LED_COUNT; i++ ) {
    setPixel(i, red, green, blue);
  }
  showStrip();
} 

// Fades pixel to nothing over time, used in meteorRain
void fadeToBlack(int ledNo, byte fadeValue) {
  uint32_t oldColor;
  uint8_t r, g, b;

  oldColor = strip.getPixelColor(ledNo);
  r = (oldColor & 0x00ff0000UL) >> 16;
  g = (oldColor & 0x0000ff00UL) >> 8;
  b = (oldColor & 0x000000ffUL);

  r = (r <= 10) ? 0 : (int) r - (r * fadeValue / 256);
  g = (g <= 10) ? 0 : (int) g - (g * fadeValue / 256);
  b = (b <= 10) ? 0 : (int) b - (b * fadeValue / 256);

  strip.setPixelColor(ledNo, r, g, b);
}

// Rainbow effect
int rainbowWait = 20; // speed of animation 
void rainbow() {
  if (millis() - lastUpdate >= rainbowWait) {
      lastUpdate = millis();
      strip.rainbow(effectStep);
      strip.show();
      effectStep += 256;
      if (effectStep >= 5 * 65536) {
          effectStep = 0; // loop back
      }
  }
}

// Theatre chase effect
int chaseWait = 100; // Sets animation speed
void theaterChaseRainbow() {
  static int firstPixelHue = 0;
  static int b = 0;
  static int a = 0;

  if (millis() - lastUpdate >= chaseWait) {
      lastUpdate = millis();
      strip.clear();
      for (int c = b; c < strip.numPixels(); c += 3) {
          int hue = firstPixelHue + c * 65536L / strip.numPixels();
          uint32_t color = strip.gamma32(strip.ColorHSV(hue));
          strip.setPixelColor(c, color);
      }
      strip.show();

      b = (b + 1) % 3;
      if (b == 0) {
          firstPixelHue += 65536 / 90;
          a++;
          if (a >= 30) {
              a = 0; // restart animation
          }
      }
  }
}

// Smooth rainbow shift
unsigned long lastRgbSolidUpdate = 0;
int solidWait = 10; // Animation speed
long rgbSolidFirstPixelHue = 0; 

void rgbSolid() {
  if (millis() - lastRgbSolidUpdate >= solidWait) {
    lastRgbSolidUpdate = millis();

    for (int i = 0; i < strip.numPixels(); i++) {
      int hue = rgbSolidFirstPixelHue + i * 65536L / strip.numPixels();
      uint32_t color = strip.gamma32(strip.ColorHSV(hue)); // hue -> RGB
      strip.setPixelColor(i, color);
    }

    strip.show(); // update LEDs
    rgbSolidFirstPixelHue += 256; // advance rainbow position

    if (rgbSolidFirstPixelHue >= 65536) {
      rgbSolidFirstPixelHue = 0; // wrap
    }
  }
}


// Shooting star effect
int shootingStarPos = 0;
long shootingStarFirstPixelHue = 0;
unsigned long lastShootingStarUpdate = 0;
int shootingStarWait = 20;   // adjust speed
int shootingStarTailLength = 10;
int shootingStarBrightness = 255;

void shootingStar() {
    if (millis() - lastShootingStarUpdate >= shootingStarWait) {
        lastShootingStarUpdate = millis();

        strip.clear();
        for (int j = 0; j < shootingStarTailLength; j++) { // tail pixels
            int pos = shootingStarPos - j;
            if (pos >= 0 && pos < strip.numPixels()) {
                int hue = shootingStarFirstPixelHue + pos * 65536L / strip.numPixels();
                int tailBrightness = shootingStarBrightness - (shootingStarBrightness / shootingStarTailLength) * j;
                uint32_t color = strip.gamma32(strip.ColorHSV(hue, 255, tailBrightness));
                strip.setPixelColor(pos, color);
            }
        }

        strip.show();

        shootingStarPos++;

        if (shootingStarPos >= strip.numPixels() + shootingStarTailLength) {
            shootingStarPos = 0;
            shootingStarFirstPixelHue += 65536 / 90;
            if (shootingStarFirstPixelHue >= 65536) {
                shootingStarFirstPixelHue = 0;
            }
        }
    }
}

// Meteor rain effect
int meteorIndex = 0;
long lastMeteorUpdate = 0;
bool meteorDirection = true; // true = forward, false = reset
byte meteorRed = 255; // Red brightness
byte meteorGreen = 255; // Green brightness
byte meteorBlue = 255; // Blue brightness
int meteorSize = 3; // Size of meteor
int meteorTrailDecay = 64; // Speed of tail decay
boolean meteorRandomDecay = true;
int meteorSpeedDelay = 50; // Animation speed
void meteorRain() {
  if (millis() - lastMeteorUpdate >= meteorSpeedDelay) {
      lastMeteorUpdate = millis();

      // Fade all LEDs
      for (int j = 0; j < strip.numPixels(); j++) {
          if ((!meteorRandomDecay) || (random(10) > 5)) {
              fadeToBlack(j, meteorTrailDecay);
          }
      }

      // Draw meteor
      for (int j = 0; j < meteorSize; j++) {
          int pos = meteorIndex - j;
          if (pos >= 0 && pos < strip.numPixels()) {
              setPixel(pos, meteorRed, meteorGreen, meteorBlue);
          }
      }

      showStrip();

      meteorIndex++;

      if (meteorIndex >= strip.numPixels() + meteorSize) {
          meteorIndex = 0;
      }
  }
}

// RunningLights effect
int runningLightsPosition = 0;
unsigned long lastRunningLightsUpdate = 0;
int runningLightsDelay = 50; // Animation speed
void RunningLights() {
  if (millis() - lastRunningLightsUpdate < runningLightsDelay) return;
  lastRunningLightsUpdate = millis();

  float speed = 0.2;          
  float phaseShift = 3.14;

  runningLightsPosition++;
  for (int i = 0; i < strip.numPixels(); i++) {
      float level1 = (sin((i + runningLightsPosition) * speed) * 127.5 + 127.5) / 255.0;
      float r1 = level1 * 128;
      float g1 = 0;
      float b1 = level1 * 128;

      float level2 = (sin((i + runningLightsPosition) * speed + phaseShift) * 127.5 + 127.5) / 255.0;
      float r2 = 0;
      float g2 = 0;
      float b2 = level2 * 255;

      byte r = min(r1 + r2, 255.0f);
      byte g = min(g1 + g2, 255.0f);
      byte b = min(b1 + b2, 255.0f);

      setPixel(i, r, g, b);
  }
  showStrip();
}

// Night Rider Scanner effect
int scannerPosition = 0;
int scannerDirection = 1; // 1 = forward, -1 = backward
unsigned long lastScannerUpdate = 0;
int scannerDelay = 10; // Animation speed
uint32_t scannerColor = strip.Color(255, 0, 0); // default red
void scanner() {
  if (millis() - lastScannerUpdate < scannerDelay) return;
  lastScannerUpdate = millis();

  setAll(0, 0, 0);
  strip.setPixelColor(scannerPosition, scannerColor);
  strip.show();

  scannerPosition += scannerDirection;
  if (scannerPosition >= strip.numPixels()) {
      scannerPosition = strip.numPixels() - 1;
      scannerDirection = -1;
  } else if (scannerPosition < 0) {
      scannerPosition = 0;
      scannerDirection = 1;
  }
}

// Random rainbow twinkle effect
unsigned long lastTwinkleUpdate = 0;
int twinkleDelay = 50; // Animation speed
void twinkleStars() {
  if (millis() - lastTwinkleUpdate < twinkleDelay) return;
  lastTwinkleUpdate = millis();

  int pixel = random(strip.numPixels());
  int hue = random(0, 65535);
  int brightness = 200;
  uint32_t color = strip.gamma32(strip.ColorHSV(hue, 255, brightness));
  strip.setPixelColor(pixel, color);
  fadeToBlack(pixel, 32);
  strip.show();
}

// Sets all LEDs to red
void red()
{
  setAll(255, 0, 0);
}

// Sets all LEDs to blue
void blue()
{
  setAll(0, 0, 255);
}

// Sets all LEDs to green
void green()
{
  setAll(0, 255, 0);
}

// Sets all LEDs to yellow
void yellow()
{
  setAll(255, 255, 0);
}

// Sets all LEDs to cyan
void cyan()
{
  setAll(0, 255, 255);
}

// Sets all LEDs to magenta
void magenta()
{
  setAll(255, 0, 255);
}

// LSU theatre chase effect
int lsuCycle = 0;
int lsuQ = 0;
unsigned long lastLSUUpdate = 0;
int lsuDelay = 200; // Animation speed
void LSU() {
  if (millis() - lastLSUUpdate < lsuDelay) return;
  lastLSUUpdate = millis();

  // Alternates between purple and yellow every third pixel
  uint8_t r = (lsuCycle % 2 == 0) ? 255 : 255;
  uint8_t g = (lsuCycle % 2 == 0) ? 0 : 255;
  uint8_t b = (lsuCycle % 2 == 0) ? 255 : 0;

  for (int i = 0; i < strip.numPixels(); i += 3) {
      setPixel(i + lsuQ, r, g, b);
  }
  showStrip();

  for (int i = 0; i < strip.numPixels(); i += 3) {
      setPixel(i + lsuQ, 0, 0, 0);
  }

  lsuQ++;
  if (lsuQ >= 3) {
      lsuQ = 0;
      lsuCycle++;
  }
}

void loop() {
  // Handles webpage clients and preset changes each loop iteration
  server.handleClient();

  // Checks button state each loop iteration
  debouncer1.update();
  debouncer2.update();
  debouncer3.update();
  debouncer4.update();
  debouncer5.update();
  debouncer6.update();
  debouncer7.update();
  debouncer8.update();

  // Button 1 pressed
  if (debouncer1.fell()) {
        currentPreset = button1Preset;
        effectStep = 0;
        Serial.println("Button 1 pressed: activating preset " + String(button1Preset));
        preferences.putInt("btn1", button1Preset);
        // Counter for how many times the arcade machine runs lights
        digitalWrite(dial2, LOW);
        delay(200);
        digitalWrite(dial2, HIGH);
  }

  // Button 2 pressed
  if (debouncer2.fell()) {
        currentPreset = button2Preset;
        effectStep = 0;
        Serial.println("Button 2 pressed: activating preset " + String(button2Preset));
        preferences.putInt("btn2", button2Preset);
        // Counter for how many times the arcade machine runs lights
        digitalWrite(dial2, LOW);
        delay(200);
        digitalWrite(dial2, HIGH);
  }

  // Button 3 pressed
  if (debouncer3.fell()) {
        currentPreset = button3Preset;
        Serial.println("Button 3 pressed: activating preset " + String(button3Preset));
        preferences.putInt("btn3", button3Preset);
        // Counter for how many times the arcade machine runs lights
        digitalWrite(dial2, LOW);
        delay(200);
        digitalWrite(dial2, HIGH);
  }

  // Button 4 pressed
  if (debouncer4.fell()) {
        currentPreset = button4Preset;
        Serial.println("Button 4 pressed: activating preset " + String(button4Preset));
        preferences.putInt("btn4", button4Preset);
        // Counter for how many times the arcade machine runs lights
        digitalWrite(dial2, LOW);
        delay(200);
        digitalWrite(dial2, HIGH);
  }

  // Button 5 pressed
  if (debouncer5.fell()) {
        currentPreset = button5Preset;
        Serial.println("Button 5 pressed: activating preset " + String(button5Preset));
        preferences.putInt("btn5", button5Preset);
        // Counter for how many times the arcade machine runs lights
        digitalWrite(dial2, LOW);
        delay(200);
        digitalWrite(dial2, HIGH);
  }

  // Button 6 pressed
  if (debouncer6.fell()) {
        currentPreset = button6Preset;
        Serial.println("Button 6 pressed: activating preset " + String(button6Preset));
        preferences.putInt("btn6", button6Preset);
        // Counter for how many times the arcade machine runs lights
        digitalWrite(dial2, LOW);
        delay(200);
        digitalWrite(dial2, HIGH);
  }

  // Button 7 pressed
  if (debouncer7.fell()) {
        currentPreset = button7Preset;
        Serial.println("Button 7 pressed: activating preset " + String(button7Preset));
        preferences.putInt("btn7", button7Preset);
        // Counter for how many times the arcade machine runs lights
        digitalWrite(dial2, LOW);
        delay(200);
        digitalWrite(dial2, HIGH);
  }

  // Button 8 pressed
  if (debouncer8.fell()) {
        currentPreset = button8Preset;
        Serial.println("Button 8 pressed: activating preset " + String(button8Preset));
        preferences.putInt("btn8", button8Preset);
        // Counter for how many times the arcade machine runs lights
        digitalWrite(dial2, LOW);
        delay(200);
        digitalWrite(dial2, HIGH);   
  }

  // If there is a preset requested by a button, 
  // the current preset will be interrupted and new preset displayed 
  if (currentPreset != activeEffect) {
    activeEffect = currentPreset;
    strip.clear();
    strip.show();
  }

  switch (activeEffect) {
    case 1:
      rainbow(); 
      break;
    case 2:
      theaterChaseRainbow();
      break;
    case 3:
      shootingStar();
      break;
    case 4:
      rgbSolid();
      break;
    case 5:
      meteorRain();
      break;
    case 6:
      RunningLights();
      break;
    case 7:
      scanner();
      break;
    case 8:
      twinkleStars();
      break;
    case 9:
      red();
      break;
    case 10:
      blue();
      break;
    case 11:
      green();
      break;
    case 12:
      yellow();
      break;
    case 13:
      magenta();
      break;
    case 14:
      cyan();
      break;
    case 15:
      LSU();
      break;

    default: // RGB off
      strip.clear();
      strip.show();
      delay(100);
      break;
    }
}

