#pragma once


class SDLGraphics {
private:
    const int FONT_SIZE_MULTIPLIER = 16; // TODO: to condig
protected:
    SDL_Window* window;
    SDL_Renderer* renderer; 
    std::string fontname;

    void errorCheck() const {
        const char* sdlError = SDL_GetError();
        if (sdlError && *sdlError) {
            // Handle SDL error
            printf("SDL error: %s", sdlError);
            throw std::exception();
        }
    }
public:

    SDLGraphics(int width, int height, std::string _fontname = "fonts/ProsperoNbpRegular-3jJ3.ttf") {
        fontname = _fontname;

        // Initialize the TTF library
        if (TTF_Init() == -1) {
            printf("Failed to initialize TTF: %s", TTF_GetError());
            throw std::exception();
        }
        errorCheck();
        
        // Initialize SDL
        SDL_Init(SDL_INIT_VIDEO);
        errorCheck();

        // Create a window
        window = SDL_CreateWindow("My Window", 100, 100, width, height, SDL_WINDOW_SHOWN);
        errorCheck();

        // Create a renderer
        renderer = SDL_CreateRenderer(window, -1, 0);
        errorCheck();
    }

    void close() const {
        // if (!began) return *this;

        // Quit the TTF library
        TTF_Quit();
        errorCheck();

        // Clean up
        SDL_DestroyRenderer(renderer);
        errorCheck();
        SDL_DestroyWindow(window);
        errorCheck();
        SDL_Quit();
        errorCheck();
    }

    int getClick(int& x, int& y) const {
        int e = EVENT_IDLE;
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            errorCheck();
            switch (event.type) {
                case SDL_QUIT:
                    close();
                    exit(0);
                case SDL_MOUSEBUTTONDOWN:
                    x = event.button.x;
                    y = event.button.y;
                    e = EVENT_TOUCH;
                    break;
                case SDL_MOUSEBUTTONUP:
                    x = event.button.x;
                    y = event.button.y;
                    e = EVENT_RELEASE;
                    break;
                case SDL_MOUSEMOTION:
                    x = event.button.x;
                    y = event.button.y;
                    e = EVENT_MOVE;
                    break;
            }
        }
        errorCheck();
        return e;
    }

    void present() const {
        SDL_RenderPresent(renderer);
        errorCheck();
    }

    ~SDLGraphics() {
        close();
    }

    void line(int x1, int y1, int x2, int y2, Color color) {
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        errorCheck();
        SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
        errorCheck();
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
        errorCheck();
    }

    void vline(int x, int y, int l, Color c) {
        line(x, y, x, y+l, c);
    }

    void hline(int x, int y, int l, Color c) {
        line(x, y, x+l, y, c);
    }

    void fillRect(int x, int y, int w, int h, Color c) {
        // Set the draw color to the specified color
        SDL_SetRenderDrawColor(renderer, c.r, c.g, c.b, c.a);
        errorCheck();

        // Define a rectangle with the specified parameters
        SDL_Rect rect = { x, y, w+1, h+1 };

        // Fill the rectangle with the draw color
        SDL_RenderFillRect(renderer, &rect);
        errorCheck();
    }

    void text(String txt, int x, int y, int size, Color c) {
        if (txt.empty()) return;
        TTF_Font* font = TTF_OpenFont(fontname.c_str(), size*FONT_SIZE_MULTIPLIER);
        if (!font) {
            printf("Failed to load font: %s", TTF_GetError());
            throw std::exception();
        }
        errorCheck();

        SDL_Color sdlColor = {c.r, c.g, c.b, c.a}; 
        SDL_Surface* surface = TTF_RenderText_Solid(font, txt.c_str(), sdlColor);
        errorCheck();
        if (!surface) {
            TTF_CloseFont(font);
            errorCheck();
            printf("Failed to create surface: %s", SDL_GetError());
            throw std::exception();
        }

        SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
        errorCheck();
        if (!texture) {
            SDL_FreeSurface(surface);
            errorCheck();
            TTF_CloseFont(font);
            errorCheck();
            printf("Failed to create texture: %s", SDL_GetError());
            throw std::exception();
        }

        SDL_Rect rect = {x, y, surface->w, surface->h};
        SDL_RenderCopy(renderer, texture, NULL, &rect);
        errorCheck();

        SDL_DestroyTexture(texture);
        errorCheck();
        SDL_FreeSurface(surface);
        errorCheck();
        TTF_CloseFont(font);
        errorCheck();
    }

    void getTextBounds(String txt, int size, uint16_t &w, uint16_t &h) {
        // Load the font you want to use.
        TTF_Font* font = TTF_OpenFont(fontname.c_str(), size*FONT_SIZE_MULTIPLIER); // TODO: make setFont method and open the font only the and set to a private member, do not open every time. - and dont forget about the close

        // The color you want to use for the text.
        // SDL_Color color = { 255, 255, 255, 255 };

        // Get the size of the text when rendered with the given font.
        int textWidth = w, textHeight = h;
        TTF_SizeText(font, txt.c_str(), &textWidth, &textHeight);
        w = textWidth;
        h = textHeight;
        // Clean up.
        TTF_CloseFont(font);
    }

    void fillScreen(Color c) {
        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        fillRect(0,0, w, h, c);
    }
};
