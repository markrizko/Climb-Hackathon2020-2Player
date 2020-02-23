#include "Card.h"
#include <iostream>
/*
Card::~Card(){
    value = NULL;
    cardTag = NULL;
}
*/
Card::Card(){
	cardTag = NULL;
	value = NULL;
	//std::cout << "Uh Oh..." << std::endl;
}

Card::Card(const Card& source) {
	cardTag = source.cardTag;
	value = source.value;
}

Card::Card(int tag){
	//std::cout << tag << std::endl << ++counter << std::endl;
    cardTag = tag;
    if (tag >= 2 && tag <= 10){
        value = tag;
    }
    else if(tag == 1){
        value = 11;
    }
    else if(tag >= 11 && tag <= 13){
        value = 10;
    }
    else{
        //std::cout << "Invalid Tag!" << std::endl;
        //exit(1);
    }
}

int Card::getValue(){
    return value;
}

int Card::getTag(){
    return cardTag;
}

bool Card::isAce(){
    if (cardTag == 1){
        return true;
    }
    return false;
}

Deck::Deck(){
	//cDeck.clear();
    cardsLeft = 0;
}

void Deck::FillDeck(){
    int i; 
    for (i = 1; i < 14; i++){
        Card *in = new Card(i);
        cDeck.push_back(*in);
        cDeck.push_back(*in);
    }
    cardsLeft = 26;
    ShuffleDeck();
}

Deck::Deck(const Deck& source) {
	cDeck = source.cDeck;
	cardsLeft = source.cardsLeft;
}

void Deck::ShuffleDeck(){
    assert(cardsLeft > 0 && cardsLeft <= 26);
    random_shuffle(cDeck.begin(), cDeck.end());
}

Card Deck::getCard(){
    if (cardsLeft == 0){
        std::cout << "No cards left" << std::endl;
        return NULL;
    }
    else{
		auto it = cDeck.begin();
		Card card = *it;
		cDeck.erase(it);
		cardsLeft--;
        return card;
    }
}

int Deck::deckSize(){
    return cardsLeft;
}

bool operator==(const Card& c1, const Card& c2){ //this should work for NULL checks
    if (c1.cardTag == c2.cardTag){ // just wanted to see if that would change anything since operator is declared as Card::operator== 
        return true;
    }
    return false;
}


/*
void Deck::operator=(const Deck& source){
    copy(source.cDeck.begin(), source.cDeck.end(), cDeck);
    cardsLeft = source.cardsLeft;
}
*/
bool operator!=(const Card& c1, const Card& c2){
    return !(c1 == c2);
}

Card Card::operator=(const Card& source){
    if (source == NULL){ 
        value = NULL;
        cardTag = NULL;
        return *this;
    }
    else if (this == nullptr){
        return NULL;
    }
    else{
        //std::cout << source.value << std::endl;
        value = source.value; //this causes a memory access exception
        cardTag = source.cardTag;
        return *this;
    }

}

Deck Deck::operator= (const Deck& source)
{
	//copy(source.cDeck.begin(), source.cDeck.end(), cDeck);
	cDeck = source.cDeck;
	cardsLeft = source.cardsLeft;
	return *this;
}

