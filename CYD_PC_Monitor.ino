#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

int page = 1;
bool lastBtn = HIGH;

int cpuHistory[150];
int ramHistory[150];
int gpuHistory[150];
float netDownHistory[70];
float netUpHistory[70];

void setup() {
  Serial.begin(115200);

  pinMode(21, OUTPUT);
  digitalWrite(21, HIGH);

  pinMode(0, INPUT_PULLUP);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE, TFT_BLACK);
  tft.setTextSize(3);
}

void loop() {
  if (digitalRead(0) == LOW && lastBtn == HIGH) {
    page = 1 - page;
    tft.fillScreen(TFT_BLACK);
    delay(200);
  }

  lastBtn = digitalRead(0);

  if (Serial.available() > 0){
    String str = Serial.readStringUntil('\n');
  
    int firstSemi = str.indexOf(';');
    int secondSemi = str.indexOf(';', firstSemi + 1);
    int thirdSemi = str.indexOf(';', secondSemi + 1);
    int fourthSemi = str.indexOf(';', thirdSemi + 1);
    int fifthSemi = str.indexOf(';', fourthSemi + 1);

    float cpu = str.substring(0, firstSemi).toFloat();
    float ram = str.substring(firstSemi + 1, secondSemi).toFloat();
    float disk = str.substring(secondSemi + 1, thirdSemi).toFloat();
    float gpu = str.substring(thirdSemi + 1, fourthSemi).toFloat();
    float netDown = str.substring(fourthSemi + 1, fifthSemi).toFloat();
    float netUp = str.substring(fifthSemi + 1).toFloat();

    for (int i = 0; i < 149; i++) {
      cpuHistory[i] = cpuHistory[i + 1];
    }
    cpuHistory[149] = (int)cpu;

    for (int i = 0; i < 149; i++) {
      ramHistory[i] = ramHistory[i + 1];
    }
    ramHistory[149] = (int)ram;

    for (int i = 0; i < 149; i++) {
      gpuHistory[i] = gpuHistory[i + 1];
    }
    gpuHistory[149] = (int)gpu;

    for (int i = 0; i < 69; i++) {
      netDownHistory[i] = netDownHistory[i + 1];
      netUpHistory[i] = netUpHistory[i + 1];
    }
    netDownHistory[69] = netDown;
    netUpHistory[69] = netUp;

    if (page == 0){
      tft.setTextSize(3);
      tft.setCursor(0, 0);
      tft.print("CPU: "); 
      tft.print(cpu); 
      tft.println("%  ");

      tft.setCursor(0, 40);
      tft.print("RAM: "); 
      tft.print(ram); 
      tft.println("%  ");

      tft.setCursor(0, 80);
      tft.print("Disk used: "); 
      tft.print(disk); 
      tft.println("%  ");

      tft.setCursor(0, 120);
      tft.print("GPU: "); 
      tft.print(gpu); 
      tft.println("%  ");

      tft.setCursor(0, 160);
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.print("NET:");
      tft.fillTriangle(76, 163, 88, 163, 82, 179, TFT_MAGENTA);
      tft.setCursor(94, 160);
      tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
      tft.print(netDown, 1);
      tft.fillTriangle(172, 179, 184, 179, 178, 163, TFT_PINK);
      tft.setCursor(190, 160);
      tft.setTextColor(TFT_PINK, TFT_BLACK);
      tft.print(netUp, 1);

      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.print("MB/s");
    }
    if (page == 1){
      tft.setTextSize(1);
      tft.setCursor(0, 0);
      tft.print("CPU: "); tft.print(cpu); tft.print("%  ");

      for (int x = 0; x < 150; x++) {
        int y = map(constrain(cpuHistory[x], 0, 100), 0, 100, 115, 20);
        tft.drawFastVLine(x + 1, 20, 95, TFT_BLACK);
        tft.drawFastVLine(x + 1, y, 115 - y, TFT_CYAN);
      }
      tft.drawRect(0, 19, 152, 97, TFT_DARKCYAN);

      tft.setTextSize(1);
      tft.setCursor(165, 0);
      tft.print("RAM: "); tft.print(ram); tft.print("%  ");

      for (int x = 0; x < 150; x++) {
        int y = map(constrain(ramHistory[x], 0, 100), 0, 100, 115, 20);
        tft.drawFastVLine(x + 165, 20, 95, TFT_BLACK);
        tft.drawFastVLine(x + 165, y, 115 - y, TFT_YELLOW);
      }
      tft.drawRect(164, 19, 152, 97, TFT_OLIVE);

      tft.setTextSize(1);
      tft.setCursor(0, 125);
      tft.print("GPU: "); tft.print(gpu); tft.print("%  ");

      for (int x = 0; x < 150; x++) {
        int y = map(constrain(gpuHistory[x], 0, 100), 0, 100, 235, 140);
        tft.drawFastVLine(x + 1, 140, 95, TFT_BLACK);
        tft.drawFastVLine(x + 1, y, 235 - y, TFT_RED);
      }
      tft.drawRect(0, 139, 152, 97, TFT_MAROON);

      tft.setTextSize(1);
      tft.setCursor(165, 125);
      tft.print("DSK: "); tft.print((int)disk); tft.print("%  ");

      int h = map(constrain((int)disk, 0, 100), 0, 100, 0, 93);
      tft.fillRect(166, 141, 68, 93 - h, TFT_BLACK);
      tft.fillRect(166, 141 + (93 - h), 68, h, TFT_WHITE);
      tft.drawRect(164, 139, 72, 97, TFT_DARKGREY);

      tft.fillTriangle(244, 126, 248, 126, 246, 131, TFT_MAGENTA);
      tft.setCursor(250, 125);
      tft.setTextColor(TFT_MAGENTA, TFT_BLACK);
      tft.print(" ");
      tft.print(netDown, 1);
      tft.print(" ");

      tft.fillTriangle(278, 131, 282, 131, 280, 126, TFT_PINK);
      tft.setCursor(284, 125);
      tft.setTextColor(TFT_PINK, TFT_BLACK);
      tft.print(" ");
      tft.print(netUp, 1);
      tft.print(" ");

      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      for (int x = 0; x < 70; x++) {
        tft.drawFastVLine(x + 245, 140, 95, TFT_BLACK);
        tft.fillTriangle(248, 143, 254, 143, 251, 149, TFT_MAGENTA);
        int yDown = map(constrain((int)(netDownHistory[x] * 100), 0, 12500), 0, 12500, 187, 140);
        tft.drawFastVLine(x + 245, yDown, 187 - yDown, TFT_MAGENTA);
        tft.fillTriangle(248, 196, 254, 196, 251, 190, TFT_PINK);
        int yUp = map(constrain((int)(netUpHistory[x] * 100), 0, 12500), 0, 12500, 235, 188);
        tft.drawFastVLine(x + 245, yUp, 235 - yUp, TFT_PINK);
      }
      tft.setCursor(290, 142);
      tft.setTextColor(TFT_DARKGREY);
      tft.print("MB/s");
      tft.setTextColor(TFT_WHITE, TFT_BLACK);
      tft.drawFastHLine(245, 187, 70, TFT_DARKGREY);
      tft.drawRect(244, 139, 72, 97, TFT_PURPLE);
    }
  }
}
