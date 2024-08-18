#include <iostream>
#include <vector>
#include <map>
#include "util.h"

using namespace std;

/*
there r special symbol rules for a low hand
it inverts the symbol values LOW_ACE to 5 is better than 4-8

lows consists of a hand with card values 8 or less
and straights and fluhes do not count within a low

BUT THIS IS TEXAS HOLD 'EM POKER SO THESE RULES DONT EXIST!

you can only raise twice per turn

low to high
ace CAN BE low or high, just not at the same time

* Basic Poker Rules
* 
* Fold, Bet, Check
* 
* Turns
* 
* Money on initial
* 
* Seperate pots
* 
* ALL IN!!
* 
* STANDARD 52 CARD DECK (no joker cards)


*/

// ENUMS
enum PokerHand{
    NONE, // = 0 
    ONE_PAIR,
    TWO_PAIR,
    THREE_OAK,
    STRAIGHT,
    FLUSH,
    FULL_HOUSE,
    FOUR_OAK,
    STRAIGHT_FLUSH,
    ROYAL_FLUSH // = 10
    // top to bottom is lowest to highest hands
}; // poker hand rankings
enum Symbol{
    LOW_ACE,
    TWO,
    THREE,
    FOUR,
    FIVE,
    SIX,
    SEVEN,
    EIGHT,
    NINE,
    TEN,
    JACK,
    QUEEN,
    KING,
    HIGH_ACE
}; // enums for symbol values and flushes
enum Action{
    FOLD,
    CHECK,
    BET,
    CALL,
    RAISE
}; // enum for player moves
enum Type{
    USER,
    BOT
}; // decision types for action

// STRUCTS
struct card{
    string name;
    string suit;
    string symbol;
    enum Symbol value;
};
struct player{
    pair<card, card> cards;
    string name;
    int chips; // in money value $$$
    enum PokerHand hand;
    enum Action move;
    enum Type type;
};

// FUNCTION HEADERS
vector<card> generateDeck();
void copyDeck(vector<card> &deck, vector<card> &copy);
void printDeck(vector<card> &deck);
void shuffleDeck(vector<card> &deck);
void printPlayerInfo(player* &user);
void printPlayers(map<string, player*> &players);
void giveCards(map<string, player*> &players, vector<card> &deck);
void addPlayer(string name, enum Type type, map<string, player*> &players);

// CONSTANTS
vector<card> STANDARD_DECK {};
const vector<string> SUITS {
    "\u2666","\u2664","\u2665","\u2667" // DIAMOND SPADES HEART CLUBS             
}; 
const vector<pair<string, Symbol> > SYMBOLS {
    {"2", TWO}, {"3", THREE}, {"4", FOUR}, {"5", FIVE},
    {"6", SIX}, {"7", SEVEN}, {"8", EIGHT}, {"9", NINE},
    {"10", TEN}, {"J", JACK}, {"Q", QUEEN},{"K", KING},
    {"A", HIGH_ACE}
};
const int INIT_MONEY = 50; // $50
const int MAX_PLAYERS = 23;
const int MIN_PLAYERS = 5;

// FUNCTIONS
vector<card> generateDeck(){

    vector<card> newDeck {};

    for (string suit : SUITS) {
        for (pair<string, Symbol> symbol : SYMBOLS) {
            
            card newCard {};
            newCard.name = symbol.first + suit;
            newCard.suit = suit;
            newCard.symbol = symbol.first;
            newCard.value = symbol.second;
            //cout << "ran" << endl;
            //cout << newCard.name << endl;
            
            newDeck.push_back(newCard);
        }
    }
    
    
    return newDeck;
}
void copyDeck(vector<card> &og, vector<card> &copy){
    
    for (card c : og) {
        copy.push_back(c);    
    }

}
void printDeck(vector<card> &deck){
    for (int card = 0; card < deck.size(); card++) {
        cout << deck[card].name << endl;    
    }
}
void shuffleDeck(vector<card> &deck){
    
    for (int i = 0; i < deck.size(); i++) {
        
        int newIndex = rand() % deck.size();
        // swap current and new
        card oldCard {};
        oldCard = deck[i];
        deck[i] = deck[newIndex];
        deck[newIndex] = oldCard;
    }
    
}
void printPlayerInfo(player* &user){
    cout << "-=" << user->name << "=-\n";
    cout << user->cards.first.name;
    cout << " " << user->cards.second.name;
    cout << " $" << user->chips << endl;
}
void printPlayers(map<string, player*> &players){
    for (map<string, player*>::iterator itr = players.begin(); itr != players.end(); itr++){
        printPlayerInfo(itr->second);
    }
}
void giveCards(map<string, player*> &players, vector<card> &deck){
    for (map<string, player*>::iterator itr = players.begin(); itr != players.end(); itr++){
        pair<card, card> newHand;
        newHand.first = deck.back();
        deck.pop_back();
        newHand.second = deck.back();
        deck.pop_back();
        itr->second->cards = newHand;
    }
}
void addPlayer(string name, enum Type type, map<string, player*> &players){
    player *p = new player;
    p->cards = {};
    p->name = name;
    p->chips = INIT_MONEY;
    p->hand = NONE;
    p->move = FOLD;
    p->type = type;
    
    players.insert(pair<string, player*>(name, p));
}
void addBots(map<string, player*> &players){
    cout << "Warning! Duplicate names will not be included.\n";
    while (true){
        string name = readLine("Name: ");

        if (name == "-1" && players.size() >= MIN_PLAYERS) return;
        else if (name == "-1" && players.size() < MIN_PLAYERS){
            cout << "Not enough players. Please add more.\n";
            continue;
        }
    
        addPlayer(name, BOT, players);
        
        if (players.size() >= MAX_PLAYERS) {
            cout << "No more players can be added. Automatically starting game.\n";
            return;
        }
    }
}
void play(map<string, player*> &players){
    
}

// MAIN METHOD
int main()
{    
    // RANDOMIZATION
    srand(time(0));
    
    
    // INITIALIZATION 
    vector<card> game_deck {};
    map<string, player*> victims {};
    vector<pair<int, player*> > pots {};
    STANDARD_DECK = generateDeck();
    copyDeck(STANDARD_DECK, game_deck);
    shuffleDeck(game_deck);
    printDeck(game_deck);

    
    // USER
    cout << "Let's Play Poker! By yourself. Against my bots." << endl;
    string name = readLine("User's Name: ");
    addPlayer(name, USER, victims);
    
    
    // OTHER PLAYERS
    addBots(victims);
    
    
    // PLAY POKER
    while (true){
        
        giveCards(victims, game_deck);
        printPlayers(victims);
        cout << "--== Pre-Flop ==--" << endl;
        
        
        
        break;
    }
    

    return 0;
    
}