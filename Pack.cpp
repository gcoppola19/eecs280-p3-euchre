// Project UID 1d9f47bfc76643019cfbf037641defe1

#include <cassert>
#include <iostream>
#include <array>
#include "Pack.h"

using namespace std;

Pack::Pack()
{
	next = 0;
	int i = 0;
	for (int s = SPADES; s <= DIAMONDS; ++s) {
		for (int r = NINE; r <= ACE; ++r)
		{
			cards[i++] = Card(static_cast<Rank>(r), static_cast<Suit>(s));
		}
	}
}

Pack::Pack(std::istream& pack_input)
{	
	next = 0;
	for (int i = 0; i < PACK_SIZE; ++i)
	{
		pack_input >> cards[i];
	}
}

Card Pack::deal_one()
{
	return cards[next++];
}

void Pack::reset()
{
	next = 0;
}

void Pack::shuffle()
{
	next = 0;
	array<Card, PACK_SIZE> temp;
	for (int i = 0; i < 7; ++i)
	{
		for (int j = 0; j < PACK_SIZE / 2; ++j)
		{
			temp[next++] = cards[j + ( PACK_SIZE / 2 )];
			temp[next++] = cards[j];	
		}
		cards = temp;
		next = 0;
	}
	
}

bool Pack::empty() const
{
	return next >= PACK_SIZE;
}