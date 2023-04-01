#pragma once

#ifdef ARDUINO
#include <Adafruit_GFX.h>
#include <TouchScreen.h>

#define ERRF(...) { /*Serial.print(__VA_ARGS__);*/ while(true); }


#else

#include <unistd.h>
#include <string>
#include <exception>
#include <chrono>   // for std::chrono::milliseconds
#include <thread>   // for std::this_thread::sleep_for
#include <iostream>
#include <cassert>

#include <SDL.h>
#include <SDL_ttf.h>

// using namespace std;

#define String std::string

#define ERRF(...) {printf(__VA_ARGS__); printf("\n"); throw exception(); }

class _Serial {
public:
    void print(String s) {
        printf("%s", s.c_str());
    }
    void println(String s) {
        printf("%s\n", s.c_str());
    }
    void println() {
        printf("\n");
    }
} Serial;

#endif



// ---------------- COLORS -----------------------

//-------------------------------------
// EGA colors:
//-------------------------------------
//Color	Name	                    RGB	    Binary	Decimal RGB565
// 0	black	                      #000000	000000	0       0x0000
// 1	blue	                      #0000AA	000001	1       0x0014
// 2	green	                      #00AA00	000010	2       0x0540
// 3	cyan	                      #00AAAA	000011	3       0x0554
// 4	red	                        #AA0000	000100	4       0xA000
// 5	magenta	                    #AA00AA	000101	5cd     0xA014
// 6	yellow / brown	            #AA5500	010100	20      0xA2A0
// 7	white / light gray	        #AAAAAA	000111	7       0xA554
// 8	dark gray / bright black	  #555555	111000	56      0x52AA
// 9	bright blue	                #5555FF	111001	57      0x52BF
// 10	bright green	              #55FF55	111010	58      0x57EA
// 11	bright cyan	                #55FFFF	111011	59      0x57FF
// 12	bright red	                #FF5555	111100	60      0xFAAA
// 13	bright magenta	            #FF55FF	111101	61      0xFABF
// 14	bright yellow	              #FFFF55	111110	62      0xFFEA
// 15	bright white	              #FFFFFF	111111	63      0xFFFF


#ifdef ARDUINO
#include "Arduino/COLOR.h"
#else
#include "SDL/COLOR.h"
#endif

enum EGA_Color: uint8_t {
    EGA_BLACK = 0,
    EGA_BLUE,
    EGA_GREEN,
    EGA_CYAN,
    EGA_RED,
    EGA_MAGENTA,
    EGA_YELLOW,
    EGA_GRAY,
    EGA_DARK_GRAY,
    EGA_BRIGHT_BLUE,
    EGA_BRIGHT_GREEN,
    EGA_BRIGHT_CYAN,
    EGA_BRIGHT_RED,
    EGA_BRIGHT_MAGENTA,
    EGA_BRIGHT_YELLOW,
    EGA_WHITE,
};

// ---------------------- TouchPad EVENTS -----------------------

#define EVENT_IDLE 0
#define EVENT_TOUCH 1
#define EVENT_MOVE 2
#define EVENT_RELEASE 3

// ------------------- SDL GRAPHICS ----------------------

#ifdef ARDUINO
#else
#include "SDL/SDLGraphics.h"
#endif


// ----------------- GFX/Touch -----------------------

#ifdef ARDUINO
#include "Arduino/Graphics.h"
#else
#include "SDL/Graphics.h"
#endif


#ifdef ARDUINO
#include "Arduino/Touch.h"
#else
#include "SDL/Touch.h"
#endif


// -------------------------------- SCREEN -------------------------------

enum Align {
    ALIGN_TOP_LEFT = 0,
    ALIGN_TOP_RIGHT,
    ALIGN_TOP_CENTER,
    ALIGN_BOTTOM_LEFT,
    ALIGN_BOTTOM_RIGHT,
    ALIGN_BOTTOM_CENTER,
    ALIGN_CENTER_LEFT,
    ALIGN_CENTER_RIGHT,
    ALIGN_CENTER_CENTER,
};


#define ELEM_PROPS(color, fcolor, align, fsize) ((uint16_t)(color) | ((uint16_t)(fcolor) << 4) | ((uint16_t)(align) << 8) | ((uint16_t)(fsize) << 12))
#define ELEM_SET_COLOR(props, color) ((props) = ((props) & 0xFFF0U) | ((uint16_t)(color)))
#define ELEM_SET_FCOLOR(props, fcolor) ((props) = ((props) & 0xFF0FU) | ((uint16_t)(fcolor) << 4))
#define ELEM_SET_ALIGN(props, align) ((props) = ((props) & 0xF0FFU) | ((uint16_t)(align) << 8))
#define ELEM_SET_FSIZE(props, fsize) ((props) = ((props) & 0x0FFFU) | ((uint16_t)(fsize) << 12))
#define ELEM_GET_COLOR(props) ((uint16_t)((props) & 0x000FU))
#define ELEM_GET_FCOLOR(props) ((uint16_t)(((props) & 0x00F0U) >> 4))
#define ELEM_GET_ALIGN(props) ((uint16_t)(((props) & 0x0F00U) >> 8))
#define ELEM_GET_FSIZE(props) ((uint16_t)(((props) & 0xF000U) >> 12))

#define ELEM_STATE_PUSHED   (uint8_t)0b00000001 // 1
#define ELEM_STATE_DISABLED (uint8_t)0b00000010 // 2
#define ELEM_STATE_HIDDEN   (uint8_t)0b00000100 // 4
#define ELEM_STATE_STICKY   (uint8_t)0b00001000 // 8
#define ELEM_STATE_STUCK    (uint8_t)0b00010000 // 16
#define ELEM_STATE_BORDER   (uint8_t)0b00100000 // 32
#define ELEM_STATE_VSLIDER  (uint8_t)0b01000000 // 64
#define ELEM_STATE_HSLIDER  (uint8_t)0b10000000 // 128

#define ELEM_SET_PUSH(state) ((state) |= ELEM_STATE_PUSHED)
#define ELEM_SET_RELEASE(state) ((state) &= ~ELEM_STATE_PUSHED)
#define ELEM_SET_DISABLE(state) ((state) |= ELEM_STATE_DISABLED)
#define ELEM_SET_ENABLE(state) ((state) &= ~ELEM_STATE_DISABLED)
#define ELEM_SET_SHOW(state) ((state) &= ~ELEM_STATE_HIDDEN)
#define ELEM_SET_HIDE(state) ((state) |= ELEM_STATE_HIDDEN)
#define ELEM_SET_STICKY(state) ((state) |= ELEM_STATE_STICKY)
#define ELEM_SET_SLIPPERY(state) ((state) &= ~ELEM_STATE_STICKY)
#define ELEM_SET_STUCK(state) ((state) |= ELEM_STATE_STUCK)
#define ELEM_SET_UNSTUCK(state) ((state) &= ~ELEM_STATE_STUCK)
#define ELEM_SET_BORDERED(state) ((state) |= ELEM_STATE_BORDER)
#define ELEM_SET_UNBORDERED(state) ((state) &= ~ELEM_STATE_BORDER)
#define ELEM_SET_VSLIDER(state) ((state) |= ELEM_STATE_VSLIDER)
#define ELEM_SET_UNVSLIDER(state) ((state) &= ~ELEM_STATE_VSLIDER)
#define ELEM_SET_HSLIDER(state) ((state) |= ELEM_STATE_HSLIDER)
#define ELEM_SET_UNHSLIDER(state) ((state) &= ~ELEM_STATE_HSLIDER)
#define IS_ELEM_PUSHED(state) ((state) & ELEM_STATE_PUSHED)
#define IS_ELEM_RELEASED(state) (!((state) & ELEM_STATE_PUSHED))
#define IS_ELEM_DISABLED(state) ((state) & ELEM_STATE_DISABLED)
#define IS_ELEM_ENABLED(state) (!((state) & ELEM_STATE_DISABLED))
#define IS_ELEM_HIDDEN(state) ((state) & ELEM_STATE_HIDDEN)
#define IS_ELEM_VISIBLE(state) (!((state) & ELEM_STATE_HIDDEN))
#define IS_ELEM_STICKY(state) ((state) & ELEM_STATE_STICKY)
#define IS_ELEM_SLIPPERY(state) (!((state) & ELEM_STATE_STICKY))
#define IS_ELEM_STUCK(state) ((state) & ELEM_STATE_STUCK)
#define IS_ELEM_UNSTUCK(state) (!((state) & ELEM_STATE_STUCK))
#define IS_ELEM_BORDERED(state) ((state) & ELEM_STATE_BORDER)
#define IS_ELEM_UNBORDERED(state) (!((state) & ELEM_STATE_BORDER))
#define IS_ELEM_VSLIDER(state) ((state) & ELEM_STATE_VSLIDER)
#define IS_ELEM_UNVSLIDER(state) (!((state) & ELEM_STATE_VSLIDER))
#define IS_ELEM_HSLIDER(state) ((state) & ELEM_STATE_HSLIDER)
#define IS_ELEM_UNHSLIDER(state) (!((state) & ELEM_STATE_HSLIDER))

template <const int SIZE>
class Screen {
private:
    bool _refresh = false;
protected:
    struct Elem {

        uint16_t top = 0, left = 0, slider = 0, width = SCREEN_THEME_ELEM_WIDTH, height = SCREEN_THEME_ELEM_HEIGHT;
        String text = "";
        uint16_t props = ELEM_PROPS(SCREEN_THEME_ELEM_COLOR, SCREEN_THEME_FONT_COLOR, SCREEN_THEME_TEXT_ALIGN, SCREEN_THEME_FONT_SIZE);
        uint8_t state = 0;

        typedef void (*Event)(int x, int y);
        Event touch = NULL, release = NULL, move = NULL;

        bool operator==(const Elem& other) {
            return 
                text == other.text &&
                top == other.top &&
                left == other.left &&
                width == other.width &&
                height == other.height &&
                props == other.props &&
                state == other.state &&
                slider == other.slider;
        }
        bool operator!=(const Elem& other) {
            return !(*this == other);
        }

        void getProps(Color& color, Color& fcolor, int& align, int& fsize) {
            
            uint16_t index1 = props & 0x0F;
            uint16_t index2 = (props >> 4) & 0x0F;

            align = (props >> 8) & 0x0F;
            fsize = (props >> 12) & 0x0F;

            // state (pushed/disabled) can override the actual colors
            if (IS_ELEM_PUSHED(state)) {
                index1 = SCREEN_THEME_PUSHED_ELEM_COLOR;
                index2 = SCREEN_THEME_PUSHED_FONT_COLOR;
            }
            if (IS_ELEM_DISABLED(state)) {
                index1 = SCREEN_THEME_DISABLED_ELEM_COLOR;
                index2 = SCREEN_THEME_DISABLED_FONT_COLOR;
            }
            color = COLOR(index1);
            fcolor = COLOR(index2);
        }

        void clearAll(Graphics& graphics, Color color) {
            graphics.fillRect(
                left - SCREEN_THEME_BORDER_THICKNESS, 
                top - SCREEN_THEME_BORDER_THICKNESS, 
                width + SCREEN_THEME_BORDER_THICKNESS, 
                height + SCREEN_THEME_BORDER_THICKNESS,
                color
            );
        }

        void clearSurface(Graphics& graphics, Color color) {
            graphics.fillRect(left, top, width, height, color);
        }

        void clearText(Graphics& graphics) {
            Color color, fcolor;
            int fsize, align;
            getProps(color, fcolor, align, fsize);
            drawText(graphics, fsize, align, color);
        }

        void drawBorder(Graphics& graphics) {
            if (IS_ELEM_UNBORDERED(state)) return;

            // state (pushed/disabled) can override the actual colors
            int topColor = SCREEN_THEME_BORDER_TOP_COLOR, leftColor = SCREEN_THEME_BORDER_LEFT_COLOR, rightColor = SCREEN_THEME_BORDER_RIGHT_COLOR, bottomColor = SCREEN_THEME_BORDER_BOTTOM_COLOR;
            if (IS_ELEM_PUSHED(state)) {
                topColor = SCREEN_THEME_PUSHED_BORDER_TOP_COLOR;
                leftColor = SCREEN_THEME_PUSHED_BORDER_LEFT_COLOR;
                rightColor = SCREEN_THEME_PUSHED_BORDER_RIGHT_COLOR;
                bottomColor = SCREEN_THEME_PUSHED_BORDER_BOTTOM_COLOR;
            }
            if (IS_ELEM_DISABLED(state)) {
                topColor = SCREEN_THEME_DISABLED_BORDER_TOP_COLOR;
                leftColor = SCREEN_THEME_DISABLED_BORDER_LEFT_COLOR;
                rightColor = SCREEN_THEME_DISABLED_BORDER_RIGHT_COLOR;
                bottomColor = SCREEN_THEME_DISABLED_BORDER_BOTTOM_COLOR;
            }

            for (int i=1; i<SCREEN_THEME_BORDER_THICKNESS; i++) {
                graphics.hline(left - i, top - i, width + i * 2, COLOR(topColor));
                graphics.vline(left - i, top - i, height + i * 2, COLOR(leftColor));
                graphics.vline(left + width + i, top - i, height + i * 2, COLOR(rightColor));
                graphics.hline(left - i, top + height + i, width + i * 2, COLOR(bottomColor));
            }
        }

        void drawText(Graphics& graphics, int fsize, int align, Color fcolor) {
            int x = left, y = top;
            uint16_t w, h;
            graphics.getTextBounds(text.c_str(), fsize, w, h);
            switch (align) {
            case ALIGN_TOP_RIGHT:
                x += width - w; // right
                break;
            case ALIGN_TOP_CENTER:
                x += (width - w) / 2; // hcenter
                break;
            case ALIGN_BOTTOM_LEFT:
                y += height - h;
                break;
            case ALIGN_BOTTOM_RIGHT:
                y += height - h;
                x += width - w;
                break;
            case ALIGN_BOTTOM_CENTER:
                y += height - h;
                x += (width - w) / 2;
                break;
            case ALIGN_CENTER_LEFT:
                y += (height - h) / 2;
                break;
            case ALIGN_CENTER_RIGHT:
                y += (height - h) / 2;
                x += width - w;
                break;
            case ALIGN_CENTER_CENTER:
                y += (height - h) / 2;
                x += (width - w) / 2;
                break;
            }
            graphics.text(text.c_str(), x, y, fsize, fcolor);
        }

        void drawSurface(Graphics& graphics) {
            Color color, fcolor;
            int fsize, align;
            getProps(color, fcolor, align, fsize);
            graphics.fillRect(left, top, width+1, height+1, color);

            bool elemIsVSlider = IS_ELEM_VSLIDER(state);
            bool elemIsHSlider = IS_ELEM_HSLIDER(state);
            if (elemIsVSlider || elemIsHSlider) {
                int sx, sy, sw, sh;
                if (elemIsVSlider) {
                    sx = left + 1;
                    sy = top + slider - SCREEN_THEME_SLIDER_THICKNESS;
                    sw = width -1;
                    sh = SCREEN_THEME_SLIDER_THICKNESS * 2;
                }
                if (elemIsHSlider) {
                    sx = left + slider - SCREEN_THEME_SLIDER_THICKNESS;
                    sy = top + 1;
                    sw = SCREEN_THEME_SLIDER_THICKNESS * 2;
                    sh = height -1;
                }
                if (sx < left + 1) sx = left + 1;
                if (sy < top + 1) sy = top + 1;
                if (sx > left + width - SCREEN_THEME_SLIDER_THICKNESS * 2) sx = left + width - SCREEN_THEME_SLIDER_THICKNESS * 2;
                if (sy > top + height - SCREEN_THEME_SLIDER_THICKNESS * 2) sy = top + height - SCREEN_THEME_SLIDER_THICKNESS * 2;
                graphics.fillRect(sx, sy, sw, sh, COLOR(SCREEN_THEME_SLIDER_COLOR));
            }

            drawText(graphics, fsize, align, fcolor);
        }

        void drawAll(Graphics& graphics) {
            drawBorder(graphics);
            drawSurface(graphics);
        }

    };

    Graphics& graphics;
    Touch& touch;

    Elem prevs[SIZE];
    Elem elems[SIZE];
    size_t _size;
    Color color;


public:
    Screen(Graphics& graphics, Touch& touch, size_t size = SIZE, Color color = COLOR(SCREEN_THEME_COLOR)): graphics(graphics), touch(touch), _size(size), color(color) {
        reset(size);
        refresh();
    }

    Screen& reset() {
      for (int i=0; i<SIZE; i++) {
        elems[i].state = 0;      
        ELEM_SET_BORDERED(elems[i].state);
        elems[i].props = ELEM_PROPS(SCREEN_THEME_ELEM_COLOR, SCREEN_THEME_FONT_COLOR, SCREEN_THEME_TEXT_ALIGN, SCREEN_THEME_FONT_SIZE);
        elems[i].top = 0;
        elems[i].left = 0;
        elems[i].slider = 0;
        elems[i].width = SCREEN_THEME_ELEM_WIDTH;
        elems[i].height = SCREEN_THEME_ELEM_HEIGHT;
        elems[i].text = "";
        elems[i].touch = NULL;
        elems[i].release = NULL;
        elems[i].move = NULL;
      }
      return *this;      
    }

    Screen& reset(size_t _size) {
      reset();
      size(_size);
      return *this;
    }

    Screen& size(size_t size) {
      _size = size;
      return *this;
    }

    Screen& refresh() {
        // if (!_inloop) graphics.fillScreen(color);
        _refresh = true;
        return *this;
    }

    Elem& operator[](int i) { return elems[i]; }
    void loop() {
        Touch::State state = touch.getState();
        
        if (_refresh) {
          _refresh = false;
          graphics.fillScreen(color);
          for (int i=0; i<SIZE; i++) if (IS_ELEM_VISIBLE(elems[i].state)) ELEM_SET_HIDE(prevs[i].state);
        }

        for (int i=0; i<_size; i++) {
            Elem& elem = elems[i]; 
            bool elemIsVisible = IS_ELEM_VISIBLE(elem.state);
            bool elemIsSticky = IS_ELEM_STICKY(elem.state);
            bool elemIsVSlider = IS_ELEM_VSLIDER(elem.state);
            bool elemIsHSlider = IS_ELEM_HSLIDER(elem.state);
            
            
            // propagate events (if elem covers the event point)
            if (
                elemIsVisible && IS_ELEM_ENABLED(elem.state) && (
                    (
                        elem.top <= state.y && elem.top + elem.height >= state.y &&
                        elem.left <= state.x && elem.left + elem.width >= state.x
                    ) || (state.event == EVENT_RELEASE && !elemIsSticky)
                )
            ) {
                if (state.event != EVENT_IDLE && state.event != EVENT_RELEASE) {
                    if (elemIsVSlider) elem.slider = state.y-elem.top;
                    if (elemIsHSlider) elem.slider = state.x-elem.left;
                }

                switch (state.event) {
                    case EVENT_TOUCH:
                        ELEM_SET_PUSH(elem.state);
                        if (!elemIsSticky) {
                            if (elem.touch) elem.touch(state.x, state.y);
                        }
                        break;
                    case EVENT_RELEASE:
                        if (!elemIsSticky) {
                            ELEM_SET_RELEASE(elem.state);
                            if (elem.release) elem.release(state.x, state.y);
                        } else {
                            if (IS_ELEM_STUCK(elem.state)) {
                                ELEM_SET_UNSTUCK(elem.state);
                                if (elem.release) elem.release(state.x, state.y);
                                ELEM_SET_RELEASE(elem.state);
                            } else {
                                ELEM_SET_STUCK(elem.state);
                                if (elem.touch) elem.touch(state.x, state.y);
                            }
                        }
                        break;
                    case EVENT_MOVE:
                        if (elem.move) elem.move(state.x, state.y);
                        break;
                }
            }

            // refresh elems        
            Elem& prev = prevs[i];        
            if (prev == elem) continue;  

                        

            bool prevIsVisible = IS_ELEM_VISIBLE(prev.state);
            // just disapeared in the moment?
            if (prevIsVisible && !elemIsVisible) { 
                prev.clearAll(graphics, color);
                prev = elem;
                continue;
            }
            // just appeared in the moment?
            if (!prevIsVisible && elemIsVisible) {
                elem.drawAll(graphics);
                prev = elem;
                continue;
            }

            // go on please, nothing to see here..
            if (!elemIsVisible) continue;


            // bool elemIsPushed = IS_ELEM_PUSHED(elem.state);
            // if (IS_ELEM_PUSHED(prev.state) != elemIsPushed) {
            //   elem.drawBorder(graphics); // (OR if state changed?)
            //   if (elemIsPushed) ELEM_SET_PUSH(prev.state); else ELEM_SET_RELEASE(prev.state);
            // }

            // if position changed? => clearAll and re-drawAll
            // if size got smaller? => clearAll and re-drawAll            
            if (
                prev.top != elem.top || prev.left != elem.left ||
                prev.width > elem.width || prev.height > elem.height
            ) {
                prev.clearAll(graphics, color);
                if (elemIsVisible) elem.drawAll(graphics);
                prev = elem;
                continue;
            }

            // if size changed? (but not get smaller - see above) => re-drawAll
            if (prev.width != elem.width || prev.height != elem.height) {
                if (elemIsVisible) elem.drawAll(graphics);
                prev = elem;
                continue;
            }

            // if props or state are changed or slider moved? (color, fcolor, align, fsize)
            bool sliderIsMoved = (elemIsVSlider || elemIsHSlider) && elem.slider != prev.slider;
            if (prev.props != elem.props || prev.state != elem.state || sliderIsMoved) {
                Color pcolor, pfcolor, ecolor, efcolor;
                int pfsize, palign, efsize, ealign;
                prev.getProps(pcolor, pfcolor, pfsize, palign);
                elem.getProps(ecolor, efcolor, efsize, ealign);
                // surface color changed? => re-draw surface
				if (prev.state != elem.state) {
                	elem.drawBorder(graphics);
                    prev = elem;
				}

                if (pcolor != ecolor || sliderIsMoved) {
                    if (elemIsVisible) elem.drawSurface(graphics);
                    prev = elem;
                    continue;
                }
                // text align changed? => clear and re-draw text
                // text size changed? => clear and re-draw text
                if (palign != ealign || pfsize != efsize) {
                    prev.clearText(graphics);
                    if (elemIsVisible) elem.drawText(graphics, efsize, ealign, efcolor);
                    prev = elem;
                    continue;
                }
                // font color changed? => re-draw text
                if (pfcolor != efcolor) {
                    if (elemIsVisible) elem.drawText(graphics, efsize, ealign, efcolor);
                    prev = elem;
                    continue;
                }
            }

            // if text changed? => clear and re-draw text
            if (prev.text != elem.text) {
                prev.clearText(graphics);
                if (elemIsVisible) {
                    Color color, fcolor;
                    int fsize, align;
                    elem.getProps(color, fcolor, align, fsize);
                    elem.drawText(graphics, fsize, align, fcolor);
                }
                prev = elem;
                continue;
            }
        }
    }
};
