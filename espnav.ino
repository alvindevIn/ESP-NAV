#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <ChronosESP32.h>

/* ================= PIN ================= */
#define OLED_SDA 21
#define OLED_SCL 22
#define TOUCH_PIN 4

/* ================= OLED ================= */
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

/* ================= CHRONOS ================= */
ChronosESP32 watch("vario alvin");
Navigation currentNavData;
bool isNavigationActive = false;
bool navChange = false;
bool modeChanged = true;
uint32_t nav_crc = 0xFFFFFFFF;

/* ================= MODE ================= */
enum DisplayMode {
  MODE_EYE,
  MODE_CLOCK,
  MODE_NAV
};
DisplayMode currentMode = MODE_EYE;

/* ================= TOUCH ================= */
bool lastTouch = false;
unsigned long lastTouchTime = 0;

/* ================= EYE VAR ================= */
int leftEyeX = 45;
int rightEyeX = 80;
int eyeY = 18;
int eyeWidth = 25;
int eyeHeight = 30;

int targetOffsetX = 0;
int targetOffsetY = 0;
int moveSpeed = 5;

int blinkState = 0;
unsigned long lastBlinkTime = 0;
unsigned long moveTime = 0;

/* ================= CALLBACK ================= */
void connectionCallback(bool state) {
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("Status: ");
  display.println(state ? "Connected" : "Disconnected");
  display.display();
}

void notificationCallback(Notification notification) {}

void configCallback(Config config, uint32_t a, uint32_t b) {
  switch (config) {
    case CF_NAV_DATA:
      isNavigationActive = a;
      currentNavData = watch.getNavigation();
      navChange = true;
      break;

    case CF_NAV_ICON:
      if (a == 2) {
        Navigation tempNav = watch.getNavigation();
        if (nav_crc != tempNav.iconCRC) {
          nav_crc = tempNav.iconCRC;
          currentNavData = tempNav;
          navChange = true;
        }
      }
      break;
  }
}

/* ================= SETUP ================= */
void setup() {
  pinMode(TOUCH_PIN, INPUT);

  Wire.begin(OLED_SDA, OLED_SCL);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.println("System Ready");
  display.display();
  delay(1500);

  watch.setConnectionCallback(connectionCallback);
  watch.setNotificationCallback(notificationCallback);
  watch.setConfigurationCallback(configCallback);
  watch.begin();
  watch.setBattery(80);
}

/* ================= TOUCH HANDLER ================= */
void handleTouch() {
  bool touch = digitalRead(TOUCH_PIN);

  if (touch && !lastTouch && millis() - lastTouchTime > 300) {
    currentMode = (DisplayMode)((currentMode + 1) % 3);
    display.clearDisplay();
    modeChanged = true;         
    lastTouchTime = millis();
  }

  lastTouch = touch;
}

/* ================= MODE : EYE ================= */
void drawEye(int x, int y, int w, int h) {
  display.fillRoundRect(x, y, w, h, 5, WHITE);
}

void renderEye() {
  unsigned long now = millis();

  if (now - lastBlinkTime > 4000 && blinkState == 0) {
    blinkState = 1;
    lastBlinkTime = now;
  } else if (now - lastBlinkTime > 150 && blinkState == 1) {
    blinkState = 0;
    lastBlinkTime = now;
  }

  if (now - moveTime > random(1500, 3000) && blinkState == 0) {
    targetOffsetX = random(-10, 11);
    targetOffsetY = random(-8, 9);
    moveTime = now;
  }

  static int offsetX = 0, offsetY = 0;
  offsetX += (targetOffsetX - offsetX) / moveSpeed;
  offsetY += (targetOffsetY - offsetY) / moveSpeed;

  display.clearDisplay();

  if (!blinkState) {
    drawEye(leftEyeX + offsetX, eyeY + offsetY, eyeWidth, eyeHeight);
    drawEye(rightEyeX + offsetX, eyeY + offsetY, eyeWidth, eyeHeight);
  } else {
    display.fillRect(leftEyeX, eyeY + eyeHeight / 2, eyeWidth, 4, WHITE);
    display.fillRect(rightEyeX, eyeY + eyeHeight / 2, eyeWidth, 4, WHITE);
  }

  display.display();
}

/* ================= MODE : CLOCK ================= */
void renderClock() {
  display.clearDisplay();

  display.setTextColor(SSD1306_WHITE);  
  display.setTextSize(2);
  display.setCursor(10, 20);
  display.printf("%02d:%02d",
    watch.getHour(),
    watch.getMinute()
  );

  display.setTextSize(1);
  display.setCursor(34, 45);
  display.printf("%02d/%02d/%04d",
    watch.getDay(),
    watch.getMonth(),
    watch.getYear()
  );

  display.display();
}

/* ================= MODE : NAV ================= */
void renderNavigation() {
  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE); 

  if (!isNavigationActive) {
    display.setCursor(0, 0);
    display.println("Navigation Inactive");
    display.display();
    return;
  }

  if (nav_crc != 0xFFFFFFFF) {
    display.drawBitmap(0, 0, currentNavData.icon, 48, 48, WHITE);
  }

  display.setTextSize(1);
  display.setCursor(55, 0);
  display.print("Dist:");
  display.println(currentNavData.distance);

  display.setCursor(55, 16);
  display.println("Title:");

  display.setTextSize(2);
  display.setCursor(55, 32);
  display.println(currentNavData.title);

  display.display();
}

/* ================= LOOP ================= */
void loop() {
  watch.loop();
  handleTouch();

  switch (currentMode) {

    case MODE_EYE:
      renderEye();
      break;

    case MODE_CLOCK:
      renderClock();       
      break;

    case MODE_NAV:
      if (modeChanged || navChange) {
        renderNavigation();
        navChange = false;
      }
      break;
  }
  modeChanged = false;
}