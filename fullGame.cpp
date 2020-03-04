

/*
 *
 * Author: Ben Jurenka
 * Title: Card Game Part 3 with problems fixed.
 * Due: March 4, 2020
 *
 * Description:  Simple card game similar to Gin. Shuffles a "deck" of 52 cards
 * and deals 10 to each player, Players take turns drawing 1 card and discarding 1 card
 * until the remaining cards are gone. Players then enter groups of matching cards (match by number or color).
 * Player with the most cards in groups wins.(score is how many cards not in a group, lowest score wins).
 *
 * Fixed: Indenting of Comments
 *        Displaying 10 cards at scoring. Previously displayed 11 cards with 1 card being a default card.
 *        
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm>
#include <ctime>

using namespace std;

//** class Card **
//creates Card objects
//getters and setters for all private variables

class Card {
private:
    char color; //color of card
    int number = 0; //number of card

public:
            //default constructor

    Card() {
        color = 'N'; //default color
        number = 0;
    }


                //Constructor that sets card's color variable and number variable

    Card(char color, int number) {
        this->color = color;
        this->number = number;
    }

                //returns card color variable
    
    char getColor() {
        return color;
    }
                //returns card number variable

    int getNumber() {
        return number;
    }

                //sets Card color variable

    void setColor(char color) {
        this->color = color;
    }
                 //sets Card number variable

    void setNumber(int number) {
        this->number = number;
    }
                 //displays card color and number

    void displayCard() {
        cout << color << number;
    }  
};

//overload operator ==
//allows comparison of two Card objects 
//returns true if both cards have same color and number

bool operator==(Card leftCard, Card rightCard) {
    return leftCard.getColor() == rightCard.getColor() &&
            leftCard.getNumber() == rightCard.getNumber();
}

bool operator!=(Card leftCard, Card rightCard){
    return leftCard == rightCard;
}

//** class Player **
// Creates a Player object

class Player {
private:
    const int HANDSZ = 11;         //maximum number of cards in a hand
    Card* hand = new Card[HANDSZ]; //stores a players cards
    string name = "";              //name of player
    int score = 10;                //set score to 10 and later subtract number of cards in groups
public:
                    //Default Constructor
                    //set hand to all default cards
    Player() {
        name = "Daisy";
        for (int i = 0; i < HANDSZ; i++) {
            Card card = Card();
            hand[i] = card;
        }
    }
                 //Constructor that sets name variable
                 //set hand to all default cards
    
    Player(string name) {
        this->name = name;
        for (int i = 0; i < HANDSZ; i++) {
            Card card = Card();
            hand[i] = card;
        }
    }

    string getName() {
        return name;
    }

                //set a players name

    void setName(string name) {
        this->name = name;
    }
                //set a players score

    void setScore(int score) {
        this->score = score;
    }
                //return a players score

    int getScore() {
        return score;
    }

                //returns the players hand

    Card* getHand() {
        Card* handPtr = hand;
        return handPtr;
    }


                //adds a card to players hand

    void addCard(Card card) {
        for (int i = 0; i < HANDSZ; i++) {
            if (hand[i].getColor() == 'N') {   //if card is default then replace it
                hand[i] = card;
                break;
            }
        }
    }

                //removes a card from players hand

    void removeCard(int indexOfCard) {
        Card card = Card();
        hand[indexOfCard - 1] = card;        //user input will be one more than actual index of card
    }

             //displays hand of player

    void showHand(int numCards) {
        Card* hand = this->getHand();
        for (int i = 0; i < numCards; i++) {
                cout << "(" << (i + 1) << " = ";
                hand[i].displayCard();
                cout << ") ";
        }
        cout << endl;
    }
};

Player* getPlayers();       //creates an array of Player objects
Card* createCards();        //creates an array of Card objects (in order)
void shuffle(Card* deck);       //shuffles an array of Card objects
void deal(Card* deck, Player* players);       //sets each players hand to non-default card objects
void play(Card* deck, Player* players);       //draws a card for the player, displays hand, then asks for card to discard
void chooseDiscard(int indexOfCard, Player player);       //makes sure card to discard is valid, then sets card to default (N0)
Card draw(int turn, Card* deck);            //returns a card from the left over cards from deal
void score(Player& player);             //scores each players hand and returns an int score
vector<int> scoreGroup(Card* hand, vector<int> cardIndexes);             //scores a group of card objects
int isValidGroup(vector<Card> group);           //checks if a group of cards is valid, returns 0 if invalid
void winner(Player* players);            //display winner 
int validInt();                  // makes sure input from user is an int.
void printDeck(Card* deck);             //tester function to make sure shuffle and deal are working
void removeEmptySlot(Card* hand);        // removes the default card in a players hand

int main() {
    const int NUM_PLAYERS = 4;

    srand(time(NULL));          

    Player* players = getPlayers(); 

    Card* deck = createCards(); 

    cout << "Shuffling the deck..." << endl;
    
    shuffle(deck);         
    
    cout << "Dealing 10 cards to each player..." << endl;
    
    deal(deck, players); 
    

    play(deck, players);                            //play through 12 rounds

    for (int i = 0; i < NUM_PLAYERS; i++) {         //score players hands
        score(players[i]);                          //changes the players score variable
    }
    winner(players);                                //display winner

    system("pause");
}

//** validInt function function **
//makes sure that user input is an integer
//loops until user does enter an integer
//returns that integer

int validInt() {
    int valid;
    cin >> valid;
    while (1) {
        if (cin.fail()) {           //cin fails if input in not an integer
            cin.clear();            //clear error state of buffer
            cin.ignore(numeric_limits<streamsize>::max(), '\n');            //ignores the rest of input after the error, returns 1 if input tried but was at end of file
            cout << "Enter an integer only." << endl;           //ask for valid input
            cin >> valid;
        }
        if (!cin.fail())             //cin did not fail
            return valid;            //return the number
    }
}

// ** score function **
//Score a players hand by asking user for number of groups,
//then scores each group by calling scoreGroup function.
//Parameter Variable: player - player object to score

void score(Player& player) {
    vector<int> cardIndexes;
    int score = 10;
    Card* hand = player.getHand();       //get the players hand

    cout << "\n" << player.getName() << "'s hand:";
    removeEmptySlot(hand);
    player.showHand(10);
    cout << player.getName() << " enter the number of groups you have: ";

    int numberOfGroups = validInt();        //makes sure numberOfGroups is an integer

    for (int i = 0; i < numberOfGroups; i++) {      //loops once for each group

        vector<int> indexesFromGroup = scoreGroup(hand, cardIndexes);

        int groupScore = indexesFromGroup.at(0);
        for (int j = 1; j < indexesFromGroup.size(); j++) {
            cardIndexes.push_back(indexesFromGroup.at(j));
        }
        score -= groupScore;            //set score of player's score variable
        player.setScore(score);
    }

    cout << player.getName() << "'s score is now " << player.getScore() << endl;
}


// ** scoreGroup function **
//Return number of cards in a valid group by getting the group of cards from user input.
//User inputs the index of the card they wish to add to the group.
//Make sure the group is valid by calling isValidGroup() function.
//if the group is not valid then call scoreGroup(hand) again.
//Parameter Variables:  hand - allows function to place cards from player's hand into a group
                        
vector<int> scoreGroup(Card* hand, vector<int> cardIndexes) {
    vector<Card> group;                     //vector to store cards of a group
    vector<int> indexesAndScore = {0};      //store card indexes to check for duplicate cards, score will be stored in first index
    int indexOfCard = -1;                   //store the user input, this variable will change

    cout << "Enter index of cards(1-10) for group. \n"
            << "Enter '0' when finished with group: " << endl;

    while (cin >> indexOfCard && indexOfCard != 0) {        // get valid int from user
        if (indexOfCard > 0 && indexOfCard < 12) {          //if index is valid, continue
            group.push_back(hand[indexOfCard - 1]);         // push card into vector
            indexesAndScore.push_back(indexOfCard);
        } else {
            cout << "Index not valid. Choose card by index(1-11), No Duplicate cards allowed.\n"
                    << "enter index of cards(1 - 10)\n";
            indexOfCard = validInt();               // user input was invalid, do not put card into group, ask for new input from user
        }
    }

    cout << endl;

    if (isValidGroup(group) == 0) {             //need to check if group is valid
        cout << "invalid group entered(all colors or all numbers must match,"
                << " group must be atleast 3 cards, no duplicate cards)" << endl;
        scoreGroup(hand, cardIndexes);          //try again until user enters valid group
    }
    indexesAndScore.at(0) = group.size();           //set IndexesAndScore first index to the group size(number to
                                                    //substract from score) 
    return indexesAndScore;             //group is valid return the size of group
}

// ** isValidGroup function **
//check if group is valid return 1 if valid, 0 if invalid
//Parameter Variables: group - a vector of Card objects

int isValidGroup(vector<Card> group) {
    int valid = 1;           //variable that will be returned

    if (group.size() < 3) {
        cout << "Group must have atleast 3 cards\n";
        return 0;           // group does not have 3 cards;
    }

    for (int i = 0; i < group.size() - 1; i++) {

        if (group.at(i) == group.at(i + 1)) {           //user input the same card twice
            cout << "Input same card twice\n";
            return 0; 
        }

        if (group.at(i).getColor() != 'w' && group.at(i + 1).getColor() != 'w'
                && group.at(i).getColor() != group.at(i + 1).getColor()) {      //if it gets past this if statement then it is not grouped by color 
            valid = 0;                                  //set valid to 0 because at this point it is not a valid group
        }                                               //but need to continue checking
    }

    if (valid == 1) {
        return valid;                           //it is a valid group, all the colors were the same. no need to check numbers
    }
                                                //check numbers for valid group because colors are not the same.
    for (int i = 0; i < group.size() - 1; i++) {
        if (group.at(i).getColor() != 'w' && group.at(i + 1).getColor() != 'w'
                && group.at(i).getNumber() != group.at(i + 1).getNumber()) {            //it is not a valid group
            return 0;
        }
    }
    valid = 1;                      //it is valid, set valid back to 1
    return valid;
}

// ** getPlayers functions **
//get names from users and sets the name of each player
//Returns an array of 4 player objects

Player* getPlayers() {
    Player* players = new Player[4];
    for (int i = 0; i < 4; i++) {
        cout << "Enter player " << i + 1 << "'s name: ";
        string name;
        getline(cin, name);
        players[i].setName(name);
    }
    return players;
}
// ** printDeck function **
// prints deck (for testing purposes)
//Parameter Variable: deck - pointer to array of card objects(deck to be printed)

void printDeck(Card* deck) {
    const int MAX = 52;
    for (int i = 0; i != MAX; i++) {
        deck[i].displayCard();
        cout << " ";
        if ((i + 1) % 10 == 0) {
            cout << "\n";
        }
    }
}

//** createCards function **
//allocates and constructs 52 Card objects
//numbered 1 - 10 and "color" r,b,g,p, and o.

Card* createCards() {
    const int MAX = 52;
    Card* deck = new Card[MAX];

    for (int i = 0; i != MAX; i++) {
        if (i < 10) {
            Card card = Card('r', i + 1);
            deck[i] = card;
        }
        if (i >= 10 && i < 20) {
            Card card = Card('b', (i % 10) + 1);
            deck[i] = card;
        }
        if (i >= 20 && i < 30) {
            Card card = Card('g', (i % 10) + 1);
            deck[i] = card;
        }
        if (i >= 30 && i < 40) {
            Card card = Card('p', (i % 10) + 1);
            deck[i] = card;
        }
        if (i >= 40 && i < 50) {
            Card card = Card('o', (i % 10) + 1);
            deck[i] = card;
        }
        if (i >= 50) {
            Card card = Card('w', i + 1);
            deck[i] = card;
        }
    }
    return deck;
}

// ** shuffle function **
//shuffles the cards, does not need to return anything because
//it will shuffle the deck at the memory location
//Parameter Variable: deck - pointer to an array of Card objects (deck).

void shuffle(Card* deck) {
    const int MAX = 52;
    Card shuffledDeck[MAX];             //fill tempDeck with default Cards
    for (int i = 0; i < MAX; i++) {
        Card card = Card();
        shuffledDeck[i] = card;
    }
    for (int i = 0; i < MAX; i++) {             //place cards from ordered deck into shuffledDeck randomly
        int random = rand() % MAX;

        while (shuffledDeck[random].getNumber() != 0) {          //if the spot in shuffledDeck is already filled try another spot by
            random = rand() % MAX;                                  //getting a new random number
        }

        if (shuffledDeck[random].getNumber() == 0) {                //if the spot in shuffledDeck is "empty" set it to deck[i]
            shuffledDeck[random] = deck[i];
        }
    }

    for (int i = 0; i != MAX; i++) {            //shuffledDeck should be filled at this point
        deck[i] = shuffledDeck[i];              //set deck to shuffledDeck
    }                                           //deck is now shuffled
}

// *** deal function ***
//set each players hand using setters of Player class
//10 cards each
//Left over cards will be the "Draw Pile" ( deck[40] through deck[52] )
//Parameter Variable: deck - pointer to array of Card Objects
//                  : players - pointer to array of Player Objects

void deal(Card* deck, Player* players) {
    const int PLAYERCARDS = 40;
    for (int i = 0; i < PLAYERCARDS; i++) {
        players[i % 4].addCard(deck[i]);
    }
}

// *** draw function ***
//Uses the remaining 12 cards returns correct card based on turn number.
//Parameter Variables: turn - integer number of what turn it is. (Will use the turn number from the game loop in main())
//                     deck - pointer to card array that is the same as the Card array used in deal funtion

Card draw(int turn, Card* deck) {
    int drawIndex = turn + 40;
    Card card = deck[drawIndex];
    return card;
}

//*** chooseDiscard function ***
//takes input from player to decide what card to discard
//Parameter Variables: player - will allow function to change players hand and to know what player to use.
//                     indexOfCard - will allow function to know what card needs to be removed

void chooseDiscard(int indexOfCard, Player player) {
        player.removeCard(indexOfCard);
}

//*** play function ***
//goes through 12 rounds in which each player draws and chooses a card to discard
//Parameter Variables: deck - the shuffled array of Card objects
//                     players - array of  player Objects

void play(Card* deck, Player* players) {
    
    cout << "\nDrawn cards are added to players hand automatically. \n"
    << "It will appear that players have 11 cards but 1 must be discarded every turn, leaving them with 10 cards." << endl;
    
    const int DRAWSZ = 12;
    for (int i = 0; i < DRAWSZ; i++) {
        cout << "\n\n" << players[i % 4].getName() << " draws ";
        draw(i, deck).displayCard();
        players[i % 4].addCard(draw(i, deck));

        cout << "\n" << players[i % 4].getName() << "s hand: ";
        players[i % 4].showHand(11);

        cout << players[i % 4].getName() << " choose a card to discard(1-11): ";

        int indexOfCard = validInt();

        
        
        while (indexOfCard == 0 || indexOfCard < 1 || indexOfCard > 11) {
            cout << " choose valid card \n";
            indexOfCard = validInt();
        }
           
            chooseDiscard(indexOfCard, players[i % 4]);
        
    }
    cout << "Rounds are over, Time to score our hands\n";
}


// ** winner function **
//prints the winner of the game by checking each players score and sorting the
//array of Player objects.
//Also prints if there is a tied score
//Parameter Variables: players - pointer to array of Player objects

void winner(Player* players) {
    const int NUM_PLAYERS = 4;
    int indexOfWinner = 0;
    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (players[indexOfWinner].getScore() > players[i].getScore()) {
            indexOfWinner = i;
        }
    }

    cout << "\nWinner is " << players[indexOfWinner].getName()
            << ", with a score of " << players[indexOfWinner].getScore() << "!" << endl;

    for (int i = 0; i < NUM_PLAYERS; i++) {
        if (i != indexOfWinner && players[i].getScore() == players[indexOfWinner].getScore()) {
            cout << "Tie! Winner is also " << players[i].getName()
                    << ", with a score of " << players[indexOfWinner].getScore() << "!" << endl;
        }
    }
}
// ** removeEmptySlot function **
// removes the default card in a players hand so that it will only display 10 cards
// during scoring phase of game.
// Parameter Variable: hand - hand of player
void removeEmptySlot(Card* hand){
    const int HAND_SZ = 11;
    for(int i = 0; i < HAND_SZ; i++){
        if(hand[i].getNumber() == 0){
            Card temp = Card(hand[10].getColor(),hand[10].getNumber());
            hand[10].setColor(hand[i].getColor());
            hand[10].setNumber(hand[i].getNumber());
            hand[i] = temp;
        }        
    }
}

//Problems : Validating user input for when they are entering cards for each group.
//           It does work if the user enters what they are suppose to.

