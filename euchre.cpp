//
//  Euchre.cpp
//  p3-euchre
//
//  Created by Alec Leal on 07/02/23.
//
//  Project UID 1d9f47bfc76643019cfbf037641defe1

#include <iostream>
#include <fstream>
#include "Player.h"
#include "Pack.h"
#include "Card.h"
#include <cassert>

using namespace std;

class Game {
public:

    Game(bool shuffle_in, int points_in, vector<Player*> players_in, Pack pack_in) {

        shuffle_toggle = shuffle_in;
        points_to_win = points_in;
        players = players_in;
        pack = pack_in;
    }

    void play();

private:

    bool shuffle_toggle;
    int points_to_win;
    vector<Player*> players;
    Pack pack;


    void shuffle() {
        if (shuffle_toggle)
        {
            pack.shuffle();
        }
        else
        {
            pack.reset();
        }
    }

    void deal(int dealerIndex)
    {
        deal(players[(dealerIndex + 1) % 4], 3);
        deal(players[(dealerIndex + 2) % 4], 2);
        deal(players[(dealerIndex + 3) % 4], 3);
        deal(players[dealerIndex], 2);

        deal(players[(dealerIndex + 1) % 4], 2);
        deal(players[(dealerIndex + 2) % 4], 3);
        deal(players[(dealerIndex + 3) % 4], 2);
        deal(players[dealerIndex], 3);
    }

    void deal(Player* player, int numCards)
    {
        for (int i = 0; i < numCards; i++)
        {
            player->add_card(pack.deal_one());
        }
    }

    int make_trump(Card upcard, int dealerIndex, Suit& trump)
    {
        
        for (int round : { 1, 2})
        {
            for (int i = 1; i < 5; ++i)
            {
                int playerIndex = (dealerIndex + i) % 4;
                
                bool orderedUp = players[playerIndex]->make_trump(upcard, 
                                playerIndex == dealerIndex,
                                  round, trump);

                if (orderedUp)
                {
                    cout << *players[playerIndex] << " orders up " << trump
                        << endl;
                    if (round == 1)
                    {
                        players[dealerIndex]->add_and_discard(upcard);                  
                    }  
                    cout << endl;
                    return playerIndex;
                }
                cout << *players[playerIndex] << " passes" << endl;
            }
        }
        return 0;
    }

    array<int, 2> play_hand(int dealerIndex, Suit trump, int orderedUpIndex)
    {
        int team1tricks = 0;
        int team2tricks = 0;
        int leaderIndex = (dealerIndex + 1) % 4;
        for (int i = 0; i < 5; ++i)
        {
           Card ledCard = players[leaderIndex]->lead_card(trump);
           cout << ledCard << " led by " << *players[leaderIndex] << endl;
           Card bestCard = ledCard;
           int takerIndex = leaderIndex;
           for (int j = 1; j < 4; ++j)
           {
               int playerIndex = (leaderIndex + j) % 4;
               Card playedCard = players[playerIndex]->play_card(ledCard, trump);
               cout << playedCard << " played by " << *players[playerIndex] << endl;
               if ( Card_less(bestCard, playedCard, ledCard, trump) )
               {
                   bestCard = playedCard;
                   takerIndex = playerIndex;
               }
           }
           cout << *players[takerIndex] << " takes the trick" << endl << endl;
           if (takerIndex == 0 || takerIndex == 2)
           {
               team1tricks++;
           }
           else
           {
               team2tricks++;
           }
           leaderIndex = takerIndex;
        }
        if (team1tricks > team2tricks )
        {
            cout << *players[0] << " and " << *players[2] << " win the hand" << endl;
            if (orderedUpIndex == 1 || orderedUpIndex == 3)
            {
                cout << "euchred!" << endl;
                return { 2,0 };
            }
            if (team1tricks == 5)
            {
                cout << "march!" << endl;
                return { 2, 0 };
            }
            return { 1,0 };

        }
        else
        {
            cout << *players[1] << " and " << *players[3] << " win the hand" << endl;
            if (orderedUpIndex == 0 || orderedUpIndex == 2)
            {
                cout << "euchred!" << endl;
                return { 0,2 };
            }
            if (team2tricks == 5)
            {
                cout << "march!" << endl;
                return { 0, 2 };
            }
            return { 0,1 };
        }
    }

};


// this is where the game is actually played
void Game::play() {

    int points1 = 0;
    int points2 = 0;
    int hand = 0;
    while (points1 < points_to_win && points2 < points_to_win)
    {
        cout << "Hand " << hand << endl;
        int dealerIndex = hand % 4;
        cout << *players[dealerIndex] << " deals" << endl;
        shuffle();
        deal(dealerIndex);
        Card upcard = pack.deal_one();
        cout << upcard << " turned up" << endl;
        Suit trump;
        int orderedUpIndex = make_trump(upcard, dealerIndex, trump);
        array<int,2> points = play_hand(dealerIndex, trump, orderedUpIndex);
        
        points1 += points[0];
        points2 += points[1];

        cout << *players[0] << " and " << *players[2] << " have " << points1
            << " points" << endl;
        cout << *players[1] << " and " << *players[3] << " have " << points2 
            << " points" << endl;
        cout << endl;

        hand++;
    }

    if (points1 > points2)
    {
        cout << *players[0] << " and " << *players[2] << " win!" << endl;
    }
    else
    {
        cout << *players[1] << " and " << *players[3] << " win!" << endl;
    }
 
}

int main(int argc, const char* argv[]) {

    for (int i = 0; i < argc; ++i) {
        cout << argv[i] << " ";
    }
    cout << endl;

    // check correct number of args
    if (argc != 12) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }

    string filename = argv[1];
    string shuffle_in = argv[2];
    int points = stoi(argv[3]);

    string names[4] = { argv[4], argv[6], argv[8], argv[10] };
    string types[4] = { argv[5], argv[7], argv[9], argv[11] };

    // check for points and shuffle
    if ((points < 1 || points > 100) ||
        (shuffle_in != "shuffle" && shuffle_in != "noshuffle")) {
        cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
            << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
            << "NAME4 TYPE4" << endl;
        return 1;
    }

    // check for correct player types
    for (int i = 0; i < 4; ++i) {
        if (types[i] != "Human" && types[i] != "Simple") {
            cout << "Usage: euchre.exe PACK_FILENAME [shuffle|noshuffle] "
                << "POINTS_TO_WIN NAME1 TYPE1 NAME2 TYPE2 NAME3 TYPE3 "
                << "NAME4 TYPE4" << endl;
            return 1;
        }
    }

    // read in the pack
    ifstream fin;
    fin.open(filename);
    if (!fin.is_open()) {
        cout << "Error opening " << filename << endl;
        return 1;
    }
    Pack pack(fin);

    // shuffle is true if we are allowed to shuffle, false otherwise
    bool shuffle = (shuffle_in == "shuffle");

    vector<Player*> players;
    for (int i = 0; i < 4; ++i) {
        players.push_back(Player_factory(names[i], types[i]));
    }

    Game game(shuffle, points, players, pack);
    game.play();


    // this needs to stay at the end of this function
    for (size_t i = 0; i < players.size(); ++i) {
        delete players[i];
    }

    return 0;
}