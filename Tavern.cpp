/*
 * File Title: Tavern.cpp
 * Author: Mahim Ali
 * Date: November 14, 2023
 * This file implements a class called Tavern
 */
#include "Tavern.hpp"

/** Default Constructor **/
Tavern::Tavern() : ArrayBag<Character *>(), level_sum_{0}, num_enemies_{0}
{
}

/**
  @param: the name of an input file
  @pre: Formatting of the csv file is as follows (each numbered item appears separated by comma, only one value for each numbered item):
    1. Name: An uppercase string
    2. Race: An uppercase string [HUMAN, ELF, DWARF, LIZARD, UNDEAD]
    3. Subclass: An uppercase string [BARBARIAN, MAGE, SCOUNDREL, RANGER]
    4. Level/Vitality/Armor: A positive integer
    5. Enemy: 0 (False) or 1 (True)
    6. Main: Uppercase string or strings representing the main weapon (Barbarian and Mage), Dagger type (Scoundrel), or arrows (Ranger). A ranger's arrows are of the form [TYPE] [QUANTITY];[TYPE] [QUANTITY], where each arrow type is separated by a semicolon, and the type and its quantity are separated with a space.
    7. Offhand: An uppercase string that is only applicable to Barbarians, and may be NONE if the Barbarian does not have an offhand weapon, or if the character is of a different subclass.
    8. School/Faction: Uppercase strings that represent a Mage's school of magic: [ELEMENTAL, NECROMANCY, ILLUSION] or a Scoundrel's faction: [CUTPURSE, SHADOWBLADE, SILVERTONGUE], and NONE where not applicable
    9. Summoning: 0 (False) or 1 (True), only applicable to Mages (summoning an Incarnate) and Rangers (Having an Animal Companion)
    10. Affinity: Only applicable to Rangers. Affinities are of the form [AFFINITY1];[AFFINITY2] where multiple affinities are separated by a semicolon. Th value may be NONE for a Ranger with no affinities, or characters of other subclasses.
    11. Disguise: 0 (False) or 1 (True), only applicable to Scoundrels, representing if they have a disguise.
    12. Enraged: 0 (False) or 1 (True), only applicable to Barbarians, representing if they are enraged.
  @post: Each line of the input file corresponds to a Character subclass and dynamically allocates Character derived objects, adding them to the Tavern.
*/
Tavern::Tavern(const std::string &input_file_name) : ArrayBag<Character *>()
{
    std::string junk, line, name, race, subclass, level_str, vitality_str, armor_str, enemy_str, main, offhand, school_or_faction, summoning_str, affinity, disguise_str, enraged_str, quantity_str, Current_affinity; // variables to read each header of the csv file.
    int level, vitality, armor, quantity;                                                                                                                                                                              // variables to read each header of the csv file.
    bool enemy, summoning, disguise, enraged;                                                                                                                                                                          // variables to read each header of the csv file.
    std::vector<Arrows> Arrows_vector;                                                                                                                                                                                 // vector to store the arrows
    Arrows Current_arrows;                                                                                                                                                                                             // arrow object that contains the Ranger's character main weapon.
    std::vector<std::string> Affinities_vector;                                                                                                                                                                        // vector to store the affinities
    Character *character_ptr;                                                                                                                                                                                          // pointer of the character that is dynamically allocated in each line of the csv file depending on the name of the character.
    std::ifstream fin(input_file_name);                                                                                                                                                                                // opens the input file
    if (fin.fail())                                                                                                                                                                                                    // error message if input file doesn't open
    {
        std::cerr << "File cannot be opened for reading." << std::endl;
        exit(1);
    }
    getline(fin, junk);        // ignores header line of the csv file.
    while (getline(fin, line)) // while loop that goes through each line of the csv file.
    {
        std::istringstream character1(line); // used to extract data from each line that is being processed from the csv file
        getline(character1, name, ',');      // gets each value for each line of the csv file  up to the comma
        getline(character1, race, ',');      // stoi is used for ints and bool to convert them from strings.
        getline(character1, subclass, ',');
        getline(character1, level_str, ',');
        level = std::stoi(level_str);
        getline(character1, vitality_str, ',');
        vitality = std::stoi(vitality_str);
        getline(character1, armor_str, ',');
        armor = std::stoi(armor_str);
        getline(character1, enemy_str, ',');
        enemy = std::stoi(enemy_str);
        if (subclass != "RANGER") // if subclass is not "RANGER" then the value for main weapon is read until the comma.
        {
            getline(character1, main, ',');
        }
        else // if it is a "RANGER" each main weapon is taken seperately depending on how many arrows a "RANGER" has along with its quantity.
        {    // then the values for type and quantity is add to the vector
            getline(character1, main, ',');
            std::istringstream main_weapon(main);
            while (getline(main_weapon, main, ';'))
            {
                std::istringstream Arrows_name(main);
                getline(Arrows_name, Current_arrows.type_, ' ');
                getline(Arrows_name, quantity_str, ';');
                quantity = std::stoi(quantity_str);
                Current_arrows.quantity_ = quantity;
                Arrows_vector.push_back(Current_arrows);
            }
        }
        getline(character1, offhand, ',');
        getline(character1, school_or_faction, ',');
        getline(character1, summoning_str, ',');
        summoning = std::stoi(summoning_str);
        getline(character1, affinity, ',');
        std::istringstream Affinity_stream(affinity); // For the "RANGER" characters their affinity is taken separately depending on how many affinities one has and then it is being added to the vector.
        while (getline(Affinity_stream, affinity, ';'))
        {
            std::istringstream Affinity_name(affinity);
            getline(Affinity_name, Current_affinity, ';');
            Affinities_vector.push_back(Current_affinity);
        }
        getline(character1, disguise_str, ',');
        disguise = std::stoi(disguise_str);
        getline(character1, enraged_str, ',');
        enraged = std::stoi(enraged_str);
        if (subclass == "MAGE") // using the pointer of the character a new character is created and its values get stored based on its subclass.
        {
            character_ptr = new Mage(name, race, vitality, armor, level, enemy, school_or_faction, main, summoning);
        }
        else if (subclass == "BARBARIAN")
        {
            character_ptr = new Barbarian(name, race, vitality, armor, level, enemy, main, offhand, enraged);
        }
        else if (subclass == "SCOUNDREL")
        {
            character_ptr = new Scoundrel(name, race, vitality, armor, level, enemy, main, school_or_faction, disguise);
        }
        else if (subclass == "RANGER")
        {
            character_ptr = new Ranger(name, race, vitality, armor, level, enemy, Arrows_vector, Affinities_vector, summoning);
        }
        enterTavern(character_ptr); // after the type of the character is created then it is added to the tavern;
        character_ptr = nullptr;    // the character pointer is set to nullptr after it is added to the tavern as it isnt being used anymore to avoid dangling pointers.
        Arrows_vector.clear();      // the arrow and affinities vector are cleared after the character is added to the tavern to not have any additional values added on from a previous character that was created.
        Affinities_vector.clear();
    }
    fin.close(); // closes the input file
}

/**
    @param:   A reference to a Character entering the Tavern
    @return:  returns true if a Character was successfully added to items_, false otherwise
    @post:    adds Character to the Tavern and updates the level sum and the enemy count if the character is an enemy.
**/
bool Tavern::enterTavern(Character *a_character)
{
    if (add(a_character))
    {
        level_sum_ += a_character->getLevel();
        if (a_character->isEnemy())
            num_enemies_++;

        return true;
    }
    else
    {
        return false;
    }
}

/** @param:   A reference to a Character leaving the Tavern
    @return:  returns true if a character was successfully removed from items_, false otherwise
    @post:    removes the character from the Tavern and updates the level sum and the enemy count if the character is an enemy.
**/
bool Tavern::exitTavern(Character *a_character)
{
    if (remove(a_character))
    {
        level_sum_ -= a_character->getLevel();
        if (a_character->isEnemy())
            num_enemies_--;

        return true;
    }
    return false;
}

/**
    @return:  The integer level count of all the characters currently in the Tavern
    **/
int Tavern::getLevelSum()
{
    return level_sum_;
}

/**
    @return:  The average level of all the characters in the Tavern
    @post:    Considers every character currently in the Tavern, updates the average level of the Tavern rounded to the NEAREST integer, and returns the integer value.
**/
int Tavern::calculateAvgLevel()
{
    return (level_sum_ > 0) ? round(double(level_sum_) / item_count_) : 0.0;
}

/**
    @return:  The integer enemy count of the Tavern
    **/
int Tavern::getEnemyCount()
{
    return num_enemies_;
}

/**
    @return:  The percentage (double) of all the enemy characters in the Tavern
    @post:    Considers every character currently in the Tavern, updates the enemy percentage of the Tavern rounded to 2 decimal places, and returns the double value.
**/
double Tavern::calculateEnemyPercentage()
{
    double enemy_percent = (num_enemies_ > 0) ? (double(num_enemies_) / item_count_) * 100 : 0.0;
    return std::ceil(enemy_percent * 100.0) / 100.0; // round up to to decimal places
}

/**
    @param:   A string reference to a race
    @return:  An integer tally of the number of characters in the Tavern of the given race
**/
int Tavern::tallyRace(const std::string &race)
{
    int frequency = 0;
    int curr_index = 0;
    while (curr_index < item_count_)
    {
        if (items_[curr_index]->getRace() == race)
        {
            frequency++;
        }

        curr_index++;
    }

    return frequency;
}

/**
  @post:    Outputs a report of the characters currently in the tavern in the form:
  "Humans: [x] \nElves: [x] \nDwarves: [x] \nLizards: [x] \nUndead: [x] \n\nThe average level is: [x] \n[x]% are enemies.\n\n"

  Example output:
  Humans: 5
  Elves: 8
  Dwarves: 3
  Lizards: 7
  Undead: 2

  The average level is: 16
  24% are enemies.
*/
void Tavern::tavernReport()
{
    int humans = tallyRace("HUMAN");
    int elves = tallyRace("ELF");
    int dwarves = tallyRace("DWARF");
    int lizards = tallyRace("LIZARD");
    int undead = tallyRace("UNDEAD");

    std::cout << "Humans: " << humans << std::endl;
    std::cout << "Elves: " << elves << std::endl;
    std::cout << "Dwarves: " << dwarves << std::endl;
    std::cout << "Lizards: " << lizards << std::endl;
    std::cout << "Undead: " << undead << std::endl;
    std::cout << "\nThe average level is: " << calculateAvgLevel() << std::endl;
    std::cout << std::fixed << std::setprecision(2) << calculateEnemyPercentage() << "% are enemies.\n\n";
}

/**
    @post: For every character in the tavern, displays each character's information
*/
void Tavern::displayCharacters()
{
    // loops through all the characters in the tavern
    for (int i = 0; i < getCurrentSize(); i++)
    {
        items_[i]->display(); // Every character's information is displayed if they are in the tavern.
    }
}

/**
    @param: a string reference to a race
    @post: For every character in the tavern of the given race (only exact matches to the input string), displays each character's information

*/
void Tavern::displayRace(const std::string &race)
{
    // loops through all the characters in the tavern
    for (int i = 0; i < getCurrentSize(); i++)
    {
        if (items_[i]->getRace() == race) // If the input string matches the given race of a character in Tavern, then it displays each character's information.
        {
            items_[i]->display();
        }
    }
}

/**
    @post: Every character in the tavern eats a tainted stew.
*/
void Tavern::taintedStew()
{
    // loops through all the characters in the tavern.
    for (int i = 0; i < getCurrentSize(); i++)
    {
        items_[i]->eatTaintedStew(); // Each character in the tavern eats a tainted stew.
    }
}

/**
@param: a Character pointer
@post: sets the Tavern's main character to the pointer that is given
*/
void Tavern::setMainCharacter(Character *main)
{
    main_character_ = main;
}

/**
@return: a pointer to the main character in the Tavern, nullptr if there is no main character
*/
Character *Tavern::getMainCharacter() const
{
    if (main_character_ == nullptr)
    {
        return nullptr;
    }
    return main_character_;
}

/**
@param: two Character pointers
@return: true if the lhs Character's level is less than the rhs Character's level
*/
bool Tavern::levelIsLess(Character *lhs, Character *rhs) const
{
    if (lhs->getLevel() < rhs->getLevel())
    {
        return true;
    }
    return false;
}

/**
@param: two Character pointers
@return: true if the lhs Character's level is greater than the rhs Character's level
*/
bool Tavern::levelIsGreater(Character *lhs, Character *rhs) const
{
    if (lhs->getLevel() > rhs->getLevel())
    {
        return true;
    }
    return false;
}

/**
@param: two Character pointers
@return: true if the lhs Character's vitality is less than the rhs Character's vitality
*/
bool Tavern::vitalityIsLess(Character *lhs, Character *rhs) const
{
    if (lhs->getVitality() < rhs->getVitality())
    {
        return true;
    }
    return false;
}

/**
@param: two Character pointers
@return: true if the lhs Character's vitality is more than the rhs Character's vitality
*/
bool Tavern::vitalityIsGreater(Character *lhs, Character *rhs) const
{
    if (lhs->getVitality() > rhs->getVitality())
    {
        return true;
    }
    return false;
}

/**
 @pre: The combat queue should be emptied before the characters are added to the queue
 @param: A string reference to a sorting filter with a default value of "NONE"
 @post: With default filter "NONE": add every character marked as an enemy to the combat queue
: With the filter "LVLASC": add every character in the Tavern marked as an enemy to the combat queue, in ascending order based on their level.
: With the filter "LVLDES": add every character in the Tavern marked as an enemy to the combat queue, in descending order based on their level.
: With the filter "HPASC": add every character in the Tavern marked as an enemy to the combat queue, in ascending order based on their vitality.
: With the filter "HPDES": add every character in the Tavern marked as an enemy to the combat queue, in descending order based on their vitality.
*/
void Tavern::createCombatQueue(const std::string &combat_filter)
{
    std::vector<Character *> my_vector; // created vector to sort based on different filters
    for (int i = 0; i < item_count_; i++)
    {
        if (items_[i]->isEnemy()) // for loop used to add all the enemies in to the vector
        {
            my_vector.push_back(items_[i]);
        }
    }
    while (!(combat_queue_.empty()))
    {
        combat_queue_.pop();
    }
    if (combat_filter == "NONE")
    {
        for (int i = 0; i < my_vector.size(); i++)
        {
            combat_queue_.push(my_vector[i]); // the vector with enemies is added to the combat queue
        }
    }
    else if (combat_filter == "LVLASC")
    {
        std::sort(my_vector.begin(), my_vector.end(), [this](Character *a, Character *b) // use of lambda expression to sort based on level ascending
                  { return levelIsLess(a, b); });
        for (int i = 0; i < my_vector.size(); i++)
        {
            combat_queue_.push(my_vector[i]); // after it is sorted based on each filter, the vector with the enemies is added to the combat queue
        }
    }
    else if (combat_filter == "LVLDES")
    {
        std::sort(my_vector.begin(), my_vector.end(), [this](Character *a, Character *b) // use of lambda expression to sort based on level descending
                  { return levelIsGreater(a, b); });
        for (int i = 0; i < my_vector.size(); i++)
        {
            combat_queue_.push(my_vector[i]); // after it is sorted based on each filter, the vector with the enemies is added to the combat queue
        }
    }
    else if (combat_filter == "HPASC")
    {
        std::sort(my_vector.begin(), my_vector.end(), [this](Character *a, Character *b) // use of lambda expression to sort based on vitality ascending
                  { return vitalityIsLess(a, b); });
        for (int i = 0; i < my_vector.size(); i++)
        {
            combat_queue_.push(my_vector[i]); // after it is sorted based on each filter, the vector with the enemies is added to the combat queue
        }
    }
    else if (combat_filter == "HPDES")
    {
        std::sort(my_vector.begin(), my_vector.end(), [this](Character *a, Character *b) // use of lambda expression to sort based on vitality descending
                  { return vitalityIsGreater(a, b); });
        for (int i = 0; i < my_vector.size(); i++)
        {
            combat_queue_.push(my_vector[i]); // after it is sorted based on each filter, the vector with the enemies is added to the combat queue
        }
    }
}

/**
 @post: returns a pointer to the Character at the front of the Combat Queue.
: If there are no characters in the queue, return nullptr
*/
Character *Tavern::getTarget() const
{
    if (combat_queue_.empty())
    {
        return nullptr;
    }
    return combat_queue_.front();
}

/**
 @post : Prints the details of each character in the combat queue in the following form:
: [ENEMY NAME]: LEVEL [ENEMY LEVEL] [ENEMY RACE]. \nVITALITY: [ENEMY VITALITY] \nARMOR: [ENEMY ARMOR]\n
*/
void Tavern::printCombatQueue()
{
    while (!combat_queue_.empty())
    {
        std::cout << "(ENEMY) " << combat_queue_.front()->getName() << ": "
                  << "LEVEL " << combat_queue_.front()->getLevel() << " " << combat_queue_.front()->getRace() << "." << std::endl;
        std::cout << "VITALITY: " << combat_queue_.front()->getVitality() << std::endl;
        std::cout << "ARMOR: " << combat_queue_.front()->getArmor() << std::endl;
        combat_queue_.pop();
    }
}

/**
 @post : If there are no enemies in the combat queue, do nothing.
: Otherwise do all of the following
: - The action queue must be emptied before the new actions are added.
:- Print out the start of turn and details of the target enemy in the form:
: YOUR TURN\n
: (ENEMY) [ENEMY NAME]: LEVEL [ENEMY LEVEL] [ENEMY RACE]. \nVITALITY: [ENEMY VITALITY] \nARMOR: [ENEMY ARMOR]\n\n
: - Repeat the following process thrice (i.e. read three actions from input):
: - Print the actions that are available to the main character, and read
a non-negative number from input, corresponding to an action.
Printing the available options should be of the following form.
You may consider writing a helper function for this.
:Choose an action(1-4):\n1: BUFF_Heal\t\t2: BUFF_MendMetal\t\t3: ATT_Strike\t\t4: ATT_ThrowTomato\n
:- If the input is invalid (valid inputs will be 1,2,3, or 4 only.),
keep prompting for a non-negative number that is within range, by
printing "Invalid input. Please enter a number between 1 and 4 inclusive.\n" and prompt for input again.
: - When a valid action is read, it is added to the main character's action queue.
*/
void Tavern::actionSelection()
{
    if (combat_queue_.empty())
    {
        return;
    }
    while (!(main_character_->getActionQueue()->empty()))
    {
        main_character_->getActionQueue()->pop();
    }
    if (getTarget())
    {
        std::cout << "YOUR TURN" << std::endl;
        std::cout << "(ENEMY) " << getTarget()->getName() << ": "
                  << "LEVEL " << getTarget()->getLevel() << " " << getTarget()->getRace() << "." << std::endl;
        std::cout << "VITALITY: " << getTarget()->getVitality() << std::endl;
        std::cout << "ARMOR: " << getTarget()->getArmor() << std::endl
                  << std::endl;
    }
    for (int i = 0; i < 3; i++)
    {
        printActionSelection(); // for loop to print the actions three times.
    }
}

/**
Helper Function
*/
void Tavern::printActionSelection() // used to print the actions three times depending on the numbers chosen
{
    int num;
    std::cout << "Choose an action(1-4):" << std::endl;
    std::cout << "1: BUFF_Heal\t\t2: BUFF_MendMetal\t\t3: ATT_Strike\t\t4: ATT_ThrowTomato" << std::endl;
    std::cin >> num;
    while (num < 1 || num > 4) // while loop that keeps printing until a valid number is entered
    {
        std::cout << "Invalid input. Please enter a number between 1 and 4 inclusive." << std::endl;
        std::cin >> num;
    }
    main_character_->getActionQueue()->push(num); // pushes the valid number of 1-4 on to the main character's action queue
}

/**
 @post : If there are no actions in the main character's action queue,
or there are no enemies in the combat queue, do nothing.
Otherwise do all of the following:
1. If the Buff stack is not empty, apply the buff once and decrement the turns.
Any time a Buff's turns_ goes to zero, it is removed
from the stack. Print out the results of the action as described below.
2. While there are actions on the action queue:
- take the next action off the queue
- if the next action is  a buff, apply once and push it onto the buff stack after
decrementing the turns.
BUFF_Heal lasts for 3 turns in total, while BUFF_MendMetal lasts
for 2 turns in total. Print out the results of the action as described below.
- if the next action is an attack, execute it against the enemy at the front of
the combat queue. Print out the results of the action as described below.
- if after this action there are no more enemies to fight, clear the action
queue and return.
3. Print "END OF TURN\n"
: More details:
: After applying each action, print out the results of the action as follows:
: [MAINCHARACTER NAME] used [ACTION NAME]!
: \n(YOU) [MAINCHARACTER NAME]: LEVEL [MAINCHARACTER LEVEL] [MAINCHARACTER RACE]. \nVITALITY: [MAINCHARACTER VITALITY] \nARMOR: [MAINCHARACTER ARMOR]\n
: \n(ENEMY) [ENEMY NAME]: LEVEL [ENEMY LEVEL] [ENEMY RACE]. \nVITALITY: [ENEMY VITALITY] \nARMOR: [ENEMY ARMOR]\n
Where [ACTION NAME] is one of [Heal, MendMetal, Strike, ThrowTomato]
: Whenever an enemy's vitality becomes <=0 after an attack, also print out
"[CHARACTER NAME] DEFEATED\n"
and remove the enemy from the combat queue AND let them exit the tavern.
Then carry out the remaining actions, if any, against the next enemy on the
combat queue.
*/
void Tavern::turnResolution()
{
    if (main_character_->getActionQueue()->empty() || combat_queue_.empty())
    {
        return;
    }
    if (!(main_character_->getBuff()->empty()))
    {
        if (main_character_->getBuff()->top().name_ == "Heal")
        {
            main_character_->heal();                    // applies buff once
            main_character_->getBuff()->top().turns_--; // decrements turns
            if (main_character_->getBuff()->top().turns_ == 0)
            {
                main_character_->getBuff()->pop(); // removes it from buff stack if the turns goes to zero
            }
        }
        else if (main_character_->getBuff()->top().name_ == "MendMetal")
        {
            main_character_->mendMetal();               // applies buff once
            main_character_->getBuff()->top().turns_--; // decrements turns
            if (main_character_->getBuff()->top().turns_ == 0)
            {
                main_character_->getBuff()->pop(); // removes it from buff stack if the turns goes to zero
            }
        }
        printTurnResolutionBuff(); // helper function to print the results of the action
    }
    while (!(main_character_->getActionQueue()->empty()))
    {
        if (getAction(main_character_) == "Heal") // use of if conditionals based on the different actions
        {
            main_character_->heal();                  // applies action once if it is a heal
            Buff Heal = {"Heal", 3};                  // new heal object is created
            Heal.turns_--;                            // decrements turns after main character uses it already once
            main_character_->getBuff()->push(Heal);   // adds the heal object to the buff stack
            printTurnResolutionBuff();                // helper function to print the results of the action
            main_character_->getActionQueue()->pop(); // takes the next action off the queue
        }
        else if (getAction(main_character_) == "MendMetal")
        {
            main_character_->mendMetal();             // applies action once if it is mendmetal
            Buff Metal = {"MendMetal", 2};            // new mendmetal object is created
            Metal.turns_--;                           // decrements turns after main character uses it already once
            main_character_->getBuff()->push(Metal);  // adds the mendmetal object to the buff stack
            printTurnResolutionBuff();                // helper function to print the results of the action
            main_character_->getActionQueue()->pop(); // takes the next action off the queue
        }
        else if (getAction(main_character_) == "Strike")
        {
            main_character_->strike(combat_queue_.front()); // main character uses this action on the first enemy in the combat queue
            printTurnResolution();                          // helper function to print the results of the action
            main_character_->getActionQueue()->pop();       // takes the next action off the queue
            if (combat_queue_.empty())
            {
                while (!(main_character_->getActionQueue()->empty()))
                {
                    main_character_->getActionQueue()->pop(); // after the action if the combat queue is empty, the main character's action queue is cleared
                }
                std::cout << "END OF YOUR TURN" << std::endl;
                return;
            }
        }
        else if (getAction(main_character_) == "ThrowTomato")
        {
            main_character_->throwTomato(combat_queue_.front()); // main character uses this action on the first enemy in the combat queue
            printTurnResolution();                               // helper function to print the results of the action
            main_character_->getActionQueue()->pop();            // takes the next action off the queue
            if (combat_queue_.empty())
            {
                while (!(main_character_->getActionQueue()->empty()))
                {
                    main_character_->getActionQueue()->pop(); // after the action if the combat queue is empty, the main character's action queue is cleared
                }
                std::cout << "END OF YOUR TURN" << std::endl;
                return;
            }
        }
    }
    std::cout << "END OF YOUR TURN" << std::endl;
}

/**
Helper Function
*/
void Tavern::printTurnResolution() // used to print the results of the actions
{
    std::cout << main_character_->getName() << " used " << getAction(main_character_) << "!" << std::endl;
    std::cout << "(YOU)"
              << " " << main_character_->getName() << ":"
              << " LEVEL " << main_character_->getLevel() << " " << main_character_->getRace() << "." << std::endl;
    std::cout << "VITALITY: " << main_character_->getVitality() << std::endl;
    std::cout << "ARMOR: " << main_character_->getArmor() << std::endl
              << std::endl;
    std::cout << "(ENEMY) " << getTarget()->getName() << ":"
              << " LEVEL " << getTarget()->getLevel() << " " << getTarget()->getRace() << "." << std::endl;
    std::cout << "VITALITY: " << getTarget()->getVitality() << std::endl;
    std::cout << "ARMOR: " << getTarget()->getArmor() << std::endl
              << std::endl;
    if (getTarget()->getVitality() <= 0)
    {
        std::cout << getTarget()->getName() << " "
                  << "DEFEATED" << std::endl;
        exitTavern(combat_queue_.front());
        combat_queue_.pop();
    }
}

/**
Helper Function
*/
void Tavern::printTurnResolutionBuff() // used to print the results of the buff objects whether is heal or mendmetal
{
    std::cout << main_character_->getName() << " used " << main_character_->getBuff()->top().name_ << "!" << std::endl;
    std::cout << "(YOU)"
              << " " << main_character_->getName() << ":"
              << " LEVEL " << main_character_->getLevel() << " " << main_character_->getRace() << "." << std::endl;
    std::cout << "VITALITY: " << main_character_->getVitality() << std::endl;
    std::cout << "ARMOR: " << main_character_->getArmor() << std::endl
              << std::endl;
    std::cout << "(ENEMY) " << getTarget()->getName() << ":"
              << " LEVEL " << getTarget()->getLevel() << " " << getTarget()->getRace() << "." << std::endl;
    std::cout << "VITALITY: " << getTarget()->getVitality() << std::endl;
    std::cout << "ARMOR: " << getTarget()->getArmor() << std::endl
              << std::endl;
    if (getTarget()->getVitality() <= 0)
    {
        std::cout << getTarget()->getName() << " "
                  << "DEFEATED" << std::endl;
        exitTavern(combat_queue_.front());
        combat_queue_.pop();
    }
}

/**
Helper Function
*/
std::string Tavern::getAction(Character *name) const // used to get the action name that the character or enemy executed based on the first value of their action queue
{
    if (!(name->getActionQueue()->empty()))
    {
        if (name->getActionQueue()->front() == 1)
        {
            return "Heal";
        }
        else if (name->getActionQueue()->front() == 2)
        {
            return "MendMetal";
        }
        else if (name->getActionQueue()->front() == 3)
        {
            return "Strike";
        }
        else if (name->getActionQueue()->front() == 4)
        {
            return "ThrowTomato";
        }
    }
}

/**
 @param: A pointer to the enemy
 @post: Do all of the following:
: 1. If the enemy's buff stack is not empty, apply buff once and decrement the
turns. Any time a Buff's turns_ goes to zero, it is removed
from the stack. Print out the results of the action as described below.
: 2. Pick a random Action
(one of BUFF_Heal, BUFF_MendMetal, ATT_Strike, ATT_ThrowTomato).
: If the selected action is a buff, apply once immediately and add it to the
enemy's buff stack, performing the necessary turns update.
Print out the results of the action as described below.
: If the selected action is an attack, execute it against the main character.
Print out the results of the action as described below.
: More details:
: After applying each action, print out the results of the action as follows:
: [ENEMY NAME] used [ACTION NAME]!
: \n(ENEMY) [ENEMY NAME]: LEVEL [ENEMY LEVEL] [ENEMY RACE]. \nVITALITY: [ENEMY VITALITY] \nARMOR: [ENEMY ARMOR]\n
: \n(YOU) [MAINCHARACTER NAME]: LEVEL [MAINCHARACTER LEVEL] [MAINCHARACTER RACE]. \nVITALITY: [MAINCHARACTER VITALITY] \nARMOR: [MAINCHARACTER ARMOR]\n
*/
void Tavern::enemyTurn(Character *enemy)
{
    int random = (int)time(nullptr); // converts time to an int value based on current time
    srand(random);                   // sets the initial state of numbers to be randomly generated
    int randomNumber = rand() % 4;   // uses random number generator to generate random number between 0 and 3.
    if (enemy == nullptr || main_character_ == nullptr)
    {
        return;
    }
    std::cout << "ENEMY TURN" << std::endl;
    if (!(enemy->getBuff()->empty()))
    {
        if (enemy->getBuff()->top().name_ == "Heal")
        {
            enemy->heal();                    // applies buff once
            enemy->getBuff()->top().turns_--; // decrements turns
            if (enemy->getBuff()->top().turns_ == 0)
            {
                enemy->getBuff()->pop(); // removes it from buff stack if the turns goes to zero
            }
        }
        else if (enemy->getBuff()->top().name_ == "MendMetal")
        {
            enemy->mendMetal();               // applies buff once
            enemy->getBuff()->top().turns_--; // decrements turns
            if (enemy->getBuff()->top().turns_ == 0)
            {
                enemy->getBuff()->pop(); // removes it from buff stack if the turns goes to zero
            }
        }
        printEnemyBuffTurn(enemy); // helper function to print the results of the action
    }
    if (randomNumber == 0) // use of if conditionals based on the whatever number is randomly generated from 0-3
    {
        enemy->heal();                // applies action once if it is a heal
        Buff Heal = {"Heal", 3};      // new heal object is created
        Heal.turns_--;                // decrements turns after enemy uses it already once
        enemy->getBuff()->push(Heal); // adds the heal object to the buff stack
        printEnemyTurn(enemy);        // helper function to print the results of the action
    }
    else if (randomNumber == 1)
    {
        enemy->mendMetal();            // applies action once if it is mendmetal
        Buff Metal = {"MendMetal", 2}; // new mendmetal object is created
        Metal.turns_--;                // decrements turns after enemy uses it already once
        enemy->getBuff()->push(Metal); // adds the heal object to the buff stack
        printEnemyTurn(enemy);         // helper function to print the results of the action
    }
    else if (randomNumber == 2)
    {
        enemy->strike(main_character_); // enemy uses this action on the main character
        printEnemyTurn(enemy);          // helper function to print the results of the action
    }
    else if (randomNumber == 3)
    {
        enemy->throwTomato(main_character_); // enemy uses this action on the main character
        printEnemyTurn(enemy);               // helper function to print the results of the action
    }
    std::cout << "END OF ENEMY TURN" << std::endl
              << std::endl;
}

/**
Helper Function
*/
void Tavern::printEnemyTurn(Character *enemy) // // used to print the results of the actions of the enemy
{
    std::cout << enemy->getName() << " used " << getAction(enemy) << "!" << std::endl;
    std::cout << "(ENEMY)"
              << " " << enemy->getName() << ":"
              << " LEVEL " << enemy->getLevel() << " " << enemy->getRace() << "." << std::endl;
    std::cout << "VITALITY: " << enemy->getVitality() << std::endl;
    std::cout << "ARMOR: " << enemy->getArmor() << std::endl
              << std::endl;
    std::cout << "(YOU) " << main_character_->getName() << ":"
              << " LEVEL " << main_character_->getLevel() << " " << main_character_->getRace() << "." << std::endl;
    std::cout << "VITALITY: " << main_character_->getVitality() << std::endl;
    std::cout << "ARMOR: " << main_character_->getArmor() << std::endl;
}

/**
Helper Function
*/
void Tavern::printEnemyBuffTurn(Character *enemy) // used to print the results of the buff objects whether is heal or mendmetal of the enemy
{
    std::cout << enemy->getName() << " used " << enemy->getBuff()->top().name_ << "!" << std::endl;
    std::cout << "(ENEMY)"
              << " " << enemy->getName() << ":"
              << " LEVEL " << enemy->getLevel() << " " << enemy->getRace() << "." << std::endl;
    std::cout << "VITALITY: " << enemy->getVitality() << std::endl;
    std::cout << "ARMOR: " << enemy->getArmor() << std::endl
              << std::endl;
    std::cout << "(YOU) " << main_character_->getName() << ":"
              << " LEVEL " << main_character_->getLevel() << " " << main_character_->getRace() << "." << std::endl;
    std::cout << "VITALITY: " << main_character_->getVitality() << std::endl;
    std::cout << "ARMOR: " << main_character_->getArmor() << std::endl;
}

/**
 @post : While the main character's vitality is > 0, and there are still enemies
in the combat queue, do the following:
:- Call actionSelection()
- Call turnResolution()
- Call enemyTurn() with a pointer to the first enemy on combat queue.
: If the main character's vitality has fallen to 0, print "YOU HAVE PERISHED."
: If there are no more enemies in the combat queue, print "NO MORE ENEMIES."
*/
void Tavern::combat()
{
    while ((main_character_->getVitality() > 0) && !(combat_queue_.empty()))
    {
        actionSelection();
        turnResolution();
        enemyTurn(combat_queue_.front());
    }
    if (main_character_->getVitality() == 0)
    {
        std::cout << "YOU HAVE PERISHED.";
    }
    if ((combat_queue_.empty()))
    {
        std::cout << "NO MORE ENEMIES.";
    }
}