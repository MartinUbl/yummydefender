#include "general.h"
#include "Stages.h"
#include "Application.h"

GameIntroStage::GameIntroStage() : StageTemplate(STAGE_GAMEINTRO)
{
    fadePause = 0;
    fadeIn = true;
}

void GameIntroStage::OnEnter()
{
    fadeStart = clock();
    fadeEnd = clock() + 1000;
    fadeIn = true;

    SDL_SetRenderDrawColor(sApplication->GetRenderer(), 0, 20, 0, 0);

    CreateText(0, FONT_BIG, "This land...", 50, 300, { 0, 200, 0, 0 });
    textNo = 0;
}

void GameIntroStage::OnLeave()
{
    //
}

void GameIntroStage::OnDraw()
{
    SDL_RenderClear(sApplication->GetRenderer());

    float opacity = 0;
    if (fadePause != 0)
    {
        if (fadeIn)
            opacity = 1;

        if (fadePause <= clock())
        {
            fadePause = 0;
            fadeIn = !fadeIn;
            fadeStart = clock();
            fadeEnd = clock() + 1000;
        }
    }
    else
    {
        if (fadeEnd <= clock())
        {
            opacity = 1;
            fadePause = clock() + 1000;

            if (!fadeIn)
            {
                SDL_DestroyTexture(m_textures[0]);
                textNo++;
                switch (textNo)
                {
                    case 0: CreateText(0, FONT_BIG, "This land...", 50, 300, { 0, 200, 0, 0 }); break;
                    case 1: CreateText(0, FONT_BIG, "Once full of life, love...", 50, 300, { 0, 200, 0, 0 }); break;
                    case 2: CreateText(0, FONT_BIG, "...flowers bloom, birds singing their love songs...", 50, 300, { 0, 200, 0, 0 }); break;
                    case 3: SDL_SetRenderDrawColor(sApplication->GetRenderer(), 20, 20, 0, 0); CreateText(0, FONT_BIG, "But then they came.", 50, 300, { 200, 200, 0, 0 }); break;
                    case 4: CreateText(0, FONT_BIG, "Vicious creatures from another world", 50, 300, { 200, 200, 0, 0 }); break;
                    case 5: CreateText(0, FONT_BIG, "Hungry for our most precious fruit - gingerberries", 50, 300, { 200, 200, 0, 0 }); break;
                    case 6: CreateText(0, FONT_BIG, "Our land slowly became desolated as monsters came", 50, 300, { 200, 200, 0, 0 }); break;
                    case 7: CreateText(0, FONT_BIG, "Now just single small garden of gingerberries left", 50, 300, { 200, 200, 0, 0 }); break;
                    case 8: CreateText(0, FONT_BIG, "And nobody has enough strength to protect our precious fruit", 50, 300, { 200, 200, 0, 0 }); break;
                    case 9: CreateText(0, FONT_BIG, "But then... you came", 50, 300, { 200, 200, 0, 0 }); break;
                    case 10: CreateText(0, FONT_BIG, "Are you strong and tenacious enough to defend the last garden?", 50, 300, { 127, 255, 0, 0 }); break;
                    case 11: sApplication->SetStage(STAGE_GAME); break;
                }
            }
        }
        else
            opacity = (float)(clock() - fadeStart) / (float)(fadeEnd - fadeStart);
        if (!fadeIn)
            opacity = 1.0f - opacity;
    }

    SDL_SetTextureBlendMode(m_textures[0], SDL_BLENDMODE_BLEND);
    SDL_SetTextureAlphaMod(m_textures[0], (uint8_t)(opacity * 255.0f));
    SDL_RenderCopy(sApplication->GetRenderer(), m_textures[0], nullptr, &m_textureDims[0]);
}

void GameIntroStage::OnKeyPress(uint16_t key, bool press)
{
    sApplication->SetStage(STAGE_GAME);
}

void GameIntroStage::OnMouseClick(int32_t x, int32_t y, bool left)
{
    //
}
