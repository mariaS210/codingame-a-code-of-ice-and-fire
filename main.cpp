#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <math.h>

using namespace std;

/*
*********** CLASSES ***********
*/
struct position
{
    int x, y;
    position(int x=0, int y=0): x(x), y(y) {}
    string __str__() { return "(" + to_string(this->x) + ", " + to_string(y) + ")";}
};

class building
{
public:
    int owner, type;
    position pos;
    building(int owner, int type, position pos);
    ~building();
};

building::building(int owner, int type, position pos)
{
    this->owner = owner; this->type = type; this->pos = pos;
}

building::~building(){}

class mine
{
public:
    position pos;
    mine(position pos);
    ~mine();
};

mine::mine(position pos)
{
    this->pos = pos;
}

mine::~mine(){}

class unit
{
public:
    int owner, unitId, level;
    position pos;

    unit(int owner, int unitId, int level, position pos);
    ~unit();
};

unit::unit(int owner, int unitId, int level, position pos)
{
    this->pos = pos; this->owner = owner; this->unitId = unitId; this->level = level;
}

unit::~unit(){}

/*
*********** GLOBALS ***********
*/

vector<building> buildings;
vector<unit> units;
vector<mine> mines;
vector<string> map;
string commands = "";

const int ME    = 0;
const int ENEMY = 1;
int gold = 0;
position my_hq = position(0,0);

/*
*********** UTILS ***********
*/

void commandWait()
{
    commands.append("WAIT;");
}

void commandMove(int unitId, position pos)
{
    string movement = "MOVE " + to_string(unitId) + " " + to_string(pos.x) + " " + to_string(pos.y) + ";";
    cerr << movement << endl;
    commands.append(movement);
}

void commandTrain(int level, position pos)
{
    string training = "TRAIN " + to_string(level) + " " + to_string(pos.x) + " " + to_string(pos.y) + ";";
    gold -= level * 10;
    cerr << training << endl;
    commands.append(training);
}

bool positionValid(position origin)
{
    if (origin.x >= 0 && origin.x < 12 && origin.y >= 0 && origin.y < 12)
    {
        cerr << "position " + origin.__str__() + " is valid" << endl;
        return true;
    }
    cerr << "position " + origin.__str__() + " is NOT valid" << endl;
    return false;
}

double distanceToPos(position origin, position destination)
{
    double result = sqrt(pow(origin.x, 2) - pow(destination.x, 2) + pow(origin.y, 2) - pow(destination.y, 2));
    cerr << "distance from " + origin.__str__() + " to " + destination.__str__() + " is " + to_string(result) << endl;
    return result;
}

bool checkMap(position pos)
{
    if (map[pos.y][pos.x] != '#' && map[pos.y][pos.x] != 'O')
        return true;
    return false;
}

position nextFreePos(position origin)
{
    if (my_hq.x == 0) //start down/right
    {
        if (positionValid(position(origin.x, origin.y + 1)) && checkMap(position(origin.x, origin.y + 1)))
            return position(origin.x, origin.y + 1);
        else if (positionValid(position(origin.x + 1, origin.y)) && checkMap(position(origin.x + 1, origin.y)))
            return position(origin.x + 1, origin.y);
        else if (positionValid(position(origin.x - 1, origin.y)) && checkMap(position(origin.x - 1, origin.y)))
            return position(origin.x - 1, origin.y);
        else if (positionValid(position(origin.x, origin.y - 1)) && checkMap(position(origin.x, origin.y - 1)))
            return position(origin.x, origin.y - 1);
    }
    else // start up/left
    {
        if (positionValid(position(origin.x, origin.y - 1)) && checkMap(position(origin.x, origin.y - 1)))
            return position(origin.x, origin.y - 1);
        else if (positionValid(position(origin.x - 1, origin.y)) && checkMap(position(origin.x - 1, origin.y)))
            return position(origin.x - 1, origin.y);
        else if (positionValid(position(origin.x + 1, origin.y)) && checkMap(position(origin.x + 1, origin.y)))
            return position(origin.x + 1, origin.y);
        else if (positionValid(position(origin.x, origin.y + 1)) && checkMap(position(origin.x, origin.y + 1)))
            return position(origin.x, origin.y + 1);
    }
    return position(-1, -1);
}


/*
*********** ALGO ***********
*/

void compute_next_move()
{
    commands = ""; // reset at start of turn

    for (int i  = 0; i < units.size(); i++)
    {
        if (units[i].owner == ME)
        {
            if (gold > 10)
            {
                position nextPos = nextFreePos(units[i].pos);
                if (nextPos.x != -1)
                    commandTrain(1, nextPos);
            }
            else
            {
                position nextPos = nextFreePos(units[i].pos);
                if (nextPos.x != -1)
                    commandMove(units[i].unitId, nextPos);
            }
        }
    }

    while (gold > 10)
    {
        if (my_hq.x == 0)
        {
            if (commands.find("TRAIN 0 1") != 0)
                commandTrain(1, position(0, 1));
            else if (commands.find("TRAIN 1 0") != 0)
                commandTrain(1, position(1, 0));
            else
                commandTrain(1, position(0, 1));
        }
        else
        {
            if (commands.find("TRAIN 11 10") != 0)
                commandTrain(1, position(11, 10));
            else if (commands.find("TRAIN 10 11") != 0)
                commandTrain(1, position(10, 11));
            else
                commandTrain(1, position(11, 10));
        }
    }

    if (commands.compare("") == 0)
    {
        commands.append("WAIT");
    }
}

/**
 * Auto-generated code below aims at helping you parse
 * the standard input according to the problem statement.
 **/
int main()
{
    int numberMineSpots;
    std::cin >> numberMineSpots; std::cin.ignore();
    for (int i = 0; i < numberMineSpots; i++) {
        int x, y;
        std::cin >> x >> y; std::cin.ignore();
        mines.push_back(mine(position(x, y)));
    }

    // game loop
    while (1) {
        std::cin >> gold; std::cin.ignore();
        int income;
        std::cin >> income; std::cin.ignore();
        int opponentGold;
        std::cin >> opponentGold; std::cin.ignore();
        int opponentIncome;
        std::cin >> opponentIncome; std::cin.ignore();
        map.clear();
        for (int i = 0; i < 12; i++) {
            string line;
            std::cin >> line; std::cin.ignore();
            cerr << line << endl;
            map.push_back(line);
        }
        if (map[11][11] == 'O') 
        {
            my_hq = position(11, 11);
        }
        int buildingCount;
        std::cin >> buildingCount; std::cin.ignore();
        buildings.clear();
        for (int i = 0; i < buildingCount; i++) {
            int owner;
            int buildingType;
            int x;
            int y;
            std::cin >> owner >> buildingType >> x >> y; std::cin.ignore();
            buildings.push_back(building(owner, buildingType, position(x, y)));
        }
        int unitCount;
        std::cin >> unitCount; std::cin.ignore();
        units.clear();
        for (int i = 0; i < unitCount; i++) {
            int owner;
            int unitId;
            int level;
            int x;
            int y;
            std::cin >> owner >> unitId >> level >> x >> y; std::cin.ignore();
            units.push_back(unit(owner, unitId, level, position(x, y)));
        }

        // Write an action using cout. DON'T FORGET THE "<< endl"
        // To debug: cerr << "Debug messages..." << endl;
        compute_next_move();

        std::cout << commands << endl;
    }
}