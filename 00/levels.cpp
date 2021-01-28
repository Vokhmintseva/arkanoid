// #include <vector>
// #include <map>
using namespace std;

Brick createBrick(sf::Color color, sf::Vector2f position, map<int, PrizeType> prizesAssignment, const int index, bool isDoubleHitBrick)
{
    brick.setPosition(position);
    brick.setColor(color);
    Brick oneBrick;
    oneBrick.brickSprite = brick;
    oneBrick.isBrokenWithDoubleHit = isDoubleHitBrick;
    if (prizesAssignment.count(index))
    {
        oneBrick.brickSprite.setColor(sf::Color(41, 255, 0));
        PrizeType prize = prizesAssignment[index];
        oneBrick.prize.prizeType = prize;
        oneBrick.prize.prizeSprite = prizesSprites[prize];
    }
    else
    {
        oneBrick.prize.prizeType = none;
    }
    return oneBrick;
}

int changeBrickIndex(int index, const int bricksNumber)
{
    index = index + 1;
    if (index > bricksNumber - 1)
    {
        index = index - 2;
    }
    return index;
}

vector<int> definePrizesBricksIndexes(const int bricksNumber, const int numberOfRandoms)
{
    srand(static_cast<unsigned int>(time(0)));
    vector<int> prizeBricksIndexes;
    while (prizeBricksIndexes.size() < numberOfRandoms)
    {
        int index = getRandomNumber(0, bricksNumber - 1);
        while (std::find(prizeBricksIndexes.begin(), prizeBricksIndexes.end(), index) != prizeBricksIndexes.end())
            index = changeBrickIndex(index, bricksNumber);
        prizeBricksIndexes.push_back(index);
    }
    prizeBricksIndexes = {24, 25, 26, 27, 28, 28};
    return prizeBricksIndexes;
}

int getBrickIndex(int row, int column, const int columnsTotal)
{
    return (row * columnsTotal + column);
}

std::vector<Brick> createBricksVector_1level(sf::Vector2f startPosition, std::vector<float> *timeFromCollisionWithCrackedBrick)
{
    const int bricksNumber = 30;
    timeFromCollisionWithCrackedBrick->resize(bricksNumber);
    for (float time : *timeFromCollisionWithCrackedBrick)
        time = 0;
    const int bricksWithPrizesNum = 6;
    vector<int> prizeBricksIndexes = definePrizesBricksIndexes(bricksNumber, bricksWithPrizesNum);
    map<int, PrizeType> prizesAssignment = {
        {prizeBricksIndexes[0], portal_door},
        {prizeBricksIndexes[1], portal_door},
        {prizeBricksIndexes[2], portal_door},
        {prizeBricksIndexes[3], portal_door},
        {prizeBricksIndexes[4], portal_door},
        {prizeBricksIndexes[5], portal_door}};
    brick.setTexture(getBrickTexture());
    std::vector<Brick> bricks;
    float xStart = startPosition.x;
    float yStart = startPosition.y;
    const int rowsTotal = 5;
    const int columnsTotal = 6;
    bool isDoubleHitBrick = false;
    for (int i = 0; i < rowsTotal; i++)
    {
        for (int j = 0; j < columnsTotal; j++)
        {
            const int index = getBrickIndex(i, j, columnsTotal);
            bricks.push_back(createBrick(sf::Color(250, 107, 235), {x : xStart, y : yStart}, prizesAssignment, index, isDoubleHitBrick));
            xStart = xStart + 45;
        }
        xStart = startPosition.x;
        yStart = yStart + 25;
    }
    return bricks;
}

std::vector<Brick> createBricksVector_2level(sf::Vector2f startPosition, std::vector<float> *timeFromCollisionWithCrackedBrick)
{
    const int bricksNumber = 21;
    timeFromCollisionWithCrackedBrick->resize(bricksNumber);
    for (float time : *timeFromCollisionWithCrackedBrick)
        time = 0;
    const int bricksWithPrizesNum = 2;
    const int rowBricksNumber = 6;
    vector<int> prizeBricksIndexes = definePrizesBricksIndexes(bricksNumber, bricksWithPrizesNum);
    std::vector<Brick> bricks;
    //map<случайный индекс, тип приза> prizesAssignment
    map<int, PrizeType> prizesAssignment = {
        {prizeBricksIndexes[0], slow_ball_down},
        {prizeBricksIndexes[1], accelerate_ball},
        {prizeBricksIndexes[2], expand_platform},
    };
    brick.setTexture(getRubyTexture(), true);
    float xStart = startPosition.x;
    float yStart = startPosition.y;
    int index = 0;
    bool isDoubleHitBrick = false;
    for (int i = 0; i < rowBricksNumber; i++)
    {
        bricks.push_back(createBrick(sf::Color(236, 80, 215), {x : xStart, y : yStart}, prizesAssignment, index, isDoubleHitBrick));
        index += 1;
        xStart = xStart + 45;
    }
    yStart += 30;
    xStart = startPosition.x + 20;
    brick.setTexture(getSapphireTexture(), true);
    for (int i = 0; i < rowBricksNumber - 1; i++)
    {
        bricks.push_back(createBrick(sf::Color(0, 255, 255), {x : xStart, y : yStart}, prizesAssignment, index, isDoubleHitBrick));
        index += 1;
        xStart = xStart + 45;
    }
    yStart += 40;
    xStart = startPosition.x + 40;
    brick.setTexture(getRubyTexture(), true);
    for (int i = 0; i < rowBricksNumber - 2; i++)
    {
        bricks.push_back(createBrick(sf::Color(0, 255, 0), {x : xStart, y : yStart}, prizesAssignment, index, isDoubleHitBrick));
        index += 1;
        xStart = xStart + 45;
    }
    yStart += 30;
    xStart = startPosition.x + 65;
    brick.setTexture(getSapphireTexture(), true);
    for (int i = 0; i < rowBricksNumber - 3; i++)
    {
        bricks.push_back(createBrick(sf::Color(183, 130, 210), {x : xStart, y : yStart}, prizesAssignment, index, isDoubleHitBrick));
        index += 1;
        xStart = xStart + 45;
    }
    yStart += 40;
    xStart = startPosition.x + 90;
    brick.setTexture(getRubyTexture(), true);
    for (int i = 0; i < rowBricksNumber - 4; i++)
    {
        bricks.push_back(createBrick(sf::Color(200, 220, 40), {x : xStart, y : yStart}, prizesAssignment, index, isDoubleHitBrick));
        index += 1;
        xStart = xStart + 45;
    }
    yStart += 30;
    xStart = startPosition.x + 112;
    brick.setTexture(getSapphireTexture(), true);
    for (int i = 0; i < rowBricksNumber - 5; i++)
    {
        bricks.push_back(createBrick(sf::Color(0, 255, 255), {x : xStart, y : yStart}, prizesAssignment, index, isDoubleHitBrick));
        index += 1;
        xStart = xStart + 45;
    }
    return bricks;
}

std::vector<Brick> createBricksVector_3level(sf::Vector2f startPosition, std::vector<float> *timeFromCollisionWithCrackedBrick)
{
    std::vector<sf::Color> colors = {sf::Color(0, 239, 255), sf::Color(211, 225, 3), sf::Color(0, 239, 255),
                                     sf::Color(211, 225, 3), sf::Color(0, 239, 255), sf::Color(211, 225, 3)};
    const int bricksNumber = 30;
    timeFromCollisionWithCrackedBrick->resize(bricksNumber);
    for (float time : *timeFromCollisionWithCrackedBrick)
        time = 0;
    //std::fill(timeFromCollisionWithCrackedBrick.begin(), timeFromCollisionWithCrackedBrick.end(), 0);
    const int bricksWithPrizesNum = 6;
    vector<int> prizeBricksIndexes = definePrizesBricksIndexes(bricksNumber, bricksWithPrizesNum);
    map<int, PrizeType> prizesAssignment = {
        {prizeBricksIndexes[0], slow_ball_down},
        {prizeBricksIndexes[1], accelerate_ball},
        {prizeBricksIndexes[2], expand_platform},
    };
    brick.setTexture(getSapphireTexture());
    std::vector<Brick> bricks;
    float xStart = startPosition.x;
    float yStart = startPosition.y;
    const int rowsTotal = 5;
    const int columnsTotal = 6;
    bool isDoubleHitBrick = false;
    for (int i = 0; i < rowsTotal; i++)
    {
        for (int j = 0; j < columnsTotal; j++)
        {
            const int index = getBrickIndex(i, j, columnsTotal);
            isDoubleHitBrick = (index < columnsTotal || index >= bricksNumber - columnsTotal) ? true : false;
            //cout << isDoubleHitBrick << endl;
            bricks.push_back(createBrick(colors[i], {x : xStart, y : yStart}, prizesAssignment, index, isDoubleHitBrick));
            xStart = xStart + 45;
        }
        xStart = startPosition.x;
        yStart = yStart + 35;
    }
    return bricks;
}