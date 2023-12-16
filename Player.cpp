// Project UID 1d9f47bfc76643019cfbf037641defe1

//
//  Player.cpp
//  p3-euchre
//
//  Created by Alec Leal on 12/02/23.
//

#include <stdio.h>
#include "Player.h"
#include <string>
#include <algorithm>
#include <cassert>
using namespace std;

class SimplePlayer : public Player {
public:

    const string& get_name() const override {
        return name;
    }

    void add_card(const Card& c) override {
        hand.push_back(c);
    }

    bool make_trump(const Card& upcard, bool is_dealer,
        int round, Suit& order_up_suit) const override {
        assert(round == 1 || round == 2);
        if (round == 1) {
            // They will order up if that would mean they have two or more cards
            // that are either face or ace cards of the trump suit

            size_t face_ace_trumps = 0;
            Suit trump = upcard.get_suit();

            for (size_t i = 0; i < hand.size(); ++i) {
                if (hand[i].is_trump(trump) && hand[i].is_face_or_ace()) {
                    ++face_ace_trumps;
                }
            }

            if (face_ace_trumps >= 2) {
                order_up_suit = trump;
                return true;
            }
            else {
                return false;
            }
        }
        else {
            // a Simple Player considers ordering up the suit with the same color
            // as the upcard, which would make that suit trump
            // They will order up if that would mean they have one or more cards
            // that are either face or ace cards of the trump suit in their hand

            size_t face_ace_trumps = 0;
            Suit trump = Suit_next(upcard.get_suit());

            for (size_t i = 0; i < hand.size(); ++i) {
                if (hand[i].is_trump(trump) && hand[i].is_face_or_ace()) {
                    ++face_ace_trumps;
                }
            }

            if (face_ace_trumps >= 1 || is_dealer) {
                order_up_suit = trump;
                return true;
            }
            else {
                return false;
            }
        }

        return false;
    }

    void add_and_discard(const Card& upcard) override {
        add_card(upcard);

        Suit trump = upcard.get_suit();
        // find lowest card
        size_t lowest_index = 0;

        for (size_t i = 0; i < hand.size(); ++i) {
            if (Card_less(hand[i], hand[lowest_index], trump)) {
                lowest_index = i;
            }
        }
        // discard lowest
        hand.erase(hand.begin() + lowest_index);
    }

    Card lead_card(Suit trump) override {
        // When a Simple Player leads a trick, they play the highest
        // non-trump card in their hand. If they have only trump cards,
        // they play the highest trump card in their hand.

        // only trump cards?
        size_t trumps = 0;

        for (size_t i = 0; i < hand.size(); ++i) {
            if (hand[i].is_trump(trump)) {
                ++trumps;
            }
        }

        Card highest = hand[0];
        size_t highest_index = 0;

        if (trumps == hand.size()) {
            // only trump cards: play the highest trump

            for (size_t i = 0; i < hand.size(); ++i) {
                if (Card_less(highest, hand[i], trump)) {
                    highest = hand[i];
                    highest_index = i;
                }
            }
        }
        else {
            // play the highest non-trump card

            for (size_t i = 0; i < hand.size(); ++i) {
                if (highest.is_trump(trump))
                {
                    highest = hand[i];
                    highest_index = i;
                }
                else if (Card_less(highest, hand[i], trump) &&
                    !hand[i].is_trump(trump)) {
                    highest = hand[i];
                    highest_index = i;
                }
            }
        }
        // discard highest
        hand.erase(hand.begin() + highest_index);
        return highest;
    }

    Card play_card(const Card& led_card, Suit trump) override {
        // If a Simple Player can follow suit, they play the highest card
        // that follows suit. Otherwise, they play the lowest card in their hand


        Suit led_suit = led_card.get_suit(trump);

        bool can_follow_suit = false;
        for (size_t i = 0; i < hand.size(); ++i) {
            if (hand[i].get_suit(trump) == led_suit) {
                can_follow_suit = true;
                break;
            }
        }

        if (can_follow_suit) {
            // find highest card in suit
            Card highest = hand[0];
            size_t highest_index = 0;

            for (size_t i = 0; i < hand.size(); ++i) {
                if (highest.get_suit(trump) != led_suit)
                {
                    highest = hand[i];
                    highest_index = i;
                }
                else if (Card_less(highest, hand[i], trump) && 
                            hand[i].get_suit(trump) == led_suit) {
                    highest = hand[i];
                    highest_index = i;
                }
            }

            // discard highest
            hand.erase(hand.begin() + highest_index);
            return highest;
        }
        else {
            // find lowest card
            Card lowest = hand[0];
            size_t lowest_index = 0;

            for (size_t i = 0; i < hand.size(); ++i) {
                if (Card_less(hand[i], lowest, trump)) {
                    lowest = hand[i];
                    lowest_index = i;
                }
            }

            // discard lowest
            hand.erase(hand.begin() + lowest_index);
            return lowest;
        }
    }

    SimplePlayer(string name_in) {
        name = name_in;
    }

private:
    string name;

    vector<Card> hand;
};

class HumanPlayer : public Player {
public:

    const string& get_name() const override {
        return name;
    }

    void add_card(const Card& c) override {
        hand.push_back(c);
        sort(hand.begin(), hand.end());
    }

    bool make_trump(const Card& upcard, bool is_dealer,
        int round, Suit& order_up_suit) const override {
        assert(round == 1 || round == 2);

        print_hand();
        cout << "Human player " << name
            << ", please enter a suit, or \"pass\":" << endl;

        string decision;
        cin >> decision;

        if (decision != "pass") {     
            order_up_suit = string_to_suit(decision);
            return true;
        }
        return false;
    }

    void add_and_discard(const Card& upcard) override {
        print_hand();
        cout << "Discard upcard: [-1]\n";
        cout << "Human player " << name << ", please select a card to discard:\n";

        int decision;
        cin >> decision;

        if (decision != -1) {
            hand[decision] = upcard;
        }

        sort(hand.begin(), hand.end());
    }

    Card lead_card(Suit trump) override {
        print_hand();
        cout << "Human player " << name << ", please select a card:\n";

        int decision;
        cin >> decision;

        Card lead = hand[decision];

        hand.erase(hand.begin() + decision);

        sort(hand.begin(), hand.end());

        return lead;
    }

    Card play_card(const Card& led_card, Suit trump) override {
        return lead_card(trump);
    }

    HumanPlayer(string name_in) {
        name = name_in;
    }

private:
    string name;

    vector<Card> hand;

    void print_hand() const {
        for (size_t i = 0; i < hand.size(); ++i)
            cout << "Human player " << name << "'s hand: "
            << "[" << i << "] " << hand[i] << "\n";
    }
};

Player* Player_factory(const string& name, const string& strategy) {
    // We need to check the value of strategy and return
    // the corresponding player type.
    if (strategy == "Simple") {
        // The "new" keyword dynamically allocates an object.
        return new SimplePlayer(name);
    }
    if (strategy == "Human") {
        return new HumanPlayer(name);
    }

    // Invalid strategy if we get here
    assert(false);
    return nullptr;
}

ostream& operator<<(ostream& os, const Player& p) {
    os << p.get_name();
    return os;
}