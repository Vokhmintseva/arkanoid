enum states
{
    menu_screen,
    input_name,
    start_game,
    playing,
    level_lost,
    level_lost_modal,
    quit,
    high_scores
};

states gameState = menu_screen;

int level = 1;