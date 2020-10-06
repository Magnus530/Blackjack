#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <conio.h>
#include <stdlib.h>
#include <numeric>

class cardInfo      /* Different values passed with inheretance and through objects */
{
public:
    
    int playerCardValue = 0, houseCardValue = 0, card = 0, ace = 0, aceValue = 0, playerBet = 0, houseBet = 0;
    int playerCapital = 100, houseCapital = 100;

    bool gameWon = false;

    std::vector<int> playerCardPick{};
    std::vector<int> houseCardPick{};

public:
    void showPlayerCards()     /* Iterates over cards in the cardPick vector before sorting. */
    {
        std::cout << "\nYou have: " << playerCardPick.size() << " card/s in your hand\n";
        std::cout << "Your hand: ";

        for (int i = 0; i < playerCardPick.size(); i++)
        {
            std::cout << playerCardPick[i] << ' ';
        }
        
        std::sort(playerCardPick.begin(), playerCardPick.end());

        playerCardValue = std::accumulate(playerCardPick.begin(), playerCardPick.end(), 0);
        std::cout << "\nYour cardvalue is: " << playerCardValue << '\n' << std::endl;
    }

    void showHouseCards()     /* Iterates over cards in the cardPick vector before sorting and adding them together. */
    {
        std::cout << "\nThe house holds: " << houseCardPick.size() << " card/s\n";
        std::cout << "House hand: ";

        for (int i = 0; i < houseCardPick.size(); i++)
        {
            std::cout << houseCardPick[i] << ' ';
        }

        std::sort(houseCardPick.begin(), houseCardPick.end());

        houseCardValue = std::accumulate(houseCardPick.begin(), houseCardPick.end(), 0);
        std::cout << "\nThe house's cardvalue is: " << houseCardValue << '\n' << std::endl;

    }

} info;


class cardPlay : public cardInfo /* A class for the main functionality of the game. */
{
public:
    void playerDraw()     /* Draws a random card between 1-10 and pushes them into the cardPick vector. */
    {
        card = rand() % 10 + 1;

        playerCardAce();

        playerCardPick.push_back(card);
         
        showPlayerCards();

        playerTurn();
    }
   
    /* Let the player draw again, if cardvalue is more than 21 the house wins the round and player draws again. 
    If the player types T the turn ends, and the house tries to match
    the players score. When E is pressed the game exits */

    void playerTurn()   
    {
        if (playerCardValue > 21)
        {
            std::cout << "Your cardvalue exceeds 21, the house takes the round.\n";

            cardReset();
            playerFee();          
        }

        std::cout << "Please press any key to draw. If your cardvalue is higher than 21 you are out of the game.\n";
        std::cout << "Please press T to end turn or E to exit.\n" << std::endl;
        char choice = _getch();

        if (playerCardValue > 21)
        {
            std::cout << "Your cardvalue exceeds 21, the house takes the round.\n";

            cardReset();
            playerFee();
            playerDraw();
        }
        else if (choice == 'T' || choice == 't')
        {
            std::cout << "Now it's the house's turn to match your score\n";
        }
        else if (choice == 'E' || choice == 'e')
        {
            std::cout << "Thanks for playing\n";
            exit(EXIT_SUCCESS);
        }
        else
        {
            std::cout << "You draw a card\n";
            playerDraw();
        }
    }

    void houseDraw()     /* Draws a random card between 1-10 and pushes them into the cardPick vector. */
    {
        card = rand() % 10 + 1;

        houseCardAce();

        houseCardPick.push_back(card);

        houseCardValue = std::accumulate(houseCardPick.begin(), houseCardPick.end(), 0);

        houseTurn();

    }

    /* Checks if the combined value of cards in the house's hand is greater than 21. If it is, reset cardvalues
    but keep the capital value. Player draw is then called to let the player draw once more.
    In other words the player takes the round if the housecardvalueu is greater then 21. */

    void houseTurn()    
    {
        if (houseCardValue > 21)
        {
            std::cout << "The house's cardvalue exceeds 21, The round is yours!\n";
            
            cardReset();
            houseFee();
            playerDraw();
        }

        while (houseCardValue < playerCardValue)
        {
            houseDraw();
        }
    }

    /* The win conditions of the rounds and the game. The winner of a round gains the bet of the other. A draw is called when the
    cardvalues of the house and player match eachother. Then a new round starts. */
    
    void cardWin()
    {
        if (playerCardValue > houseCardValue)
        {
            playerCapital = houseBet + playerBet;

            showHouseCards();

            std::cout << "Contratulations the round is yours! You have: " << playerCapital << "$ and a cardvalue of: " << playerCardValue << '\n';

            cardReset();
        }
        else if (playerCardValue < houseCardValue)
        {
            houseCapital = houseCapital + playerBet + houseBet;

            showHouseCards();

            std::cout << "The house takes the round with a cardvalue of: " << houseCardValue << ", and has a capital of: " << houseCapital << "$\n";

            cardReset();
        }
        else if (playerCardValue == houseCardValue)
        {
            playerCapital += playerBet;
            houseCapital += houseBet;

            showHouseCards();

            std::cout << "The house's cardvalue matches yours, it's a draw. You replay the house.\n";
           
            cardReset();
        }
        
        if (playerCapital == 0)
        {
             houseCapital = houseCapital + houseBet + playerBet;

             std::cout << "The house wins the game.\n";

             gameWon = true;
        }
        else if (houseCapital == 0)
        {
             playerCapital = playerCapital + houseBet + playerBet;

             std::cout << "Congratulations, you win the game!\n";

             gameWon = true;
        }
    }

    /* When an Ace is drawn the player may select 1 or 11 of preferred value. The house selects 11 when the cardvalue is lower or
    equal to 10 and a 1 when the cardvalue is over 10.*/

    void playerCardAce()
    {
        if (card == 1)
        {
            std::cout << "\nYou draw an Ace! Choose a cardvalue of 1 or 11.\n";
            std::cin >> ace;
            std::cin.clear();
            std::cin.ignore(32767, '\n');

            if (ace == 1 || ace == 11)
            {
                card = ace;
            }
          
            while (!(ace == 1 || ace == 11))
            {
                std::cout << "Choose a cardvalue of 1 or 11 please.\n";
                std::cin >> ace;
                std::cin.clear();
                std::cin.ignore(32767, '\n');
            }

            card = ace;

        }
    }

    void houseCardAce()
    {
        if (card == 1)
        {            
            if (houseCardValue <= 10)
            {
                ace = 11;
                card = ace;
            }
            else
            {
                ace = 1;
                card = ace;
            }

        }
    }

    /* Asks the player for a bet that matches the house's capital, and asks the house to copy the same bet as the player. */

    void cardBet()
    {
        std::cout << "\nThe house has a hand.\n" << '\n';
        std::cout << "Your capital is: " << playerCapital << "$, and the house's capital is: " << houseCapital << "$\n";
        std::cout << "Please select the amount you wish to bet on your hand.\n";
        std::cin >> playerBet;
        std::cin.clear();
        std::cin.ignore(32767, '\n');

        if (playerBet <= playerCapital && playerBet > 0 && playerBet <= houseCapital)
        {
            playerCapital = playerCapital - playerBet;
            std::cout << "Your bet is: " << playerBet << "$, Your capital is: " << playerCapital << '\n';
        }
        else
        {
            while (playerBet > playerCapital || playerBet > houseCapital || playerBet <= 0)
            {
                std::cout << "Please select an amount equal to or less than your capital, and equal to or less than the house's capital.\n";
                std::cin >> playerBet;
                std::cin.clear();
                std::cin.ignore(32767, '\n');
            }

            playerCapital = playerCapital - playerBet;
            std::cout << "Your bet is: " << playerBet << ". Your capital is: " << playerCapital << '\n';
        }

        houseBet = playerBet;
        houseCapital -= houseBet;
            
        std::cout << "The house bets: " << houseBet << "$\n";

    }

    /* Resets the cardvalues, bets and hands of the player and the house. To start a new round with fresh values. */

    void cardReset()
    {
        playerCardPick.erase(playerCardPick.begin(), playerCardPick.end());
        playerCardValue = 0;
        playerBet = 0;
        
        houseCardPick.erase(houseCardPick.begin(), houseCardPick.end());
        houseCardValue = 0;
        houseBet = 0;
    }

    /* Respective fees for the player and the house. Requested at gamestart. */

    void playerFee()
    {
        if (playerCapital <= 10)
        {
            std::cout << "Seems you are short on money. The house takes the game (one needs atleast 11$ to draw and bet).\n";
            exit(EXIT_SUCCESS);
        }
        std::cout << "\nYou pay 10$ to start drawing.\n" << std::endl;
        playerCapital -= 10;
    }

    void houseFee()
    {
        if (houseCapital <= 10)
        {
        std::cout << "Seems the house is short on money. Congratlations! the game is yours.\n";
        exit(EXIT_SUCCESS);
        }
        houseCapital -= 10;
    }

} play;

void mainMenu()
{
    std::cout << "**************Welcome to Blackjack**************\n" << std::endl;
    std::cout << "Please press a key to pick a card\n" << std::endl;
    system("pause");
}

int main()
{
    std::srand(static_cast<unsigned int>(std::time(nullptr))); /* set seed value to system clock. */
    
    mainMenu();

    while (play.gameWon == false)
    {
        play.playerFee();
        play.houseFee();

        play.playerDraw();
        play.houseDraw();

        play.cardBet();

        play.cardWin();
    }
}

