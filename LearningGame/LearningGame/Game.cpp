#include "Game.h"

#define CARDWIDTH 75
#define CARDHEIGHT 150
#define BUTTONWIDTH 50
#define BUTTONHEIGHT 20

#define WINXPOS 100
#define WINYPOS 100
#define WINWIDTH 1024
#define WINHEIGHT 768
//#define SUBWINWIDTH 512
//#define SUBWINHEIGHT 384

#define REDCARD1POSX 50
#define REDCARD1POSY 50


#define REDCARD2POSX 250
#define REDCARD2POSY 50


#define REDCARD3POSX 550
#define REDCARD3POSY 50


#define BLACKCARD1POSX 50
#define BLACKCARD1POSY 400


#define BLACKCARD2POSX 250
#define BLACKCARD2POSY 400


#define BLACKCARD3POSX 550
#define BLACKCARD3POSY 400


#define GOBUTTONX 800
#define GOBUTTONY 500

/*
typedef struct {
	SDL_Rect draw_rect;
	struct {
		Uint8 r, g, b, a;
	} color;

	bool pressed;
} button_t;
*/
/*
bool Game::buttonPress(SDL_Renderer* r, button_t* btn) {
	// draw button
	SDL_SetRenderDrawColor(r, btn->color.r, btn->color.g, btn->color.b, btn->color.a);
	SDL_RenderFillRect(r, &btn->draw_rect);

	// if button press detected - reset it so it wouldn't trigger twice
	if (btn->pressed) {
		btn->pressed = false;
		return true;
	}
	return false;
}
*/

Game::Game()
	:window(nullptr), renderer(nullptr), running (true)
{
	//window = nullptr;
	initFlag = false;
}

bool Game::init()
{
	//initialize all sdl elements
	//initialize sdl
	int sdlinit = SDL_Init(SDL_INIT_VIDEO);
	int ttfinit = TTF_Init();
	if (sdlinit != 0)
	{
		SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
		return false;
	}

	//create sdl window
	window = SDL_CreateWindow(
		"Card Learning Game",
		WINXPOS, //window x
		WINYPOS, //window y
		WINWIDTH, //window width
		WINHEIGHT, //window height
		0
	);
	if (!window)
	{
		SDL_Log("Failed to create window: %s", SDL_GetError());
		return false;
	}

	//create sdl renderer

	renderer = SDL_CreateRenderer(
		window,
		-1,
		SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
	);
	if (!renderer)
	{
		SDL_Log("Failed to create renderer: %s", SDL_GetError());
		return false;
	}

	//define button
	//button_t new_button;
	goButton.color.r = 50;
	goButton.color.g = 200;
	goButton.color.b = 50;
	goButton.color.a = 255;
	goButton.draw_rect.x = GOBUTTONX;
	goButton.draw_rect.y = GOBUTTONY;
	goButton.draw_rect.w = BUTTONWIDTH;
	goButton.draw_rect.h = BUTTONHEIGHT;
	//*goButton = new_button;

	//initialzie actual game items
	//TODO
	redDeck = new Deck();
	blackDeck = new Deck();

	redDeck->FillDeck();
	blackDeck->FillDeck();

	//fill playspace
	Draw();

	invalidMove = false;
	initFlag = true;
	turn = false; // black first every time
	return true;
}

void Game::runGame()
{
	genOutput();
	while (running)
	{
		processInput();
		updateGame();
		genOutput();
		goButton.pressed = false;
	}
	//winMsg();
	endGame();
}

void Game::onMouseClick(int mousex, int mousey)
{
	//Check where the mouse is on click, add that cards index to selected<Color>

	//check if over button
	if (((mousex >= GOBUTTONX) && (mousex <= GOBUTTONX + BUTTONWIDTH))
		&& ((mousey >= GOBUTTONY) && (mousey <= GOBUTTONY + BUTTONWIDTH)))
	{
		goButton.pressed = true;
	}

	//Check if over Red cards
	if (((mousex >= REDCARD1POSX) && (mousex <= REDCARD1POSX + CARDWIDTH))
		&& ((mousey >= REDCARD1POSY) && (mousey <= REDCARD1POSY + CARDHEIGHT)))
	{
		selectedRed.push_back(0);
		redNumIP++;
		//if mouse is over a card, select it for play
	}
	if (((mousex >= REDCARD2POSX) && (mousex <= REDCARD2POSX + CARDWIDTH))
		&& ((mousey >= REDCARD2POSY) && (mousey <= REDCARD2POSY + CARDHEIGHT)))
	{
		selectedRed.push_back(1);
		redNumIP++;
		//if mouse is over a card, select it for play
	}
	if (((mousex >= REDCARD3POSX) && (mousex <= REDCARD3POSX + CARDWIDTH))
		&& ((mousey >= REDCARD3POSY) && (mousey <= REDCARD3POSY + CARDHEIGHT)))
	{
		selectedRed.push_back(2);
		redNumIP++;
		//if mouse is over a card, select it for play
	}


	//Check if over black cards
	if (((mousex >= BLACKCARD1POSX) && (mousex <= BLACKCARD1POSX + CARDWIDTH))
		&& ((mousey >= BLACKCARD1POSY) && (mousey <= BLACKCARD1POSY + CARDHEIGHT)))
	{
		selectedBlack.push_back(0);
		blackNumIP++;
		//if mouse is over a card, select it for play
	}
	if (((mousex >= BLACKCARD2POSX) && (mousex <= BLACKCARD2POSX + CARDWIDTH))
		&& ((mousey >= BLACKCARD2POSY) && (mousey <= BLACKCARD2POSY + CARDHEIGHT)))
	{
		selectedBlack.push_back(1);
		blackNumIP++;
		//if mouse is over a card, select it for play
	}
	if (((mousex >= BLACKCARD3POSX) && (mousex <= BLACKCARD3POSX + CARDWIDTH))
		&& ((mousey >= BLACKCARD3POSY) && (mousey <= BLACKCARD3POSY + CARDHEIGHT)))
	{
		selectedBlack.push_back(2);
		blackNumIP++;
		//if mouse is over a card, select it for play
	}
}

void Game::processInput()
{
	/* Probably should get set when we flip turns
	selectedBlack.clear();
	selectedRed.clear();
	invalidMove = false;
	*/
	SDL_Event e;
	int mousex, mousey;
	while (!goButton.pressed) {
		while (SDL_PollEvent(&e))
		{
			switch (e.type)
			{
			case SDL_QUIT:
				running = false;
				break;
			case SDL_MOUSEBUTTONDOWN:
				SDL_GetMouseState(&mousex, &mousey);
				onMouseClick(mousex, mousey);
				break;
			}
		}
	}
}

void Game::updateGame()
{
	if (goButton.pressed == true && compare() == 0)
	{
		invalidMove = true;
		return;
	}
	else if (!goButton.pressed) return;
	Turn();
	while (!question());
}

bool Game::question()
{
	int sel = rand() % 3;
	int p = rand() % 100;
	int q = rand() % 100;
	int answer;
	int correctAnswer;
	switch (sel) {
	case 0:
		std::cout << p << " + " << q << " = " << std::endl;
		correctAnswer = p + q;
		std::cin >> answer;
		if (answer == correctAnswer) return true;
		else return false;
	case 1:
		std::cout << p << " - " << q << " = " << std::endl;
		correctAnswer = p - q;
		std::cin >> answer;
		if (answer == correctAnswer) return true;
		else return false;
	case 2:
		std::cout << p << " * " << q << " = " << std::endl;
		correctAnswer = p * q;
		std::cin >> answer;
		if (answer == correctAnswer) return true;
		else return false;
	default:
		return true;
	}
}

void Game::genOutput()
{
	SDL_SetRenderDrawColor(renderer, 200, 200, 200, 255);
	SDL_RenderClear(renderer);
	displayCards();
	SDL_RenderPresent(renderer);
}

const char* faceValue(int tag)
{
	if (tag > 10 || tag == 1)
	{
		switch (tag)
		{
		case 1:
			return "Ace";
		case 11:
			return "J";
		case 12:
			return "Q";
		case 13:
			return "K";
		}
	}
	char* result = new char[3];
	SDL_itoa(tag, result, 10);
	return result;
}

void Game::displayCards()
{
	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_Rect redCard1
	{
		REDCARD1POSX,
		REDCARD1POSY,
		CARDWIDTH,
		CARDHEIGHT
	};
	SDL_RenderFillRect(renderer, &redCard1);
	SDL_Rect redCard2
	{
		REDCARD2POSX,
		REDCARD2POSY,
		CARDWIDTH,
		CARDHEIGHT
	};
	SDL_RenderFillRect(renderer, &redCard2);
	SDL_Rect redCard3
	{
		REDCARD3POSX,
		REDCARD3POSY,
		CARDWIDTH,
		CARDHEIGHT
	};
	SDL_RenderFillRect(renderer, &redCard3);
	SDL_Rect blackCard1
	{
		BLACKCARD1POSX,
		BLACKCARD1POSY,
		CARDWIDTH,
		CARDHEIGHT
	};
	SDL_RenderFillRect(renderer, &blackCard1);
	SDL_Rect blackCard2
	{
		BLACKCARD2POSX,
		BLACKCARD2POSY,
		CARDWIDTH,
		CARDHEIGHT
	};
	SDL_RenderFillRect(renderer, &blackCard2);
	SDL_Rect blackCard3
	{
		BLACKCARD3POSX,
		BLACKCARD3POSY,
		CARDWIDTH,
		CARDHEIGHT
	};
	SDL_RenderFillRect(renderer, &blackCard3);
	SDL_RenderPresent(renderer);
	TTF_Font* font = TTF_OpenFont("comic.ttf", 12);
	if (font == NULL)
	{
		//std::cout << TTF_GetError << std::endl;
	}
	//Writing Black Card Values
	SDL_Color color = {0, 0, 0};
	int textW, textH;
	char* fv;
	SDL_Rect dstrect;
	SDL_Texture* texture;
	SDL_Surface* surface;
	
	surface = TTF_RenderText_Solid(font, "Climb", color);
	texture = SDL_CreateTextureFromSurface(renderer, surface);
	SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
	dstrect = { 5, 5, textW, textH };
	SDL_RenderCopy(renderer, texture, NULL, &dstrect);
	SDL_RenderPresent(renderer);

	//font = TTF_OpenFont("comic.ttf", 12);
	
	if (blackInPlay[0].getTag() != NULL)
	{
		fv = (char*)faceValue(blackInPlay[0].getTag());
		surface = TTF_RenderText_Solid(font, fv, color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
		dstrect = { BLACKCARD1POSX + 5, BLACKCARD1POSY + 5, textW, textH };
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
	}

	
	if (blackInPlay[1].getTag() != NULL)
	{
		fv = (char*)faceValue(blackInPlay[1].getTag());
		surface = TTF_RenderText_Solid(font, fv, color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
		dstrect = { BLACKCARD2POSX + 5, BLACKCARD2POSY + 5, textW, textH };
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
	}

	if (blackInPlay[2].getTag() != NULL)
	{
		fv = (char*)faceValue(blackInPlay[2].getTag());
		surface = TTF_RenderText_Solid(font, fv, color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
		dstrect = { BLACKCARD3POSX + 5, BLACKCARD3POSY + 5, textW, textH };
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
	}

	color = { 200, 0, 0 };

	if (redInPlay[0].getTag() != NULL)
	{
		fv = (char*)faceValue(redInPlay[0].getTag());
		surface = TTF_RenderText_Solid(font, fv, color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
		dstrect = { REDCARD1POSX + 5, REDCARD1POSY + 5, textW, textH };
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
	}


	if (redInPlay[1].getTag() != NULL)
	{
		fv = (char*)faceValue(redInPlay[1].getTag());
		surface = TTF_RenderText_Solid(font, fv, color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
		dstrect = { REDCARD2POSX + 5, REDCARD2POSY + 5, textW, textH };
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
	}

	if (redInPlay[2].getTag() != NULL)
	{
		fv = (char*)faceValue(redInPlay[2].getTag());
		surface = TTF_RenderText_Solid(font, fv, color);
		texture = SDL_CreateTextureFromSurface(renderer, surface);
		SDL_QueryTexture(texture, NULL, NULL, &textW, &textH);
		dstrect = { REDCARD3POSX + 5, REDCARD3POSY + 5, textW, textH };
		SDL_RenderCopy(renderer, texture, NULL, &dstrect);
		SDL_RenderPresent(renderer);
	}

	SDL_SetRenderDrawColor(renderer, goButton.color.r, goButton.color.g, goButton.color.b, goButton.color.a);
	SDL_RenderFillRect(renderer, &goButton.draw_rect);

	//SDL_RenderPresent(renderer);
}



void Game::Draw(){
        for (int i = 0; i < 3; i++){
            if (redInPlay[i] == NULL){
				redInPlay[i] = redDeck->getCard();
			}        }
        for (int j = 0; j < 3; j++){
			if (blackInPlay[j] == NULL){
				blackInPlay[j] = blackDeck->getCard();
			}
        }
}

int Game::Tie(){ // 0 if tie again, 1 if win, 2 if lose
    Card red = redDeck->getCard();
    Card black = blackDeck->getCard();
    if (turn){
        if (red.getValue() == black.getValue()){
            if (red.getTag() > black.getTag()){
                return 1; // win
            }
            else if (red.getTag() < black.getTag()){
                return 2; // loss
            }
            else{
				return 0; //tie
			}
		}
	}
	else{
		if (black.getValue() == red.getValue()){
			if (black.getTag() > red.getTag()) {
				return 1; //win
			}
			else if (black.getTag() < red.getTag()) {
				return 2; //loss
			}
			else {
				return 0; //tie
			}
		}
	}
}



void Game::Turn(){
	// fill selectedArrays
	int result = compare();
	if (result == 2){ // if tie, call Tie()
		int tres;
		do {
			tres = Tie();
		}
		while(tres == 0);
	}
	// after that, remove cards from redInPlay and blackInPlay
	for (std::list<int>::iterator i = selectedRed.begin(); i != selectedRed.end(); ++i){
		redInPlay[*i] = NULL;
	}
	for (std::list<int>::iterator j = selectedBlack.begin(); j != selectedBlack.end(); ++j){
		blackInPlay[*j] = NULL;
	}
	selectedRed.clear();
	selectedBlack.clear();
	bool allNull = true;
	if (redDeck->deckSize() == 0){
		for (int i = 0; i < 3; i++){
			if (redInPlay[i] != NULL){
				allNull = false;
			}
		}
		if (allNull){
			whoWins = true; //red wins
			running = !running;
		}
	}
	else if (blackDeck->deckSize() == 0){
		allNull = true; // might not need to be redeclared
		for (int i = 0; i < 3; i++){
			if (blackInPlay[i] != NULL){
				allNull = false;
			}
		}
		if(allNull){
			whoWins = false;
			running = !running;
		}
	}
	else{
		Draw();
	}
	turn = !turn;
}

int Game::redTotal(){//dont  need an argument, turn is already a class variable
    int sum = 0;
    for (std::list<int>::iterator i = selectedRed.begin(); i != selectedRed.end(); ++i){
        // FIX with iterator for selectedRed/Black list
        sum+=(redInPlay[*i].getValue());
    }
    return sum;
}
int Game::blackTotal(){
    int sum = 0;
    for (std::list<int>::iterator i = selectedBlack.begin(); i != selectedBlack.end(); ++i){
        // FIX with iterator for selectedRed/Black list
        sum+=(blackInPlay[*i].getValue());
    }
    return sum;
}

int Game::compare(){
    if (turn && (redTotal() < blackTotal())){
        // invalid move
        return 0;
    }
    else if (turn && (redTotal() == blackTotal())){
        int r_tag = 0, b_tag = 0;
        for(std::list<int>::iterator i = selectedRed.begin(); i != selectedRed.end(); ++i){
            if (redInPlay[*i].getTag() > r_tag){
                r_tag = redInPlay[*i].getTag();
            }
        }
        for(std::list<int>::iterator i = selectedBlack.begin(); i != selectedBlack.end(); ++i){
            if (blackInPlay[*i].getTag() > b_tag){
                b_tag = blackInPlay[*i].getTag();
            }
        }
        if (r_tag == b_tag){
            return 2; // tie
        }
        else if (r_tag > b_tag){
            return 1; // win
        }
        else{
            return 0; // invalid move
        }
    }

    else if (turn && (redTotal() > blackTotal())){
        return 1; // win
    }
    else if (!turn && (blackTotal() < redTotal())){
        // invalid move
        return 0;
    }
    else if (!turn && (blackTotal() == redTotal())){
        int r_tag = 0, b_tag = 0;
        for(std::list<int>::iterator i = selectedRed.begin(); i != selectedRed.end(); ++i){
            if (redInPlay[*i].getTag() > r_tag){
                r_tag = redInPlay[*i].getTag();
            }
        }
        for(std::list<int>::iterator i = selectedBlack.begin(); i != selectedBlack.end(); ++i){
            if (blackInPlay[*i].getTag() > b_tag){
                b_tag = blackInPlay[*i].getTag();
            }
        }
        if (r_tag == b_tag){
            return 2; // tie
        }
        else if (r_tag < b_tag){
            return 1; // win
        }
        else{
            return 0; // invalid move
        }
    }
    else if (!turn && (blackTotal() > redTotal())){
        return 1; // win
    }
    else{
        return 0; // if any of these cases don't fit (which may be impossible but just in case)
    }
}

void Game::endGame(){
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_Quit();
}
/*
void Game::winMsg() {
	char* win_text;
	SDL_RenderClear(renderer);
	SDL_Rect w_rect;
	w_rect.x = 512;
	w_rect.y = 384;
	w_rect.w = 1024;
	w_rect.h = 768;
	if (whoWins) {
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 200);
		win_text = "Red Wins!";
	}
	else {
		SDL_SetRenderDrawColor(renderer, 255, 255, 255, 200);
		win_text = "Black Wins!";
	}
	SDL_RenderDrawRect(renderer, &w_rect);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 200);
	SDL_RenderPresent(renderer);
	// TODO add text
}
*/