#include "screen.hpp"

#include <SDL2/SDL.h>

#include <cassert>
#include <cstring>
#include <map>
#include <thread>
#include <mutex>
#include <atomic>
#include <vector>

class SDLSingleton
{
    struct Window
    {
        ArScreen screen;
        int width;
        int height;
        bool created;

        Window(ArScreen s, int w, int h) : screen(s), width(w), height(h), created(false)
        {
            assert(screen);
        }
        ~Window()
        {
            if (screen && created)
            {
                SDL_DestroyTexture(screen->texture);
                SDL_DestroyRenderer(screen->renderer);
                SDL_DestroyWindow(screen->win);
            }
        }

        void Create()
        {
            screen->win = SDL_CreateWindow("Hello World", 100, 100, width, height, 0);
            screen->renderer = SDL_CreateRenderer(screen->win, -1, SDL_RENDERER_ACCELERATED);
            screen->texture = SDL_CreateTexture(screen->renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STREAMING, width, height);
            created = true;
        }

        bool Update()
        {
            if (created)
            {
                SDL_Event e;
                if (SDL_PollEvent(&e))
                {
                    if (e.type == SDL_QUIT)
                    {
                        return true;
                    }
                }

                SDL_RenderClear(screen->renderer);
                SDL_RenderCopy(screen->renderer, screen->texture, NULL, NULL);
                SDL_RenderPresent(screen->renderer);
            }

            return false;
        }
    };

    std::atomic<bool> mustClose;
    std::map<ArScreen, Window> windows;
    std::mutex mutex;

    std::thread thread;

    SDLSingleton()
    {
        mustClose = false;
        thread = std::thread([this]
            {
                struct SDLRRID
                {
                    SDLRRID()
                    {
                        SDL_Init(SDL_INIT_VIDEO);
                    }
                    ~SDLRRID()
                    {
                        SDL_Quit();
                    }
                };
                SDLRRID instance;

                while (mustClose == false || windows.empty() == false)
                {
                    std::lock_guard guard(mutex);
                    std::vector<std::map<ArScreen, Window>::iterator> toDelete;
                    for (auto it = windows.begin(); it != windows.end(); ++it)
                    {
                        if (it->second.created == false)
                            it->second.Create();

                        if (it->second.Update())
                        {
                            toDelete.push_back(it);
                        }
                    }
                    for (auto it : toDelete)
                    {
                        windows.erase(it);
                    }
                }
            }
        );
    }

public:
    ~SDLSingleton()
    {
        mustClose = true;

        thread.join();
    }

    static SDLSingleton& instance()
    {
        static SDLSingleton i;
        return i;
    }

    void pushScreen(ArScreen screen, int width, int height)
    {
        std::lock_guard guard(mutex);
        windows.insert(std::make_pair(screen, Window(screen, width, height)));
    }

    void waitForClosed(ArScreen screen)
    {
        for (;;)
        {
            std::lock_guard guard(mutex);
            if (windows.find(screen) == windows.end())
                break;
        }
    }
};

ArResult arCreateScreen(ArVirtualMachine virtualMachine, const ArScreenCreateInfo* pInfo, ArScreen* pScreen)
{
    assert(virtualMachine);
    assert(pInfo);
    assert(pInfo->sType == AR_STRUCTURE_TYPE_SCREEN_CREATE_INFO);
    assert(pInfo->width > 0);
    assert(pInfo->height > 0);
    assert(pInfo->graphicsProcessor != NULL);
    assert(pScreen);

    const ArScreen output = new ArScreen_T;
    if(!output)
    {
        return AR_ERROR_HOST_OUT_OF_MEMORY;
    }

    memset(output, 0, sizeof(ArScreen_T));

    output->parent = virtualMachine;
    output->graphicsProcessor = pInfo->graphicsProcessor;

    int width = pInfo->width;
    int height = pInfo->height;
    SDLSingleton::instance().pushScreen(output, width, height);

    *pScreen = output;

    return AR_SUCCESS;
}

void arDestroyScreen(ArVirtualMachine virtualMachine, ArScreen screen)
{
    assert(virtualMachine);

    if (screen)
    {
        SDLSingleton::instance().waitForClosed(screen);

        delete screen;
    }
}
