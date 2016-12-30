#ifndef YD_STAGES_H
#define YD_STAGES_H

class WorldObject;

enum StageType
{
    STAGE_MENU,
    STAGE_GAMEINTRO,
    STAGE_GAME
};

class StageTemplate
{
    public:
        StageTemplate(StageType type) : m_type(type) { };

        virtual void OnEnter() { };
        virtual void OnLeave() { };
        virtual void OnDraw() { };
        virtual void OnKeyPress(uint16_t key, bool press) { };
        virtual void OnMouseMove(int32_t x, int32_t y) { };
        virtual void OnMouseClick(int32_t x, int32_t y, bool left) { };

    protected:
        StageType m_type;

        void CreateText(uint32_t texture, uint32_t font, const char* text, int x, int y, SDL_Color color = {255,255,255,0});
        void PrepareTextureDimension(uint32_t texture, int x, int y);

        std::map<uint32_t, SDL_Texture*> m_textures;
        std::map<uint32_t, SDL_Rect> m_textureDims;
        std::vector<WorldObject*> m_objects;
};

class MenuStage : public StageTemplate
{
    public:
        MenuStage();

        void OnEnter();
        void OnLeave();
        void OnDraw();
        void OnKeyPress(uint16_t key, bool press);
        void OnMouseClick(int32_t x, int32_t y, bool left);

    protected:
        //
};

class GameIntroStage : public StageTemplate
{
    public:
        GameIntroStage();

        void OnEnter();
        void OnLeave();
        void OnDraw();
        void OnKeyPress(uint16_t key, bool press);
        void OnMouseClick(int32_t x, int32_t y, bool left);

    protected:
        clock_t fadeStart, fadeEnd, fadePause;
        bool fadeIn;

        uint32_t textNo;
};

struct FlyingBerry
{
    FlyingBerry() : srcX(0), srcY(0), dstX(0), dstY(0), timeStart(0), duration(0) { isFlying = true; };
    FlyingBerry(float _srcX, float _srcY, float _dstX, float _dstY, uint32_t _timeStart, uint32_t _duration) :
        srcX(_srcX), srcY(_srcY), dstX(_dstX), dstY(_dstY), timeStart(_timeStart), duration(_duration)
    {
        isFlying = true;
    };

    bool isFlying;
    float srcX, srcY, dstX, dstY;
    uint32_t timeStart;
    uint32_t duration;
};

class GameStage : public StageTemplate
{
    public:
        GameStage();

        void OnEnter();
        void OnLeave();
        void OnDraw();
        void OnKeyPress(uint16_t key, bool press);
        void OnMouseClick(int32_t x, int32_t y, bool left);

    protected:
        WorldObject* m_player;
        uint32_t m_lastSwingCheck;

        void UpdateStolenBerryCount(int delta);
        SDL_Texture* m_stolenBerryTexture;
        uint32_t m_stolenBerries;

        float m_gameSpeed;

        SDL_Rect m_bomb;
        bool m_bombPlanted;
        uint32_t m_bombRemoveTimer;

        std::list<FlyingBerry> m_flyingBerries;

        void DrawOutro();
        void EndGame(bool happyend);

        bool m_happyEnd;
        uint32_t m_outroTimer;
        uint32_t m_outroStage;

        WorldObject* m_catcherSlots[2];

        void KillInRadius(float radius, bool bomb = false);

        WorldObject* SpawnMonster(uint32_t path, float speed);
        void MonsterControl();
        uint8_t m_gamePhase;
        time_t m_gameStartTime;
        time_t m_lastRemTime;
        SDL_Texture* m_timeTexture;
        uint32_t m_lastSpawn;

        WorldObject* m_groupSpawnSlots[6];
        uint32_t m_startGroupSpawn;
        uint8_t m_groupSpawnPhase;

        uint32_t m_energy;
        uint32_t m_lastEnergyTick;

        WorldObject* m_arciZmrd;

        uint32_t m_lastBombPlant;
};

#endif
