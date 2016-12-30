#include "general.h"
#include "ImageStorage.h"
#include "Application.h"

#include <string>

static const char* imgFileMap[MAX_GI] = {
    "player/b1.png", "player/b0.png",
    "player/br1.png", "player/br0.png",
    "player/r1.png", "player/r0.png",
    "player/fr1.png", "player/fr0.png",
    "player/f1.png", "player/f0.png",
    "player/fl1.png", "player/fl0.png",
    "player/l1.png", "player/l0.png",
    "player/bl1.png", "player/bl0.png",

    "monster1/b1.png", "monster1/b2.png", "monster1/b3.png", "monster1/b4.png", "monster1/b5.png", "monster1/b6.png", "monster1/b7.png", "monster1/b8.png", "monster1/b0.png",
    "monster1/br1.png", "monster1/br2.png", "monster1/br3.png", "monster1/br4.png", "monster1/br5.png", "monster1/br6.png", "monster1/br7.png", "monster1/br8.png", "monster1/br0.png",
    "monster1/r1.png", "monster1/r2.png", "monster1/r3.png", "monster1/r4.png", "monster1/r5.png", "monster1/r6.png", "monster1/r7.png", "monster1/r7.png", "monster1/r0.png",
    "monster1/fr1.png", "monster1/fr2.png", "monster1/fr3.png", "monster1/fr4.png", "monster1/fr5.png", "monster1/fr6.png", "monster1/fr7.png", "monster1/fr8.png", "monster1/fr0.png",
    "monster1/f1.png", "monster1/f2.png", "monster1/f3.png", "monster1/f4.png", "monster1/f5.png", "monster1/f6.png", "monster1/f7.png", "monster1/f8.png", "monster1/f0.png",
    "monster1/fl1.png", "monster1/fl2.png", "monster1/fl3.png", "monster1/fl4.png", "monster1/fl5.png", "monster1/fl6.png", "monster1/fl7.png", "monster1/fl8.png", "monster1/fl0.png",
    "monster1/l1.png", "monster1/l2.png", "monster1/l3.png", "monster1/l4.png", "monster1/l5.png", "monster1/l6.png", "monster1/l7.png", "monster1/l8.png", "monster1/l0.png",
    "monster1/bl1.png", "monster1/bl2.png", "monster1/bl3.png", "monster1/bl4.png", "monster1/bl5.png", "monster1/bl6.png", "monster1/bl7.png", "monster1/bl8.png", "monster1/l0.png",
    "monster1/dead.png",

    "map/dry.png",
    "map/berries/0.png","map/berries/1.png","map/berries/2.png","map/berries/3.png","map/berries/4.png",

    "gingerberry.png",
    "player/starfeature.png",
    "bomb.png",

    "monster1/thr1.png", "monster1/thr2.png", "monster1/thr3.png", "monster1/thr4.png", "monster1/thr5.png", "monster1/thr6.png", "monster1/thr7.png", "monster1/thr8.png",
    "monster1/catch1.png", "monster1/catch2.png", "monster1/catch3.png",

    "arci/arci1.png", "arci/arci2.png", "arci/arci3.png", "arci/arci4.png", "arci/arci5.png", "arci/arci6.png", "arci/arci7.png", "arci/arci8.png",

    "map/nice_phase1.png", "map/nice_phase2.png", "map/nice_phase3.png", "map/nice_phase4.png", "map/nice_phase5.png",
    "player/pr1.png", "player/pr2.png", "player/pr3.png",
    "player/prl1.png", "player/prl2.png", "player/prl3.png", "player/prl4.png",
    "player/sr1.png"
};

ImageStorage::ImageStorage()
{
    //
}

bool ImageStorage::LoadTexture(GameImages img)
{
    if (img > MAX_GI)
        return false;

    std::string path = std::string("data/") + imgFileMap[img];

    SDL_Surface* surf = IMG_Load(path.c_str());
    if (!surf)
        return false;

    m_textures[img] = SDL_CreateTextureFromSurface(sApplication->GetRenderer(), surf);
    if (!m_textures[img])
        return false;

    return true;
}

SDL_Texture* ImageStorage::GetImage(GameImages img)
{
    if (m_textures.find(img) != m_textures.end())
        return m_textures[img];

    if (!LoadTexture(img))
        return nullptr;

    return m_textures[img];
}

SDL_Texture* ImageStorage::GetBigNumberImage(int num)
{
    if (m_numTextures.find(num) != m_numTextures.end())
        return m_numTextures[num];

    std::string toprint = (num >= 0 && num <= 9) ? std::to_string(num) : ":";

    m_numTextures[num] = SDL_CreateTextureFromSurface(sApplication->GetRenderer(), TTF_RenderText_Solid(sApplication->GetFont(FONT_BIG), toprint.c_str(), { 127, 255, 127, 0 }));

    return m_numTextures[num];
}
