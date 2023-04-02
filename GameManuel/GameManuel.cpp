/*
                        GAME DESCRIPTION:
    The task in this game is to collect as many points as you can
    Each time you collect 10 points, you get one life
    Each time you step on the monster you loose 1 life and monster appears in different position

    This game has proggression saves and here how it works:
    Before the beggining you need to sign in into your account or sign up new account.
    Firstly you need to enter your nickname and then game checks if the user with this name exists
    If your account exists, you must enter your password to sign in, else the game will offer you to create new account by entering your new password

    Game saves your progress every time you make a movement on a playground
    To exit the game, you need to pres SPACE and follow the instructions.
*/


#include <iostream> 
#include <windows.h>
#include <conio.h>
#include <string>
#include <fstream>
using namespace std;

#define H 10 // Map height
#define W 10 // Map width
ofstream fout;
ifstream fin;

class Entity {
public:
    
    
    void setCordX(int x) {
        if (x < W + 1 && x >= 1)
            posX = x;
    }
    void setCordY(int y) {
        if (y < H + 1 && y >= 1)
            posY = y;
    }
    int getCordX() {
        return posX;
    }
    int getCordY() {
        return posY;
    }
private:
    int posX;
    int posY;
};

class Player : public Entity {
public:
    Player() {

        this->setCordX(1);
        this->setCordY(1);
        this->hp = 1;
    } // constructor, that sets starting position of the player

    int dispHP()
    {
        return hp;
    }
    int dispPoints() {
        return points;
    }
    void addPoints()
    {
        points++;
    }
    void modHP(bool aor) // true adds 1 HP, false removes 1 HP
    {
        if (aor)
            hp++;
        else if (hp >= 0)
            hp--;
    }
    void writePin(int a)
    {
       pincode = a;
    }
    int readPin()
    {
        return pincode;
    }

private:
    int pincode = 0;
    int points = 0;
    int hp = 1;
};

Player player;
Entity monster;
Entity point;

//Writes file with your account info
void writeFile(string nick)
{
    fout.open(nick + ".txt");
    if(fout.is_open())
    fout.write((char*)&player, sizeof(Player));
    fout.close();
}
//reads file with name taht equals your nickname\creates new user
void readFile(string nick)
{
    fin.open(nick + ".txt");
    if (fin.is_open()) {
        fin.read((char*)&player, sizeof(Player));
        int pass;
        cout << "Enter your password:";
        cin >> pass;
        if (pass != player.readPin())
        {
            cout << "Wrong password. Try again\n";
            readFile(nick);
        }

    }
    else {
        cout << "New user registration.\nEnter new pin: ";
        int pin;
        cin >> pin;
        player.writePin(pin);
        writeFile(nick);
    }
    fin.close();
}

//This function draws the playground
void pgDraw()
{
    system("cls");
    for (int i = 0; i <= H + 1; i++) {
        for (int j = 0; j <= W + 1; j++) {
            if (i == 0 || i == H + 1)
                cout << "=";
            else if (j == 0 || j == W + 1)
                cout << "|";
            else if (j == player.getCordX() && i == player.getCordY()) // Draws player
                cout << char(1);
            else if (j == monster.getCordX() && i == monster.getCordY()) // Draws monster
                cout << char(6);
            else if (j == point.getCordX() && i == point.getCordY()) // Draws point
                cout << char(4);
            else
                cout << ".";

            cout << " ";
        }
        cout << endl;
    }
    cout << "HP: ";
    for (int hpAmount = 0; hpAmount < player.dispHP(); hpAmount++)
        cout << char(3);
    cout << " | Points: " << player.dispPoints() << endl;
    cout << "Xpos: " << player.getCordX() << " " << "Ypos: " << player.getCordY();
    cout << "\nMonster: " << char(6) << " Points: " << char(4);
}
//this function generates monster on a random cords
void monsterRand() {
    int a = rand();
    int b = rand();
    if ((a != player.getCordX() && b != player.getCordY()) && (a != point.getCordX() && b != point.getCordY())) {
        monster.setCordX(a % W + 1);
        monster.setCordY(b % H + 1);

    }
    else 
        monsterRand();
}
//this function generates point on a random cords
void pointRand() {
    int a = rand();
    int b = rand();
    if ((a != player.getCordX() && b != player.getCordY()) && (a != monster.getCordX() && b != monster.getCordY())) {
        point.setCordX(a % W + 1);
        point.setCordY(b % H + 1);
    }
    else
        pointRand();
}
// this function checks if player iterated with the monster or the point
void checkCollision(string nick)
{
    if (monster.getCordX() == player.getCordX() && monster.getCordY() == player.getCordY()) {
        player.modHP(false);
        
        player.setCordX(1);
        player.setCordY(1);
        monsterRand();
        writeFile(nick);
    }
    if (point.getCordX() == player.getCordX() && point.getCordY() == player.getCordY()) {
        player.addPoints();

        if (player.dispPoints() % 10 == 0)
            player.modHP(true);
        pointRand();
        writeFile(nick);
    }
}


int main() {
    srand(time(0));
    monsterRand();
    pointRand();
    string playernick;
    cout << "Enter your nickname: ";
    cin >> playernick;
    readFile(playernick);
    

    pgDraw();
    bool play = true;
    while (play)
    {

        if (_kbhit())
        {
            
            switch (_getch())
            {
           // default:
           //     pgDraw();
            case 72: //moveup
                player.setCordY(player.getCordY() - 1);
                checkCollision(playernick);
               pgDraw();
                break;
            case 80: //movedown
                player.setCordY(player.getCordY() + 1);
                checkCollision(playernick);
              pgDraw();
                break;
            case 75: //moveleft 
                player.setCordX(player.getCordX() - 1);
                checkCollision(playernick);
               pgDraw();
                break;
            case 77: //moveright
                player.setCordX(player.getCordX() + 1);
                checkCollision(playernick); 
               pgDraw();
                break;
            case 32:
                cout << "\n\nPress Space again to exit!" << endl;
                cout << "Or another key to continue\n\nYour progress will be saved if you exit." << endl;
                if (_getch() == 32) {
                    play = false;
                    break;
                }
                else {
                  pgDraw();
                    break;
                    
                }

               
                
            }
            
        }
        if (player.dispHP() == 0)
        {
            system("cls");
            cout << "You lose!\n";
            player.modHP(true);
            writeFile(playernick);
            _getch();
            main();
            break;
       }
    }
    return 0;
}

