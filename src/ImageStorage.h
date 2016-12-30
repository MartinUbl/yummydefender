#ifndef YD_IMAGESTORAGE_H
#define YD_IMAGESTORAGE_H

#include "Singleton.h"

enum GameImages
{
    GI_PLAYER_UP_1 = 0, GI_PLAYER_UP_IDLE,
    GI_PLAYER_UPRIGHT_1, GI_PLAYER_UPRIGHT_IDLE,
    GI_PLAYER_RIGHT_1, GI_PLAYER_RIGHT_IDLE,
    GI_PLAYER_DOWNRIGHT_1, GI_PLAYER_DOWNRIGHT_IDLE,
    GI_PLAYER_DOWN_1, GI_PLAYER_DOWN_IDLE,
    GI_PLAYER_DOWNLEFT_1, GI_PLAYER_DOWNLEFT_IDLE,
    GI_PLAYER_LEFT_1, GI_PLAYER_LEFT_IDLE,
    GI_PLAYER_UPLEFT_1, GI_PLAYER_UPLEFT_IDLE,

    GI_MONSTER_UP_1, GI_MONSTER_UP_2, GI_MONSTER_UP_3, GI_MONSTER_UP_4, GI_MONSTER_UP_5, GI_MONSTER_UP_6, GI_MONSTER_UP_7, GI_MONSTER_UP_8, GI_MONSTER_UP_IDLE,
    GI_MONSTER_UPRIGHT_1, GI_MONSTER_UPRIGHT_2, GI_MONSTER_UPRIGHT_3, GI_MONSTER_UPRIGHT_4, GI_MONSTER_UPRIGHT_5, GI_MONSTER_UPRIGHT_6, GI_MONSTER_UPRIGHT_7, GI_MONSTER_UPRIGHT_8, GI_MONSTER_UPRIGHT_IDLE,
    GI_MONSTER_RIGHT_1, GI_MONSTER_RIGHT_2, GI_MONSTER_RIGHT_3, GI_MONSTER_RIGHT_4, GI_MONSTER_RIGHT_5, GI_MONSTER_RIGHT_6, GI_MONSTER_RIGHT_7, GI_MONSTER_RIGHT_8, GI_MONSTER_RIGHT_IDLE,
    GI_MONSTER_DOWNRIGHT_1, GI_MONSTER_DOWNRIGHT_2, GI_MONSTER_DOWNRIGHT_3, GI_MONSTER_DOWNRIGHT_4, GI_MONSTER_DOWNRIGHT_5, GI_MONSTER_DOWNRIGHT_6, GI_MONSTER_DOWNRIGHT_7, GI_MONSTER_DOWNRIGHT_8, GI_MONSTER_DOWNRIGHT_IDLE,
    GI_MONSTER_DOWN_1, GI_MONSTER_DOWN_2, GI_MONSTER_DOWN_3, GI_MONSTER_DOWN_4, GI_MONSTER_DOWN_5, GI_MONSTER_DOWN_6, GI_MONSTER_DOWN_7, GI_MONSTER_DOWN_8, GI_MONSTER_DOWN_IDLE,
    GI_MONSTER_DOWNLEFT_1, GI_MONSTER_DOWNLEFT_2, GI_MONSTER_DOWNLEFT_3, GI_MONSTER_DOWNLEFT_4, GI_MONSTER_DOWNLEFT_5, GI_MONSTER_DOWNLEFT_6, GI_MONSTER_DOWNLEFT_7, GI_MONSTER_DOWNLEFT_8, GI_MONSTER_DOWNLEFT_IDLE,
    GI_MONSTER_LEFT_1, GI_MONSTER_LEFT_2, GI_MONSTER_LEFT_3, GI_MONSTER_LEFT_4, GI_MONSTER_LEFT_5, GI_MONSTER_LEFT_6, GI_MONSTER_LEFT_7, GI_MONSTER_LEFT_8, GI_MONSTER_LEFT_IDLE,
    GI_MONSTER_UPLEFT_1, GI_MONSTER_UPLEFT_2, GI_MONSTER_UPLEFT_3, GI_MONSTER_UPLEFT_4, GI_MONSTER_UPLEFT_5, GI_MONSTER_UPLEFT_6, GI_MONSTER_UPLEFT_7, GI_MONSTER_UPLEFT_8, GI_MONSTER_UPLEFT_IDLE,
    GI_MONSTER_DEAD,

    GI_BG_NASRACKY,
    GI_BERRY_0, GI_BERRY_1, GI_BERRY_2, GI_BERRY_3, GI_BERRY_4,

    GI_GINGERBERRY,
    GI_PLAYERSTAR,
    GI_BOMB,

    GI_MONSTER_THROW_1, GI_MONSTER_THROW_2, GI_MONSTER_THROW_3, GI_MONSTER_THROW_4, GI_MONSTER_THROW_5, GI_MONSTER_THROW_6, GI_MONSTER_THROW_7, GI_MONSTER_THROW_8,
    GI_MONSTER_CATCH_1, GI_MONSTER_CATCH_2, GI_MONSTER_CATCH_3,

    GI_ARCI_1, GI_ARCI_2, GI_ARCI_3, GI_ARCI_4, GI_ARCI_5, GI_ARCI_6, GI_ARCI_7, GI_ARCI_8,

    GI_BG_LEPSI_1, GI_BG_LEPSI_2, GI_BG_LEPSI_3, GI_BG_LEPSI_4, GI_BG_LEPSI_5,

    GI_PRINCEZNA_1, GI_PRINCEZNA_2, GI_PRINCEZNA_3,
    GI_PRINCEZNA_L_1, GI_PRINCEZNA_L_2, GI_PRINCEZNA_L_3, GI_PRINCEZNA_L_4,
    GI_SR_1,

    MAX_GI
};

class ImageStorage
{
    friend class Singleton<ImageStorage>;
    public:
        SDL_Texture* GetImage(GameImages img);
        SDL_Texture* GetBigNumberImage(int num);

    protected:
        ImageStorage();

        bool LoadTexture(GameImages img);

    private:
        std::map<uint32_t, SDL_Texture*> m_textures;
        std::map<int, SDL_Texture*> m_numTextures;
};

#define sImageStorage Singleton<ImageStorage>::getInstance()

#endif