#include "general.h"
#include "PathStorage.h"

#define BERRY_GET_TIME 500

PathStorage::PathStorage()
{
    //
}

void PathStorage::Init()
{
    std::vector<PathNode> *p;
    
    // simple left, down, and back
    p = &m_pathMap[0];
    p->push_back(PathNode(0, 410, 0));
    p->push_back(PathNode(165, 410, 0));
    p->push_back(PathNode(376, 613, 0));
    p->push_back(PathNode(165, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(0, 410, 0));

    // meet second in center, down, then back
    p = &m_pathMap[1];
    p->push_back(PathNode(0, 365, 0));
    p->push_back(PathNode(550, 365, 0));
    p->push_back(PathNode(550, 613, 0));
    p->push_back(PathNode(376, 613, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(165, 410, 0));
    p->push_back(PathNode(0, 410, 0));

    // meet second in center, down, then back
    p = &m_pathMap[2];
    p->push_back(PathNode(1229, 365, 0));
    p->push_back(PathNode(679, 365, 0));
    p->push_back(PathNode(679, 613, 0));
    p->push_back(PathNode(853, 613, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(1021, 410, 0));
    p->push_back(PathNode(1229, 410, 0));

    // simple left, down, and back
    p = &m_pathMap[3];
    p->push_back(PathNode(1229, 410, 0));
    p->push_back(PathNode(1064, 410, 0));
    p->push_back(PathNode(853, 613, 0));
    p->push_back(PathNode(1064, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(1229, 410, 0));

    // complicated path, left to right
    p = &m_pathMap[4];
    p->push_back(PathNode(0, 439, 0));
    p->push_back(PathNode(55, 439, 0));
    p->push_back(PathNode(128, 526, 0));
    p->push_back(PathNode(413, 526, 0));
    p->push_back(PathNode(512, 619, 0));
    p->push_back(PathNode(739, 411, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(1229, 411, 0));

    // complicated path, right to left
    p = &m_pathMap[5];
    p->push_back(PathNode(1229, 439, 0));
    p->push_back(PathNode(1174, 439, 0));
    p->push_back(PathNode(1101, 526, 0));
    p->push_back(PathNode(816, 526, 0));
    p->push_back(PathNode(717, 619, 0));
    p->push_back(PathNode(490, 411, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(0, 411, 0));

    // GROUP1
    p = &m_pathMap[6];
    p->push_back(PathNode(120, 0, 0));
    p->push_back(PathNode(273, 140, 0));
    p->push_back(PathNode(466, 140, 0));
    p->push_back(PathNode(466, 619, 5000));
    p->push_back(PathNode(466, 411, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(0, 411, 0));
    p = &m_pathMap[7];
    p->push_back(PathNode(120, 0, 0));
    p->push_back(PathNode(273, 140, 0));
    p->push_back(PathNode(388, 140, 0));
    p->push_back(PathNode(388, 619, 5000));
    p->push_back(PathNode(388, 411, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(0, 411, 0));
    p = &m_pathMap[8];
    p->push_back(PathNode(120, 0, 0));
    p->push_back(PathNode(273, 140, 0));
    p->push_back(PathNode(335, 140, 0));
    p->push_back(PathNode(335, 619, 5000));
    p->push_back(PathNode(335, 411, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(0, 411, 0));
    p = &m_pathMap[9];
    p->push_back(PathNode(765, 0, 0));
    p->push_back(PathNode(765, 140, 0));
    p->push_back(PathNode(574, 140, 0));
    p->push_back(PathNode(574, 619, 5000));
    p->push_back(PathNode(574, 411, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(1229, 411, 0));
    p = &m_pathMap[10];
    p->push_back(PathNode(765, 0, 0));
    p->push_back(PathNode(765, 140, 0));
    p->push_back(PathNode(639, 140, 0));
    p->push_back(PathNode(639, 619, 5000));
    p->push_back(PathNode(639, 411, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(1229, 411, 0));
    p = &m_pathMap[11];
    p->push_back(PathNode(765, 0, 0));
    p->push_back(PathNode(765, 140, 0));
    p->push_back(PathNode(687, 140, 0));
    p->push_back(PathNode(687, 619, 5000));
    p->push_back(PathNode(687, 411, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(1229, 411, 0));

    // thrower 1
    p = &m_pathMap[12];
    p->push_back(PathNode(0, 410, 0));
    p->push_back(PathNode(165, 410, 0));
    p->push_back(PathNode(376, 613, 0));
    p->push_back(PathNode(376, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(376, 410, 10, FEAT_THROW));
    p->push_back(PathNode(376, 410, 400));
    p->push_back(PathNode(376, 613, 0));
    p->push_back(PathNode(376, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(376, 410, 10, FEAT_THROW));
    p->push_back(PathNode(376, 410, 400));
    p->push_back(PathNode(376, 613, 0));
    p->push_back(PathNode(165, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(0, 410, 0));

    // thrower 2
    p = &m_pathMap[13];
    p->push_back(PathNode(0, 410, 0));
    p->push_back(PathNode(285, 410, 0));
    p->push_back(PathNode(496, 613, 0));
    p->push_back(PathNode(496, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(496, 410, 10, FEAT_THROW));
    p->push_back(PathNode(496, 410, 400));
    p->push_back(PathNode(496, 613, 0));
    p->push_back(PathNode(496, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(496, 410, 10, FEAT_THROW));
    p->push_back(PathNode(496, 410, 400));
    p->push_back(PathNode(496, 613, 0));
    p->push_back(PathNode(285, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(0, 410, 0));

    // thrower 3
    p = &m_pathMap[14];
    p->push_back(PathNode(1229, 410, 0));
    p->push_back(PathNode(1064, 410, 0));
    p->push_back(PathNode(853, 613, 0));
    p->push_back(PathNode(853, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(853, 410, 10, FEAT_THROW));
    p->push_back(PathNode(853, 410, 400));
    p->push_back(PathNode(853, 613, 0));
    p->push_back(PathNode(853, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(853, 410, 10, FEAT_THROW));
    p->push_back(PathNode(853, 410, 400));
    p->push_back(PathNode(853, 613, 0));
    p->push_back(PathNode(1064, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(1229, 410, 0));

    // thrower 4
    p = &m_pathMap[15];
    p->push_back(PathNode(1229, 410, 0));
    p->push_back(PathNode(944, 410, 0));
    p->push_back(PathNode(733, 613, 0));
    p->push_back(PathNode(733, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(733, 410, 10, FEAT_THROW));
    p->push_back(PathNode(733, 410, 400));
    p->push_back(PathNode(733, 613, 0));
    p->push_back(PathNode(733, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(733, 410, 10, FEAT_THROW));
    p->push_back(PathNode(733, 410, 400));
    p->push_back(PathNode(733, 613, 0));
    p->push_back(PathNode(944, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(1229, 410, 0));

    // catcher 1
    p = &m_pathMap[16];
    p->push_back(PathNode(142, 0, 0));
    p->push_back(PathNode(233, 110, 0));
    p->push_back(PathNode(380, 110, 0));
    p->push_back(PathNode(380, 120, 0));
    p->push_back(PathNode(380, 120, 6000000, FEAT_CATCH));
    p->push_back(PathNode(233, 120, 400));
    p->push_back(PathNode(142, 0, 0));

    // catcher 2
    p = &m_pathMap[17];
    p->push_back(PathNode(724, 0, 0));
    p->push_back(PathNode(724, 66, 0));
    p->push_back(PathNode(800, 110, 0));
    p->push_back(PathNode(800, 120, 0));
    p->push_back(PathNode(800, 120, 6000000, FEAT_CATCH));
    p->push_back(PathNode(724, 66, 400));
    p->push_back(PathNode(724, 0, 0));

    // thrower to master 1
    p = &m_pathMap[18];
    p->push_back(PathNode(1229, 410, 0));
    p->push_back(PathNode(1064, 410, 0));
    p->push_back(PathNode(853, 613, 0));
    p->push_back(PathNode(660, 465, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(660, 465, 10, FEAT_THROW));
    p->push_back(PathNode(660, 465, 400));
    p->push_back(PathNode(853, 613, 0));
    p->push_back(PathNode(660, 465, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(660, 465, 10, FEAT_THROW));
    p->push_back(PathNode(660, 465, 400));
    p->push_back(PathNode(853, 613, 0));
    p->push_back(PathNode(1064, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(1229, 410, 0));

    // thrower to master 2
    p = &m_pathMap[19];
    p->push_back(PathNode(0, 410, 0));
    p->push_back(PathNode(165, 410, 0));
    p->push_back(PathNode(376, 613, 0));
    p->push_back(PathNode(569, 465, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(569, 465, 10, FEAT_THROW));
    p->push_back(PathNode(569, 465, 400));
    p->push_back(PathNode(376, 613, 0));
    p->push_back(PathNode(569, 465, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(569, 465, 10, FEAT_THROW));
    p->push_back(PathNode(569, 465, 400));
    p->push_back(PathNode(376, 613, 0));
    p->push_back(PathNode(165, 410, BERRY_GET_TIME, FEAT_GET_BERRY));
    p->push_back(PathNode(0, 410, 0));
}

bool PathStorage::GetNextNode(uint32_t path, uint32_t currentNode, float &dstX, float &dstY, uint32_t &moveDuration, uint32_t &moveStart, float curX, float curY, bool *moveDirs, uint32_t &features, float speed)
{
    if (m_pathMap.find(path) == m_pathMap.end())
        return false;

    std::vector<PathNode> &v0 = m_pathMap[path];

    if (v0.size() <= currentNode)
        return false;

    PathNode &trg = v0[currentNode];

    dstX = trg.x;
    dstY = trg.y;
    moveDuration = 0;
    moveStart = getMSTime() + trg.waitBeforeStart;
    features = trg.features;

    if (currentNode != 0)
    {
        float dx = fabs(curX - dstX);
        float dy = fabs(curY - dstY);
        float dist = sqrt(dx*dx + dy*dy);

        moveDuration = (uint32_t)(10.0f * dist / speed);
    }

    int32_t sX = (int32_t)(curX + 0.1f);
    int32_t sY = (int32_t)(curY + 0.1f);
    int32_t dX = (int32_t)(dstX + 0.1f);
    int32_t dY = (int32_t)(dstY + 0.1f);

    if (dX > sX) moveDirs[1] = true;
    if (dX < sX) moveDirs[3] = true;
    if (dY > sY) moveDirs[2] = true;
    if (dY < sY) moveDirs[0] = true;

    return true;
}
