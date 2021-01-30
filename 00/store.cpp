constexpr unsigned WINDOW_WIDTH = 800;
constexpr unsigned WINDOW_HEIGHT = 600;
std::string playerName = "";
int level = 1;
int scores;
std::string highScoresStr;

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
    level_passed
};

states gameState = menu_screen;

enum PrizeType
{
    expand_platform,
    two_balls,
    slow_ball_down,
    accelerate_ball,
    extra_life,
    portal_door,
    sticky_ball,
    none
};

enum BrickKind
{
    Usual_brick,
    Ruby,
    Sapphire,
    Emerald
};

struct Player
{
    int scores;
    std::string name;
};

struct Prize
{
    PrizeType prizeType;
    sf::Sprite *prizeSprite;
};

struct Brick
{
    sf::Sprite brickSprite;
    Prize prize;
    bool isBrokenWithDoubleHit;
};

struct PrizeEffect
{
    PrizeType prizeEffectType;
    float timeOfEffectApplying;
};
