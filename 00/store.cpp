enum states
{
    menu_screen,
    input_name,
    play_game,
    repeat_level,
    victory,
    fail,
    quit
};

states gameState = menu_screen;

int level = 1;