#include "general.h"
#include "Stages.h"
#include "Application.h"
#include "Object.h"
#include "ImageStorage.h"

#define BERRY_COUNT 200
#define BERRY_LIMIT_4 160
#define BERRY_LIMIT_3 130
#define BERRY_LIMIT_2 80
#define BERRY_LIMIT_1 40

#define ENERGY_REGEN_TICK 25

GameStage::GameStage() : StageTemplate(STAGE_GAME)
{
    m_lastSwingCheck = 0;
    m_gamePhase = 0;
    m_lastSpawn = 0;
    srand((unsigned int)time(nullptr));
    m_gameStartTime = time(nullptr);

    m_lastBombPlant = getMSTime() - 4000;

    m_bombRemoveTimer = 0;
    m_bombPlanted = false;

    m_startGroupSpawn = 0;
    m_groupSpawnSlots[0] = nullptr;

    m_gameSpeed = 1.0f;
    m_lastRemTime = 0;
    m_stolenBerries = 0;
    m_energy = 100;
    m_lastEnergyTick = getMSTime();
}

void GameStage::OnEnter()
{
    sApplication->SetGameOver(false);
    m_stolenBerries = 0;
    UpdateStolenBerryCount(0);

    m_textures[0] = sImageStorage->GetImage(GI_BG_NASRACKY);
    m_textureDims[0] = { 0, 0, DEF_WIN_WIDTH, DEF_WIN_HEIGHT };

    m_textures[1] = sImageStorage->GetImage(GI_BERRY_0);
    m_textureDims[1] = { 0, 0, DEF_WIN_WIDTH, DEF_WIN_HEIGHT };

    WorldObject* obj = new WorldObject(OBJTEX_PLAYER);
    obj->Create();
    obj->SetPosition(DEF_WIN_WIDTH / 2, DEF_WIN_HEIGHT / 2);
    m_player = obj;
    m_objects.push_back(obj);

    m_catcherSlots[0] = nullptr;
    m_catcherSlots[1] = nullptr;

    Mix_Music *music = Mix_LoadMUS("data/music.mp3");
    Mix_PlayMusic(music, 1);

    m_arciZmrd = nullptr;
}

void GameStage::OnLeave()
{
    //
}

WorldObject* GameStage::SpawnMonster(uint32_t path, float speed)
{
    WorldObject* obj = new WorldObject(OBJTEX_MONSTER1);
    obj->Create();
    obj->SetSpeed(speed);
    obj->SetPath(path);
    m_objects.push_back(obj);

    return obj;
}

void GameStage::MonsterControl()
{
    if (sApplication->IsGameOver())
        return;

    if (m_gamePhase == 0) // 0:00 - 1:30
    {
        if (m_startGroupSpawn)
        {
            if (getMSTimeDiff(m_startGroupSpawn, getMSTime()) / 800 > m_groupSpawnPhase)
            {
                m_groupSpawnSlots[m_groupSpawnPhase] = SpawnMonster(6 + m_groupSpawnPhase, m_gameSpeed);
                m_groupSpawnSlots[3 + m_groupSpawnPhase] = SpawnMonster(9 + m_groupSpawnPhase, m_gameSpeed);
                m_groupSpawnPhase++;

                if (m_groupSpawnPhase == 3)
                    m_startGroupSpawn = 0;
            }
        }

        if (m_groupSpawnSlots[0] != nullptr)
        {
            bool allgood = true;
            for (size_t i = 0; i < 6; i++)
            {
                if (m_groupSpawnSlots[i] == nullptr || (m_groupSpawnSlots[i]->GetCurrentPathPoint() != 3 && !m_groupSpawnSlots[i]->GetDeathTime()))
                {
                    allgood = false;
                    break;
                }
            }

            if (allgood)
            {
                for (size_t i = 0; i < 6; i++)
                {
                    if (m_groupSpawnSlots[i] == nullptr || m_groupSpawnSlots[i]->GetDeathTime())
                        continue;
                    m_groupSpawnSlots[i]->ContinueInPath();
                }
                m_groupSpawnSlots[0] = nullptr;
            }
        }

        if (getMSTimeDiff(m_lastSpawn, getMSTime()) > 1500.0f / m_gameSpeed)
        {
            int thr1 = rand() % 4;
            if (thr1 == 0)
            {
                SpawnMonster(1, m_gameSpeed);
                SpawnMonster(2, m_gameSpeed);
            }
            else if (thr1 == 1 && m_groupSpawnSlots[0] == nullptr)
            {
                for (size_t i = 0; i < 6; i++)
                    m_groupSpawnSlots[i] = nullptr;
                m_startGroupSpawn = getMSTime();
                m_groupSpawnPhase = 0;
            }
            else
            {
                int thr = rand() % 4;
                switch (thr)
                {
                    case 0: SpawnMonster(0, m_gameSpeed); break;
                    case 1: SpawnMonster(3, m_gameSpeed); break;
                    case 2: SpawnMonster(4, m_gameSpeed + 0.2f); break;
                    case 3: SpawnMonster(5, m_gameSpeed + 0.2f); break;
                }
            }

            m_lastSpawn = getMSTime();
        }

        if (time(nullptr) - m_gameStartTime > 10 && m_gameSpeed < 1.01f)
            m_gameSpeed = 1.2f;
        if (time(nullptr) - m_gameStartTime > 30 && m_gameSpeed < 1.21f)
            m_gameSpeed = 1.4f;
        if (time(nullptr) - m_gameStartTime > 50 && m_gameSpeed < 1.41f)
            m_gameSpeed = 1.6f;

        if (time(nullptr) - m_gameStartTime >= 90)
            m_gamePhase = 1;
    }
    else if (m_gamePhase == 1) // 1:30 - 3:00
    {
        if (getMSTimeDiff(m_lastSpawn, getMSTime()) > 1200.0f / m_gameSpeed)
        {
            if (rand() % 3 > 0)
            {
                int thr1 = rand() % 4;
                SpawnMonster(12 + thr1, m_gameSpeed + 0.5f);
            }
            else
            {
                int thr1 = rand() % 2;
                SpawnMonster(4 + thr1, m_gameSpeed + 0.2f);
            }
            m_lastSpawn = getMSTime();

            if (m_catcherSlots[0] == nullptr || m_catcherSlots[0]->GetDeathTime() || m_catcherSlots[0]->IsPathFinished())
                m_catcherSlots[0] = SpawnMonster(16, m_gameSpeed + 1.0f);
            if (m_catcherSlots[1] == nullptr || m_catcherSlots[1]->GetDeathTime() || m_catcherSlots[1]->IsPathFinished())
                m_catcherSlots[1] = SpawnMonster(17, m_gameSpeed + 1.0f);

            if (rand() % 2 == 0)
            {
                // "callers"
                if (time(nullptr) - m_gameStartTime >= 150)
                {
                    int thr1 = rand() % 2;
                    SpawnMonster(18 + thr1, m_gameSpeed + 1.0f);
                }
            }
        }

        if (time(nullptr) - m_gameStartTime >= 177)
            m_gamePhase = 2;
    }
    else if (m_gamePhase == 2) // 3:00 - 3:30
    {
        if (!m_arciZmrd)
        {
            m_arciZmrd = new WorldObject(OBJTEX_ARCIZMRD);
            m_arciZmrd->Create();
            m_arciZmrd->SetPosition(600, 285);
            m_objects.push_back(m_arciZmrd);
        }

        if (getMSTimeDiff(m_lastSpawn, getMSTime()) > 1200.0f / m_gameSpeed)
        {
            int thr1 = rand() % 2;
            SpawnMonster(18 + thr1, m_gameSpeed + 1.0f);
            m_lastSpawn = getMSTime();

            //
        }
    }

    // > 210s (3:30), end game - happyend
    if (time(nullptr) - m_gameStartTime >= 210)
        EndGame(true);
}

void GameStage::EndGame(bool happyend)
{
    // for app control, every "end" is "game over"; game stage will then decide whether we succeeded, or we fucked up
    sApplication->SetGameOver(true);

    m_player->RemoveMoveDirection(DIR_UP);
    m_player->RemoveMoveDirection(DIR_RIGHT);
    m_player->RemoveMoveDirection(DIR_DOWN);
    m_player->RemoveMoveDirection(DIR_LEFT);
    m_player->SetSpecial(SPECIAL_STUJ_A_DRZ_HUBU);

    sApplication->SetLastScore(BERRY_COUNT - m_stolenBerries);

    if (!happyend)
        Mix_HaltMusic();

    m_happyEnd = happyend;
    m_outroStage = 0;
    m_outroTimer = getMSTime();
}

void GameStage::DrawOutro()
{
    SDL_Rect txr;

    for (std::pair<uint32_t, SDL_Texture*> txt : m_textures)
    {
        if (txt.first != 1)
            SDL_RenderCopy(sApplication->GetRenderer(), txt.second, nullptr, &m_textureDims[txt.first]);
    }

    //
    for (size_t i = 0; i < m_objects.size(); i++)
    {
        m_objects[i]->UpdateAndDraw();
    }
    //

    if (m_happyEnd)
    {
        switch (m_outroStage)
        {
            case 0:
            {
                if (getMSTimeDiff(m_outroTimer, getMSTime()) >= 1000)
                {
                    m_outroStage++;
                    m_outroTimer = getMSTime();

                    for (size_t i = 0; i < m_objects.size(); i++)
                    {
                        if (m_objects[i]->GetType() == OBJTEX_MONSTER1)
                        {
                            m_objects[i]->SetSpeed(4.0f);
                            m_objects[i]->StorePositionX();
                            if (m_objects[i]->GetPositionX() < 610)
                                m_objects[i]->AddMoveDirection(DIR_LEFT);
                            else
                                m_objects[i]->AddMoveDirection(DIR_RIGHT);
                        }
                    }

                    m_arciZmrd->SetSpecial(SPECIAL_GTFO);
                }
                break;
            }
            case 1:
            {
                float diff = (float)getMSTimeDiff(m_outroTimer, getMSTime());
                for (size_t i = 0; i < m_objects.size(); i++)
                {
                    if (m_objects[i]->GetType() == OBJTEX_MONSTER1)
                    {
                        if (m_objects[i]->GetPositionX() < 610)
                            m_objects[i]->SetPosition(m_objects[i]->GetStoredPositionX() - diff, m_objects[i]->GetPositionY());
                        else
                            m_objects[i]->SetPosition(m_objects[i]->GetStoredPositionX() + diff, m_objects[i]->GetPositionY());
                    }
                }
                if (diff > 2500)
                {
                    m_outroStage++;
                    m_outroTimer = getMSTime();

                    m_textures[11] = sImageStorage->GetImage(GI_BG_LEPSI_1);
                    m_player->SetPosition(663, 342);
                }
                break;
            }
            case 2:
            {
                float progress = ((float)getMSTimeDiff(m_outroTimer, getMSTime())) / 2000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                    m_textures[12] = sImageStorage->GetImage(GI_BG_LEPSI_2);
                }

                SDL_SetTextureAlphaMod(m_textures[11], (uint8_t)(255.0f * progress));
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[11], nullptr, &m_textureDims[0]);
                break;
            }
            case 3:
            {
                float progress = ((float)getMSTimeDiff(m_outroTimer, getMSTime())) / 2000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                    m_textures[13] = sImageStorage->GetImage(GI_BG_LEPSI_3);
                }

                SDL_SetTextureAlphaMod(m_textures[12], (uint8_t)(255.0f * progress));
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[11], nullptr, &m_textureDims[0]);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[12], nullptr, &m_textureDims[0]);
                break;
            }
            case 4:
            {
                float progress = ((float)getMSTimeDiff(m_outroTimer, getMSTime())) / 2000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                    m_textures[14] = sImageStorage->GetImage(GI_BG_LEPSI_4);
                }

                SDL_SetTextureAlphaMod(m_textures[13], (uint8_t)(255.0f * progress));
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[12], nullptr, &m_textureDims[0]);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[13], nullptr, &m_textureDims[0]);
                break;
            }
            case 5:
            {
                float progress = ((float)getMSTimeDiff(m_outroTimer, getMSTime())) / 2000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                    m_textures[15] = sImageStorage->GetImage(GI_BG_LEPSI_5);
                }

                SDL_SetTextureAlphaMod(m_textures[14], (uint8_t)(255.0f * progress));
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[13], nullptr, &m_textureDims[0]);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[14], nullptr, &m_textureDims[0]);
                break;
            }
            case 6:
            {
                float progress = ((float)getMSTimeDiff(m_outroTimer, getMSTime())) / 2000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();

                    m_textures[20] = sImageStorage->GetImage(GI_PRINCEZNA_1);
                    m_textures[21] = sImageStorage->GetImage(GI_PRINCEZNA_2);
                    m_textures[22] = sImageStorage->GetImage(GI_PRINCEZNA_3);
                    m_textureDims[20].w = 96;
                    m_textureDims[20].h = 96;
                    m_textureDims[20].x = -96;
                    m_textureDims[20].y = 309;
                }

                SDL_SetTextureAlphaMod(m_textures[15], (uint8_t)(255.0f * progress));
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[14], nullptr, &m_textureDims[0]);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[15], nullptr, &m_textureDims[0]);
                m_textures[1] = sImageStorage->GetImage(GI_BERRY_0);
                break;
            }
            case 7:
            {
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[15], nullptr, &m_textureDims[0]);

                uint32_t rem = getMSTimeDiff(m_outroTimer, getMSTime());
                float progress = ((float)rem) / 7000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                    m_textures[23] = sImageStorage->GetImage(GI_PRINCEZNA_L_1);
                    m_textures[24] = sImageStorage->GetImage(GI_PRINCEZNA_L_2);
                    m_textures[25] = sImageStorage->GetImage(GI_PRINCEZNA_L_3);
                    m_textures[26] = sImageStorage->GetImage(GI_PRINCEZNA_L_4);
                }
                m_textureDims[20].x = (int)((-96) + (570 + 96)*progress);

                uint32_t tex = (rem / 200) % 3;

                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[20 + tex], nullptr, &m_textureDims[20]);

                break;
            }
            case 8:
            {
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[15], nullptr, &m_textureDims[0]);

                uint32_t rem = getMSTimeDiff(m_outroTimer, getMSTime());
                uint32_t tex = (rem / 200);
                if (tex >= 4)
                {
                    if (tex >= 8)
                    {
                        m_outroStage++;
                        m_outroTimer = getMSTime();

                        m_textures[30] = sImageStorage->GetImage(GI_SR_1);
                        m_textureDims[30].w = 12;
                        m_textureDims[30].h = 12;
                        m_textureDims[30].x = (int) m_player->GetPositionX() - 6;
                        m_textureDims[30].y = (int)m_player->GetPositionY() - 42;

                        tex = 7;
                    }
                    tex = 7 - tex;
                }
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[23 + tex], nullptr, &m_textureDims[20]);
                break;
            }
            case 9:
            {
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[15], nullptr, &m_textureDims[0]);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[20], nullptr, &m_textureDims[20]);

                uint32_t rem = getMSTimeDiff(m_outroTimer, getMSTime()) / 80;
                if (rem > 20)
                {
                    rem = 20;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                }

                m_textureDims[30].w = 12 + rem;
                m_textureDims[30].h = 12 + rem;
                m_textureDims[30].x = (int)(m_player->GetPositionX() - m_textureDims[30].w/2);
                m_textureDims[30].y = (int)(m_player->GetPositionY() - rem - 42);

                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[30], nullptr, &m_textureDims[30]);
                break;
            }
            case 10:
            {
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[15], nullptr, &m_textureDims[0]);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[20], nullptr, &m_textureDims[20]);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[30], nullptr, &m_textureDims[30]);

                if (getMSTimeDiff(m_outroTimer, getMSTime()) >= 1500)
                {
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                }
                break;
            }
            case 11:
            {
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[15], nullptr, &m_textureDims[0]);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[20], nullptr, &m_textureDims[20]);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[30], nullptr, &m_textureDims[30]);

                m_player->UpdateAndDraw();
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[1], nullptr, &m_textureDims[1]);

                float progress = ((float)getMSTimeDiff(m_outroTimer, getMSTime())) / 2000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                    CreateText(5, FONT_BIGGER, "You saved our kingdom!", 40, 350, { 0,255,0,0 });
                }
                boxRGBA(sApplication->GetRenderer(), 0, 0, sApplication->GetWindowWidth(), sApplication->GetWindowHeight(), 0, 0, 0, (uint8_t)(255.0f * progress));
                break;
            }
            case 12:
            {
                float progress = ((float)getMSTimeDiff(m_outroTimer, getMSTime())) / 2000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                }
                boxRGBA(sApplication->GetRenderer(), 0, 0, sApplication->GetWindowWidth(), sApplication->GetWindowHeight(), 0, 0, 0, 255);

                SDL_SetTextureAlphaMod(m_textures[5], (uint8_t)(255.0f * progress));
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[5], nullptr, &m_textureDims[5]);
                break;
            }
            case 13:
            {
                boxRGBA(sApplication->GetRenderer(), 0, 0, sApplication->GetWindowWidth(), sApplication->GetWindowHeight(), 0, 0, 0, 255);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[5], nullptr, &m_textureDims[5]);
                if (getMSTimeDiff(m_outroTimer, getMSTime()) >= 2000)
                {
                    boxRGBA(sApplication->GetRenderer(), 0, 0, sApplication->GetWindowWidth(), sApplication->GetWindowHeight(), 0, 0, 0, 255);
                    sApplication->SetStage(STAGE_MENU);
                    return;
                }
                break;
            }
        }

        if (m_outroStage < 11)
        {
            m_player->UpdateAndDraw();

            SDL_RenderCopy(sApplication->GetRenderer(), m_textures[1], nullptr, &m_textureDims[1]);
        }
    }
    else
    {
        SDL_RenderCopy(sApplication->GetRenderer(), m_textures[1], nullptr, &m_textureDims[1]);

        switch (m_outroStage)
        {
            case 0:
            {
                if (getMSTimeDiff(m_outroTimer, getMSTime()) >= 1000)
                {
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                }
                break;
            }
            case 1:
            {
                float progress = ((float)getMSTimeDiff(m_outroTimer, getMSTime())) / 2000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                    CreateText(5, FONT_BIGGER, "Game over", 40, 350, {255,0,0,0});
                }
                boxRGBA(sApplication->GetRenderer(), 0, 0, sApplication->GetWindowWidth(), sApplication->GetWindowHeight(), 0, 0, 0, (uint8_t)(255.0f * progress));
                break;
            }
            case 2:
            {
                float progress = ((float)getMSTimeDiff(m_outroTimer, getMSTime())) / 2000.0f;
                if (progress >= 1.0f)
                {
                    progress = 1.0f;
                    m_outroStage++;
                    m_outroTimer = getMSTime();
                }
                boxRGBA(sApplication->GetRenderer(), 0, 0, sApplication->GetWindowWidth(), sApplication->GetWindowHeight(), 0, 0, 0, 255);

                SDL_SetTextureAlphaMod(m_textures[5], (uint8_t)(255.0f * progress));
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[5], nullptr, &m_textureDims[5]);
                break;
            }
            case 3:
            {
                boxRGBA(sApplication->GetRenderer(), 0, 0, sApplication->GetWindowWidth(), sApplication->GetWindowHeight(), 0, 0, 0, 255);
                SDL_RenderCopy(sApplication->GetRenderer(), m_textures[5], nullptr, &m_textureDims[5]);
                if (getMSTimeDiff(m_outroTimer, getMSTime()) >= 2000)
                {
                    boxRGBA(sApplication->GetRenderer(), 0, 0, sApplication->GetWindowWidth(), sApplication->GetWindowHeight(), 0, 0, 0, 255);
                    sApplication->SetStage(STAGE_MENU);
                    return;
                }
                break;
            }
        }
    }

    SDL_QueryTexture(m_stolenBerryTexture, nullptr, nullptr, &txr.w, &txr.h);
    txr.x = 1229 - 10 - txr.w;
    txr.y = 10;
    SDL_RenderCopy(sApplication->GetRenderer(), m_stolenBerryTexture, nullptr, &txr);
}

void GameStage::UpdateStolenBerryCount(int delta)
{
    m_stolenBerries += delta;

    std::string toprint;
    
    if (m_stolenBerries <= BERRY_COUNT)
        toprint = std::to_string(BERRY_COUNT - m_stolenBerries) + " / " + std::to_string(BERRY_COUNT);
    else
        toprint = "0 / " + std::to_string(BERRY_COUNT);

    SDL_Color txtcol = { 127,255,127,0 };

    if (m_stolenBerries > BERRY_LIMIT_4)
    {
        m_textures[1] = sImageStorage->GetImage(GI_BERRY_4);
        txtcol = { 255,0,0,0 };
    }
    else if (m_stolenBerries > BERRY_LIMIT_3)
    {
        m_textures[1] = sImageStorage->GetImage(GI_BERRY_3);
        txtcol = { 240,120,0,0 };
    }
    else if (m_stolenBerries > BERRY_LIMIT_2)
    {
        m_textures[1] = sImageStorage->GetImage(GI_BERRY_2);
        txtcol = { 192,192,0,0 };
    }
    else if (m_stolenBerries > BERRY_LIMIT_1)
    {
        m_textures[1] = sImageStorage->GetImage(GI_BERRY_1);
        txtcol = { 120,230,0,0 };
    }

    m_stolenBerryTexture = SDL_CreateTextureFromSurface(sApplication->GetRenderer(), TTF_RenderText_Solid(sApplication->GetFont(FONT_BIG), toprint.c_str(), txtcol));

    // sad ending :( they stole all berries
    if (m_stolenBerries >= BERRY_COUNT)
        EndGame(false);
}

void GameStage::OnDraw()
{
    if (sApplication->IsGameOver())
    {
        DrawOutro();
        return;
    }

    for (std::pair<uint32_t, SDL_Texture*> txt : m_textures)
    {
        if (txt.first != 1)
            SDL_RenderCopy(sApplication->GetRenderer(), txt.second, nullptr, &m_textureDims[txt.first]);
    }

    if (m_bombPlanted)
    {
        SDL_RenderCopy(sApplication->GetRenderer(), sImageStorage->GetImage(GI_BOMB), nullptr, &m_bomb);
        if (m_bombRemoveTimer)
        {
            if (getMSTimeDiff(m_bombRemoveTimer, getMSTime()) > 150)
            {
                KillInRadius(90.0f, true);
                m_bombPlanted = false;
            }
            else
            {
                float tm = ((float)getMSTimeDiff(m_bombRemoveTimer, getMSTime()) / 150.0f);
                SDL_Rect trg;
                trg.w = 16;
                trg.h = 16;

                SDL_SetTextureAlphaMod(sImageStorage->GetImage(GI_PLAYERSTAR), (uint8_t)((1.0f-tm) * 255.0f));

                tm *= 70.0f;
                for (size_t i = 0; i < 8; i++)
                {
                    trg.x = (int)( (m_bomb.x + 8) + tm * cos((float)i*M_PI / 4.0f) );
                    trg.y = (int)( (m_bomb.y + 8) + tm * sin((float)i*M_PI / 4.0f) * 0.5f );
                    SDL_RenderCopy(sApplication->GetRenderer(), sImageStorage->GetImage(GI_PLAYERSTAR), nullptr, &trg);
                }
                for (size_t i = 0; i < 8; i++)
                {
                    trg.x = (int)( (m_bomb.x + 8) + tm * 0.5f * cos((float)i*M_PI / 4.0f) );
                    trg.y = (int)( (m_bomb.y + 8) + tm * 0.5f * sin((float)i*M_PI / 4.0f) * 0.5f );
                    SDL_RenderCopy(sApplication->GetRenderer(), sImageStorage->GetImage(GI_PLAYERSTAR), nullptr, &trg);
                }

                SDL_SetTextureAlphaMod(sImageStorage->GetImage(GI_PLAYERSTAR), 255);
            }
        }
    }

    if (m_lastSwingCheck && getMSTimeDiff(m_lastSwingCheck, getMSTime()) > 150)
    {
        KillInRadius(40.0f);
        m_lastSwingCheck = 0;
    }

    SDL_Rect txr;

    for (FlyingBerry& berry : m_flyingBerries)
    {
        txr.w = 16;
        txr.h = 16;

        if (!berry.isFlying)
        {
            txr.x = (int)berry.dstX;
            txr.y = (int)berry.dstY;

            SDL_RenderCopy(sApplication->GetRenderer(), sImageStorage->GetImage(GI_GINGERBERRY), nullptr, &txr);
        }
    }

    time_t remTime = max(0, (m_gameStartTime + 210) - time(nullptr));
    if (remTime != m_lastRemTime)
    {
        time_t mins = remTime / 60;
        time_t secs = remTime % 60;

        std::string toprint = std::to_string(mins) + ":" + (secs < 10 ? "0" : "") + std::to_string(secs);

        m_timeTexture = SDL_CreateTextureFromSurface(sApplication->GetRenderer(), TTF_RenderText_Solid(sApplication->GetFont(FONT_BIG), toprint.c_str(), { 127,255,127,0 }));
        m_lastRemTime = remTime;
    }

    if (getMSTimeDiff(m_lastEnergyTick, getMSTime()) > ENERGY_REGEN_TICK)
    {
        m_lastEnergyTick = getMSTime();
        m_energy = min(100, m_energy + 1);
    }

    std::list<size_t> torem;

    for (size_t i = 0; i < m_objects.size(); i++)
    {
        // visibility
        if (i < m_objects.size() - 1 && !m_objects[i]->GetDeathTime() && (m_objects[i]->GetPositionY() > m_objects[i + 1]->GetPositionY() || m_objects[i + 1]->GetDeathTime()))
        {
            WorldObject* tmp = m_objects[i];
            m_objects[i] = m_objects[i + 1];
            m_objects[i + 1] = tmp;
        }

        m_objects[i]->UpdateAndDraw();

        if (m_objects[i]->GetType() == OBJTEX_MONSTER1)
        {
            if ((m_objects[i]->IsPathFinished() || (m_objects[i]->GetDeathTime() && getMSTimeDiff(m_objects[i]->GetDeathTime(), getMSTime()) > 2000)))
            {
                if (m_objects[i]->IsPathFinished() && m_objects[i]->HasBerry())
                    UpdateStolenBerryCount(1);
                torem.push_back(i);
            }
            else
            {
                if (m_objects[i]->GetAnim() == ANIM_THROW && m_objects[i]->HasBerry() && m_objects[i]->GetAnimFrame() >= 4)
                {
                    float baseX = (m_objects[i]->GetPositionX() < 600.0f) ? 380.0f : 800.0f;

                    m_objects[i]->CancelBerry();

                    // throw to master
                    if (m_objects[i]->GetPath() >= 18 && m_objects[i]->GetPath() <= 21)
                    {
                        m_flyingBerries.push_back(FlyingBerry(m_objects[i]->GetPositionX() - 8, m_objects[i]->GetPositionY() - 24,
                            585,
                            329,
                            getMSTime(), 200));

                        UpdateStolenBerryCount(1);
                    }
                    else // throw to far end
                    {
                        m_flyingBerries.push_back(FlyingBerry(m_objects[i]->GetPositionX() - 8, m_objects[i]->GetPositionY() - 24,
                            baseX + ((rand() % 40) - 20),
                            120.0f + (float)(rand() % 20),
                            getMSTime(), 300));
                    }
                }
                else if ((m_objects[i]->GetPath() == 16 || m_objects[i]->GetPath() == 17) && m_objects[i]->GetCurrentPathPoint() == 4 && !m_objects[i]->HasBerry())
                {
                    for (std::list<FlyingBerry>::iterator itr = m_flyingBerries.begin(); itr != m_flyingBerries.end(); ++itr)
                    {
                        if (itr->isFlying)
                            continue;
                        if (itr->dstX < 600)
                        {
                            if (m_objects[i]->GetPath() != 16)
                                continue;
                        }
                        else
                        {
                            if (m_objects[i]->GetPath() != 17)
                                continue;
                        }

                        m_objects[i]->GiveBerry();
                        m_objects[i]->SetBerryCatchHide();
                        m_objects[i]->ContinueInPath(1);
                        m_flyingBerries.erase(itr);
                        break;
                    }
                }
            }
        }
    }

    for (std::list<size_t>::reverse_iterator itr = torem.rbegin(); itr != torem.rend(); ++itr)
        m_objects.erase(m_objects.begin() + *itr);

    for (FlyingBerry& berry : m_flyingBerries)
    {
        txr.w = 16;
        txr.h = 16;

        if (berry.isFlying)
        {
            float progress = ((float)getMSTimeDiff(berry.timeStart, getMSTime())) / (float)berry.duration;
            if (progress > 1.0f)
            {
                progress = 1.0f;
                berry.isFlying = false;
            }

            txr.x = (int)(berry.srcX + (berry.dstX - berry.srcX) * progress);
            txr.y = (int)(berry.srcY + (berry.dstY - berry.srcY) * progress);

            SDL_RenderCopy(sApplication->GetRenderer(), sImageStorage->GetImage(GI_GINGERBERRY), nullptr, &txr);
        }
    }

    SDL_RenderCopy(sApplication->GetRenderer(), m_textures[1], nullptr, &m_textureDims[1]);

    MonsterControl();

    txr.x = 10;
    txr.y = 10;
    SDL_QueryTexture(m_timeTexture, nullptr, nullptr, &txr.w, &txr.h);
    SDL_RenderCopy(sApplication->GetRenderer(), m_timeTexture, nullptr, &txr);

    SDL_QueryTexture(m_stolenBerryTexture, nullptr, nullptr, &txr.w, &txr.h);
    txr.x = 1229 - 10 - txr.w;
    SDL_RenderCopy(sApplication->GetRenderer(), m_stolenBerryTexture, nullptr, &txr);

    rectangleRGBA(sApplication->GetRenderer(), 464, 5, 764, 15, 0, 192, 0, 255);
    boxRGBA(sApplication->GetRenderer(), 464, 5, (Sint16)(464 + 300.0f*((float)m_energy / 100.0f)), 15, 0, 192, 0, 255);
}

void GameStage::OnKeyPress(uint16_t key, bool press)
{
    if (sApplication->IsGameOver())
        return;

    if (key == SDLK_w)
    {
        if (press)
            m_player->AddMoveDirection(DIR_UP);
        else
            m_player->RemoveMoveDirection(DIR_UP);
    }
    else if (key == SDLK_a)
    {
        if (press)
            m_player->AddMoveDirection(DIR_LEFT);
        else
            m_player->RemoveMoveDirection(DIR_LEFT);
    }
    else if (key == SDLK_s)
    {
        if (press)
            m_player->AddMoveDirection(DIR_DOWN);
        else
            m_player->RemoveMoveDirection(DIR_DOWN);
    }
    else if (key == SDLK_d)
    {
        if (press)
            m_player->AddMoveDirection(DIR_RIGHT);
        else
            m_player->RemoveMoveDirection(DIR_RIGHT);
    }
}

void GameStage::KillInRadius(float radius, bool bomb)
{
    if (sApplication->IsGameOver())
        return;

    for (WorldObject* obj : m_objects)
    {
        if (obj->GetType() == OBJTEX_MONSTER1 && !obj->GetDeathTime())
        {
            float dx = obj->GetPositionX() - (bomb ? m_bomb.x : m_player->GetPositionX());
            float dy = obj->GetPositionY() - (bomb ? m_bomb.y : m_player->GetPositionY());
            float dist = sqrt(dx*dx + dy*dy);

            if (dist < radius)
                obj->Kill();
        }
    }
}

void GameStage::OnMouseClick(int32_t x, int32_t y, bool left)
{
    printf("Click: %i : %i\n", x, y);
    if (sApplication->IsGameOver())
        return;

    if (left)
    {
        if (!m_player->HasSpecial(SPECIAL_ATTACK_SWING) && m_energy > 25)
        {
            m_energy -= 25;
            m_player->SetSpecial(SPECIAL_ATTACK_SWING);
            m_lastSwingCheck = getMSTime();

            KillInRadius(40.0f);
        }
    }
    else
    {
        if (!m_bombPlanted)
        {
            // once per 4s
            if (getMSTimeDiff(m_lastBombPlant, getMSTime()) < 4000)
                return;

            m_bomb.w = 32;
            m_bomb.h = 32;
            m_bomb.x = (int) m_player->GetPositionX();
            m_bomb.y = (int) m_player->GetPositionY();
            m_bombPlanted = true;
            m_bombRemoveTimer = 0;
            m_lastBombPlant = getMSTime();
        }
        else if (m_bombRemoveTimer == 0)
        {
            KillInRadius(120.0f, true);
            m_bombRemoveTimer = getMSTime();
        }
    }
}
