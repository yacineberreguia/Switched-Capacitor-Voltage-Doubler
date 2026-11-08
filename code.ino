#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// ---------- OLED ----------
#define SCREEN_WIDTH   128
#define SCREEN_HEIGHT  64
#define OLED_ADDR      0x3C   // change to 0x3D if display doesn't init
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

#define SDA_PIN 0   // GP0
#define SCL_PIN 1   // GP1

// ---------- PWM ----------
#define PWM_PIN 15          // GP15 -> 100R -> IR2104 IN
#define PWM_FREQ 150000UL   // 150kHz
#define PWM_DUTY 128        // ~50% (0-255 range)

// ---------- ADC ----------
#define VOUT_ADC_PIN 26     // GP26 = ADC0
#define VIN_ADC_PIN  27     // GP27 = ADC1
#define ADC_VREF     3.3f
#define ADC_MAXCOUNT 4095.0f

// Divider ratios: (R1+R2)/R2  — adjust if you measure actual resistor values
#define VOUT_DIV_RATIO  (12.2f / 2.2f)   // 10k + 2.2k divider on Vout
#define VIN_DIV_RATIO   (14.7f / 4.7f)   // 10k + 4.7k divider on Vin

#define NUM_SAMPLES 16      // oversampling for stable readings

// ---------- Timing ----------
unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 250; // ms

float readVoltage(int pin, float ratio) {
  uint32_t sum = 0;
  for (int i = 0; i < NUM_SAMPLES; i++) {
    sum += analogRead(pin);
    delayMicroseconds(100);
  }
  float avgCount = (float)sum / NUM_SAMPLES;
  float pinVoltage = (avgCount / ADC_MAXCOUNT) * ADC_VREF;
  return pinVoltage * ratio;
}

void setup() {
  Serial.begin(115200);

  Wire.setSDA(SDA_PIN);
  Wire.setSCL(SCL_PIN);
  Wire.begin();

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    Serial.println("OLED init failed");
  }
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("Booting...");
  display.display();

  pinMode(PWM_PIN, OUTPUT);
  analogWriteFreq(PWM_FREQ);
  analogWriteRange(255);
  analogWrite(PWM_PIN, PWM_DUTY);

  // ADC pins (Pico default ADC resolution is 12-bit)
  analogReadResolution(12);

  delay(300);
}

void loop() {
  if (millis() - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = millis();

    float vin  = readVoltage(VIN_ADC_PIN,  VIN_DIV_RATIO);
    float vout = readVoltage(VOUT_ADC_PIN, VOUT_DIV_RATIO);
    float ratio = (vin > 0.01f) ? (vout / vin) : 0.0f;

    // --- Serial debug ---
    Serial.print("Vin: ");  Serial.print(vin, 3);  Serial.print(" V   ");
    Serial.print("Vout: "); Serial.print(vout, 3); Serial.print(" V   ");
    Serial.print("Ratio: "); Serial.println(ratio, 3);

    // --- OLED display ---
    display.clearDisplay();
    display.setTextSize(1);
    display.setCursor(0, 0);
    display.println("Voltage Doubler");
    display.drawLine(0, 10, 128, 10, SSD1306_WHITE);

    display.setCursor(0, 16);
    display.print("Vin : ");
    display.print(vin, 2);
    display.println(" V");

    display.setCursor(0, 28);
    display.print("Vout: ");
    display.print(vout, 2);
    display.println(" V");

    display.setCursor(0, 40);
    display.print("Ratio: ");
    display.print(ratio, 2);
    display.println("x");

    display.setCursor(0, 52);
    display.print("Target: 2.00x");

    display.display();
  }
}
