using namespace std;

bool comparePlayers(const Player &a, const Player &b)
{
    return a.scores >= b.scores;
}

void readHighScores(std::list<Player> &bestScores)
{
    std::ifstream fileout("HighScores.txt");
    if (fileout.is_open())
    {
        std::string line;
        while (getline(fileout, line))
        {
            if (line.empty())
                break;
            size_t i = line.find(' ');
            std::string scoreStr = line.substr(0, i);
            int score = std::stoi(scoreStr);
            std::string name = line.substr(i + 1, line.length());
            bestScores.push_back({score, name});
        }
    }
    fileout.close();
}

void writeHighScores(std::list<Player> &bestScores)
{
    int numberOfRecords = 5;
    std::ofstream filein;          // поток для записи
    filein.open("HighScores.txt"); // окрываем файл для записи
    bestScores.sort(comparePlayers);
    for (Player player : bestScores)
    {
        filein << player.scores << " " << player.name << endl;
        numberOfRecords--;
        if (numberOfRecords < 1)
            break;
    }
    filein.close();
}

void handleScores()
{
    std::list<Player> bestScores;
    if (scores != 0)
    {
        Player currPlayer = {scores, playerName};
        bestScores.push_front(currPlayer);
    }
    readHighScores(bestScores);
    writeHighScores(bestScores);
}

void decreaseScores()
{
    scores >= 50
        ? scores = scores - 50
        : scores = 0;
}

void getBestScores()
{
    std::ifstream fileout("HighScores.txt");
    if (fileout.is_open())
    {
        std::string line;
        if (getline(fileout, line))
        {
            size_t i = line.find(' ');
            highScoresStr = line.substr(0, i);
        }
    }
    fileout.close();
}