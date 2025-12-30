#include "entity.h"
#include "raylib.h"

class Dummy : public Entity {
  public:
    Dummy(int id, Vector2 pos, Vector2 vel, float scale)
        : Entity(id, pos, vel) {
        siz = Vector2{12 * scale, 15 * scale};
        col = BEIGE;

        healthMax = 50.0f;
        health = healthMax;
    }
};
