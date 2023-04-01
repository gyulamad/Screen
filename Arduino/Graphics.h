#pragma once

#include <Adafruit_GFX.h>

// TODO: implement Graphics on Arduino (based on ADAFRUIT_gfx)
class Graphics {
protected:
  Adafruit_GFX& GFX;
public:
    Graphics(Adafruit_GFX& GFX): GFX(GFX) {}
    void vline(int x, int y, int l, Color c) {
        GFX.writeFastVLine(x, y, l, c);
    }
    void hline(int x, int y, int l, Color c) {
        GFX.writeFastHLine(x, y, l, c);
    }
    void borders(int x, int y, int w, int h, Color ctop, Color cleft, Color cright, Color cbottom) {
        hline(x, y, w, ctop);
        hline(x, y+h, w, cbottom);
        vline(x, y, h, cleft);
        vline(x+w, y, h, cright);
    }
    void fillRect(int x, int y, int w, int h, Color c) {
        GFX.fillRect(x, y, w, h, c);
    }
    void text(const String& txt, int x, int y, int size, Color c) {
        GFX.setTextColor(c);
        GFX.setTextSize(size);
        GFX.setCursor(x, y);
        GFX.print(txt);
    }
    void getTextBounds(const String& txt, int size, uint16_t &w, uint16_t &h) {
        // TODO: works only with 6x8 monospace
        w = txt.length()*size * 6;
        h = size * 8;
        
        // GFX.getTextBounds(txt, x,y, x1, y1, w_, h_);
    }
    void fillScreen(Color c) {
        GFX.fillScreen(c);
    }
};
