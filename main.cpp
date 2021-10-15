// I chose task requirement 2
#include <iostream>
#include <tuple>
#include <thread>
#include <chrono>
#include <vector>
#include <map>
#include <fstream>

using namespace std;
using namespace std::chrono_literals;

ofstream file;// Item 3.d: this is my feature where I save the states of the game in a history file
const int TIMEOUT = 400; // maximum number of milliseconds that a player is allowed to take
/**
 * @brief this is the position class with implementing some operators
 *  Item 3.b
 */
class Position {
public:
    std::tuple<int, int> pos;

    Position(int i, int j) {
        pos = make_tuple(i, j);
    }

    Position() {}

    /**
     * @brief to compare if 2 positions are equal
     *
     * @param second
     * @return true
     * @return false
     */
    bool operator==(Position second)// Item 3.c
    {
        if (get<0>(this->pos) == get<0>(second.pos) && get<1>(this->pos) == get<1>(second.pos))
            return true;
        return false;
    }

    /**
     * @brief to assign the position
     *
     * @param second
     */
    void operator=(Position second)// Item 3.c
    {
        get<0>(this->pos) = get<0>(second.pos);
        get<1>(this->pos) = get<1>(second.pos);
    }

    /**
     * @brief I implemented this operator to be able to use class
     * position as the key in the itemsMap
     * @param second
     * @return true
     * @return false
     */
    bool operator<(const Position &second) const { // Item 3.c
        if (get<0>(this->pos) < get<0>(second.pos)) {
            return true;
        } else if (get<0>(this->pos) > get<0>(second.pos)) {
            return false;
        } else if (get<1>(this->pos) < get<1>(second.pos)) {
            return true;
        } else if (get<1>(this->pos) > get<1>(second.pos)) {
            return false;
        } else {
            return false;
        }
    }

    virtual int getColumn() { //Item 3.e
        return get<1>(pos);
    }

    virtual int getRow() {
        return get<0>(pos);
    }

    ~Position() = default;
};

/**
 * @brief the general class units which all items inherit from
 *
 */
class units {
public:
    char name;
    Position position;

    void operator=(units second) {
        this->name = second.name;
        this->position = second.position;
    }


};

map<Position, units> itemsMap;// A map to every item in the game to use it to draw the world
/**
 * @brief A general class soldeir to save the objects s,r,p,S,R,P
 *
 */
class soldier : public units, public Position {
public:
    char enemy;

    soldier(Position k, char name, char enemy) {
        this->position = k;
        this->enemy = enemy;
        this->name = name;
    }

    soldier() {}

    int getColumn() {//Item 3.e
        return get<1>(this->position.pos);
    }

    int getRow() {
        return get<0>(this->position.pos);
    }
};

class Name {
    char nam;

    virtual char getName() = 0; //Item 3.e
};

/**
 * @brief these classes are for both of the players
 *
 */
class rock : public soldier, public Name {
public:

    rock(char name, char enemy, int row, int column) {
        this->name = name;
        this->enemy = enemy;
        get<0>(this->position.pos) = row;
        get<1>(this->position.pos) = column;
        itemsMap[this->position].name = this->name;
    }

    char getName() {//Item 3.e
        return name;
    }
};

class scissor : public soldier {
public:
    scissor(char name, char enemy, int row, int column) {
        this->name = name;
        this->enemy = enemy;
        get<0>(this->position.pos) = row;
        get<1>(this->position.pos) = column;
        itemsMap[this->position].name = this->name;
    }

    char getName() {
        return name;
    }
};

class paper : public soldier {
public:
    paper(char name, char enemy, int row, int column) {
        this->name = name;
        this->enemy = enemy;
        get<0>(this->position.pos) = row;
        get<1>(this->position.pos) = column;
        itemsMap[this->position].name = this->name;
    }

    char getName() {
        return name;
    }
};

/**
 * @brief the class mountain which only save their positions
 *
 */
class mountain : public Position, public units {
public:
    char name = 'M';

    mountain(int row, int column) {
        get<0>(this->position.pos) = row;
        get<1>(this->position.pos) = column;
        itemsMap[this->position].name = this->name;
    }

    mountain() {

    }
};

/**
 * @brief the class flag for both of the players
 *
 */
class flag : public Position, public units {
    int player;
public:
    flag(int row, int column, int player) {
        this->name = 'f';
        get<0>(this->position.pos) = row;
        get<1>(this->position.pos) = column;
        if (player == 1)
            itemsMap[position].name = 'F';
        else
            itemsMap[position].name = 'f';
        this->player = player;
    }

    flag(const flag &) = delete; //Item 3.g
    flag &operator=(const flag &) = delete;

};

class World {
public:
    // Item 3.b: I used the smart pointer shared pointer because I need to have shared ownership of the object from different pointer at the same time
    // Item 3.k: using STL container vector
    vector<shared_ptr<units>> p0Container;// container of shared pointers of units and flags of player 0
    vector<shared_ptr<soldier>> p0Soldiers;
    vector<shared_ptr<units>> p1Container;// container of shared pointers of units and flags of player 1
    vector<shared_ptr<soldier>> p1Soldiers;
    vector<shared_ptr<mountain>> mountains;//container of shared pointers of mountains
    shared_ptr<soldier> leader1;
    map<Position, units> items;// A map to every item in the game to use it to draw the world
    /**
     * @brief this function assign the first items of the players, set the mountains, and print the initial set up
     *  Item 3.a.2.a: Initial setup
     */
    void initialSetUp() {
        setP0Units();
        setP1Units();
        setMountains();
        printWorld();
        leader1 = p1Soldiers[9];
        items = itemsMap;
    }

    /**
     * @brief print the world function
     *
     */
    void printWorld() const { // Item 3.f
        cout << " ";
        file << " ";
        for (int i = 1; i <= 9; i++) {
            cout << "   " << i;
            file << "   " << i;
        }
        for (int i = 10; i <= 15; i++) {
            cout << "  " << i;
            file << "  " << i;
        }
        cout << endl;
        file << endl;
        for (int i = 1; i <= 15; i++) {
            if (i < 10) {
                cout << i << " | ";
                file << i << " | ";
            } else {
                cout << i << "| ";
                file << i << "| ";
            }
            for (int j = 1; j <= 15; j++) {
                Position k;
                k.pos = make_tuple(i, j);
                if (items.find(k) != items.end()) {
                    auto p = items.find(k);

                    cout << p->second.name;
                    file << p->second.name;
                    cout << " | ";
                    file << " | ";


                } else {
                    cout << "  | ";
                    file << "  | ";
                }
            }
            cout << endl << "  |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---| " << endl;
            file << endl << "  |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---| " << endl;
        }
    }

private:
/**
 * @brief these functions are used at the begining only to set the items of each player and mountins
 * Item 3.a.1: Each player (player 0 and player 1) controls a set of units
 *
 */
    void setP0Units() {
        for (int i = 1; i <= 6; i++)//initializing his soldiers
        {
            for (int j = 2; j <= 6; j++) {
                if (i == 3 || i == 6)//adding s row
                {
                    scissor s('s', 'P', i, j);
                    shared_ptr<soldier> sharedPtr = make_shared<soldier>(s);// item 3.d
                    p0Container.push_back(sharedPtr);
                    p0Soldiers.push_back(sharedPtr);
                    itemsMap[s.position].name = 's';
                }
                if (i == 2 || i == 5)//adding P row
                {
                    paper p('p', 'R', i, j);
                    shared_ptr<soldier> sharedPtr = make_shared<soldier>(p);
                    p0Container.push_back(sharedPtr);
                    p0Soldiers.push_back(sharedPtr);
                    itemsMap[p.position].name = 'p';
                }
                if (i == 1 || i == 4)//adding R row
                {
                    rock r('r', 'S', i, j);
                    shared_ptr<soldier> sharedPtr = make_shared<soldier>(r);
                    p0Container.push_back(sharedPtr);
                    p0Soldiers.push_back(sharedPtr);
                    itemsMap[r.position].name = 'r';
                }
            }
        }
        flag f0(1, 1, 0);
        itemsMap[f0.position].name = 'f';
        items = itemsMap;
    }

    void setP1Units() {
        for (int i = 10; i <= 15; i++)//initializing his soldiers
        {
            for (int j = 10; j <= 14; j++) {
                if (i == 10 || i == 13)//adding S row
                {
                    scissor S('S', 'p', i, j);
                    shared_ptr<soldier> sharedPtr = make_shared<soldier>(S);
                    p1Container.push_back(sharedPtr);
                    p1Soldiers.push_back(sharedPtr);
                    itemsMap[S.position].name = 'S';
                }
                if (i == 11 || i == 14)//adding P row
                {
                    paper P('P', 'r', i, j);
                    shared_ptr<soldier> sharedPtr = make_shared<soldier>(P);
                    p1Container.push_back(sharedPtr);
                    p1Soldiers.push_back(sharedPtr);
                    itemsMap[P.position].name = 'P';
                }
                if (i == 12 || i == 15)//adding P row
                {
                    rock R('R', 's', i, j);
                    shared_ptr<soldier> sharedPtr = make_shared<soldier>(R);
                    p1Container.push_back(sharedPtr);
                    p1Soldiers.push_back(sharedPtr);
                    itemsMap[R.position].name = 'R';
                }
            }
        }
        flag f1(15, 15, 1);
        itemsMap[f1.position].name = 'F';
        items = itemsMap;
    }

    /**
     * Item 3.a.2.b: the mountains
     */
    void setMountains() {

        mountain m1(3, 11);
        shared_ptr<mountain> mountain1 = make_shared<mountain>(m1);
        mountains.push_back(mountain1);


        mountain m2(3, 13);
        shared_ptr<mountain> mountain2 = make_shared<mountain>(m2);
        mountains.push_back(mountain2);


        mountain m3(5, 10);
        shared_ptr<mountain> mountain3 = make_shared<mountain>(m3);
        mountains.push_back(mountain3);


        mountain m4(5, 14);
        shared_ptr<mountain> mountain4 = make_shared<mountain>(m4);
        mountains.push_back(mountain4);


        mountain m5(6, 11);
        shared_ptr<mountain> mountain5 = make_shared<mountain>(m5);
        mountains.push_back(mountain5);


        mountain m6(6, 12);
        shared_ptr<mountain> mountain6 = make_shared<mountain>(m6);
        mountains.push_back(mountain6);


        mountain m7(6, 13);
        shared_ptr<mountain> mountain7 = make_shared<mountain>(m7);
        mountains.push_back(mountain7);


        mountain m8(13, 6);
        shared_ptr<mountain> mountain8 = make_shared<mountain>(m8);
        mountains.push_back(mountain8);


        for (int i = 7; i <= 9; i++) {
            mountain m(8, i);
            shared_ptr<mountain> mountainK = make_shared<mountain>(m);
            mountains.push_back(mountainK);
        }
        for (int i = 11; i <= 13; i++) {
            mountain m(i, 3);
            shared_ptr<mountain> mountainK = make_shared<mountain>(m);
            mountains.push_back(mountainK);
        }
        for (int i = 11; i <= 13; i++) {
            mountain m(i, 5);
            shared_ptr<mountain> mountainK = make_shared<mountain>(m);
            mountains.push_back(mountainK);
        }
        for (int i = 11; i <= 13; i++) {
            mountain m(i, 7);
            shared_ptr<mountain> mountainK = make_shared<mountain>(m);
            mountains.push_back(mountainK);
        }
        items = itemsMap;
    }

};

/**
 * @brief the class action
 *
 */
class Action {
public:
    Position from; // current row, column of the unit to be moved
    Position to; // position to where the unit must be moved
};

int chooseSoldier = 1;// to choose random soldier to move
int chooseMove = 0;// to choose random direction to move to
Action ac0;

/**
 * @brief this function is used make sure that the player moved something
 *
 * @param pSoldier
 * @return true
 * @return false
 */
bool moveSoldier(shared_ptr<soldier> pSoldier, World world) {
    int &i = get<0>(pSoldier.get()->position.pos);
    int &j = get<1>(pSoldier.get()->position.pos);
    bool moved = false;
    if (chooseMove % 4 == 0)//going up
    {
        if (i != 1) {
            Position temp;
            temp.pos = make_tuple(i - 1, j);
            if (world.items.find(temp)->second.name != 's' && world.items.find(temp)->second.name != 'p' &&
                world.items.find(temp)->second.name != 'r' && world.items.find(temp)->second.name != 'M') {
                ac0.from.pos = make_tuple(i, j);
                ac0.to.pos = make_tuple(i - 1, j);
                moved = true;
                return moved;
            }
        }
    }
    if (chooseMove % 4 == 1)//right
    {
        if (j != 15) {
            Position temp;
            temp.pos = make_tuple(i, j + 1);
            if (world.items.find(temp)->second.name != 's' && world.items.find(temp)->second.name != 'p' &&
                world.items.find(temp)->second.name != 'r' && world.items.find(temp)->second.name != 'M') {
                ac0.from.pos = make_tuple(i, j);
                ac0.to.pos = make_tuple(i, j + 1);
                moved = true;
                return moved;
            }
        }
    }
    if (chooseMove % 4 == 2)// down
    {
        if (i != 15) {
            Position temp;
            temp.pos = make_tuple(i + 1, j);
            if (world.items.find(temp)->second.name != 's' && world.items.find(temp)->second.name != 'p' &&
                world.items.find(temp)->second.name != 'r' && world.items.find(temp)->second.name != 'M') {

                ac0.from.pos = make_tuple(i, j);
                ac0.to.pos = make_tuple(i + 1, j);
                moved = true;
                return moved;
            }
        }
    }
    if (chooseMove % 4 == 3) {
        if (j != 1) {
            Position temp;
            temp.pos = make_tuple(i, j - 1);
            if (world.items.find(temp)->second.name != 's' && world.items.find(temp)->second.name != 'p' &&
                world.items.find(temp)->second.name != 'r' &&
                world.items.find(temp)->second.name != 'M')// check if it is not a mountain or an ally
            {

                ac0.from.pos = make_tuple(i, j);
                ac0.to.pos = make_tuple(i, j - 1);
                moved = true;
                return moved;
            }
        }
    }
    return moved;
}

/**
 * Item 3.c: this player plays randomly but it avoids mountains
 * @param world
 * @return
 */
Action actionPlayerZero(const World world) {
    auto current = (world.p0Soldiers[chooseSoldier % world.p0Soldiers.size()]); //Item 3.d
    int i = 4;
    bool played = false;// this make sure that the player played in its turn
    while (i--) {
        if (moveSoldier(current, world)) {
            played = true;
            break;
        } else {
            chooseMove += i;
        }
    }
    if (!played) {
        chooseSoldier++;
        actionPlayerZero(world);
    } else
        chooseSoldier++;
    return ac0;

}

/**
 * @brief this function is used to explor the path to the other player flag and report what was found in the path
 *
 * @return char
 */
int signP = 14;

/**
 * @brief this function is called when there is a rock in the path to the flag so the player can send a paper to distroy it
 *
 * @param leader
 * @return Action
 */
Action sendP(shared_ptr<soldier> leader) {
    Position k;
    shared_ptr<soldier> temp;
    Action action;
    if (leader->name != 'P') {
        if (get<0>(leader->position.pos) != -1) {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos) - 1, get<1>(leader->position.pos));
            action.to = k;
            leader->name = 'P';
            leader->position.pos = make_tuple(11, signP);
            signP--;
            return action;
        } else {
            leader->name = 'P';
            leader->position.pos = make_tuple(11, signP);
            leader->enemy = 'r';
            k.pos = make_tuple(11, signP + 1);
            action.from = leader->position;
            action.to = k;
            signP--;
            return action;
        }
    } else {
        if (get<1>(leader->position.pos) == 14 && get<0>(leader->position.pos) != 1) {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos), get<1>(leader->position.pos) + 1);
            action.to = k;
            return action;
        }
        if (get<0>(leader->position.pos) != 1) {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos) - 1, get<1>(leader->position.pos));
            action.to = k;
            return action;
        } else {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos), get<1>(leader->position.pos) - 1);
            action.to = k;
            return action;
        }
    }

}

/**
 * @brief Item 3.c: the basic stratigy for player 1 is to send a paper to the flag directly given that it can distory all the rocks in
 * row 1 and reach the flag
 * @param world
 * @return Action
 */
int chooseR = 14;

Action sendR(shared_ptr<soldier> leader, World world) {
    Position k;
    shared_ptr<soldier> temp;
    Action action;
    k.pos = make_tuple(12, chooseR);
    if (leader->position == k) {
        if (get<0>(leader->position.pos) != -1) {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos) - 1, get<1>(leader->position.pos));
            action.to = k;
            leader->name = 'R';
            leader->position.pos = make_tuple(12, chooseR);
            chooseR--;
            return action;
        } else {
            leader->name = 'R';
            leader->position.pos = make_tuple(11, chooseR);
            leader->enemy = 's';
            k.pos = make_tuple(12, chooseR + 1);
            action.from = leader->position;
            action.to = k;
            chooseR--;
            return action;
        }
    } else {
        if (get<1>(leader->position.pos) == 14 && get<0>(leader->position.pos) != 1) {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos), get<1>(leader->position.pos) + 1);
            action.to = k;
            return action;
        }
        if (get<0>(leader->position.pos) != 1) {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos) - 1, get<1>(leader->position.pos));
            action.to = k;
            return action;
        } else {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos), get<1>(leader->position.pos) - 1);
            action.to = k;
            return action;
        }
    }
}

int chooseS = 19;

Action sendS(shared_ptr<soldier> leader, World world) {
    Position k;
    shared_ptr<soldier> temp;
    Action action;
    k.pos = make_tuple(13, chooseS);
    if (leader->position == k) {
        if (get<0>(leader->position.pos) != -1) {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos) - 1, get<1>(leader->position.pos));
            action.to = k;
            leader->name = 'S';
            leader->position.pos = make_tuple(13, chooseS);
            chooseR--;
            return action;
        } else {
            leader->name = 'R';
            leader->position.pos = make_tuple(13, chooseS);
            leader->enemy = 's';
            k.pos = make_tuple(13, chooseS + 1);
            action.from = leader->position;
            action.to = k;
            chooseS--;
            return action;
        }
    } else {
        if (get<1>(leader->position.pos) == 14 && get<0>(leader->position.pos) != 1) {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos), get<1>(leader->position.pos) + 1);
            action.to = k;
            return action;
        }
        if (get<0>(leader->position.pos) != 1) {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos) - 1, get<1>(leader->position.pos));
            action.to = k;
            return action;
        } else {
            action.from = leader->position;
            k.pos = make_tuple(get<0>(leader->position.pos), get<1>(leader->position.pos) - 1);
            action.to = k;
            return action;
        }
    }
}

Action actionPlayerOne(const World world) { // this player plays with a basic strategy
    decltype(world.leader1->position) k;
    k = world.leader1->position;//3.i
    if (get<0>(k.pos) != 1) {
        k.pos = make_tuple(get<0>(k.pos) + 1, get<1>(k.pos));
        if (world.items.find(k)->second.name != 's' && world.leader1->name == 'P') {
            return sendP(world.leader1);
        } else if (world.items.find(k)->second.name ==
                   's') // this means that we need to change the leader to be able to reach the flag so we will sacrifice the current leader to be able to send another in the same path
        {
            if (world.leader1->name == 'P' || world.leader1->name == 'S') {
                Action a;
                a.from = world.leader1->position;
                a.to = k;
                world.leader1->name = 'R';
                return a;
            }
            return sendR(world.leader1, world);
        } else if (world.items.find(k)->second.name == 'r') {
            if (world.leader1->name == 'P' || world.leader1->name == 'R') {
                Action a;
                a.from = world.leader1->position;
                a.to = k;
                world.leader1->name = 'S';
                return a;
            }
            return sendS(world.leader1, world);
        }
    } else {
        k.pos = make_tuple(get<0>(k.pos), get<1>(k.pos) - 1);
        if (world.items.find(k)->second.name != 's' && world.leader1->name == 'P') {
            return sendP(world.leader1);
        } else // this means that we need to change the leader to be able to reach the flag so we will sacrifice the current leader to be able to send another in the same path
        {
            if (world.leader1->name == 'P') {
                Action a;
                a.from = world.leader1->position;
                a.to = k;
                world.leader1->name = 'R';
                return a;
            }
            return sendR(world.leader1, world);
        }
    }
}

/**
 * The return is a pair: action and a boolean whether a timeout happened
 */
std::tuple<Action, bool> waitPlayer(Action (*f)(World), World world) {
    auto start = std::chrono::high_resolution_clock::now();
    // Below, action should be the result of calling the function passed as parameter
    Action action = (*f)(world);
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;
    if (elapsed.count() > TIMEOUT) // if time > 0.3 s
        return {action, true}; // player failed to answer in less than 400 ms
    else return {action, false};
}

/**
 * @brief this is a generic function that report the result of the fighting between 2 soldiers
 * Item 3.b.ii:I used template here because this function deals directly with the fight without knowing the parts
 * it returns the winner and the looser
 * // Item 3.k: using templates
 * @tparam T
 * @param s1
 * @param s2
 * @return tuple<T,T>
 */
template<typename T>
tuple<T, T> fighting(T s1, T s2) {
    T winner, looser;
    if (s1->enemy == s2->name) {
        winner = s1;
        looser = s2;
    } else {
        winner = s2;
        looser = s1;
    }
    return {winner, looser};
}

tuple<rock, scissor> fighting(rock r, scissor s) // Item 3.h
{
    return {r, s};// because we know that the rock will always win
}

/**
 * @brief this function updates the world for player 0, where if the move is to an empty position it will implement it
 * if there is an item it tries to attack it by calling the function fighting()
 * Note: the other item will not be a mountain or item from the same team because this will be checked in the validation befor
 * calling this function
 * @param World
 * @param action
 */
void updateWorldFor0(World &World, Action action) {

    if (itemsMap.find(action.to)->second.name == '\000' || itemsMap.find(action.to)->second.name == 'f') {
        itemsMap.erase(action.from);

        int i = 0;
        for (i; i < World.p0Container.size(); i++) {
            if (World.p0Container[i].get()->position == action.from) {
                World.p0Container[i].get()->position = action.to;
                World.p0Soldiers[i].get()->position = action.to;
                break;
            }
        }
        itemsMap[action.to].name = World.p0Container[i].get()->name;
        World.items = itemsMap;
    } else// now we know that there will be a fight over this cell
    {
        shared_ptr<soldier> s1, s2;
        for (int i = 0; i < World.p0Soldiers.size(); i++) {
            if (World.p0Soldiers[i].get()->position == action.from) {
                s1 = World.p0Soldiers[i];
            }
            if (World.p1Soldiers[i].get()->position == action.to) {
                s2 = World.p1Soldiers[i];
            }
        }
        auto[winner, looser]=fighting(s1, s2);
        itemsMap.erase(looser->position);
        itemsMap.erase(winner->position);
        itemsMap[action.to].name = winner->name;
        winner->position = action.to;
        cout << "the looser who died is: " << looser->name << endl;
        file << "the looser who died is: " << looser->name << endl;
        World.items = itemsMap;
    }
}

/**
 * @brief this function updates the world for player 1, where if the move is to an empty position it will implement it
 * if there is an item it tries to attack it by calling the function fighting()
 * Note: the other item will not be a mountain or item from the same team because this will be checked in the validation befor
 * calling this function
 * @param World
 * @param action
 */
void updateWorldFor1(World &World, Action action) {
    if (itemsMap.find(action.to)->second.name == '\000' || itemsMap.find(action.to)->second.name == 'f') {
        itemsMap.erase(action.from);
        int i = 0;
        for (i; i < World.p1Container.size(); i++) {
            if (World.p1Container[i].get()->position == action.from) {
                World.p1Container[i].get()->position = action.to;
                World.p1Soldiers[i].get()->position = action.to;
                break;
            }
        }
        itemsMap[action.to].name = World.p1Container[i].get()->name;
    } else// now we know that there will be a fight over this cell
    {
        shared_ptr<soldier> s1, s2;
        for (int i = 0; i < World.p1Soldiers.size(); i++) {
            if (World.p1Soldiers[i].get()->position == action.from) {
                s1 = World.p1Soldiers[i];
            }
            if (World.p0Soldiers[i].get()->position == action.to) {
                s2 = World.p0Soldiers[i];
            }
        }
        auto[winner, looser]=fighting(s1, s2);
        itemsMap.erase(looser->position);
        itemsMap.erase(winner->position);
        itemsMap[action.to].name = winner->name;
        winner->position = action.to;
        cout << "the looser who died is: " << looser->name << endl;
        file << "the looser who died is: " << looser->name << endl;
    }
    World.items = itemsMap;
}

bool checkForUnits0(Position k) {
    Position temp;
    temp.pos = make_tuple(get<0>(k.pos) - 1, get<1>(k.pos));
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) - 2, get<1>(k.pos));
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 1, get<1>(k.pos));
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 2, get<1>(k.pos));
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos), get<1>(k.pos) + 1);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos), get<1>(k.pos) + 2);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos), get<1>(k.pos) - 1);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos), get<1>(k.pos) - 2);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }

    temp.pos = make_tuple(get<0>(k.pos) - 1, get<1>(k.pos) - 1);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) - 2, get<1>(k.pos) - 2);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 1, get<1>(k.pos) + 1);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 2, get<1>(k.pos) + 2);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) - 1, get<1>(k.pos) + 1);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) - 2, get<1>(k.pos) + 2);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 1, get<1>(k.pos) - 1);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 2, get<1>(k.pos) - 2);
    if (itemsMap[temp].name == 's' || itemsMap[temp].name == 'p' || itemsMap[temp].name == 'r') {
        return false;
    }
    return true;
}

bool checkForUnits1(Position k) {
    Position temp;
    temp.pos = make_tuple(get<0>(k.pos) - 1, get<1>(k.pos));
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) - 2, get<1>(k.pos));
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 1, get<1>(k.pos));
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 2, get<1>(k.pos));
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos), get<1>(k.pos) + 1);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos), get<1>(k.pos) + 2);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos), get<1>(k.pos) - 1);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos), get<1>(k.pos) - 2);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }

    temp.pos = make_tuple(get<0>(k.pos) - 1, get<1>(k.pos) - 1);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) - 2, get<1>(k.pos) - 2);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 1, get<1>(k.pos) + 1);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 2, get<1>(k.pos) + 2);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) - 1, get<1>(k.pos) + 1);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) - 2, get<1>(k.pos) + 2);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 1, get<1>(k.pos) - 1);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    temp.pos = make_tuple(get<0>(k.pos) + 2, get<1>(k.pos) - 2);
    if (itemsMap[temp].name == 'S' || itemsMap[temp].name == 'P' || itemsMap[temp].name == 'R') {
        return false;
    }
    return true;
}

/**
 * @brief this function check if any of the players done an illegal move or if any of them won the game
 *  Item 3.a.4: applying movement rules
 * @param action
 * @return int
 */
int validate(Action action) {
    if (itemsMap[action.to].name == 'M') {
        return -1;//means lose
    }
    if (itemsMap[action.to].name < 91 && itemsMap[action.from].name < 91 && itemsMap[action.to].name > 64 &&
        itemsMap[action.from].name > 64) {
        return -1;
    }
    if (itemsMap[action.to].name > 91 && itemsMap[action.from].name > 91 && itemsMap[action.to].name < 173 &&
        itemsMap[action.from].name < 173) {
        return -1;
    }

    if (action.to.pos == make_tuple(1, 1)) {
        return 1;//player1 won
    }
    if (action.to.pos == make_tuple(1, 1)) {
        return 1;//player0 won
    }
    return 0;

}

/**
 * Item 3.a: implementing task requirement 2
 * @param world
 * @return
 */
World makeItDarkForP0(World world) {
    World darkWorld = world;
    for (int i = 0; i <= 15; i++) {
        for (int j = 0; j <= 15; j++) {
            Position k;
            k.pos = make_tuple(i, j);
            if (world.items.find(k)->second.name != 's' && world.items.find(k)->second.name != 'p' &&
                world.items.find(k)->second.name != 'r' && checkForUnits0(k)) {
                units u;
                u.position = k;
                u.name = 'D';
                darkWorld.items[k] = u;
            } else {
                darkWorld.items[k] = world.items[k];
            }
        }
    }
    cout << "Player 0 view:" << endl;
    cout << " ";
    file << " ";
    for (int i = 1; i <= 9; i++) {
        cout << "   " << i;
        file << "   " << i;
    }
    for (int i = 10; i <= 15; i++) {
        cout << "  " << i;
        file << "  " << i;
    }
    cout << endl;
    file << endl;
    for (int i = 1; i <= 15; i++) {
        if (i < 10) {
            cout << i << " | ";
            file << i << " | ";
        } else {
            cout << i << "| ";
            file << i << "| ";
        }
        for (int j = 1; j <= 15; j++) {
            Position k;
            k.pos = make_tuple(i, j);
            if (darkWorld.items.find(k) != darkWorld.items.end()) {
                auto p = darkWorld.items.find(k);

                cout << p->second.name;
                file << p->second.name;
                cout << " | ";
                file << " | ";


            } else {
                cout << "  | ";
                file << "  | ";
            }
        }
        cout << endl << "  |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---| " << endl;
        file << endl << "  |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---| " << endl;
    }
    return darkWorld;
}

World makeItDarkForP1(World world) {
    World darkWorld = world;
    for (int i = 0; i <= 15; i++) {
        for (int j = 0; j <= 15; j++) {
            Position k;
            k.pos = make_tuple(i, j);
            if (world.items.find(k)->second.name != 'S' && world.items.find(k)->second.name != 'P' &&
                world.items.find(k)->second.name != 'R' && checkForUnits1(k)) {
                units u;
                u.position = k;
                u.name = 'D';
                darkWorld.items[k] = u;
            } else {
                darkWorld.items[k] = world.items[k];
            }
        }
    }
    cout << "Player 1 view:" << endl;
    cout << " ";
    file << " ";
    for (int i = 1; i <= 9; i++) {
        cout << "   " << i;
        file << "   " << i;
    }
    for (int i = 10; i <= 15; i++) {
        cout << "  " << i;
        file << "  " << i;
    }
    cout << endl;
    file << endl;

    for (int i = 1; i <= 15; i++) {
        if (i < 10) {
            cout << i << " | ";
            file << i << " | ";
        } else {
            cout << i << "| ";
            file << i << "| ";
        }
        for (int j = 1; j <= 15; j++) {
            Position k;
            k.pos = make_tuple(i, j);
            if (darkWorld.items.find(k) != darkWorld.items.end()) {
                auto p = darkWorld.items.find(k);

                cout << p->second.name;
                file << p->second.name;
                cout << " | ";
                file << " | ";


            } else {
                cout << "  | ";
                file << "  | ";
            }
        }
        cout << endl << "  |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---| " << endl;
        file << endl << "  |---|---|---|---|---|---|---|---|---|---|---|---|---|---|---| " << endl;
    }
    return darkWorld;
}

int main() {
    file.open("games_history.txt", ios::app);// this is the file where the states of the game is saved
    World world;// initializing the world
    world.initialSetUp();
    bool endGame = false;
    int p0Won = 0;//neither won nor lost
    int p1Won = 0;
    while (!endGame) {

        auto[action0, timeout0] = waitPlayer(actionPlayerZero, makeItDarkForP0(
                world));//Item 3.a.3: requesting the action from each player
        auto[action1, timeout1] = waitPlayer(actionPlayerOne, makeItDarkForP1(world));
        if (timeout0 || timeout1) {
            endGame = true;
            if (timeout1) {
                cout << "player 1 exceeded the time limit";
                file << "player 1 exceeded the time limit";
            } else {
                cout << "player 0 exceeded the time limit";
                file << "player 0 exceeded the time limit";
            }
        } else {
            //Validate the actions: actionPlayer0 and actionPlayer1
            p1Won = validate(action1);
            p0Won = validate(action0);
            cout << "Their moves:" << endl;
            if (p1Won == 0 && p0Won == 0)//means everything is normal
            {
                updateWorldFor0(world, action0);
                updateWorldFor1(world, action1);
                world.printWorld();// Item 3.f
            } else if (p1Won == -1)// means this player done an illegal move I update the world to see the move
            {
                updateWorldFor0(world, action0);
                updateWorldFor1(world, action1);
                world.printWorld();
                cout << "player1 done an illegal move";
                file << "player1 done an illegal move";
                endGame = true;
            } else if (p0Won == -1)// means this player done an illegal move I update the world to see the move
            {
                updateWorldFor0(world, action0);
                updateWorldFor1(world, action1);
                world.printWorld();
                cout << "player0 done an illegal move";
                file << "player0 done an illegal move";
                endGame = true;
            } else if (p1Won == 1)// means this player reached the other flag
            {
                updateWorldFor0(world, action0);
                updateWorldFor1(world, action1);
                world.printWorld();
                cout << "player1 won";
                file << "player1 won";
                endGame = true;
            } else if (p0Won == 1)// means this player reached the other flag
            {
                updateWorldFor0(world, action0);
                updateWorldFor1(world, action1);
                world.printWorld();
                cout << "player0 won";
                file << "player0 won";
                endGame = true;
            }
        }

        cout << endl;
        file << endl;
    }
    file.close();
    return 0;
}
