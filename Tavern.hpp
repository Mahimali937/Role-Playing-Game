/*
 * File Title: Tavern.hpp
 * Author: Mahim Ali
 * Date: November 14, 2023
 * This file is the interface of a class called Tavern
 */
#ifndef TAVERN_
#define TAVERN_

#include "ArrayBag.hpp"
#include "Character.hpp"
#include "Mage.hpp"
#include "Barbarian.hpp"
#include "Ranger.hpp"
#include "Scoundrel.hpp"
#include <vector>
#include <iostream>
#include <cmath>
#include <iomanip>
#include <fstream>
#include <sstream>

class Tavern : public ArrayBag<Character *>
{
public:
  /** Default Constructor **/
  Tavern();

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
  Tavern(const std::string &input_file_name);
  /**
      @param:   A reference to a Character entering the Tavern
      @return:  returns true if a Character was successfully added to items_, false otherwise
      @post:    adds Character to the Tavern and updates the level sum and the enemy count if the character is an enemy.
  **/
  bool enterTavern(Character *a_character);

  /** @param:   A reference to a Character leaving the Tavern
      @return:  returns true if a character was successfully removed from items_, false otherwise
      @post:    removes the character from the Tavern and updates the level sum  and the enemy if the character is an enemy.
  **/
  bool exitTavern(Character *a_character);

  /**
  @return:  The integer level count of all the characters currently in the Tavern
  **/
  int getLevelSum();

  /**
      @return:  The average level of all the characters in the Tavern
      @post:    Considers every character currently in the Tavern, updates the average level of the Tavern rounded to the NEAREST integer,
                and returns the integer value.
  **/
  int calculateAvgLevel();

  /**
  @return:  The integer enemy count of the Tavern
  **/
  int getEnemyCount();

  /**
      @return:  The percentage (double) of all the enemy characters in the Tavern
      @post:    Considers every character currently in the Tavern, updates the enemy percentage of the Tavern rounded to 2 decimal places,
               and returns the double value.
  **/
  double calculateEnemyPercentage();

  /** @param:   A string reference to a race
      @return:  An integer tally of the number of characters in the Tavern of the given race
  **/
  int tallyRace(const std::string &race);

  /**
    @post:    Outputs a report of the characters currently in the tavern in the form:
    "Humans: [x] \nElves: [x] \nDwarves: [x] \nLizards: [x] \nUndead: [x] \n\nThe average level is: [x] \n[x]% are enemies.\n"
    Note that the average level should be rounded to the NEAREST integer, and the enemy percentage should be rounded to 2 decimal

    Example output:
    Humans: 3
    Elves: 5
    Dwarves: 8
    Lizards: 6
    Undead: 0

    The average level is: 7
    46.67% are enemies.
  */
  void tavernReport();

  /**
    @post: For every character in the tavern, displays each character's information
  */
  void displayCharacters();

  /**
    @param: a string reference to a race
    @post: For every character in the tavern of the given race (only exact matches to the input string), displays each character's information

  */
  void displayRace(const std::string &race);

  /**
    @post: Every character in the tavern eats a tainted stew.
  */
  void taintedStew();

  /**
    @param: a Character pointer
    @post: sets the Tavern's main character to the pointer that is given
    */
  void setMainCharacter(Character *main);

  /**
  @return: a pointer to the main character in the Tavern, nullptr if there is no main character
  */
  Character *getMainCharacter() const;

  /**
  @param: two Character pointers
  @return: true if the lhs Character's level is less than the rhs Character's level
  */
  bool levelIsLess(Character *lhs, Character *rhs) const;

  /**
  @param: two Character pointers
  @return: true if the lhs Character's level is greater than the rhs Character's level
  */
  bool levelIsGreater(Character *lhs, Character *rhs) const;

  /**
  @param: two Character pointers
  @return: true if the lhs Character's vitality is less than the rhs Character's vitality
  */
  bool vitalityIsLess(Character *lhs, Character *rhs) const;

  /**
  @param: two Character pointers
  @return: true if the lhs Character's vitality is more than the rhs Character's vitality
  */
  bool vitalityIsGreater(Character *lhs, Character *rhs) const;

  /**
  @pre  : The combat queue should be emptied before the characters are added to the queue
  @param  : A string reference to a sorting filter with a default value of "NONE"
  @post   : With default filter "NONE": add every character marked as an enemy to the combat queue
  : With the filter "LVLASC": add every character in the Tavern marked as an enemy to the combat queue, in ascending order based on their level.
  : With the filter "LVLDES": add every character in the Tavern marked as an enemy to the combat queue, in descending order based on their level.
  : With the filter "HPASC": add every character in the Tavern marked as an enemy to the combat queue, in ascending order based on their vitality.
  : With the filter "HPDES": add every character in the Tavern marked as an enemy to the combat queue, in descending order based on their vitality.
  */
  void createCombatQueue(const std::string &combat_filter = "NONE");

  /**
  @post   : returns a pointer to the Character at the front of the Combat Queue.
          : If there are no characters in the queue, return nullptr
  */
  Character *getTarget() const;

  /**
  @post : Prints the details of each character in the combat queue in the following form:
        : [ENEMY NAME]: LEVEL [ENEMY LEVEL] [ENEMY RACE]. \nVITALITY: [ENEMY VITALITY] \nARMOR: [ENEMY ARMOR]\n
  */
  void printCombatQueue();

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
  : - When a valid action is read,
  it is added to the main character's action queue.
  */
  void actionSelection();

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

  :   Whenever an enemy's vitality becomes <=0 after an attack, also print out
  "[CHARACTER NAME] DEFEATED\n"
  and remove the enemy from the combat queue AND let them exit the tavern.
  Then carry out the remaining actions, if any, against the next enemy on the
  combat queue.
  */
  void turnResolution();

  /**
  @param  : A pointer to the enemy
  @post   : Do all of the following:
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
  void enemyTurn(Character *enemy);

  /**
  @post : While the main character's vitality is > 0, and there are still enemies
  in the combat queue, do the following:
  :- Call actionSelection()
  - Call turnResolution()
  - Call enemyTurn() with a pointer to the first enemy on combat queue.
  : If the main character's vitality has fallen to 0, print "YOU HAVE PERISHED."
  : If there are no more enemies in the combat queue, print "NO MORE ENEMIES."
  */
  void combat();

  // Helper functions
  void printActionSelection();
  void printTurnResolution();
  void printTurnResolutionBuff();
  std::string getAction(Character *name) const;
  void printEnemyTurn(Character *enemy);
  void printEnemyBuffTurn(Character *enemy);

private:
  int level_sum_;                        // An integer sum of the levels of all the characters currently in the tavern
  int num_enemies_;                      // number of enemies currently in the Tavern
  std::string input_file_name_;          // The name of the input file
  Character *main_character_;            // A pointer to a main character.
  std::queue<Character *> combat_queue_; // A combat queue of Character pointers, enemies that the main character must fight.
};
#endif
