#pragma once

#include <TouchScreen.h>

// TODO: implement Touch on Arduino (based on TouchScreen)

// TODO add into a config?
#ifndef SCREEN_DEFAULT_CLK_ALPHA
#define SCREEN_DEFAULT_CLK_ALPHA 10
#endif

#ifndef SCREEN_DEFAULT_CLK_MAX
#define SCREEN_DEFAULT_CLK_MAX 150
#endif

#ifndef SCREEN_DEFAULT_CLK_MIN
#define SCREEN_DEFAULT_CLK_MIN 30
#endif

struct Size {
    int width = SCREEN_THEME_WIDTH;
    int height = SCREEN_THEME_HEIGHT;
    bool operator==(const Size& other) const {
        return 
            width == other.width &&
            height == other.height;
    }
    bool operator!=(const Size& other) const {
        return !(*this == other);
    }
};

class Touch {
public:

    struct Calibration {
      unsigned int tsMinX;
      unsigned int tsMinY;
      unsigned int tsMaxX;
      unsigned int tsMaxY;

      // touch
      double alpha;
      double cmax;
      double cmin;
    };

protected:
    Graphics& graphics;
    TouchScreen& TS;
    const uint8_t& yp; 
    const uint8_t& xm;
    int rotation;

    Size size;
    Calibration calibration;
    int calibrating = 0;
    mutable double clkz = 0;

    TSPoint p;
    void read() {
      prevState = state;

      p = TS.getPoint();  
      pinMode(xm, OUTPUT); 
      pinMode(yp, OUTPUT);

      // smooth EMA on sensor value
      clkz = (clkz * calibration.alpha + (double)p.z) / (calibration.alpha + 1);
      if (clkz > calibration.cmax) clkz = calibration.cmax;
      if (clkz < calibration.cmin) clkz = 0;
      state.z = clkz;

      switch (state.event) {
        case EVENT_IDLE:
          if (clkz >= calibration.cmax) state.event = EVENT_TOUCH;
          break;
        case EVENT_TOUCH:
          state.event = EVENT_MOVE;
          break;
        case EVENT_MOVE:
          if (clkz <= calibration.cmin) state.event = EVENT_RELEASE;
          break;
        case EVENT_RELEASE:
          state.event = EVENT_IDLE;
          break;

        default:
          state.event = EVENT_IDLE;
          break;
      }


      if (state.event == EVENT_TOUCH || (EVENT_MOVE && state.z == calibration.cmax)) {
        switch (rotation) {
          default:
          case 0:
            state.x = p.x;
            state.y = p.y; 
            break;
          case 1:
            state.x = p.y;
            state.y = p.x; 
            break;
          case 2:
            state.x = size.width - p.x;
            state.y = p.y; 
            break;
          case 3:
            state.x = p.x;
            state.y = size.height - p.y; 
            break;
          case 4:
            state.x = size.width - p.x;
            state.y = size.height - p.y; 
            break;
          case 5:
            state.x = size.width - p.y;
            state.y = p.x;
            break;
          case 6:
            state.x = p.y;
            state.y = size.height - p.x;
            break;
          case 7:
            state.x = size.width - p.y;
            state.y = size.height - p.x;
            break;
        }
        if (!calibrating) {
          state.x = map(state.x, calibration.tsMaxX, calibration.tsMinX, size.width, 0); 
          state.y = map(state.y, calibration.tsMaxY, calibration.tsMinY, size.height, 0); 
        }
      }

    }

public:
    struct State {
      int event; 
      int x; 
      int y; 
      int z;
    };
protected:
    State state, prevState; 
public:
    Touch(TouchScreen& TS, const uint8_t& yp, const uint8_t& xm, const int rotation = 0): TS(TS), yp(yp), xm(xm), rotation(rotation) {

        state.event = EVENT_IDLE;
        state.x = -1;
        state.y = -1;
    }

    // TODO: add on desktop?
    void begin(
      int width, int height, unsigned int tsMinX, unsigned int tsMinY, unsigned int tsMaxX, unsigned int tsMaxY,
      const double alpha = SCREEN_DEFAULT_CLK_ALPHA, const double cmax = SCREEN_DEFAULT_CLK_MAX, const double cmin = SCREEN_DEFAULT_CLK_MIN
    ) {
      size.width = width;
      size.height = height;      
      calibration.tsMinX = tsMinX;
      calibration.tsMinY = tsMinY;
      calibration.tsMaxX = tsMaxX;
      calibration.tsMaxY = tsMaxY;
      calibration.alpha = alpha;
      calibration.cmax = cmax;
      calibration.cmin = cmin; 
      calibrating = 0;
    }

    Calibration& begin(
      int width, int height, Graphics& graphics,
      const double alpha = SCREEN_DEFAULT_CLK_ALPHA, const double cmax = SCREEN_DEFAULT_CLK_MAX, const double cmin = SCREEN_DEFAULT_CLK_MIN
    ) {
      size.width = width;
      size.height = height;     
      calibrating = 1;
      calibration.alpha = alpha;
      calibration.cmax = cmax;
      calibration.cmin = cmin;

      Color textcolor = COLOR(EGA_WHITE);
      const char* text = "Calibration...";

      while(calibrating) {
        read();
        switch (calibrating) {
          case 1:
            graphics.fillScreen(EGA_BLACK);
            // const char* s = "Calibration...";
            graphics.text(text, 0, 40, 2, textcolor);
            graphics.text("X <- Press here", 1, 1, 2, textcolor);
            calibrating = 2;
            break;
          case 2:
            // waiting for first point...
            if (state.event == EVENT_TOUCH) {
              calibration.tsMinX = state.x;
              calibration.tsMinY = state.y;

              graphics.fillScreen(EGA_BLACK);
              graphics.text(text, 0, 0, 2, textcolor);
              uint16_t w,h;
              const char* txt = "Press here -> X";
              graphics.getTextBounds(txt, 2, w, h);              
              graphics.text(txt, width-w, height-h, 2, textcolor);

              calibrating = 3;
            }
            break;
          
          case 3:
            // waiting for second point...
            if (state.event == EVENT_TOUCH) {
              calibration.tsMaxX = state.x;
              calibration.tsMaxY = state.y;
              calibrating = 0;
            }
            break;
          default: 
            calibrating = 0;
            break;
        }
      }

      graphics.fillScreen(COLOR(EGA_BLACK));
      return calibration;
    }

    State& getState() {
        read();
        return state;
    }
};
