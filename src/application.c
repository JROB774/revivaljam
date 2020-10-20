#define GAME_BACKGROUND_COLOR 0xFF2F4B99

INTERNAL bool init_application ()
{
    SDL_ShowCursor(SDL_DISABLE);
    return true;
}

INTERNAL void quit_application ()
{
    SDL_ShowCursor(SDL_ENABLE);
}

INTERNAL void update_application (float dt)
{
    // @Temporary: Pull this spawner code out...
    static float t = 0.0f;
    if (t <= 0.0f)
    {
        create_fish();
        t = 0.5f;
    }
    else
    {
        t -= dt;
    }

    update_fish(dt);
}

INTERNAL void render_application (float dt)
{
    render_clear(GAME_BACKGROUND_COLOR);

    render_fish(dt);

    // Render the in-game mouse cursor/crosshair.
    float cx = get_mouse_x()-(SPR_CURSOR_0.w/2);
    float cy = get_mouse_y()-(SPR_CURSOR_0.h/2);
    render_bitmap(cx,cy,PAL_CURSOR,&SPR_CURSOR_0);
}
