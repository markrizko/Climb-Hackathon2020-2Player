#pragma once
#include <SDL.h>
#include <SDL_ttf.h>
#include <stack>
#include "Card.h"

typedef struct {
	SDL_Rect draw_rect;
	struct {
		Uint8 r, g, b, a;
	} color;

	bool pressed;
} button_t;

class Game
{
public:
	Game();
	bool init();
	void runGame();
	void endGame();
		//Will take a card from the inputed deck
		//Then remove said card from deck, putting it into play
	//
	// Game Operations
	void Draw();
	int Tie(); // 0 if tie again, 1 if win, 2 if lose
	void Turn(); 
	int compare(); // compares cards - returns 0 if invalid move - 1 if win - 2 if draw
	//void winMsg(); // displays who wins at end of game
	bool question();
	//SDL Operations
	void onMouseClick(int mousex, int mousey);
	//bool buttonPress(SDL_Renderer* r, button_t* btn);
private:
	void processInput();
	void updateGame();
	void genOutput();
	void displayCards();
	//SDL Operations
	SDL_Window* window;
	SDL_Window* subWindow;
	SDL_Renderer* subRenderer;
	SDL_Renderer* renderer;
	bool running;
	button_t goButton;

	int redTotal();
	int blackTotal(); 
	bool turn; // false if black turn, true if red turn
	bool whoWins; // false if black wins, true if red wins
	Deck *redDeck;
	Deck *blackDeck;
	std::list <int> selectedRed;
	std::list <int> selectedBlack;
	//Deck of cards 
	Card redInPlay[3];
	int redNumIP; // # of red cards in play
	Card blackInPlay[3];
	int blackNumIP; // # of black cards in play
		//Card arrays to hold cards in play

	bool invalidMove;
	bool initFlag;
};