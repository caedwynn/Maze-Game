#include <iostream>
#include <string>
#include <time.h>
#include <windows.h>
//#include <stdio.h>
 
using namespace std;
const int ROWS = 7;
const int COLS = 7;
const int PLAYER_LIFE = 20;  //A random value above this value means the player is killed by the monster
const int PLAYER_WEAPON = 75; //Hit points are higher if you have a weapon 
const int PLAYER_ARMOR  = 75;//Hit points are higher if you have armor 
const int PLAYER_ARMOR_AND_WEAPON = 95;//Hit points are really high with both a weapon and armor
const int MONSTER_LIFE = 30;  //A random value above this number means the player kills the monster

const unsigned char ULC = 201; //upper left corner
const unsigned char HB = 205; //horizontal border
const unsigned char URC = 187; //upper right corner
const unsigned char VB = 186; //vertical border
const unsigned char LRC = 188; //Lower right corner
const unsigned char LLC = 200; //lower left corner

const char PlayerSymbol = 'P';
const char MonsterSymbol = 'M';
const char TreasureSymbol = '$';
const char ArmorSymbol = 'A';
const char WeaponSymbol = 'W';
const char MT = ' ';
const char ExitSymbol = 'X';
enum gameObject {EMPTY, TREASURE, MONSTER, WEAPON, ARMOR, PLAYER, EXIT};	

void gamecolor(unsigned char);
 int resolveCombat(bool Armor, bool Weapon);

int main () {
  
	srand((unsigned int)time(NULL)); //seed the random number function;
	cout<<"Welcome to the dungeon"<<endl;
	char command;
	char gameboard[ROWS+4][COLS+4] = 
	{
		{MT, MT, '0', '1', '2', '3', '4', '5', '6', '7', MT},
		{MT, ULC, HB, HB, HB, HB, HB, HB, HB, HB, URC},
		{'A', VB, MT, MT, MT, MT, MT, MT, MT, MT, VB},
	{'B', VB, MT, MT, MT, MT, MT, MT, MT, MT, VB},
	{'C', VB, MT, MT, MT, MT, MT, MT, MT, MT, VB},
	{'D', VB, MT, MT, MT, MT, MT, MT, MT, MT, VB},
	{'E', VB, MT, MT, MT, MT, MT, MT, MT, MT, VB},
	{'F', VB, MT, MT, MT, MT, MT, MT, MT, MT, VB},
	{'G', VB, MT, MT, MT, MT, MT, MT, MT, MT, VB},
	{'H', VB, MT, MT, MT, MT, MT, MT, MT, MT, VB},
		{MT, LLC, HB, HB, HB, HB, HB, HB, HB, HB, LRC}

	};
	gameObject dungeon[ROWS+1][COLS+1]; //Create the dungeon
	//Initialize the dungeon to all empty values
	for(int i=0;i<ROWS;i++)
	{
		for(int j=0;j<COLS;j++)
		{
			dungeon[i][j]=EMPTY;
		}
	}
	bool gameOver = false;
	bool hasArmor = false;
	bool hasWeapon = false;
	bool hasTreasure = false;
	bool monsterFound=false;
	bool monsterAlive=true;
	int r, c, newLoc,combatResult;
	gameObject temp=EMPTY;
	//Starting configuration
	//add the player to the dungeon in rows/col 0
	int playerRow = rand() % 3;
	int playerCol = 0;
	dungeon[playerRow][playerCol] = PLAYER;
	//add the treasure in row 4-6 and col 1-6
	int treasureRow = rand()%3 +4;
	int treasureCol = rand()%6 + 1;
	dungeon[treasureRow][treasureCol] = TREASURE;
	//add the monster at treasure row +1 and col -1
	int monsterRow = treasureRow +1;
	int monsterCol = treasureCol -1;
	dungeon[monsterRow][monsterCol] = MONSTER;
	//Add the weapon in any location
	do
	{
		r = rand()%8;
		c = rand()%8;
	}while (dungeon[r][c] != EMPTY);
	int weaponRow = r;
	int weaponCol = c;
	dungeon[weaponRow][weaponCol]=WEAPON;
	//Add the Armor in any location
	do
	{
		r = rand()%8;
		c = rand()%8;
	}while (dungeon[r][c] != EMPTY);
	int ArmorRow = r;
	int ArmorCol = c;
	dungeon[ArmorRow][ArmorCol]=ARMOR;
	//Last row and col is the exit from the dungeon
	int exitRow = ROWS;
	int exitCol = COLS;
	dungeon[exitRow][exitCol]= EXIT;

	while(!gameOver)
	{

	//place objects on gameboard
		if(!hasArmor) {gameboard[ArmorRow+2][ArmorCol+2] = ArmorSymbol;}
		if(!hasWeapon) {gameboard[weaponRow+2][weaponCol+2] = WeaponSymbol;}
		if(!hasTreasure) {gameboard[treasureRow+2][treasureCol+2] = TreasureSymbol;}
		if(monsterAlive){gameboard[monsterRow+2][monsterCol+2] = MonsterSymbol;}
		gameboard[playerRow+2][playerCol+2] = PlayerSymbol;
		gameboard[exitRow+2][exitCol+2] = ExitSymbol;
		
		cout<<endl;
		//Display game board
		for(int i=0;i<(ROWS+4);i++)
		{
			for(int j=0;j<(COLS+4);j++)
			{
				
				gamecolor(gameboard[i][j]);
			
				cout<<gameboard[i][j];
				
		
			}
			cout<<endl;
		}
		gamecolor('Z'); //Change font color back to black

		cout<<"\nPlease type in a command: \na to move left, s to move down, w to move up, d to move right, q to quit: ";
		cin>>command;

		//Based on what the user types in, update the player's location
		r = playerRow;
		c= playerCol;
		switch(command)
		{
		case 'a':
			c = playerCol -1;
			if(c<0)
			{
				c = playerCol;
				cout<<"You have hit a wall"<<endl;
			}
			break;
		case 's':
			r = playerRow+1;
			if(r>ROWS)
			{
				r=playerRow;
				cout<<"You have hit a wall"<<endl;
			}
			break;
		case 'd':
			c=playerCol+1;
			if(c>COLS)
			{
				c=playerCol;
				cout<<"You have hit a wall"<<endl;
			}
			break;
		case 'w':
			r=playerRow-1;
			if(r<0)
			{
				r=playerRow;
				cout<<"You have hit a wall"<<endl;
			}
			break;
		case 'q':
			gameOver=true;
			cout<<"Quitting-bye"<<endl;
			break;
		}
		//Check if the game is over
		if(!gameOver)
		{
			//Check for events based on the player's position
			switch(dungeon[r][c])
			{
			case EXIT:
				gameOver=true;
				cout<<"You exit the cave"<<endl;
				break;
			case TREASURE:
				cout<<"\n\nYou found the treasure"<<endl;
				hasTreasure=true; //If treasure is found, set flag to show the player has the treasure
				break;
			case WEAPON:
				cout<<"\n\nYou now have a weapon"<<endl;
				hasWeapon=true; //If weapon is found, set flag to show the player has a weapon
				break;
			case ARMOR:
				cout<<"\n\nYou are strong now - you found the Armor"<<endl;
				hasArmor=true; //If armor is found, set flag to show player has armor
				break;
			case MONSTER:
				cout<<"\nYou found the monster"<<endl;
				monsterFound=true; //If monster is found, set flag
				break;
			}
			
		}

		//Move player
		dungeon[playerRow][playerCol]=EMPTY;
		gameboard[playerRow+2][playerCol+2]=MT;
		dungeon[r][c]=PLAYER;
		playerRow=r;
		playerCol=c;

		//Process finding the monster
		if(monsterFound)
		{
				//resolve combat
			combatResult = resolveCombat(hasArmor, hasWeapon);
			//-1 return means the monster has killed player
			//0 result means the player survived the attack
			//1 result means the player killed monster
				if(combatResult==-1)
				{
					gameOver=true;
				}
				else if(combatResult ==1)
				{
					monsterAlive=false;
				}
		}
		else
		{
			//Move monster
			r=monsterRow;
			c=monsterCol;
			newLoc=rand()%4; //Pick a random number, based on the number
			//move the monster 1 row or column
		switch(newLoc)
		{
		case 0:
			c = monsterCol -1;
			if(c<0) {c = monsterCol;}
			break;
		case 1:
			r = monsterRow+1;
			if(r>ROWS){r=monsterRow;}
			break;
		case 2:
			c=monsterCol+1;
			if(c>COLS){c=monsterCol;}
			break;
		case 3:
			r=monsterRow-1;
			if(r<0){r=monsterRow;}
			break;
	
		}
			if (dungeon[r][c]==PLAYER)
			{
				cout<<"The monster has found you!";
				//resolve combat
				combatResult = resolveCombat(hasArmor, hasWeapon);
				if(combatResult==-1)
				{
					gameOver=true;
				}
				else if(combatResult ==1)
				{
					monsterAlive=false;
				}
			}
			
			dungeon[monsterRow][monsterCol]=EMPTY;
			gameboard[monsterRow+2][monsterCol+2]=MT;
		
			monsterRow=r;			//update row and column
			monsterCol=c;
			if(monsterAlive){
			dungeon[monsterRow][monsterCol]=MONSTER;	//Change monster's location in the cave
			}
		}
		
	}
     
		 cin.ignore(2);
		 return 0;
}
//gameColor function
//Input: position in gameboard
//Processing: change color and background of character
//Output: None
void gamecolor(unsigned char type)
{
	switch(type)
	{
	case 201: case 205: case 200: case 188: case 186: case 187:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_INTENSITY);
		break;
	case 'W':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_INTENSITY);
		break;
	case '$':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		break;
	case 'P':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_INTENSITY|BACKGROUND_RED|BACKGROUND_INTENSITY);
		break;
	case 'T':
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
		break;
	case 'X':
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_INTENSITY|BACKGROUND_RED|BACKGROUND_BLUE|BACKGROUND_GREEN);
		break;
	case 'M':
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_RED|FOREGROUND_BLUE|FOREGROUND_INTENSITY|BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE);
		break;
	default:
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE),FOREGROUND_BLUE|FOREGROUND_GREEN|FOREGROUND_RED);
		break;
	}
}
//resolveCombat function
//Input: Armor and Weapon status for player
//Output: integer:
//-1 return means the monster has killed player
//0 result means the player survived the attack
//1 result means the player killed monster
int resolveCombat(bool Armor, bool Weapon)
{
	int combat = rand()%100; //combat random value
	
	if(Armor && Weapon)
	{
		if(combat > PLAYER_ARMOR_AND_WEAPON)
		{
			cout<<"\n\nYou have been eaten by the monster - game over!"<<endl;
			return -1;
		}
		else if(combat>(MONSTER_LIFE-15)) //subtract 15 because of weapon
		{
			cout<<"\n\nYou have defeated the monster in combat!"<<endl;
			return 1;
		}
		else
		{
			cout<<"\n\nYou have survived the monster's attack."<<endl;
			return 0;
		}
	}
	else if(Armor)
	{
		if(combat > PLAYER_ARMOR)
		{
			cout<<"\n\nYou have been eaten by the monster - game over!"<<endl;
			return -1;
		}
		else if(combat>MONSTER_LIFE)
		{
			cout<<"\n\nYou have defeated the monster in combat!"<<endl;
			return 1;
		}
		else
		{
			cout<<"\n\nYou have survived the monster's attack."<<endl;
			return 0;
		}
	}

	else if(Weapon)
	{
		if(combat > PLAYER_WEAPON)
		{
			cout<<"\n\nYou have been eaten by the monster - game over!"<<endl;
			return -1;
		}
		else if(combat>(MONSTER_LIFE-15)) //subtract 15 because of weapon
		{
			cout<<"\n\nYou have defeated the monster in combat!"<<endl;
			return 1;
		}
		else
		{
			cout<<"\n\nYou have survived the monster's attack."<<endl;
			return 0;
		}
	}
	else
	{
		if(combat > PLAYER_LIFE)
		{
			cout<<"\n\nYou have been eaten by the monster - game over!"<<endl;
			return -1;
		}
		else if(combat>(MONSTER_LIFE)) 
		{
			cout<<"\n\nYou have defeated the monster in combat!"<<endl;
			return 1;
		}
		else
		{
			cout<<"\n\nYou have survived the monster's attack."<<endl;
			return 0;
		}
		
	}

}

