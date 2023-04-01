#pragma once


class Touch {
protected:
    SDLGraphics& SDL;
public:
    struct State {int event; int x; int y;} state; 
    Touch(SDLGraphics& SDL): SDL(SDL) {}
    State& getState() {
        state.event = SDL.getClick(state.x, state.y);
        return state;
    }
};
