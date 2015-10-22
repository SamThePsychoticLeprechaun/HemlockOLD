#include "Timing.h"

#include <SDL\SDL.h>

namespace Xylem
{
    FpsLimiter::FpsLimiter()
    {
    }

    void FpsLimiter::init(float maxFps)
    {
        setMaxFps(maxFps);
    }

    void FpsLimiter::setMaxFps(float maxFps)
    {
        _maxFPS = maxFps;
    }

    void FpsLimiter::begin()
    {
        _startTicks = SDL_GetTicks();
    }

    float FpsLimiter::end()
    {
        calculateFps();

        float frameTicks = SDL_GetTicks() - _startTicks;
        if (1000.0f / _maxFPS > frameTicks) {
            SDL_Delay((1000.0f / _maxFPS) - frameTicks);
        }

        return _fps;
    }

    void FpsLimiter::calculateFps()
    {
        static const int NUM_SAMPLES = 10;
        static float frameTimes[NUM_SAMPLES];
        static int currentFrame = 0;

        static float previousTicks = SDL_GetTicks();
        float currentTicks = SDL_GetTicks();

        _frameTime = currentTicks - previousTicks;
        frameTimes[currentFrame % NUM_SAMPLES] = _frameTime;

        previousTicks = currentTicks;

        int count;

        ++currentFrame;
        if (currentFrame < NUM_SAMPLES) {
            count = currentFrame;
        } else {
            count = NUM_SAMPLES;
        }

        float frameTimeAverage = 0;
        for (size_t i = 0; i < count; ++i) {
            frameTimeAverage += frameTimes[i];
        }
        frameTimeAverage /= count;

        if (frameTimeAverage > 0) {
            _fps = 1000.0f / frameTimeAverage;
        } else {
            _fps = 60.0f;
        }
    }
}