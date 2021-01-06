enum states
{
    menu_screen,
    input_name,
    start_game,
    playing,
    level_lost,
    level_lost_modal,
    quit,
    high_scores,
    platform_expansion,
    ball_acceleration,
    ball_slowdown,
    open_portal,
    ball_stick_platform,
    add_extra_life
};

states gameState = menu_screen;

int level = 1;