#include <MCUFRIEND_kbv.h>
// #include "ScreenThemeNeon.h"
#include "ScreenThemeGray.h"
#include "Screen.h"


MCUFRIEND_kbv tft;

#define YP A1  // must be an analog pin, use "An" notation!   a2   a1
#define XM A2  // must be an analog pin, use "An" notation!   a3   a2
#define YM 7   // can be a digital pin                        8    7
#define XP 6   // can be a digital pin                        9    6
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

uint16_t ID;

const int BUTTONS = 5;
Graphics graphics(tft);
Touch touch(ts, YP, XM);
Screen<BUTTONS> screen(graphics, touch);

void build_buttons() {
    screen.reset(BUTTONS);
    const int H = 60;
    for (int i=0; i<BUTTONS; i++) {
        screen[i].top = i * H + 20;
        screen[i].left = 20;
        screen[i].width = SCREEN_THEME_WIDTH - 40;
        screen[i].height = H - 20;
        screen[i].text = String("Button ") + i;
    }    
    screen.refresh();
}

void setup() {
    Serial.begin(9600);        
    
    tft.reset();
    ID = tft.readID();
    tft.begin(ID);
    tft.invertDisplay(true);
    tft.setRotation(2);
    tft.fillScreen(COLOR(EGA_BLACK));

    touch.begin(SCREEN_THEME_WIDTH, SCREEN_THEME_HEIGHT, graphics);

    build_buttons();

}

void loop() {
    screen.loop();
}
