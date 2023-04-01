#pragma once


class Graphics {
protected:
    SDLGraphics& SDL;
public:
    Graphics(SDLGraphics& SDL): SDL(SDL) {}
    void vline(int x, int y, int l, Color c) {
        SDL.vline(x, y, l, c);
    }
    void hline(int x, int y, int l, Color c) {
        SDL.hline(x, y, l, c);
    }
    void borders(int x, int y, int w, int h, Color ctop, Color cleft, Color cright, Color cbottom) {
        hline(x, y, w, ctop);
        hline(x, y+h, w, cbottom);
        vline(x, y, h, cleft);
        vline(x+w, y, h, cright);
    }
    void fillRect(int x, int y, int w, int h, Color c) {
        SDL.fillRect(x, y, w, h, c);
    }
    void text(String txt, int x, int y, int size, Color c) {
        SDL.text(txt, x, y, size, c);
    }
    void getTextBounds(String txt, int size, uint16_t &w, uint16_t &h) {
        SDL.getTextBounds(txt, size, w, h);
    }
    void fillScreen(Color c) {
        SDL.fillScreen(c);
    }

    // only on PC
    void present() {
        SDL.present();
    }
};
