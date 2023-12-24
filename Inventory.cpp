/*
 * File Title: Inventory.cpp
 * Author: Mahim Ali
 * Date: November 30, 2023
 * This file implements a class called Inventory
 */
#include "Inventory.hpp"

/**
 * @param : A Pointer to the Item object to be added to the inventory.
 * @post  : The Item is added to the inventory, preserving the BST structure. The BST property is based on (ascending) alphabetical order of the item's name.
 *          If the item is type UNKNOWN, WEAPON, or ARMOR and is already in the inventory, it is not added.
     However, if another instance of an Item of type CONSUMABLE is being added (an item with the same name), its quantity is updated to the sum of the quantities of the two objects, and the time_picked_up_ variable is updated to that of the latest instance of the item being added.
 * @return true if the item was added to the inventory or updated, false otherwise.
 */
bool Inventory::addItem(Item *add)
{
     if (((add->getType() == "UNKNOWN") || (add->getType() == "WEAPON") || (add->getType() == "ARMOR")) && contains(add))
     {
          return false;
     }
     if (contains(add))
     {
          if (add->getType() == "CONSUMABLE")
          {
               findItem(add->getName())->setQuantity(add->getQuantity() + findItem(add->getName())->getQuantity());
               findItem(add->getName())->updateTimePickedUp();
               return true;
          }
     }
     else
     {
          this->add(add);
          return true;
     }
     return false;
}

/**
 * @param   : A reference to string name of the item to be found.
 * @return  : An Item pointer to the found item, or nullptr if the item is not in the inventory.
 */
Item *Inventory::findItem(const std::string &find) const
{
     Item *name = new Item(find, UNKNOWN, 0, 0, 0);
     if (contains(name))
     {
          return getPointerTo(name)->getItem();
     }
     return nullptr;
}

/**
* @param   : A reference to string name of the item to be removed from the inventory.
* @return  : True if the item was found and removed or updated successfully, false otherwise.
* @post    : If the item is found in the inventory, it is removed while preserving the BST structure.
If a CONSUMABLE is removed, its quantity is decremented by one, but its time_picked_up_ remains the same. However, if it is the last item (it's quantity is 1 and is being removed), the item should be removed. Non-CONSUMABLE items should always be removed when they are found.
*/
bool Inventory::removeItem(const std::string &remove)
{
     Item *name = new Item(remove, UNKNOWN, 0, 0, 0);
     if (contains(name))
     {
          if (findItem(name->getName())->getType() != "CONSUMABLE")
          {
               this->remove(name);
               return true;
          }
          else if (findItem(name->getName())->getType() == "CONSUMABLE")
          {
               if (findItem(name->getName())->getQuantity() == 1)
               {
                    this->remove(name);
                    return true;
               }
               else
               {
                    findItem(name->getName())->setQuantity(findItem(name->getName())->getQuantity() - 1);
                    return true;
               }
          }
     }
     return false;
}
/**
 * @return: The total sum of gold values of all items in the inventory.
Note: consumable items may have quantity >1 and gold value must be
added for each item
 */
int Inventory::getTotalGoldValue()
{
     return goldItem(getRoot());
}

// Helper Function
int Inventory::goldItem(std::shared_ptr<BinaryNode<Item *>> Gold)
{
     if (Gold == nullptr)
     {
          return 0;
     }
     if (Gold->getItem()->getType() == "CONSUMABLE" && Gold->getItem()->getQuantity() > 1)
     {
          return (Gold->getItem()->getGoldValue() * Gold->getItem()->getQuantity()) + (goldItem(Gold->getLeftChildPtr())) + (goldItem(Gold->getRightChildPtr()));
     }
     return (Gold->getItem()->getGoldValue()) + (goldItem(Gold->getLeftChildPtr())) + (goldItem(Gold->getRightChildPtr()));
}
/**
* @post: The names of all items in the Inventory are printed in ascending order.
* This function performs an in-order traversal of the binary search tree and prints the details of each item in the following format.
NOTE: QUANTITY ONLY NEEDS TO BE DISPLAYED FOR CONSUMABLE ITEMS.

* [NAME] ([TYPE])
Level: [LEVEL]
Value: [VALUE]
Quantity: [QUANTITY]

Example:
TIRED GAUNTLETS (ARMOR)
Level: 3
Value: 25

SMALL HEALTH POTION (CONSUMABLE)
Level: 1
Value: 10
Quantity: 2
*
*/
void Inventory::printInventoryInOrder()
{
     std::vector<Item *> my_vector;
     inorderTraversal(my_vector, getRoot());
     std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on name ascending
               { return a->operator<(*b); });
     for (int i = 0; i < my_vector.size(); i++)
     {
          printItemDetails(my_vector[i]);
     }
}

// Helper Function
void Inventory::printItemDetails(Item *item)
{
     std::cout << item->getName() << " "
               << "(" << item->getType() << ")" << std::endl;
     std::cout << "Level: " << item->getLevel() << std::endl;
     std::cout << "Value: " << item->getGoldValue() << std::endl;
     if (item->getType() == "CONSUMABLE")
     {
          std::cout << "Quantity: " << item->getQuantity() << std::endl;
     }
     std::cout << std::endl;
}

// Helper Function
void Inventory::inorderTraversal(std::vector<Item *> &my_vector, std::shared_ptr<BinaryNode<Item *>> name)
{
     if (name == NULL)
     {
          return;
     }
     inorderTraversal(my_vector, name->getLeftChildPtr());
     my_vector.push_back(name->getItem());
     inorderTraversal(my_vector, name->getRightChildPtr());
}

/**
* @param : a reference to bool if the items are to be printed in ascending order.
* @param : a reference to string attribute which defines the order by which the items are to be printed. You may assume that the given parameter will be in one of the following forms: ["NAME"/"TYPE"/"LEVEL"/"VALUE"/"TIME"]
* @post: All items in the Inventory are printed in the order specified by the parameter.
* Example usage: inventory.printInventory(false, "LEVEL");
NOTE: QUANTITY ONLY NEEDS TO BE DISPLAYED FOR CONSUMABLE ITEMS:

FEATHER DUSTER (WEAPON)
Level: 5
Value: 100

TIRED GAUNTLETS (ARMOR)
Level: 3
Value: 50

SMALL HEALTH POTION (CONSUMABLE)
Level: 1
Value: 10
Quantity: 2

NOODLES (CONSUMABLE)
Level: 0
Value: 120
Quantity: 5
*/
void Inventory::printInventory(const bool &print, const std::string &inventory)
{
     if (inventory == "NAME")
     {
          if (print)
          {
               printInventoryInOrderAscending("NAME");
          }
          else
          {
               printInventoryInOrderDescending("NAME");
          }
     }
     else if (inventory == "TYPE")
     {
          if (print)
          {
               printInventoryInOrderAscending("TYPE");
          }
          else
          {
               printInventoryInOrderDescending("TYPE");
          }
     }
     else if (inventory == "LEVEL")
     {
          if (print)
          {
               printInventoryInOrderAscending("LEVEL");
          }
          else
          {
               printInventoryInOrderDescending("LEVEL");
          }
     }
     else if (inventory == "VALUE")
     {
          if (print)
          {
               printInventoryInOrderAscending("VALUE");
          }
          else
          {
               printInventoryInOrderDescending("VALUE");
          }
     }
     else if (inventory == "TIME")
     {
          if (print)
          {
               printInventoryInOrderAscending("TIME");
          }
          else
          {
               printInventoryInOrderDescending("TIME");
          }
     }
}

// Helper Function
void Inventory::printInventoryInOrderDescending(const std::string &attribute)
{
     std::vector<Item *> my_vector;
     inorderTraversal(my_vector, getRoot());
     if (attribute == "NAME")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on name descending
                    { return a->operator>(*b); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
     else if (attribute == "TYPE")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on type descending
                    { return a->getType() > b->getType(); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
     else if (attribute == "LEVEL")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on level descending
                    { return a->getLevel() > b->getLevel(); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
     else if (attribute == "VALUE")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on value descending
                    { return a->getGoldValue() > b->getGoldValue(); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
     else if (attribute == "TIME")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on time descending
                    { return a->getTimePickedUp() > b->getTimePickedUp(); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
}

// Helper Function
void Inventory::printInventoryInOrderAscending(const std::string &attribute)
{
     std::vector<Item *> my_vector;
     inorderTraversal(my_vector, getRoot());
     if (attribute == "NAME")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on name ascending
                    { return a->operator<(*b); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
     else if (attribute == "TYPE")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on type ascending
                    { return a->getType() < b->getType(); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
     else if (attribute == "LEVEL")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on level ascending
                    { return a->getLevel() < b->getLevel(); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
     else if (attribute == "VALUE")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on value ascending
                    { return a->getGoldValue() < b->getGoldValue(); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
     else if (attribute == "TIME")
     {
          std::sort(my_vector.begin(), my_vector.end(), [this](Item *a, Item *b) // use of lambda expression to sort based on time ascending
                    { return a->getTimePickedUp() < b->getTimePickedUp(); });
          for (int i = 0; i < my_vector.size(); i++)
          {
               printItemDetails(my_vector[i]);
          }
     }
}