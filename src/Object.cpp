#include "general.h"
#include "Object.h"
#include "ImageStorage.h"
#include "Application.h"
#include "PathStorage.h"

WorldObject::WorldObject(ObjectTypes type) : m_type(type)
{
    m_anim = ANIM_IDLE;
    m_direction = DIR_DOWN;
    m_animNextTime = 0;
    m_pathType = (uint32_t)-1;

    for (size_t i = 0; i < 4; i++)
        m_moveElems[i] = false;

    m_hasBerry = false;
    m_speed = 1.0f;
    m_deathTime = 0;
    m_berryCatchHide = false;
}

void WorldObject::Create()
{
    if (m_type == OBJTEX_PLAYER)
    {
        for (size_t dir = 0; dir < DIR_IDLE; dir++)
        {
            for (size_t animLen = 0; animLen < 1; animLen++)
                m_dirAnim[dir].push_back(sImageStorage->GetImage((GameImages)(GI_PLAYER_UP_1 + animLen + 2*dir)));
        }

        for (size_t dir = 0; dir < DIR_IDLE; dir++)
            m_dirAnim[DIR_IDLE].push_back(sImageStorage->GetImage((GameImages)(2*dir + 1)));
    }
    else if (m_type == OBJTEX_MONSTER1)
    {
        for (size_t dir = 0; dir < DIR_IDLE; dir++)
        {
            for (size_t animLen = 0; animLen < 8; animLen++)
                m_dirAnim[dir].push_back(sImageStorage->GetImage((GameImages)(GI_MONSTER_UP_1 + animLen + 9 * dir)));
        }

        for (size_t dir = 0; dir < DIR_IDLE; dir++)
            m_dirAnim[DIR_IDLE].push_back(sImageStorage->GetImage((GameImages)(GI_MONSTER_UP_1 + 8 * dir + 7)));

        m_miscAnims[ANIM_THROW].resize(8);
        for (size_t i = 0; i < 8; i++)
            m_miscAnims[ANIM_THROW][i] = sImageStorage->GetImage((GameImages)(GI_MONSTER_THROW_1 + i));
        m_miscAnims[ANIM_CATCH].resize(3);
        for (size_t i = 0; i < 3; i++)
            m_miscAnims[ANIM_CATCH][i] = sImageStorage->GetImage((GameImages)(GI_MONSTER_CATCH_1 + i));
    }
    else if (m_type == OBJTEX_ARCIZMRD)
    {
        m_anim = ANIM_APPEAR;

        m_miscAnims[ANIM_APPEAR].resize(8);
        for (size_t i = 0; i < 8; i++)
            m_miscAnims[ANIM_APPEAR][i] = sImageStorage->GetImage((GameImages)(GI_ARCI_1 + i));

        m_miscAnims[ANIM_DISAPPEAR].resize(8);
        for (size_t i = 0; i < 8; i++)
            m_miscAnims[ANIM_DISAPPEAR][i] = sImageStorage->GetImage((GameImages)(GI_ARCI_1 + 7 - i));
    }

    SetPosition(0, 0);
}

void WorldObject::Kill()
{
    m_deathTime = getMSTime();

    StopMovement();
    m_pathType = (uint32_t)-1;
    m_anim = ANIM_IDLE;
    m_direction = DIR_UP;
    m_dirAnim[DIR_IDLE][DIR_UP] = sImageStorage->GetImage(GI_MONSTER_DEAD);
}

void WorldObject::SetPosition(float x, float y)
{
    if (!sApplication->IsGameOver())
    {
        if (x < 0) x = 0;
        if (x > sApplication->GetWindowWidth()) x = (float)sApplication->GetWindowWidth();
        if (y < 0) y = 0;
        if (y > sApplication->GetWindowHeight()) y = (float)sApplication->GetWindowHeight();
    }

    m_positionX = x;
    m_positionY = y;
}

void WorldObject::SetSpecial(uint32_t specialType)
{
    if (m_type == OBJTEX_PLAYER && specialType == SPECIAL_ATTACK_SWING)
    {
        m_anim = ANIM_SWING;
        //m_direction = DIR_UP;
        m_lastMoveTime = getMSTime();
        m_animNextTime = clock() + 20;
        m_animFrame = 0;
    }
    else if (m_type == OBJTEX_ARCIZMRD && specialType == SPECIAL_GTFO)
    {
        m_anim = ANIM_DISAPPEAR;
        m_animNextTime = clock() + 20;
        m_animFrame = 0;
    }
    else if (m_type == OBJTEX_PLAYER && specialType == SPECIAL_STUJ_A_DRZ_HUBU)
    {
        m_anim = ANIM_IDLE;
        m_direction = DIR_DOWN;
    }
}

bool WorldObject::HasSpecial(uint32_t specialType)
{
    if (specialType == SPECIAL_ATTACK_SWING)
        return (m_anim == ANIM_SWING);

    return false;
}

bool WorldObject::IsMoving()
{
    for (size_t i = 0; i < 4; i++)
    {
        if (m_moveElems[i])
            return true;
    }
    return false;
}

void WorldObject::UpdateAndDraw()
{
    if (m_anim != ANIM_IDLE)
    {
        if (m_animNextTime <= clock())
        {
            if (m_anim == ANIM_WALK)
            {
                if (m_type == OBJTEX_MONSTER1)
                {
                    if (m_animFrame == 7)
                        m_animFrame = 0;
                    else
                        m_animFrame++;
                }
                else if (m_type == OBJTEX_PLAYER)
                {
                    m_animFrame = 0;
                }

                m_animNextTime = clock() + (clock_t)(100.0f / m_speed);
            }
            else if (m_anim == ANIM_SWING && m_type == OBJTEX_PLAYER)
            {
                if (m_animFrame == 15)
                {
                    m_anim = ANIM_WALK;
                    m_animFrame = 0;
                    m_lastMoveTime = getMSTime();
                    DecideMoveDirection();
                }
                else
                {
                    m_animFrame++;
                }

                m_animNextTime = clock() + 20;
            }
            else if (m_anim == ANIM_APPEAR || m_anim == ANIM_DISAPPEAR)
            {
                m_animFrame++;
                if (m_animFrame >= m_miscAnims[m_anim].size())
                {
                    m_animFrame = (uint32_t)(m_miscAnims[m_anim].size() - 1);
                    m_animNextTime = clock() + 1000000; // never
                }
                else
                    m_animNextTime = clock() + 100;
            }
            else
            {
                m_animFrame++;
                if (m_miscAnims.find(m_anim) == m_miscAnims.end() || m_animFrame >= m_miscAnims[m_anim].size())
                {
                    m_animFrame = 0;
                    m_anim = ANIM_WALK;
                    DecideMoveDirection();
                }

                if (m_anim == ANIM_CATCH)
                    m_animNextTime = clock() + 100;
                else
                    m_animNextTime = clock() + 40;
            }
        }
    }

    if (m_type == OBJTEX_PLAYER)
    {
        if (m_anim != ANIM_IDLE && IsMoving())
        {
            float mdiff = ((float)getMSTimeDiff(m_lastMoveTime, getMSTime())) - 5.0f;
            if (mdiff > 0)
            {
                float nx = m_positionX + (mdiff * 0.5f / m_speed) * cos(GetMoveAngle());
                float ny = m_positionY + (mdiff * 0.5f / m_speed) * sin(GetMoveAngle());
                SetPosition(nx, ny);

                m_lastMoveTime = getMSTime();
            }
        }
    }

    if (m_type == OBJTEX_MONSTER1)
        PerformMovement();

    SDL_Rect trg;

    if (m_anim == ANIM_SWING)
    {
        trg.w = 16;
        trg.h = 16;

        for (size_t i = 4; i < 8; i++)
        {
            trg.x = (int)( (m_positionX - 32 + 24) + ((float)m_animFrame * 2.5f) * cos((float)i*M_PI / 4.0f) );
            trg.y = (int)( (m_positionY - 6) + ((float)m_animFrame * 2.5f) * sin((float)i*M_PI / 4.0f) * 0.5f );
            SDL_RenderCopy(sApplication->GetRenderer(), sImageStorage->GetImage(GI_PLAYERSTAR), nullptr, &trg);
        }
    }

    if (m_type == OBJTEX_ARCIZMRD)
    {
        trg.h = 256;
        trg.w = 256;
    }
    else
    {
        trg.h = 64;
        trg.w = 64;
    }
    trg.x = (int)m_positionX - trg.w / 2;
    trg.y = (int)m_positionY - trg.h / 2;

    // fadeout on death
    if (m_deathTime)
    {
        float ratio = (float)getMSTimeDiff(m_deathTime, getMSTime());
        if (ratio < 500)
            ratio = 500.0f;
        else if (ratio > 1500)
            ratio = 1500.0f;
        ratio = 1.0f - ((ratio - 500.0f) / 1000.0f);
        SDL_SetTextureAlphaMod(m_dirAnim[DIR_IDLE][DIR_UP], (uint8_t)(ratio * 255.0f));
    }

    if (m_anim == ANIM_WALK)
        SDL_RenderCopy(sApplication->GetRenderer(), m_dirAnim[m_direction][m_animFrame], nullptr, &trg);
    if (m_anim == ANIM_SWING)
        SDL_RenderCopy(sApplication->GetRenderer(), m_dirAnim[m_animFrame % 8][0], nullptr, &trg);
    else if (m_anim == ANIM_IDLE)
        SDL_RenderCopy(sApplication->GetRenderer(), m_dirAnim[DIR_IDLE][m_direction], nullptr, &trg);
    else if (m_miscAnims.find(m_anim) != m_miscAnims.end())
        SDL_RenderCopy(sApplication->GetRenderer(), m_miscAnims[m_anim][m_animFrame], nullptr, &trg);

    // restore fadeout texture alpha mod
    if (m_deathTime)
        SDL_SetTextureAlphaMod(m_dirAnim[DIR_IDLE][DIR_UP], 255);

    if (m_hasBerry)
    {
        trg.w = 16;
        trg.h = 16;
        trg.x += 20;
        trg.y -= 24;

        if (m_anim == ANIM_THROW)
        {
            if (m_animFrame < 4)
                trg.y += 4 * m_animFrame;
        }
        else if (m_anim == ANIM_CATCH)
        {
            m_berryCatchHide = false;
            trg.y += (2 * 30 - m_animFrame * 30);
        }

        if (!m_berryCatchHide)
            SDL_RenderCopy(sApplication->GetRenderer(), sImageStorage->GetImage(GI_GINGERBERRY), nullptr, &trg);
    }

    if (m_anim == ANIM_SWING)
    {
        trg.w = 16;
        trg.h = 16;

        for (size_t i = 0; i < 4; i++)
        {
            trg.x = (int)( (m_positionX - 32 + 24) + ((float)m_animFrame * 2.5f) * cos((float)i*M_PI / 4.0f) );
            trg.y = (int)( (m_positionY - 6) + ((float)m_animFrame * 2.5f) * sin((float)i*M_PI / 4.0f) * 0.5f );
            SDL_RenderCopy(sApplication->GetRenderer(), sImageStorage->GetImage(GI_PLAYERSTAR), nullptr, &trg);
        }
    }
}

ObjectTypes WorldObject::GetType()
{
    return m_type;
}

void WorldObject::DecideMoveDirection()
{
    uint32_t prevanim = m_anim;

    // UP+DOWN both true or both false (moving just horizontally)
    if (!(m_moveElems[0] ^ m_moveElems[2]))
    {
        if (m_moveElems[1])
        {
            m_anim = m_moveElems[3] ? ANIM_IDLE : ANIM_WALK;
            if (!m_moveElems[3])
                m_direction = DIR_RIGHT;
        }
        else if (m_moveElems[3])
        {
            m_anim = ANIM_WALK;
            m_direction = DIR_LEFT;
        }
        else
            m_anim = ANIM_IDLE;
    }
    else // UP or DOWN
    {
        m_anim = ANIM_WALK;

        // just UP
        if (m_moveElems[0])
        {
            if (m_moveElems[1])
                m_direction = m_moveElems[3] ? DIR_UP : DIR_UPRIGHT;
            else if (m_moveElems[3])
                m_direction = DIR_UPLEFT;
            else
                m_direction = DIR_UP;
        }
        else // just DOWN
        {
            if (m_moveElems[1])
                m_direction = m_moveElems[3] ? DIR_DOWN : DIR_DOWNRIGHT;
            else if (m_moveElems[3])
                m_direction = DIR_DOWNLEFT;
            else
                m_direction = DIR_DOWN;
        }
    }

    if (prevanim != m_anim)
    {
        if (prevanim == ANIM_SWING || prevanim == ANIM_THROW || prevanim == ANIM_CATCH)
            m_anim = prevanim;
        else
        {
            m_lastMoveTime = getMSTime();
            m_animNextTime = clock() + 100;
            m_animFrame = 0;
        }
    }
}

#define M_FPI ((float)M_PI)

float WorldObject::GetMoveAngle()
{
    switch (m_direction)
    {
        case DIR_UP:
            return (-M_FPI / 2.0f);
        case DIR_UPRIGHT:
            return (-M_FPI / 4.0f);
        case DIR_RIGHT:
            return 0;
        case DIR_DOWNRIGHT:
            return (M_FPI / 4.0f);
        case DIR_DOWN:
            return (M_FPI / 2.0f);
        case DIR_DOWNLEFT:
            return (3.0f * M_FPI / 4.0f);
        case DIR_LEFT:
            return (M_FPI);
        case DIR_UPLEFT:
            return (5.0f * M_FPI / 4.0f);
    }

    return 0;
}

void WorldObject::AddMoveDirection(Directions dir)
{
    if (dir == DIR_UP)
    {
        if (m_moveElems[0])
            return;

        m_moveElems[0] = true;
    }
    else if (dir == DIR_RIGHT)
    {
        if (m_moveElems[1])
            return;

        m_moveElems[1] = true;
    }
    else if (dir == DIR_DOWN)
    {
        if (m_moveElems[2])
            return;

        m_moveElems[2] = true;
    }
    else if (dir == DIR_LEFT)
    {
        if (m_moveElems[3])
            return;

        m_moveElems[3] = true;
    }

    DecideMoveDirection();
}

void WorldObject::RemoveMoveDirection(Directions dir)
{
    if (dir == DIR_UP)
        m_moveElems[0] = false;
    else if (dir == DIR_RIGHT)
        m_moveElems[1] = false;
    else if (dir == DIR_DOWN)
        m_moveElems[2] = false;
    else if (dir == DIR_LEFT)
        m_moveElems[3] = false;

    DecideMoveDirection();
}

void WorldObject::StopMovement()
{
    for (size_t i = 0; i < 4; i++)
        m_moveElems[i] = false;
    DecideMoveDirection();
}

void WorldObject::ProcessFeatures()
{
    if (m_storedFeatures & FEAT_GET_BERRY)
        m_hasBerry = true;
    else if (m_storedFeatures & FEAT_THROW)
    {
        StopMovement();
        m_anim = ANIM_THROW;
        m_animFrame = 0;
        m_animNextTime = clock() + 50;
    }
    else if (m_storedFeatures & FEAT_CATCH)
    {
        StopMovement();
        m_direction = DIR_DOWN;
        m_anim = ANIM_CATCH;
        m_animFrame = 0;
        m_animNextTime = clock() + 50;
    }
    else if (m_storedFeatures & FEAT_CATCH_PREP)
    {
        StopMovement();
        m_direction = DIR_DOWN;
        m_animFrame = 0;
    }

    m_storedFeatures = 0;
}

void WorldObject::PerformMovement()
{
    if (m_pathType == (uint32_t)-1)
        return;
    if (getMSTimeDiff(m_pathLastStart, getMSTime()) > INT_MAX)
        return;

    if (sApplication->IsGameOver())
    {
        if (IsMoving())
            StopMovement();
        return;
    }

    if (m_delayMoveStart)
    {
        StopMovement();
        if (m_storedMoveElems[0])
            AddMoveDirection(DIR_UP);
        if (m_storedMoveElems[1])
            AddMoveDirection(DIR_RIGHT);
        if (m_storedMoveElems[2])
            AddMoveDirection(DIR_DOWN);
        if (m_storedMoveElems[3])
            AddMoveDirection(DIR_LEFT);
        m_delayMoveStart = false;
        ProcessFeatures();
    }

    if (getMSTimeDiff(m_pathLastStart, getMSTime()) > m_pathMoveDuration)
    {
        SetPosition(m_dstX, m_dstY);
        for (size_t i = 0; i < 4; i++)
            m_storedMoveElems[i] = false;
        m_pathPoint++;
        if (sPathStorage->GetNextNode(m_pathType, m_pathPoint, m_dstX, m_dstY, m_pathMoveDuration, m_pathLastStart, m_positionX, m_positionY, m_storedMoveElems, m_storedFeatures, m_speed))
        {
            m_srcX = m_positionX;
            m_srcY = m_positionY;

            m_delayMoveStart = (m_pathLastStart > getMSTime());

            StopMovement();

            if (!m_delayMoveStart)
            {
                if (m_storedMoveElems[0])
                    AddMoveDirection(DIR_UP);
                if (m_storedMoveElems[1])
                    AddMoveDirection(DIR_RIGHT);
                if (m_storedMoveElems[2])
                    AddMoveDirection(DIR_DOWN);
                if (m_storedMoveElems[3])
                    AddMoveDirection(DIR_LEFT);

                ProcessFeatures();
            }
        }
        else
        {
            StopMovement();
            m_pathType = (uint32_t)-1;
            m_pathFinished = true;
        }
    }
    else
    {
        float timeCoef = (float)(getMSTime() - m_pathLastStart) / (float)m_pathMoveDuration;
        float nx = m_srcX + (m_dstX - m_srcX) * timeCoef;
        float ny = m_srcY + (m_dstY - m_srcY) * timeCoef;
        SetPosition(nx, ny);
    }
}

void WorldObject::SetPath(uint32_t pathType)
{
    m_pathFinished = false;
    m_pathType = pathType;
    m_pathPoint = 0;
    m_pathLastStart = getMSTime();
    m_pathMoveDuration = 0;
    m_delayMoveStart = false;

    for (size_t i = 0; i < 4; i++)
        m_storedMoveElems[i] = false;

    if (sPathStorage->GetNextNode(m_pathType, m_pathPoint, m_dstX, m_dstY, m_pathMoveDuration, m_pathLastStart, m_positionX, m_positionY, m_storedMoveElems, m_storedFeatures, m_speed))
    {
        StopMovement();
        SetPosition(m_dstX, m_dstY);
        m_pathLastStart = getMSTime() - m_pathMoveDuration - 100;
        ProcessFeatures();
    }
    else
    {
        StopMovement();
        m_pathType = (uint32_t)-1;
        m_pathFinished = true;
    }
}
