#ifndef __DUMMY_ENEMY_H__
#define __DUMMY_ENEMY_H__

#include <uLib.h>

struct DummyEnemy;

typedef void (* DummyEnemyFunc)(struct DummyEnemy*, PlayState*);

typedef struct DummyEnemy {
    Actor actor;
} DummyEnemy;

#endif // __DUMMY_ENEMY_H__