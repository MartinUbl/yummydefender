#include "general.h"
#include "Stages.h"
#include "Application.h"
#include "Object.h"

#include <string>

MenuStage::MenuStage() : StageTemplate(STAGE_MENU)
{
    //
}

void MenuStage::OnEnter()
{
    CreateText(0, FONT_BIGGER, "Yummy Defender", 20, 13);
    CreateText(1, FONT_SMALLER, "I'm too lazy to create nice menu", 20, 70);

    CreateText(2, FONT_SMALLER, "Just click this button: ", 20, 90);
    CreateText(3, FONT_SMALLER, "START", 220, 110, { 0, 62, 0, 0 });


    CreateText(4, FONT_SMALLER, "Controls: ", 20, 200);
    CreateText(5, FONT_SMALLER, "W, A, S, D = move", 150, 200);
    CreateText(6, FONT_SMALLER, "LEFT mouse button  = rotate, hit nearby enemies (costs 25 energy)", 150, 220);
    CreateText(7, FONT_SMALLER, "RIGHT mouse button = plant / detonate bomb (plant once per 4 seconds)", 150, 240);
    CreateText(8, FONT_SMALLER, "Player has total of 100 energy points.", 20, 270);

    if (sApplication->GetLastScore() != (uint32_t)(-1))
        CreateText(99, FONT_BIG, (std::string("Last score: ") + std::to_string(sApplication->GetLastScore())).c_str(), 20, DEF_WIN_HEIGHT - 35);
}

void MenuStage::OnLeave()
{
    //
}

void MenuStage::OnDraw()
{
    boxRGBA(sApplication->GetRenderer(), 170, 95, 320, 140, 0, 127, 0, 255);

    for (std::pair<uint32_t, SDL_Texture*> txt : m_textures)
        SDL_RenderCopy(sApplication->GetRenderer(), txt.second, nullptr, &m_textureDims[txt.first]);
}

void MenuStage::OnKeyPress(uint16_t key, bool press)
{
    //
}

void MenuStage::OnMouseClick(int32_t x, int32_t y, bool left)
{
    if (x > 170 && x < 320 && y > 75 && y < 120)
    {
        sApplication->SetStage(STAGE_GAMEINTRO);
    }
}
