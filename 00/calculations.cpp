sf::Vector2f toEuclidean(float radius, float &angle)
{
    return {
        radius * std::cos(angle),
        radius * std::sin(angle)};
}

float toDegrees(float radians)
{
    return float(double(radians) * 180.0 / M_PI);
}

int getRandomNumber(int min, int max)
{
    return rand() % (max + 1);
}