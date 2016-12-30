#ifndef YD_PATHSTORAGE_H
#define YD_PATHSTORAGE_H

#include "Singleton.h"

enum NodeFeatures
{
    FEAT_GET_BERRY  = 0x00000001,
    FEAT_THROW      = 0x00000002,
    FEAT_CATCH      = 0x00000004,
    FEAT_CATCH_PREP = 0x00000008,
};

struct PathNode
{
    PathNode() : x(0), y(0), waitBeforeStart(0) { };
    PathNode(float _x, float _y, uint32_t _waitBeforeStart = 0, uint32_t _features = 0) : x(_x), y(_y), waitBeforeStart(_waitBeforeStart), features(_features) { };

    float x;
    float y;
    uint32_t waitBeforeStart;
    uint32_t features;
};

class PathStorage
{
    friend class Singleton<PathStorage>;
    public:
        void Init();

        bool GetNextNode(uint32_t path, uint32_t currentNode, float &dstX, float &dstY, uint32_t &moveDuration, uint32_t &moveStart, float curX, float curY, bool *moveDirs, uint32_t &features, float speed = 1.0f);

    protected:
        PathStorage();

        std::map<uint32_t, std::vector<PathNode>> m_pathMap;
};

#define sPathStorage Singleton<PathStorage>::getInstance()

#endif
