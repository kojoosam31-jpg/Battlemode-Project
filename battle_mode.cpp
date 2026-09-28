#include <iostream>    // for cin/cout (input/output)
#include <cstdlib>     // for rand() and srand() (random numbers)
#include <ctime>       // for time() (used to seed the random generator)
#include <string>      // for std::string

// Constants representing each possible choice as a number
const int ROCK = 1;
const int PAPER = 2;
const int SCISSORS = 3;
const int SPECIAL = 4;

// Function prototypes - let the compiler know these functions exist
// before main() calls them; full definitions are below main()
int getPlayerChoice(bool specialUsed);
int getComputerChoice(bool specialUsed);
std::string determineWinner (int playerChoice, int computerChoice);
std::string nameOf(int choice);

int main(){
    srand(time(0));  // seed the random number generator using the current time,
                     // so computer choices differ between runs
    char playAgainAnswer;

    // Outer loop: lets the whole game restart if the player chooses to play again
    do{
        // Reset all game state at the start of every game/restart
        int playerHP = 3;
        int computerHP = 3;
        bool playerSpecialUsed = false;
        bool computerSpecialUsed = false;
        bool shieldUsed = false;
        bool shieldActive = false;
        int roundNumber = 0;

           // Welcome message and rules
           std::cout << "======================================================" << std::endl;
           std::cout << "  WELCOME  TO  ROCK,  PAPER, SCISSORS:  BATTLE  MODE " << std::endl;
           std::cout << "======================================================" << std::endl;
           std::cout << "GAME RULES: " << std::endl;
           std::cout << "START: Each player has 3 HP " << std::endl;
           std::cout << "DAMAGE: Round loser losses 1 HP " << std::endl;
           std::cout << "SPECIAL MOVE: Beats any standard move, but ties against another Special Move " << std::endl;
           std::cout << "SHIELD: Once per game, you may activate a shield to block one round's damage " << std::endl;
           std::cout << "GOAL: Reduce your opponent's HP to 0 to win! " << std::endl;
           std::cout << "HOW TO PLAY: " << std::endl;
           std::cout << "[Standard Move] 1. ROCK | 2. PAPER | 3. SCISSORS " << std::endl;
           std::cout << "[Special Move] 4. Special Move (1 - Time Use Only) " << std::endl;

           // Battle loop: keeps playing rounds until someone's HP hits 0
           while (playerHP > 0 && computerHP > 0) {
            roundNumber++;
            std::cout << "\n--- Round " << roundNumber << "---" << std::endl;
            std::cout << "Player HP: " << playerHP << " Computer HP: " << computerHP << std::endl;

           // Shield only protects the round it's activated in, so reset it each round
            shieldActive = false;

            // Offer the shield only if it hasn't been used yet in this game
            if (!shieldUsed) {
                char shieldAnswer;
                std::cout << "\nActivate your shield this round? (y/n): ";
                std::cin >> shieldAnswer;
                if (shieldAnswer == 'y') {
                    shieldActive = true;
                    shieldUsed = true;
                    std::cout << "Shield activated! You are protected this round." << std::endl;
                }
            }

        
            // Get each side's move for this round
            int playerChoice = getPlayerChoice(playerSpecialUsed);
            if (playerChoice == SPECIAL) {
                playerSpecialUsed = true;
            }

            int computerChoice = getComputerChoice(computerSpecialUsed);
            if (computerChoice == SPECIAL) {
                computerSpecialUsed = true;
            }

            // Show both choices to the player
            std::cout << "You chose: " << nameOf(playerChoice) << std::endl;
            std::cout << "Computer chose: " << nameOf(computerChoice) << std::endl;

            // Determine the round's outcome
            std::string result = determineWinner(playerChoice, computerChoice);

            // Apply HP changes based on the result
            if (result == "PLAYER_WINS") {
                computerHP = computerHP - 1;
                std::cout << "You win the round!" << std::endl;
            } else if (result == "COMPUTER_WINS") {
                // If the shield is active, block the damage instead of losing HP
                if (shieldActive) {
                    std::cout << "Your shield blocked the damage!" << std::endl;
                } else {
                    playerHP = playerHP - 1;
                    std::cout << "Computer wins the round!" << std::endl;
                }
            } else {
                std::cout << "It's a draw!" << std::endl;
            }
           }
           // Battle loop ends here - either playerHP or computerHP has  reached 0
          
           // Announce the overall winner of the battle
           if (playerHP == 0) {
            std::cout << "\nComputer wins the battle!" << std::endl;
           }else {
            std::cout << "\nYou win the battle!" << std::endl;
           }
           std::cout << "Total rounds played: " << roundNumber << std::endl;

           // Ask if the player wants to restart the whole game
           std::cout << "\nPlay again?  (y/n): ";
           std::cin >> playAgainAnswer;        

    } while (playAgainAnswer == 'y');

    return 0;

} 

// Repeatedly prompts the player for a choice until a valid one is entered.
// Shows the Special Move option only if it hasn't been used yet.
int getPlayerChoice(bool specialUsed) {
    int input;

    while (true) {
        std::cout << "\n1. Rock" << std::endl;
        std::cout << "2. Paper" << std::endl;
        std::cout << "3. Scissors" << std::endl;
        if (!specialUsed) {
            std::cout << "4. Special Move" << std::endl;
        }
        std::cout << "\nEnter your choice: ";
        std::cin >> input;

        if (input >= 1 && input <= 4) {
            // Block re-selecting Special Move if it's already been used
            if (input == SPECIAL && specialUsed) {
                std::cout << "You already used your Special Move!" << std::endl;
                continue;    // go back to the top of the loop and ask again
            }
            return input;
        } else {
            std::cout << "Invalid choice, try again." << std::endl;
        }

    }
} // getPlayerChoice closes here

// Randomly picks the computer's move. Gives a 20% chance of using the
// Special Move if it hasn't been used yet; otherwise picks Rock/Paper/Scissors.
int getComputerChoice(bool specialUsed) {
    int chance = rand() % 100;   // random number from 0 to 99

    if (!specialUsed && chance < 20) {
        return SPECIAL;
    } else {
        return (rand() % 3) + 1;   // random number from 1 to 3
    }
}    

// Compares both choices and returns the round's outcome as text.
// Special Move rules are checked first, since they override normal rules.
std::string determineWinner(int playerChoice, int computerChoice) {
   // Both used Special Move in the same round -> draw
    if (playerChoice == SPECIAL && computerChoice == SPECIAL) {
        return "DRAW";
    }
    // Only the player used Special Move -> automatic win
    if (playerChoice == SPECIAL) {
        return "PLAYER_WINS";
    }
    // Only the computer used Special Move -> automatic win
    if (computerChoice == SPECIAL) {
        return "COMPUTER_WINS";
    }
    // Same normal choice on both sides ->
    if (playerChoice == computerChoice) {
        return "DRAW";
    }
    // Standard Rock/Paper/Scissors winning combinations
    if ((playerChoice == ROCK && computerChoice == SCISSORS) ||
        (playerChoice == SCISSORS && computerChoice == PAPER) ||
        (playerChoice == PAPER && computerChoice == ROCK)) {
        return "PLAYER_WINS";
        } else {
            return "COMPUTER_WINS";

        }
}

// Translates a choice number into its readable name for display purposes.
std::string nameOf(int choice) {
    if (choice == ROCK) {
        return "Rock";
    }
    if (choice == PAPER) {
        return "Paper";
    }
    if (choice == SCISSORS) {
        return "Scissors";
    }
    if (choice == SPECIAL) {
        return "Special";
    }
}