// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Card.h"
#include "unit_test_framework.h"
#include <iostream>
#include <cassert>
using namespace std;


TEST(test_card_ctor) {
    Card c(ACE, HEARTS);
    ASSERT_EQUAL(ACE, c.get_rank());
    ASSERT_EQUAL(HEARTS, c.get_suit());
}

TEST(test_card_default_ctor)
{
    Card p;
    ASSERT_EQUAL(TWO, p.get_rank());
    ASSERT_EQUAL(SPADES, p.get_suit());
}

TEST(test_is_face_or_ace)
{
    Card c(JACK, HEARTS);
    assert(c.is_face_or_ace());
    Card p(FOUR, SPADES);
    ASSERT_FALSE(p.is_face_or_ace());
    Card q(ACE, CLUBS);
    assert(q.is_face_or_ace());
}

TEST(test_bowers_and_trump)
{
    Card c(JACK, HEARTS);
    Suit trump = HEARTS;
    ASSERT_FALSE(c.is_left_bower(trump));
    assert(c.is_right_bower(trump));
    assert(c.is_trump(trump));

    Card p(JACK, SPADES);
    trump = CLUBS;
    ASSERT_FALSE(p.is_right_bower(trump));
    assert(p.is_left_bower(trump));
    assert(p.is_trump(trump));

    Card q(TEN, SPADES);
    ASSERT_FALSE(q.is_right_bower(trump));
    ASSERT_FALSE(q.is_left_bower(trump));
    ASSERT_FALSE(q.is_trump(trump));

    Card d(JACK, HEARTS);
    trump = DIAMONDS;
    assert(d.is_trump(trump));
}

TEST(test_iostream_operator)
{
    Card c(JACK, HEARTS);

    ostringstream s;
    s << c;

    ostringstream correct;
    correct << "Jack of Hearts";
    
    ASSERT_EQUAL(s.str(), correct.str());

    Card p;
    string input = "King of Spades";
    istringstream ss_input(input);
    ss_input >> p;
    ASSERT_EQUAL(p, Card(KING, SPADES));
}

TEST(test_operators)
{
    Card c(JACK, SPADES);
    Card p(KING, HEARTS);
    assert(c < p);
    Card d(JACK, DIAMONDS);
    assert(c < d);
    Card f(KING, HEARTS);
    Card r(TEN, CLUBS);
    assert(f == p);
    assert(f != d);
    assert(c <= p);
    assert(f > d);
    assert(p > r);
    assert(d >= r);
}

TEST(test_suit_next)
{
    Suit trump = CLUBS;
    ASSERT_EQUAL(Suit_next(trump), SPADES);
    trump = SPADES;
    ASSERT_EQUAL(Suit_next(trump), CLUBS);
    trump = HEARTS;
    ASSERT_EQUAL(Suit_next(trump), DIAMONDS);
    trump = DIAMONDS;
    ASSERT_EQUAL(Suit_next(trump), HEARTS);
}

TEST(test_card_less)
{
    Suit trump = HEARTS;
    Card p(QUEEN, DIAMONDS);
    Card c(JACK, HEARTS);
    Card d(JACK, DIAMONDS);
    Card f(TEN, SPADES);
    assert(Card_less(p,c,trump));
    assert(Card_less(d, c, trump));
    assert(Card_less(f, c, trump));
    assert(Card_less(f, d, trump));
}

TEST(test_card_less_led_suit)
{
    Suit trump = HEARTS;
    Card ledCard(TEN, SPADES);
    Card p(JACK, HEARTS);
    Card c(JACK, DIAMONDS);
    Card f(ACE, SPADES);
    Card d(TEN, SPADES);
    Card g(KING, HEARTS);
    Card e(QUEEN, CLUBS);
    assert(Card_less(c, p,ledCard, trump));
    assert(Card_less(d, c, ledCard,trump));
    assert(Card_less(f, c, ledCard,trump));
    assert(Card_less(d, f, ledCard, trump));
    assert(Card_less(f, g, ledCard, trump));

}

TEST_MAIN()
