// COLLISION HELPERS

INTERNAL bool point_vs_rect_collision (float px, float py, float rx, float ry, float rw, float rh)
{
    return ((px >= rx) && (py >= ry) && (px < (rx+rw)) && (py < (ry+rh)));
}

INTERNAL bool rect_vs_rect_collision (float ax, float ay, float aw, float ah, float bx, float by, float bw, float bh)
{
    return ((ax < (bx+bw)) && ((ax+aw) > bx) && (ay < (by+bh)) && ((ay+ah) > by));
}

// ENT_FISH

#define FISH_SPAWN_START 0.0f
#define FISH_SPAWN_RATE  0.5f
#define FISH_ANIM_SPEED  0.2f
#define FISH_SPEED 50
#define FISH_SCORE 10
#define FISH_DIR_L 0
#define FISH_DIR_R 1

INTERNAL void create_fish (Entity* entity)
{
    entity->palette = random_int_range(PAL_FISH_0,PAL_FISH_2);
    entity->dir = random_int_range(FISH_DIR_L,FISH_DIR_R);
    entity->t = 0.0f;

    if (entity->dir == FISH_DIR_L) entity->x = SCREEN_W;
    if (entity->dir == FISH_DIR_R) entity->x = 0 - SPR_FISH_R_0.w;

    entity->y = random_int_range(12, SCREEN_H-SPR_FISH_R_0.h);
}
INTERNAL void update_fish (Entity* entity, float dt)
{
    entity->t += dt;

    // Move the fish and deactivate when off-screen.
    if (entity->dir == FISH_DIR_L)
    {
        entity->x -= FISH_SPEED * dt;
        if (entity->x < -SPR_FISH_L_0.w) entity->alive = false;
    }
    if (entity->dir == FISH_DIR_R)
    {
        entity->x += FISH_SPEED * dt;
        if (entity->x > SCREEN_W) entity->alive = false;
    }
}
INTERNAL const Clip* render_fish (Entity* entity, float dt)
{
    // Determine what animation frame to play for the fish.
    if (entity->dir == FISH_DIR_L)
    {
        if (entity->t < 0.2f) return &SPR_FISH_L_0;
        else
        {
            if (entity->t > FISH_ANIM_SPEED*2) entity->t = 0.0f;
            return &SPR_FISH_L_1;
        }
    }
    else
    {
        if (entity->t < FISH_ANIM_SPEED) return &SPR_FISH_R_0;
        else
        {
            if (entity->t > FISH_ANIM_SPEED*2) entity->t = 0.0f;
            return &SPR_FISH_R_1;
        }
    }
}
INTERNAL void collide_fish (Entity* entity, int mx, int my, int mw, int mh, bool shot)
{
    if (shot)
    {
        int x = CAST(int,entity->x)+3;
        int y = CAST(int,entity->y)+2;
        int w = 9;
        int h = 4;

        if (rect_vs_rect_collision(mx,my,mw,mh, x,y,w,h))
        {
            // Kill the fish.
            create_effect(EFX_BLOOD, x,y,w,h, 10,15);
            play_sound(SND_HIT,0);
            gScore += FISH_SCORE;
            entity->alive = false;
        }
    }
}

// ENT_URCHIN

#define URCHIN_SPAWN_START 3.0f
#define URCHIN_SPAWN_RATE  1.0f
#define URCHIN_ANIM_SPEED  0.2f
#define URCHIN_WIDTH  16
#define URCHIN_HEIGHT 16
#define URCHIN_SPEED  30


INTERNAL void create_urchin (Entity* entity)
{
    entity->palette = PAL_URCHIN;
    entity->t = URCHIN_ANIM_SPEED;
    entity->x = random_int_range(0, SCREEN_W-URCHIN_WIDTH);
    entity->y = SCREEN_H;
    entity->frame = 0;
}
INTERNAL void update_urchin (Entity* entity, float dt)
{
    // Slowly moves upwards.
    entity->y -= URCHIN_SPEED * dt;

    // If off-screen then the urchin is deactivated.
    if (entity->y + URCHIN_HEIGHT < 0)
    {
        entity->alive = false;
    }
}
INTERNAL const Clip* render_urchin (Entity* entity, float dt)
{
    entity->t -= dt;
    if (entity->t <= 0.0f)
    {
        entity->t = URCHIN_ANIM_SPEED;
        entity->frame++;
        if (entity->frame >= ARRAYSIZE(ANM_URCHIN))
        {
            entity->frame = 0;
        }
    }
    return ANM_URCHIN[entity->frame];
}
INTERNAL void collide_urchin (Entity* entity, int mx, int my, int mw, int mh, bool shot)
{
    int x = CAST(int,entity->x)+4;
    int y = CAST(int,entity->y)+4;
    int w = 8;
    int h = 8;

    if (rect_vs_rect_collision(mx,my,mw,mh, x,y,w,h))
    {
        // @Incomplete: Add a game over state...
        printf("GAME OVER!\n");
    }
}

// ENT_SQUID

INTERNAL void create_squid (Entity* entity)
{
    // @Incomplete: ...
}
INTERNAL void update_squid (Entity* entity, float dt)
{
    // @Incomplete: ...
}
INTERNAL const Clip* render_squid (Entity* entity, float dt)
{
    // @Incomplete: ...
    return NULL;
}
INTERNAL void collide_squid (Entity* entity, int mx, int my, int mw, int mh, bool shot)
{
    // @Incomplete: ...
}

// ENTITIES

INTERNAL void create_entity (EntityID id)
{
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (!entity->alive)
        {
            entity->alive = true;
            entity->type = id;
            entity->t = 0.0f;

            // Specific creation logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_FISH  ): create_fish  (entity); break;
                case (ENT_URCHIN): create_urchin(entity); break;
                case (ENT_SQUID ): create_squid (entity); break;
            }

            break;
        }
    }
}

INTERNAL void update_entity (float dt)
{
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific update logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_FISH  ): update_fish  (entity, dt); break;
                case (ENT_URCHIN): update_urchin(entity, dt); break;
                case (ENT_SQUID ): update_squid (entity, dt); break;
            }
        }
    }
}

INTERNAL void render_entity (float dt)
{
    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific render logic for the different entity types.
            const Clip* frame = NULL;
            switch (entity->type)
            {
                case (ENT_FISH  ): frame = render_fish  (entity, dt); break;
                case (ENT_URCHIN): frame = render_urchin(entity, dt); break;
                case (ENT_SQUID ): frame = render_squid (entity, dt); break;
            }
            render_bitmap(entity->x,entity->y,entity->palette,frame);
        }
    }
}

INTERNAL void collide_entity (bool shot)
{
    // We use a box around the mouse point for collision as it feels better to give
    // some leeway when shooting entities, rather than needing a point collision.
    int mx = CAST(int,get_mouse_x())-2;
    int my = CAST(int,get_mouse_y())-2;
    int mw = 4;
    int mh = 4;

    for (int i=0; i<ENTITY_MAX; ++i)
    {
        Entity* entity = gEntity+i;
        if (entity->alive)
        {
            // Specific collision logic for the different entity types.
            switch (entity->type)
            {
                case (ENT_FISH  ): collide_fish  (entity, mx,my,mw,mh, shot); break;
                case (ENT_URCHIN): collide_urchin(entity, mx,my,mw,mh, shot); break;
                case (ENT_SQUID ): collide_squid (entity, mx,my,mw,mh, shot); break;
            }
        }
    }
}

INTERNAL void create_spawner ()
{
    gSpawner.t_fish = FISH_SPAWN_START;
    gSpawner.t_urchin = URCHIN_SPAWN_START;
}

INTERNAL void update_spawner (float dt)
{
    // FISH
    if (gSpawner.t_fish > 0.0f) gSpawner.t_fish -= dt;
    else
    {
        create_entity(ENT_FISH);
        gSpawner.t_fish = FISH_SPAWN_RATE;
    }
    // URCHIN
    if (gSpawner.t_urchin > 0.0f) gSpawner.t_urchin -= dt;
    else
    {
        create_entity(ENT_URCHIN);
        gSpawner.t_urchin = URCHIN_SPAWN_RATE;
    }
}
