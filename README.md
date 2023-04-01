# Lightweight UI Buttons for `SDL` and `Arduino`
## Install
- Download this repository and place it on `{your-workplace-folder}/Arduino/libraries` folder (may you have to create it if you don't have any library installed)
- include `<ScreenThemeXXX.h>` and `<Screen.h>`

## Themes
You can define a new theme based on the exists or just use any existing theme by including the theme definition file:
```
#include <ScreenThemeXXX.h>
```
At the time I write this, the library contains two themes:
 - `ScreenThemeGray.h` - Grey old Windows-style buttons
 - `ScreenThemeNeon.h` - Neon-style buttons

 ## Usages

 To use the Screen UI buttons library first you have to include a theme and then the library itself (order is important)
```
#include <Screen.h>
```

Next, you have to create a `Graphics` and `Touch` object that will be used by the main `Screen` object:
```
Graphics graphics(tft);
Touch touch(ts, YP, XM);
```
And then create a Screen template object with the number of UI elements in it. For example, if you need `5` button use the following:
```
Screen<5> screen(graphics, touch);
```

Then you have to add the `Screen::loop()` function in your main loop so that the screen can automatically update the UI when any interaction happens:
```
loop() {
    screen.loop();
}
```

## `Graphics`
When creating the `Graphics` object you have to pass an `Adafruit_GFX` object or any extended version of it in the constructor parameter that will be used to render the UI elements. In the examples I used the `MCUFRIEND_kbv` library:
```
#include <MCUFRIEND_kbv.h>
MCUFRIEND_kbv tft;
Graphics graphics(tft);
```
If you duing the same you may want to setup your screen as usual in the `setup()` function:
```
setup() {
    tft.reset();
    uint16_t ID = tft.readID();
    tft.begin(ID);
    tft.invertDisplay(true);
    tft.setRotation(2);
    tft.fillScreen(COLOR(EGA_BLACK));
    ...
```
## `Touch`
When creating the `Touch` object you will need to pass a `TouchScreen` object, and some additional parameters:
```

#define YP A1  // must be an analog pin, use "An" notation!   a2   a1
#define XM A2  // must be an analog pin, use "An" notation!   a3   a2
#define YM 7   // can be a digital pin                        8    7
#define XP 6   // can be a digital pin                        9    6
TouchScreen ts = TouchScreen(XP, YP, XM, YM, 300);

int rotation = 0;
Touch touch(ts, YP, XM, rotation);
```
And then you have to `begin` to prompt the user to the calibration in the setup by passing the screen size and the `Graphics` object:
```
Touch::Calibration calibration = touch.begin(320, 240, graphics);
// ... save the calibration

```
This will returns a `Calibration` struct that you can save (for e.g into the prog-mem or an SD card) and then you can use after the next restart:
```
// ... load the calibration and or prompt the user "if" this is the first start:
touch.begin(320, 240, calibration.tsMinX, calibration.tsMinY, calibration.tsMaxX, calibration.tsMaxY, calibration.alpha, calibration.cmax, calibration.cmin);

```
## `Touch::Calibration` and `Touch::begin()`

For touch screen anti-spin the `Touch` object use and EMA (exponential moving average calculation with high/low threshold) that could be differ on each device thepending on the touch-screen sensiticity and device speed, you have to make an estimated pre-calculation to find the best parameters. By default the following constats are used:
```
#define SCREEN_DEFAULT_CLK_ALPHA 10
#define SCREEN_DEFAULT_CLK_MAX 150
#define SCREEN_DEFAULT_CLK_MIN 30
```
You can predefine them before including the `Screen.h` or by passing them directly to the `Touch::begin` function as optional parameters.

The `Touch::begin` function calibration parameters and the `Calibration` sturct:
```
struct Calibration {
    
    // Top-left corner
    unsigned int tsMinX; 
    unsigned int tsMinY;

    // Bottom-right corner
    unsigned int tsMaxX;
    unsigned int tsMaxY;

    // touch EMA settings:
    double alpha;
    double cmax;
    double cmin;
};
```

## `Screen`
After you created a `Screen` object, for e.g:
```
Screen<5> screen(gfx, ts);
```
then you can access to each element as an array like:
```
Screen::Elem elem = screen[n]; // where `n` is the index of UI element (starts from zero)
```

## `Screen::reset(size_t size)`
Resets the screen and redraw all element. `size` parameter is optional, tells the `Screen` object how many elemet you are using. It's usefull when you set different pages and just redefined the screen elements bt using less or even more elements.

Example:
```
screen.reset(3); // the screen.loop() method will taking care only the first 3 elements
```
## `Screen::size(size_t size)`
Same as the `Screen::reset(size_t size)` but it won't reset the elements onli re-size the screen.

Example:
```
screen.size(3); // the screen.loop() method will taking care only the first 3 elements
```
## `Screen::refresh()`
Refreshes the screen. Usefull when you re-define the UI elements or re-size the screen.

Example:
```
screen.refresh(); // clears the screen and re-draws the elements
```

## `Screen::color`
The default Screen background color is defined by the theme file (`ScreenThemeXXX.h`) but you can override this value from the color palette by using the `COLOR()` macro:
```
screen.color = COLOR(EGA_BRIGHT_BLUE);
```
or by using the direct `Color` type which is different on each platform. If you are using SDL library, the Color will be an RED/GREEN/BLUE/Alpha SDL_Color struct: 
```
struct SDL_Color_ext {
    Uint8 r;
    Uint8 g;
    Uint8 b;
    Uint8 a;
};
```
While on `Arduino` it's typically an `uint16_t` RGB565 formatted color for eg:
```
color = 0x1234;
```

## `Screen::Elem` struct
Each UI elements (buttons) on the screen represented by the `Elem` struct and it's properties can be overwritten any time and the `Screen::loop()` function will update it accordingly
```
struct Elem {
    uint16_t top = 0, left = 0, slider = 0, width = SCREEN_THEME_ELEM_WIDTH, height = SCREEN_THEME_ELEM_HEIGHT;
    String text = "";
    uint16_t props = ELEM_PROPS(SCREEN_THEME_ELEM_COLOR, SCREEN_THEME_FONT_COLOR, SCREEN_THEME_TEXT_ALIGN, SCREEN_THEME_FONT_SIZE);
    uint8_t state = 0;

    typedef void (*Event)(int x, int y);
    Event touch = NULL, release = NULL, move = NULL;
    ...
}
```

## `Screen::Elem::Top/Left/Width/Height/Text`
The size and text of an elemenet.

Example:
```
scr[n].top = 10;
scr[n].left = 20;
scr[n].width = 140;
scr[n].height = 50;
scr[n].text = "Touch ME!"
```

## `Screen::touch/release/move`
The event handlers that can be attached to an element.

Example:
```
void touch_event(int x, int y) {
    Serial.println("Button touched!");
}

...

screen[n].touch = touch_event;
```
Note: each event handler needs two (x/y) parameter that will be passed where the event happend.

## Screen::Elem::props
Elem properties, packed into a single `uint16_t` variable containing the following properties:
 - 0-3 bits: element color (using 16 color EGA_XXXX palette)
 - 4-7 bits: font color (using 16 color EGA_XXXX palette)
 - 8-11 bits: text alignments:
 ```
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
 ```
 - 12-15 bits: font size
 
You can read and override the `props` with the following macros:
```
#define ELEM_PROPS(color, fcolor, align, fsize) ((uint16_t)(color) | ((uint16_t)(fcolor) << 4) | ((uint16_t)(align) << 8) | ((uint16_t)(fsize) << 12))
#define ELEM_SET_COLOR(props, color) ((props) = ((props) & 0xFFF0U) | ((uint16_t)(color)))
#define ELEM_SET_FCOLOR(props, fcolor) ((props) = ((props) & 0xFF0FU) | ((uint16_t)(fcolor) << 4))
#define ELEM_SET_ALIGN(props, align) ((props) = ((props) & 0xF0FFU) | ((uint16_t)(align) << 8))
#define ELEM_SET_FSIZE(props, fsize) ((props) = ((props) & 0x0FFFU) | ((uint16_t)(fsize) << 12))
#define ELEM_GET_COLOR(props) ((uint16_t)((props) & 0x000FU))
#define ELEM_GET_FCOLOR(props) ((uint16_t)(((props) & 0x00F0U) >> 4))
#define ELEM_GET_ALIGN(props) ((uint16_t)(((props) & 0x0F00U) >> 8))
#define ELEM_GET_FSIZE(props) ((uint16_t)(((props) & 0xF000U) >> 12))
```

Example:
```
// sets the element red and the text to blue:
ELEM_SET_COLOR(screen[n].props, EGA_RED);
ELEM_SET_FCOLOR(screen[n].props, EGA_BLUE);

// reads the font size:
int font_size = ELEM_GET_FSIZE(screen[n].props);
```

## `Screen::Elem::state`
uint8_t combined 8 bits value that contains the element "state". Each element can be pushed/hidden/disabled etc.: 
```

#define ELEM_STATE_PUSHED   (uint8_t)0b00000001 // 1
#define ELEM_STATE_DISABLED (uint8_t)0b00000010 // 2
#define ELEM_STATE_HIDDEN   (uint8_t)0b00000100 // 4
#define ELEM_STATE_STICKY   (uint8_t)0b00001000 // 8
#define ELEM_STATE_STUCK    (uint8_t)0b00010000 // 16
#define ELEM_STATE_BORDER   (uint8_t)0b00100000 // 32
#define ELEM_STATE_VSLIDER  (uint8_t)0b01000000 // 64
#define ELEM_STATE_HSLIDER  (uint8_t)0b10000000 // 128
```

The following macros can be used the read and override an element state:
```
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
```

Examples:
```
// hide an element:
ELEM_SET_HIDE(screen[n].state);

// set the element as a "sticky" button
ELEM_SET_STICKY(screen[n].state);

// disable an element:
ELEM_SET_DISABLE(screen[n].state);

// enable an element:
ELEM_SET_ENABLE(screen[n].state);
```

## Slider elements
You can make slider elements by using the "slider state" see more in the slider example at `examples/slider/slider.ino`.


