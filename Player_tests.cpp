// Project UID 1d9f47bfc76643019cfbf037641defe1

#include "Player.h"
#include "unit_test_framework.h"

#include <iostream>

using namespace std;

TEST(test_player_get_name) {
    Player* alice = Player_factory("Alice", "Simple");
    ASSERT_EQUAL("Alice", alice->get_name());

    delete alice;
}

// Add more tests here

TEST(test_player_insertion_2) {
    // Create a Human player
    Player* human = Player_factory("NotRobot", "Human");

    // Print the player using the stream insertion operator
    ostringstream oss1;
    oss1 << *human;

    // Verify that the output is the player's name
    ASSERT_EQUAL(oss1.str(), "NotRobot");

    // Create a Simple player
    Player* bob = Player_factory("Bob", "Simple");

    // Print the player using the stream insertion operator
    ostringstream oss2;
    oss2 << *bob;
    ASSERT_EQUAL(oss2.str(), "Bob");

    // Clean up players that were created using Player_factory()
    delete human;
    delete bob;
}

TEST(test_simple_player_make_trump_2) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, HEARTS));
    bob->add_card(Card(TEN, HEARTS));
    bob->add_card(Card(QUEEN, HEARTS));
    bob->add_card(Card(KING, HEARTS));
    bob->add_card(Card(ACE, HEARTS));

    // Bob makes trump
    Card nine_hearts(NINE, HEARTS);
    Suit trump;
    bool orderup = bob->make_trump(
        nine_hearts,    // Upcard
        true,           // Bob is also the dealer
        1,              // First round
        trump           // Suit ordered up (if any)
    );

    // Verify Bob's order up and trump suit
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, HEARTS);

    delete bob;
}

TEST(test_simple_player_make_trump_3) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, CLUBS));
    bob->add_card(Card(TEN, CLUBS));
    bob->add_card(Card(QUEEN, CLUBS));
    bob->add_card(Card(KING, DIAMONDS));
    bob->add_card(Card(ACE, DIAMONDS));

    // Bob makes trump
    Card nine_diamonds(NINE, DIAMONDS);
    Suit trump;
    bool orderup = bob->make_trump(
        nine_diamonds,  // Upcard
        true,           // Bob is also the dealer
        1,              // First round
        trump           // Suit ordered up (if any)
    );

    // Verify Bob's order up and trump suit
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, DIAMONDS);

    delete bob;
}

TEST(test_simple_player_make_trump_4) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, CLUBS));
    bob->add_card(Card(TEN, DIAMONDS));
    bob->add_card(Card(QUEEN, CLUBS));
    bob->add_card(Card(KING, DIAMONDS));
    bob->add_card(Card(ACE, HEARTS));

    // Bob makes trump
    Card nine_diamonds(NINE, DIAMONDS);
    Suit trump;
    bool orderup = bob->make_trump(
        nine_diamonds,  // Upcard
        true,           // Bob is also the dealer
        1,              // First round
        trump           // Suit ordered up (if any)
    );

    // Verify Bob didn't order up
    ASSERT_FALSE(orderup);

    delete bob;
}

TEST(test_simple_player_make_trump_8) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, CLUBS));
    bob->add_card(Card(TEN, DIAMONDS));
    bob->add_card(Card(QUEEN, CLUBS));
    bob->add_card(Card(KING, DIAMONDS));
    bob->add_card(Card(ACE, HEARTS));

    // Bob makes trump
    Card nine_diamonds(NINE, DIAMONDS);
    Suit trump;
    bool orderup = bob->make_trump(
        nine_diamonds,  // Upcard
        false,           // Bob is not dealer
        1,              // First round
        trump           // Suit ordered up (if any)
    );

    // Verify Bob didn't order up
    ASSERT_FALSE(orderup);

    delete bob;
}

TEST(test_simple_player_make_trump_5) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, CLUBS));
    bob->add_card(Card(TEN, CLUBS));
    bob->add_card(Card(QUEEN, SPADES));
    bob->add_card(Card(KING, DIAMONDS));
    bob->add_card(Card(ACE, HEARTS));

    // Bob makes trump
    Card nine_hearts(NINE, HEARTS);
    Suit trump;
    bool orderup = bob->make_trump(
        nine_hearts,    // Upcard
        false,          // Bob is not the dealer
        2,              // Second round
        trump           // Suit ordered up (if any)
    );

    // Verify Bob's order up and trump suit
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, DIAMONDS);

    delete bob;
}

TEST(test_simple_player_make_trump_6) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, CLUBS));
    bob->add_card(Card(TEN, CLUBS));
    bob->add_card(Card(QUEEN, SPADES));
    bob->add_card(Card(KING, CLUBS));
    bob->add_card(Card(ACE, HEARTS));

    // Bob makes trump
    Card nine_hearts(NINE, HEARTS);
    Suit trump;
    bool orderup = bob->make_trump(
        nine_hearts,    // Upcard
        true,           // Bob is also the dealer
        2,              // Second round
        trump           // Suit ordered up (if any)
    );

    // Verify Bob's order up and trump suit
    ASSERT_TRUE(orderup);
    ASSERT_EQUAL(trump, DIAMONDS);

    delete bob;
}

TEST(test_simple_player_make_trump_7) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, CLUBS));
    bob->add_card(Card(TEN, CLUBS));
    bob->add_card(Card(QUEEN, SPADES));
    bob->add_card(Card(KING, CLUBS));
    bob->add_card(Card(ACE, HEARTS));

    // Bob makes trump
    Card nine_hearts(NINE, HEARTS);
    Suit trump;
    bool orderup = bob->make_trump(
        nine_hearts,    // Upcard
        false,          // Bob is not the dealer
        2,              // Second round
        trump           // Suit ordered up (if any)
    );

    // Verify Bob didn't order up
    ASSERT_FALSE(orderup);

    delete bob;
}




TEST(test_simple_player_lead_card_2) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, CLUBS));
    bob->add_card(Card(TEN, CLUBS));
    bob->add_card(Card(QUEEN, CLUBS));
    bob->add_card(Card(KING, CLUBS));
    bob->add_card(Card(ACE, CLUBS));

    // Bob adds a card to his hand and discards one card
    bob->add_and_discard(
        Card(NINE, DIAMONDS) // upcard
    );

    // Bob leads
    Card card_led = bob->lead_card(DIAMONDS);

    // Verify the card Bob selected to lead
    Card ace_clubs(ACE, CLUBS);
    ASSERT_EQUAL(card_led, ace_clubs); //check led card

    delete bob;
}

TEST(test_simple_player_lead_card_3) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, HEARTS));
    bob->add_card(Card(TEN, HEARTS));
    bob->add_card(Card(QUEEN, HEARTS));
    bob->add_card(Card(KING, HEARTS));
    bob->add_card(Card(ACE, HEARTS));

    // Bob adds a card to his hand and discards one card
    bob->add_and_discard(
        Card(NINE, HEARTS) // upcard
    );

    // Bob leads
    Card card_led = bob->lead_card(HEARTS);

    // Verify the card Bob selected to lead
    Card ace_hearts(ACE, HEARTS);
    ASSERT_EQUAL(card_led, ace_hearts); //check led card

    delete bob;
}

TEST(test_simple_player_lead_card_4) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, HEARTS));
    bob->add_card(Card(TEN, HEARTS));
    bob->add_card(Card(KING, HEARTS));
    bob->add_card(Card(ACE, HEARTS));
    bob->add_card(Card(JACK, HEARTS));

    // Bob adds a card to his hand and discards one card
    bob->add_and_discard(
        Card(NINE, HEARTS) // upcard
    );

    // Bob leads
    Card card_led = bob->lead_card(HEARTS);

    // Verify the card Bob selected to lead
    Card jack_hearts(JACK, HEARTS);
    ASSERT_EQUAL(card_led, jack_hearts); //check led card

    delete bob;
}

TEST(test_simple_player_lead_card_5) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, HEARTS));
    bob->add_card(Card(TEN, HEARTS));
    bob->add_card(Card(KING, HEARTS));
    bob->add_card(Card(ACE, HEARTS));
    bob->add_card(Card(JACK, DIAMONDS));

    // Bob adds a card to his hand and discards one card
    bob->add_and_discard(
        Card(NINE, HEARTS) // upcard
    );

    // Bob leads
    Card card_led = bob->lead_card(HEARTS);

    // Verify the card Bob selected to lead
    Card jack_diamonds(JACK, DIAMONDS);
    ASSERT_EQUAL(card_led, jack_diamonds); //check led card

    delete bob;
}



TEST(test_simple_player_play_card_2) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, CLUBS));
    bob->add_card(Card(TEN, CLUBS));
    bob->add_card(Card(QUEEN, CLUBS));
    bob->add_card(Card(KING, CLUBS));
    bob->add_card(Card(ACE, CLUBS));

    // Bob plays a card
    Card nine_hearts(NINE, HEARTS);
    Card card_played = bob->play_card(
        nine_hearts,  // Nine of Hearts is led
        DIAMONDS      // Trump suit
    );

    // Verify the card Bob played
    ASSERT_EQUAL(card_played, Card(NINE, CLUBS));
    delete bob;
}

TEST(test_simple_player_play_card_3) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, HEARTS));
    bob->add_card(Card(TEN, DIAMONDS));
    bob->add_card(Card(QUEEN, CLUBS));
    bob->add_card(Card(KING, SPADES));
    bob->add_card(Card(ACE, HEARTS));

    // Bob plays a card
    Card nine_diamonds(NINE, DIAMONDS);
    Card card_played = bob->play_card(
        nine_diamonds,  // Nine of Diamonds is led
        DIAMONDS        // Trump suit
    );

    // Verify the card Bob played
    ASSERT_EQUAL(card_played, Card(TEN, DIAMONDS));
    delete bob;
}

TEST(test_simple_player_play_card_4) {
    // Bob's hand
    Player* bob = Player_factory("Bob", "Simple");
    bob->add_card(Card(NINE, HEARTS));
    bob->add_card(Card(TEN, DIAMONDS));
    bob->add_card(Card(QUEEN, CLUBS));
    bob->add_card(Card(KING, SPADES));
    bob->add_card(Card(ACE, HEARTS));

    // Bob plays a card
    Card jack_diamonds(JACK, DIAMONDS);
    Card card_played = bob->play_card(
        jack_diamonds,  // Jack of Diamonds is led
        HEARTS        // Trump suit
    );

    // Verify the card Bob played
    ASSERT_EQUAL(card_played, Card(ACE, HEARTS));
    delete bob;
}

TEST_MAIN()

