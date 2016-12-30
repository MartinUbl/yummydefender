#ifndef YD_APPLICATION_H
#define YD_APPLICATION_H

#include "Singleton.h"

#define DEF_WIN_WIDTH 1229
#define DEF_WIN_HEIGHT 691

enum AppFonts
{
    FONT_BIG = 0,
    FONT_SMALLER = 1,
    FONT_BIGGER = 2,
};

enum StageType;
class StageTemplate;

class Application
{
    public:
        Application();

        bool Init();
        int Run();

        SDL_Renderer* GetRenderer();
        TTF_Font* GetFont(uint32_t fnt);

        uint32_t GetWindowWidth() { return DEF_WIN_WIDTH; };
        uint32_t GetWindowHeight() { return DEF_WIN_HEIGHT; };

        void SetStage(StageType st);

        void SetGameOver(bool state) { m_gameOver = state; };
        bool IsGameOver() { return m_gameOver; };

        void SetLastScore(uint32_t score) { m_lastScore = score; };
        uint32_t GetLastScore() { return m_lastScore; };

    protected:
        SDL_Renderer* m_renderer;
        SDL_Window* m_window;
        std::map<uint32_t, TTF_Font*> m_fonts;
        StageTemplate* m_stage;

        int m_requestedStage;
        bool m_gameOver;

        uint32_t m_lastScore;

    private:
        int32_t m_mouseX, m_mouseY;
};

#define sApplication Singleton<Application>::getInstance()

#endif
