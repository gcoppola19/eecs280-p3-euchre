// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <array>
#include "Card.h"

using namespace std;

/////////////// Rank operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const RANK_NAMES[] = {
  "Two",   // TWO
  "Three", // THREE
  "Four",  // FOUR
  "Five",  // FIVE
  "Six",   // SIX
  "Seven", // SEVEN
  "Eight", // EIGHT
  "Nine",  // NINE
  "Ten",   // TEN
  "Jack",  // JACK
  "Queen", // QUEEN
  "King",  // KING
  "Ace"    // ACE
};

//REQUIRES str represents a valid rank ("Two", "Three", ..., "Ace")
Rank string_to_rank(const std::string &str) {
  for(int r = TWO; r <= ACE; ++r) {
    if (str == RANK_NAMES[r]) {
      return static_cast<Rank>(r);
    }
  }
  assert(false); // Input string didn't match any rank
}

//EFFECTS Prints Rank to stream, for example "Two"
std::ostream & operator<<(std::ostream &os, Rank rank) {
  os << RANK_NAMES[rank];
  return os;
}

//EFFECTS Reads a Rank from a stream, for example "Two" -> TWO
std::istream & operator>>(std::istream &is, Rank &rank) {
  string str;
  is >> str;
  rank = string_to_rank(str);
  return is;
}



/////////////// Suit operator implementations - DO NOT CHANGE ///////////////

constexpr const char *const SUIT_NAMES[] = {
  "Spades",   // SPADES
  "Hearts",   // HEARTS
  "Clubs",    // CLUBS
  "Diamonds", // DIAMONDS
};

//REQUIRES str represents a valid suit ("Spades", "Hearts", "Clubs", or "Diamonds")
Suit string_to_suit(const std::string &str) {
  for(int s = SPADES; s <= DIAMONDS; ++s) {
    if (str == SUIT_NAMES[s]) {
      return static_cast<Suit>(s);
    }
  }
  assert(false); // Input string didn't match any suit
}

//EFFECTS Prints Suit to stream, for example "Spades"
std::ostream & operator<<(std::ostream &os, Suit suit) {
  os << SUIT_NAMES[suit];
  return os;
}

//EFFECTS Reads a Suit from a stream, for example "Spades" -> SPADES
std::istream & operator>>(std::istream &is, Suit &suit) {
  string str;
  is >> str;
  suit = string_to_suit(str);
  return is;
}


/////////////// Write your implementation for Card below ///////////////

Card::Card()
{
    suit = SPADES;
    rank = TWO;
}

Card::Card(Rank rank_in, Suit suit_in)
{
    rank = rank_in;
    suit = suit_in;
}

Rank Card::get_rank() const
{
    return rank;
}

Suit Card::get_suit() const
{
    return suit;
}

Suit Card::get_suit(Suit trump) const
{
    if (is_left_bower(trump))
        return trump;
    else
        return suit;
}

bool Card::is_face_or_ace() const
{
    return get_rank() > TEN;

}

bool Card::is_right_bower(Suit trump) const
{
    return get_rank() == JACK && get_suit() == trump;
}

bool Card::is_left_bower(Suit trump) const
{

    return get_rank() == JACK && get_suit() == Suit_next(trump);
}

bool Card::is_trump(Suit trump) const
{
    return get_suit() == trump || is_left_bower(trump);
}

std::ostream& operator<<(std::ostream& os, const Card& card)
{
    os << card.get_rank() << " of " << card.get_suit();
    return os;
}

std::istream& operator>>(std::istream& is, Card& card)
{
    string str_rank;
    string str_suit;
    string word;
    is >> str_rank >> word >> str_suit;
    card.rank = string_to_rank(str_rank);
    card.suit = string_to_suit(str_suit);
    return is;
}

bool operator<(const Card& lhs, const Card& rhs)
{
    if (lhs.get_rank() == rhs.get_rank())
        return lhs.get_suit() < rhs.get_suit();
    return lhs.get_rank() < rhs.get_rank();
}

bool operator<=(const Card& lhs, const Card& rhs)
{
    return !(lhs > rhs);
}

bool operator>(const Card& lhs, const Card& rhs)
{
    return rhs < lhs;
}

bool operator>=(const Card& lhs, const Card& rhs)
{
    return !(lhs < rhs);
}

bool operator==(const Card& lhs, const Card& rhs)
{
    return !(lhs < rhs) && !(rhs < lhs);
}


bool operator!=(const Card& lhs, const Card& rhs)
{
    return !(lhs == rhs);
}

Suit Suit_next(Suit suit)
{
    if (suit == SPADES)
        return CLUBS;
    else if (suit == HEARTS)
        return DIAMONDS;
    else if (suit == CLUBS)
        return SPADES;
    else
        return HEARTS;
}

bool Card_less(const Card& a, const Card& b, Suit trump)
{
    if (a == b)
    {
        return false;
    }
    if (a.is_trump(trump) && !b.is_trump(trump))
    {
        return false;
    }
    if (!a.is_trump(trump) && b.is_trump(trump))
    {
        return true;
    }
    if (!a.is_trump(trump) && !b.is_trump(trump))
    {
        return a < b;
    }
    if (a.is_right_bower(trump) )
    {
        return false;
    }
    if (b.is_right_bower(trump))
    {
        return true;
    }
    if (a.is_left_bower(trump))
    {
        return false;
    }
    if (b.is_left_bower(trump))
    {
        return true;
    }
    return a < b;
    
}

bool Card_less(const Card& a, const Card& b, const Card& led_card, Suit trump)
{
    if (a.is_trump(trump) || b.is_trump(trump))
    {
        return Card_less(a, b, trump);
    }
    Suit ledSuit = led_card.get_suit(trump);
    if (a.get_suit() == ledSuit && b.get_suit() != ledSuit)
    {
        return false;
    }
    if (b.get_suit() == ledSuit && a.get_suit() != ledSuit)
    {
        return true;
    }
    return a < b;
}
