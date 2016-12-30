#ifndef YD_OBJECT_H
#define YD_OBJECT_H

enum ObjectTypes
{
    OBJTEX_PLAYER = 0,
    OBJTEX_MONSTER1 = 1,
    OBJTEX_ARCIZMRD = 2,
};

enum Directions
{
    DIR_UP = 0,
    DIR_UPRIGHT = 1,
    DIR_RIGHT = 2,
    DIR_DOWNRIGHT = 3,
    DIR_DOWN = 4,
    DIR_DOWNLEFT = 5,
    DIR_LEFT = 6,
    DIR_UPLEFT = 7,
    DIR_IDLE = 8,
    MAX_DIR
};

enum AnimType
{
    ANIM_IDLE,
    ANIM_WALK,
    ANIM_SWING,
    ANIM_THROW,
    ANIM_CATCH,
    ANIM_APPEAR,
    ANIM_DISAPPEAR,
};

enum SpecialType
{
    SPECIAL_NONE = 0,
    SPECIAL_ATTACK_SWING = 1,
    SPECIAL_GTFO = 2,
    SPECIAL_STUJ_A_DRZ_HUBU = 3,
};

class WorldObject
{
    public:
        WorldObject(ObjectTypes type);

        void Create();
        void SetPosition(float x, float y);
        void UpdateAndDraw();

        void AddMoveDirection(Directions dir);
        void RemoveMoveDirection(Directions dir);
        void StopMovement();
        void Kill();
        uint32_t GetDeathTime() { return m_deathTime; };

        uint32_t GetCurrentPathPoint() { return m_pathPoint; };
        void ContinueInPath(uint32_t delay = 0) { m_pathLastStart = getMSTime() + delay; };

        float GetPositionX() { return m_positionX; };
        float GetPositionY() { return m_positionY; };

        bool IsPathFinished() { return m_pathFinished; };
        void SetPath(uint32_t pathType);
        uint32_t GetPath() { return m_pathType; };

        void SetSpeed(float speed) { m_speed = speed; };

        void SetSpecial(uint32_t specialType);
        bool HasSpecial(uint32_t specialType);
        void GiveBerry() { m_hasBerry = true; };
        bool HasBerry() { return m_hasBerry; };
        void CancelBerry() { m_hasBerry = false; };
        void SetBerryCatchHide() { m_berryCatchHide = true; };
        bool IsMoving();

        uint32_t GetAnim() { return m_anim; };
        uint32_t GetAnimFrame() { return m_animFrame; };

        void StorePositionX() { m_storedX = m_positionX; };
        float GetStoredPositionX() { return m_storedX; };

        ObjectTypes GetType();

    protected:
        ObjectTypes m_type;
        float m_positionX, m_positionY;

        void DecideMoveDirection();
        float GetMoveAngle();

        void PerformMovement();
        void ProcessFeatures();

        bool m_moveElems[4]; // U, R, D, L

        float m_storedX;

        uint32_t m_anim;
        uint32_t m_animFrame;
        clock_t m_animNextTime;
        uint32_t m_lastMoveTime;
        uint32_t m_direction;

        bool m_berryCatchHide;

        uint32_t m_deathTime;

        std::vector<SDL_Texture*> m_dirAnim[MAX_DIR];
        std::map<uint32_t, std::vector<SDL_Texture*>> m_miscAnims;

        // path related stuff

        uint32_t m_pathType;
        uint32_t m_pathPoint;
        uint32_t m_pathLastStart;
        uint32_t m_pathMoveDuration; // lastStart + moveDuration = endTime

        float m_speed;

        float m_srcX, m_srcY, m_dstX, m_dstY;
        bool m_storedMoveElems[4];
        uint32_t m_storedFeatures;
        bool m_delayMoveStart;
        bool m_pathFinished;

        bool m_hasBerry;
};

#endif
