#include <MCUFRIEND_kbv.h>
#include "ScreenThemeNeon.h"
// #include "ScreenThemeGray.h"
#include "Screen.h"

// ------------------------------------------------------------

MCUFRIEND_kbv tft;

#define YP A1  // must be an analog pin, use "An" notation!   a2   a1
#define XM A2  // must be an analog pin, use "An" notation!   a3   a2
#define YM 7   // can be a digital pin                        8    7
#define XP 6   // can be a digital pin                        9    6
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

uint16_t ID;


Graphics graphics(tft);
Touch touch(ts, YP, XM);


Screen<7> screen(graphics, touch);

enum { PAGE_MENU, PAGE_PAINTER, PAGE_TIMER };
int page = PAGE_MENU;

// --------- Menu -----------

void to_menu(int x, int y) {
    page = PAGE_MENU;
    build_menu();
}

void to_painter(int x, int y) {
    page = PAGE_PAINTER;
    build_painter();
}

void to_timer(int x, int y) {
    page = PAGE_TIMER;
    build_timer();
}

void to_calib(int x, int y) {
    touch.begin(SCREEN_THEME_WIDTH, SCREEN_THEME_HEIGHT, graphics);
    screen.refresh();  
}

void build_menu() {
    screen.reset(5);
    // --- Demo menu ---
    int i=0, w=200, h=40, p=20;
    screen[i].text = "Demo"; screen[i].top = p + i * (h+p); screen[i].left = p; 
    ELEM_SET_UNBORDERED(screen[i].state);
    ELEM_SET_FSIZE(screen[i].props, 4);
    i++;
    screen[i].text = "Paint"; screen[i].top = p + i * (h+p); screen[i].left = p; 
    screen[i].touch = to_painter;
    i++;
    screen[i].text = "Timer"; screen[i].top = p + i * (h+p); screen[i].left = p; 
    screen[i].touch = to_timer;
    i++;
    screen[i].text = "Disabled"; screen[i].top = p + i * (h+p); screen[i].left = p; 
    ELEM_SET_DISABLE(screen[i].state);
    i++;
    screen[i].text = "Calibrate"; screen[i].top = p + i * (h+p); screen[i].left = p; 
    screen[i].touch = to_calib;
    i++;  

    screen.refresh();
}

// ------- Painter ------------
int c;

void paint(int x, int y) {
    tft.fillCircle(x, y, 5, COLOR(c));
}

void colorpick(int x, int y) {
  for (int i=0; i<4; i++) {
    if (screen[i].left <= x && screen[i].left + screen[i].width > x && screen[i].top < y && screen[i].top + screen[i].height > y) {
      c = i;
    } 
  }
}

void clear(int x, int y) {
  // Serial.println("CLEAR clicked");
  screen.refresh();
}

void build_painter() {
    screen.reset(7);
    // --- Painter ---
    int x = 0, y=0, w=40, h=40;

    screen[5].text = "<-";
    screen[5].top = y+3;
    screen[5].left = x+3;
    screen[5].width = w-6;
    screen[5].height = h-6;
    screen[5].touch = to_menu;

    x += screen[5].width+6;

    screen[6].text = "X";
    screen[6].top = y+3;
    screen[6].left = x+3;
    screen[6].width = w-6;
    screen[6].height = h-6;
    screen[6].touch = clear;

    x += screen[6].width+6;

    for (int i=0; i<=3; i++) {
      screen[i].text = ""; //String(i);
      screen[i].top = y+3;
      screen[i].left = x+3;
      screen[i].width = w-6;
      screen[i].height = h-6;
      screen[i].touch = colorpick;
      //ELEM_SET_STICKY(screen[i].props);
      ELEM_SET_COLOR(screen[i].props, i);
      // if (i < 10) ELEM_SET_FCOLOR(screen[i].props, EGA_WHITE);
      x+=w; if (x>SCREEN_THEME_WIDTH-w) {x=0; y+=h;}
    }
    if (x>0) {x=0;y+=h;}
    screen[4].top = y+3;
    screen[4].left = x+3;
    screen[4].width = SCREEN_THEME_WIDTH-6;
    screen[4].height = SCREEN_THEME_HEIGHT - y-6;
    screen[4].move = paint;
    ELEM_SET_COLOR(screen[4].props, SCREEN_THEME_PUSHED_ELEM_COLOR);

    screen.refresh();
}

//------------------------


// ------------ timer ------------------
long tstart, tnow;
const char tvalue[30] = "";
void timer_start(int x, int y) {
    tstart = millis();
    screen[1].text = "Stop";
}

void timer_stop(int x, int y) {
    screen[1].text = "Start";
}

void timer_loop() {
    if (IS_ELEM_PUSHED(screen[1].state)) {
        tnow = millis();
        // String(tnow - tstart);
        sprintf(tvalue, "%d", tnow - tstart);
        screen[2].text = String(tvalue);
    }
}

void build_timer() {
    screen.reset(3);
    // ----- Timer ---------
    int i=0, w=200, h=40, p=20;

    screen[i].text = "<-"; screen[i].top = p + i * (h+p); screen[i].left = p; 
    screen[i].touch = to_menu;
    i++;

    screen[i].text = "Start"; screen[i].top = p + i * (h+p); screen[i].left = p; 
    ELEM_SET_STICKY(screen[i].state);
    screen[i].touch = timer_start;
    screen[i].release = timer_stop;
    i++;
    
    screen[i].text = tvalue; screen[i].top = p + i * (h+p); screen[i].left = p; 
    ELEM_SET_UNBORDERED(screen[i].state);
    i++;

    screen.refresh();
}
// ------------------------------------



void setup() {
    Serial.begin(9600);        
    
    tft.reset();
    ID = tft.readID();
    tft.begin(ID);
    tft.invertDisplay(true);
    tft.setRotation(2);
    tft.fillScreen(COLOR(EGA_BLACK));

    Serial.println("calibration..");
    touch.begin(SCREEN_THEME_WIDTH, SCREEN_THEME_HEIGHT, graphics);


    // ------------------
    build_menu();
}

void loop() {
    screen.loop();
    if (page == PAGE_TIMER) timer_loop();
}
