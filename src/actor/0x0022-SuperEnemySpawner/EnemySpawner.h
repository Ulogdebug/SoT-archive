#ifndef __ENEMY_SPAWNER_H__
#define __ENEMY_SPAWNER_H__

#include <uLib.h>

struct EnemySpawner;

typedef void (* EnemySpawnerFunc)(struct EnemySpawner*, PlayState*);

typedef struct EnemySpawner {
    Actor actor;
    s16 flagType;
    s16 enemyType;
    s16 flag;
    EnemySpawnerFunc actionFunc;
} EnemySpawner;

#endif // __ENEMY_SPAWNER_H__