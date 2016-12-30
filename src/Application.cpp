#include "general.h"
#include "Application.h"
#include "Stages.h"
#include "PathStorage.h"

void StageTemplate::CreateText(uint32_t texture, uint32_t font, const char* text, int x, int y, SDL_Color color)
{
    m_textures[texture] = SDL_CreateTextureFromSurface(sApplication->GetRenderer(), TTF_RenderText_Solid(sApplication->GetFont(font), text, color));
    PrepareTextureDimension(texture, x, y);
}

void StageTemplate::PrepareTextureDimension(uint32_t texture, int x, int y)
{
    m_textureDims[texture] = { x, y, 0, 0 };
    SDL_QueryTexture(m_textures[texture], nullptr, nullptr, &m_textureDims[texture].w, &m_textureDims[texture].h);
}

Application::Application()
{
    m_requestedStage = -1;
    m_lastScore = (uint32_t)(-1);
}

bool Application::Init()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) == -1)
        return false;

    // init SDL_image library
    if (!IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG | IMG_INIT_TIF))
        return false;

    Mix_Init(MIX_INIT_MP3);
    Mix_OpenAudio(48000, AUDIO_S16SYS, 2, 640);

    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 2);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);

    SDL_Window* m_window = SDL_CreateWindow("Yummy Defender",
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        DEF_WIN_WIDTH, DEF_WIN_HEIGHT,
        SDL_WINDOW_OPENGL);

    if (!m_window)
        return false;

    m_renderer = SDL_CreateRenderer(m_window, -1, 0);

    SDL_SetHint(SDL_HINT_RENDER_VSYNC, "1");
    SDL_GL_SetSwapInterval(-1);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

    if (TTF_Init() == -1)
        return false;

    m_fonts[FONT_BIG] = TTF_OpenFont("data/appfont.ttf", 24);
    m_fonts[FONT_SMALLER] = TTF_OpenFont("data/appfont.ttf", 16);
    m_fonts[FONT_BIGGER] = TTF_OpenFont("data/appfont.ttf", 42);

    sPathStorage->Init();

    return true;
}

void Application::SetStage(StageType type)
{
    m_requestedStage = type;
}

int Application::Run()
{
    SDL_Event ev;

    m_stage = new MenuStage();
    m_stage->OnEnter();

    int quit = 0;
    // main application loop
    while (!quit)
    {
        while (SDL_PollEvent(&ev))
        {
            switch (ev.type)
            {
                case SDL_QUIT:
                    quit = true;
                    break;
                    // mouse move - store coordinates
                case SDL_MOUSEMOTION:
                    m_mouseX = ev.motion.x;
                    m_mouseY = ev.motion.y;
                    break;
                    // mouse click - signal UI and stage
                case SDL_MOUSEBUTTONDOWN:
                    m_stage->OnMouseClick(m_mouseX, m_mouseY, ev.button.button == 1);
                    break;
                    // key pressed event - signal UI and stage
                case SDL_KEYDOWN:
                    m_stage->OnKeyPress(ev.key.keysym.sym, true);
                    break;
                case SDL_KEYUP:
                    m_stage->OnKeyPress(ev.key.keysym.sym, false);
                    break;
            }
        }

        // update, draw
        m_stage->OnDraw();

        SDL_RenderPresent(m_renderer);

        if (m_requestedStage > -1)
        {
            m_stage->OnLeave();
            delete m_stage;
            m_stage = nullptr;

            switch (m_requestedStage)
            {
                case STAGE_MENU:
                    m_stage = new MenuStage();
                    break;
                case STAGE_GAMEINTRO:
                    m_stage = new GameIntroStage();
                    break;
                case STAGE_GAME:
                    m_stage = new GameStage();
                    break;
            }

            if (m_stage == nullptr)
                exit(5);

            m_stage->OnEnter();
            m_requestedStage = -1;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }

    SDL_Quit();

    return 0;
}

SDL_Renderer* Application::GetRenderer()
{
    return m_renderer;
}

TTF_Font* Application::GetFont(uint32_t fnt)
{
    return m_fonts[fnt];
}
