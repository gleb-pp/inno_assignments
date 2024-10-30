#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>

// Open output.txt for printing the info
// output file is global, so we can write to it from all the functions
std::ofstream outputFile("output.txt");

// Possible types of the characters
enum CharType {
    F,
    A,
    W
};

// Function for transforming CharType to string (when printing the characters)
std::string charTypeToString(CharType type) {
    switch (type) {
        case F:
            return "fighter";
        case A:
            return "archer";
        default:
            return "wizard";
    }
}

// later I will put "this" pointer as an argument to the function,
// and to use smart pointer I will need to convert "this" pointer to shared
class Character : public std::enable_shared_from_this<Character> {
private:
    int hp;
    std::string name;
    CharType type;

public:
    Character(int h, const std::string& n, CharType t) : hp(h), name(n), type(t) {}

    // virtual destructor to call child's desctructor
    virtual ~Character() {}

    std::string getName() const {
        return name;
    }

    int getHP() const {
        return hp;
    }

    CharType getCharType() const {
        return type;
    }

    // function that damages the character by given parameter and return true if it dies
    bool damage(int d) {
        hp -= d;
        return hp <= 0;
    }

    // function that heal the character by given parameter
    void heal(int h) {
        hp += h;
    }
};

class PhysicalItem {
private:
    std::shared_ptr<const Character> owner;
    std::string name;
public:
    PhysicalItem(std::shared_ptr<const Character> o, const std::string& n) : owner(o), name(n) {}

    std::string getName() const {
        return name;
    }

    std::shared_ptr<const Character> getOwner() const {
        return owner;
    }
};

class Weapon : public PhysicalItem {
private:
    int damage;
public:
    Weapon(std::shared_ptr<const Character> owner, const std::string& name, int d) : PhysicalItem(owner, name), damage(d) {}

    int getDamage() const {
        return damage;
    }
};

class Potion : public PhysicalItem {
private:
    int healValue;
public:
    Potion(std::shared_ptr<const Character> owner, const std::string& name, int hv) : PhysicalItem(owner, name), healValue(hv) {}

    int getHealValue() const {
        return healValue;
    }
};

class SpellSheet : public PhysicalItem {
private:
    std::vector<std::shared_ptr<const Character>> allowedTargets;
public:
    SpellSheet(std::shared_ptr<const Character> owner, const std::string& name, const std::vector<std::shared_ptr<const Character>>& ch) : PhysicalItem(owner, name), allowedTargets(ch) {}

    int getNumAllowedTargets() const {
        return allowedTargets.size();
    }

    // function that checks whether the given Character allowed target or not by comparing their names
    bool targetAllowed(const std::shared_ptr<const Character> c) const {
        for (const std::shared_ptr<const Character> check : allowedTargets) {
            if (c->getName() == check->getName()) {
                return true;
            }
        }
        return false;
    }
};

// creating a concept for objects derived from the class PhysicalItem
template<class T>
concept Derived = std::is_base_of<PhysicalItem, T>::value;

// Usual class Container
template<class T>
class Container {
protected:
    std::vector<T> elements;  // stores elements in the vector
    int maxSize = 0;
public:
    // Constructor get the maximum capacity and put it in the special variable
    Container(int n) : maxSize(n) {}

    void addItem(const T& item) {
        elements.push_back(item);
    }

    void removeItem(const T& item) {
        return;
    }

    bool isFull() const {
        return elements.size() == maxSize;
    }

    // show() function will be used to print the items, but it's implementations is defferent for different types
    virtual void show() = 0;
};

// Class Container for objects derived from PhysicalItem
template<class T> requires Derived<PhysicalItem>
class Container<T> {
protected:
    std::map<std::string, T> elements;   // stores elements in the maps
    int maxSize = 0;
public:
    Container(int n) : maxSize(n) {}

    // these two functions are copied from the usual class Constructor
    void addItem(const T& item) {
        elements.insert({ item.getName(), item });
    }

    void removeItem(const T& item) {
        return;
    }

    // this function is new, it allows us to remove the item by its name
    void removeItemName(const std::string& n) {
        elements.erase(n);
    }

    // this function is new, it allows us to get the item by its name
    const T* findItemName(const std::string& n) const {
        auto it = elements.find(n);
        return &(it->second);
    }

    // this function is new, it allows us to understand whether the container contains the given element or not by its name 
    bool containsName(const std::string& n) const {
        auto it = elements.find(n);
        if (it == elements.end()) {
            return false;
        } else {
            return true;
        }
    }

    // these two functions are copied from the usual class Constructor
    bool isFull() const {
        return elements.size() == maxSize;
    }

    virtual void show() = 0;
};

// class Arsenal derived from Containter allows us to contain the weapons
class Arsenal : public Container<Weapon> {
public:
    Arsenal(int n) : Container(n) {}

    // function that display all the weapons and its damage values in an alphabetical order
    // (no need to sort because map is always contain elements in an alphabetical order)
    void show() {
        for (auto w : elements) {
            outputFile << w.first << ":" << w.second.getDamage() << " ";
        }
        outputFile << std::endl;
    }
};

// class MedicalBag derived from Containter allows us to contain the potions
class MedicalBag  : public Container<Potion> {
public:
    MedicalBag(int n) : Container(n) {}

    // function that display all the potions and its heal values in an alphabetical order
    // (no need to sort because map is always contain elements in an alphabetical order)
    void show() {
        for (auto w : elements) {
            outputFile << w.first << ":" << w.second.getHealValue() << " ";
        }
        outputFile << std::endl;
    }
};

// class SpellBook derived from Containter allows us to contain the spells
class SpellBook : public Container<SpellSheet> {
public:
    SpellBook(int n) : Container(n) {}

    // function that display all the spells and its allowed targets in an alphabetical order
    // (no need to sort because map is always contain elements in an alphabetical order)
    void show() {
        for (auto w : elements) {
            outputFile << w.first << ":" << w.second.getNumAllowedTargets() << " ";
        }
        outputFile << std::endl;
    }
};

class Fighter : public Character {
private:
    Arsenal arsenal;
    MedicalBag medicalBag;
public:
    // Fighter constructor creates a Character (as a base class), arsenal with 3 weapons max, and medical bag with 5 potions max
    Fighter(const std::string& name, int hp) : Character(hp, name, CharType::F), arsenal(Arsenal(3)), medicalBag(MedicalBag(5)) {
        outputFile << "A new fighter came to town, " << name << "." << std::endl;
    };

    // we need destructor to delete arsenal and medical bag after deleting the fighter
    ~Fighter() {}

    // function that gives a new weapon to the fighter or print the error if the arsenal is full
    void giveWeapon(const std::string& n, int d) {
        if (arsenal.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            arsenal.addItem(Weapon(shared_from_this(), n, d));
            outputFile << this->getName() << " just obtained a new weapon called " << n << "." << std::endl;
        }
    }

    // function that gives a new potion to the fighter or print the error if the medical bag is full
    void givePotion(const std::string& n, int hv) {
        if (medicalBag.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            medicalBag.addItem(Potion(shared_from_this(), n, hv));
            outputFile << this->getName() << " just obtained a new potion called " << n << "." << std::endl;
        }
    }

    // function that attack the character with a given weapon or print the error if the arsenal doesn't contain it
    // returns true if the character has die
    bool attack(const std::string& n, std::shared_ptr<Character> ch) const {
        if (arsenal.containsName(n)) {
            outputFile << this->getName() << " attacks " << ch->getName() << " with their " << n << "!" << std::endl;
            return ch->damage(arsenal.findItemName(n)->getDamage());
        } else {
            outputFile << "Error caught" << std::endl;
            return false;
        }
    }

    // function that heals the character with a given potion or print the error if the medical bag doesn't contain it
    void drink(const std::string& n, std::shared_ptr<Character> ch) {
        if (medicalBag.containsName(n)) {
            ch->heal(medicalBag.findItemName(n)->getHealValue());
            outputFile << ch->getName() << " drinks " << n << " from " << this->getName() << "." << std::endl;
            medicalBag.removeItemName(n);
        } else {
            outputFile << "Error caught" << std::endl;
        }
    }

    void showWeapons() {
        arsenal.show();
    }

    void showPotions() {
        medicalBag.show();
    }
};

class Archer : public Character {
private:
    Arsenal arsenal;
    MedicalBag medicalBag;
    SpellBook spellBook;
public:
    // Archer constructor creates a Character (as a base class), arsenal with 2 weapons max, medical bag with 3 potions max, and spell book with 2 spell max
    Archer(const std::string& name, int hp) : Character(hp, name, CharType::A), arsenal(Arsenal(2)), medicalBag(MedicalBag(3)), spellBook(SpellBook(2)) {
        outputFile << "A new archer came to town, " << name << "." << std::endl;
    }

    // we need destructor to delete arsenal, medical bag, and spell book after deleting the arhcer
    ~Archer() {}

    // function that gives a new weapon to the archer or print the error if the arsenal is full
    void giveWeapon(const std::string& n, int d) {
        if (arsenal.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            arsenal.addItem(Weapon(shared_from_this(), n, d));
            outputFile << this->getName() << " just obtained a new weapon called " << n << "." << std::endl;
        }
    }

    // function that gives a new potion to the archer or print the error if the medical bag is full
    void givePotion(const std::string& n, int hv) {
        if (medicalBag.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            medicalBag.addItem(Potion(shared_from_this(), n, hv));
            outputFile << this->getName() << " just obtained a new potion called " << n << "." << std::endl;
        }
    }

    // function that gives a new spell to the archer or print the error if the spell book is full
    void giveSpell(const std::string& n, const std::vector<std::shared_ptr<const Character>>& ch) {
        if (spellBook.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            spellBook.addItem(SpellSheet(shared_from_this(), n, ch));
            outputFile << this->getName() << " just obtained a new spell called " << n << "." << std::endl;
        }
    }

    // function that attack the character with a given weapon or print the error if the arsenal doesn't contain it
    // returns true if the character has die
    bool attack(const std::string& n, std::shared_ptr<Character> ch) const {
        if (arsenal.containsName(n)) {
            outputFile << this->getName() << " attacks " << ch->getName() << " with their " << n << "!" << std::endl;
            return ch->damage(arsenal.findItemName(n)->getDamage());
        } else {
            outputFile << "Error caught" << std::endl;
            return false;
        }
    }

    // function that casts the character with a given spell or print the error if the spell book doesn't contain it
    // returns true if the character has died successfully
    bool cast(const std::string& n, std::shared_ptr<const Character> ch) {
        if (spellBook.containsName(n)) {
            if (spellBook.findItemName(n)->targetAllowed(ch)) {
                spellBook.removeItemName(n);
                outputFile << this->getName() << " casts " << n << " on " << ch->getName() << "!" << std::endl;
                return true;
            } else {
                outputFile << "Error caught" << std::endl;
                return false;
            }
        } else {
            outputFile << "Error caught" << std::endl;
            return false;
        }
    }

    // function that heals the character with a given potion or print the error if the medical bag doesn't contain it
    void drink(const std::string& n, std::shared_ptr<Character> ch) {
        if (medicalBag.containsName(n)) {
            ch->heal(medicalBag.findItemName(n)->getHealValue());
            outputFile << ch->getName() << " drinks " << n << " from " << this->getName() << "." << std::endl;
            medicalBag.removeItemName(n);
        } else {
            outputFile << "Error caught" << std::endl;
        }
    }

    void showWeapons() {
        arsenal.show();
    }

    void showPotions() {
        medicalBag.show();
    }

    void showSpells() {
        spellBook.show();
    }
};

class Wizard : public Character {
private:
    SpellBook spellBook;
    MedicalBag medicalBag;
public:
    // Wizard constructor creates a Character (as a base class), medical bag with 10 potions max, and spell book with 10 spell max
    Wizard(const std::string& name, int hp) : Character(hp, name, CharType::W), spellBook(SpellBook(10)), medicalBag(MedicalBag(10)) {
        outputFile << "A new wizard came to town, " << name << "." << std::endl;
    }

    // we need destructor to delete medical bag and spell book after deleting the wizard
    ~Wizard() {}

    // function that gives a new potion to the wizard or print the error if the medical bag is full
    void givePotion(const std::string& n, int hv) {
        if (medicalBag.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            medicalBag.addItem(Potion(shared_from_this(), n, hv));
            outputFile << this->getName() << " just obtained a new potion called " << n << "." << std::endl;
        }
    }

    // function that gives a new spell to the wizard or print the error if the spell book is full
    void giveSpell(const std::string& n, const std::vector<std::shared_ptr<const Character>>& ch) {
        if (spellBook.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            spellBook.addItem(SpellSheet(shared_from_this(), n, ch));
            outputFile << this->getName() << " just obtained a new spell called " << n << "." << std::endl;
        }
    }

    // function that casts the character with a given spell or print the error if the spell book doesn't contain it
    // returns true if the character has died successfully
    bool cast(const std::string& n, std::shared_ptr<const Character> ch) {
        if (spellBook.containsName(n)) {
            if (spellBook.findItemName(n)->targetAllowed(ch)) {
                spellBook.removeItemName(n);
                outputFile << this->getName() << " casts " << n << " on " << ch->getName() << "!" << std::endl;
                return true;
            } else {
                outputFile << "Error caught" << std::endl;
                return false;
            }
        } else {
            outputFile << "Error caught" << std::endl;
            return false;
        }
    }

    // function that heals the character with a given potion or print the error if the medical bag doesn't contain it
    void drink(const std::string& n, std::shared_ptr<Character> ch) {
        if (medicalBag.containsName(n)) {
            ch->heal(medicalBag.findItemName(n)->getHealValue());
            outputFile << ch->getName() << " drinks " << n << " from " << this->getName() << "." << std::endl;
            medicalBag.removeItemName(n);
        } else {
            outputFile << "Error caught" << std::endl;
        }
    }

    void showPotions() {
        medicalBag.show();
    }

    void showSpells() {
        spellBook.show();
    }
};

int main() {
    // opening the input file
    std::ifstream inputFile("input.txt");
    // creating a map for storing the characters
    std::map<std::string, std::shared_ptr<Character>> players;

    int n;
    inputFile >> n;
    for (int i = 0; i < n; i++) {
        // reading the first word in the input file
        std::string word;
        inputFile >> word;

        if (word == "Create") {
            // reading the second word in the input file
            inputFile >> word;

            if (word == "character") {
                std::string type, name;
                int hp;
                inputFile >> type >> name >> hp;

                // adding the new character to the map
                if (type == "fighter") {
                    players[name] = std::make_unique<Fighter>(name, hp);
                } else if (type == "wizard") {
                    players[name] = std::make_unique<Wizard>(name, hp);
                } else if (type == "archer") {
                    players[name] = std::make_unique<Archer>(name, hp);
                } else {
                    outputFile << "Error caught" << std::endl;
                }

            } else if (word == "item") {
                std::string type;
                inputFile >> type;

                if (type == "weapon") {
                    std::string ownerName, weaponName;
                    int damageValue;
                    inputFile >> ownerName >> weaponName >> damageValue;
                    if (damageValue <= 0) {
                        outputFile << "Error caught" << std::endl;
                        continue;
                    }

                    // searching for the weapon owner and throwing error if we couldn't find him
                    auto it = players.find(ownerName);
                    if (it == players.end()) {
                        outputFile << "Error caught" << std::endl;
                        continue;
                    }

                    // giving weapon to the character according to his type
                    Character* character = it->second.get();
                    if (character->getCharType() == CharType::F) {
                        static_cast<Fighter*>(character)->giveWeapon(weaponName, damageValue);
                    } else if (character->getCharType() == CharType::A) {
                        static_cast<Archer*>(character)->giveWeapon(weaponName, damageValue);
                    } else {
                        outputFile << "Error caught" << std::endl;
                    }

                } else if (type == "potion") {
                    std::string ownerName, potionName;
                    int healValue;
                    inputFile >> ownerName >> potionName >> healValue;
                    if (healValue <= 0) {
                        outputFile << "Error caught" << std::endl;
                        continue;
                    }

                    // searching for the potion owner and throwing error if we couldn't find him
                    auto it = players.find(ownerName);
                    if (it == players.end()) {
                        outputFile << "Error caught" << std::endl;
                        continue;
                    }

                    // giving potion to the character according to his type
                    Character* character = it->second.get();
                    if (character->getCharType() == CharType::F) {
                        static_cast<Fighter*>(character)->givePotion(potionName, healValue);
                    } else if (character->getCharType() == CharType::A) {
                        static_cast<Archer*>(character)->givePotion(potionName, healValue);
                    } else {
                        static_cast<Wizard*>(character)->givePotion(potionName, healValue);
                    }

                } else if (type == "spell") {
                    std::string ownerName, spellName;
                    int m;
                    inputFile >> ownerName >> spellName >> m;
                    std::vector<std::shared_ptr<const Character>> chars;

                    // reading the list of allowed targets
                    bool full = true;
                    for (int i = 0; i < m; i++) {
                        std::string target;
                        inputFile >> target;
                        if (full) {
                            auto it = players.find(target);
                            // if we haven't found the target, we print the error and mark flag as false
                            if (it == players.end()) {
                                outputFile << "Error caught" << std::endl;
                                full = false;
                            } else {
                                chars.push_back(it->second);
                            }
                        }
                    }
                    // if only we found all the targets, we continue
                    if (full) {
                        // searching for the spell owner and throwing error if we couldn't find him
                        auto it = players.find(ownerName);
                        if (it == players.end()) {
                            outputFile << "Error caught" << std::endl;
                            continue;
                        }
                        // giving spell to the character according to his type
                        Character* character = it->second.get();
                        if (character->getCharType() == CharType::W) {
                            static_cast<Wizard*>(character)->giveSpell(spellName, chars);
                        } else if (character->getCharType() == CharType::A) {
                            static_cast<Archer*>(character)->giveSpell(spellName, chars);
                        } else {
                            outputFile << "Error caught" << std::endl;
                        }
                    }
                } else {
                    outputFile << "Error caught" << std::endl;
                }
            }

        } else if (word == "Attack") {
            std::string attackerName, targetName, weaponName;
            inputFile >> attackerName >> targetName >> weaponName;
            // searching for the weapon owner and for the target and throwing error if we couldn't find at least one of them
            auto it1 = players.find(attackerName);
            auto it2 = players.find(targetName);
            if (it1 == players.end() || it2 == players.end()) {
                outputFile << "Error caught" << std::endl;
                continue;
            }
            // calling the attack function according to the attacker's type
            Character* character = it1->second.get();
            if (character->getCharType() == CharType::F) {
                // if the character died, delete him 
                if (static_cast<Fighter*>(character)->attack(weaponName, it2->second)) {
                    outputFile << (it2->second.get())->getName() << " has died..." << std::endl;
                    it2 = players.erase(it2);
                }
            } else if (character->getCharType() == CharType::A) {
                // if the character died, delete him 
                if (static_cast<Archer*>(character)->attack(weaponName, it2->second)) {
                    outputFile << (it2->second.get())->getName() << " has died..." << std::endl;
                    it2 = players.erase(it2);
                }
            } else {
                outputFile << "Error caught" << std::endl;
            }

        } else if (word == "Cast") {
            std::string casterName, targetName, spellName;
            inputFile >> casterName >> targetName >> spellName;
            // searching for the spell owner and for the target and throwing error if we couldn't find at least one of them
            auto it1 = players.find(casterName);
            auto it2 = players.find(targetName);
            if (it1 == players.end() || it2 == players.end()) {
                outputFile << "Error caught" << std::endl;
                continue;
            }
            // calling the cast function according to the caster's type
            Character* character = it1->second.get();
            if (character->getCharType() == CharType::W) {
                // if the character died successfully, delete him 
                if (static_cast<Wizard*>(character)->cast(spellName, it2->second)) {
                    outputFile << (it2->second.get())->getName() << " has died..." << std::endl;
                    it2 = players.erase(it2);
                }
            } else if (character->getCharType() == CharType::A) {
                // if the character died successfully, delete him 
                if (static_cast<Archer*>(character)->cast(spellName, it2->second)) {
                    outputFile << (it2->second.get())->getName() << " has died..." << std::endl;
                    it2 = players.erase(it2);
                }
            } else {
                outputFile << "Error caught" << std::endl;
            }

        } else if (word == "Drink") {
            std::string supplierName, drinkerName, potionName;
            inputFile >> supplierName >> drinkerName >> potionName;
            // searching for the potion owner and for the target and throwing error if we couldn't find at least one of them
            auto it1 = players.find(supplierName);
            auto it2 = players.find(drinkerName);
            if (it1 == players.end() || it2 == players.end()) {
                outputFile << "Error caught" << std::endl;
                continue;
            }
            // calling the drink function according to the owner's type
            Character* character = it1->second.get();
            if (character->getCharType() == CharType::W) {
                static_cast<Wizard*>(character)->drink(potionName, it2->second);
            } else if (character->getCharType() == CharType::A) {
                static_cast<Archer*>(character)->drink(potionName, it2->second);
            } else {
                static_cast<Fighter*>(character)->drink(potionName, it2->second);
            }

        } else if (word == "Dialogue") {
            std::string speaker;
            inputFile >> speaker;
            bool flag = true;
            if (speaker == "Narrator") {
                outputFile << "Narrator:";
            } else {
                // searching for the speaker and printing error if we couldn't find him
                auto it = players.find(speaker);
                if (it == players.end()) {
                    outputFile << "Error caught";
                    flag = false;
                } else {
                    outputFile << speaker << ":";
                }
            }
            int m;
            inputFile >> m;
            for (int i = 0; i < m; i++) {
                std::string speech;
                inputFile >> speech;
                // printing speech if only we found the speaker
                if (flag) {
                    outputFile << " " << speech;
                }
            }
            outputFile << std::endl;

        } else if (word == "Show") {
            std::string object;
            inputFile >> object;

            // display characters in an alphabetical order
            // (no need to sort because map is always contain elements in an alphabetical order)
            if (object == "characters") {
                for (auto c : players) {
                    outputFile << c.second->getName() << ":" << charTypeToString(c.second->getCharType()) << ":" << c.second->getHP() << " ";
                }
                outputFile << std::endl;

            } else if (object == "weapons") {
                std::string name;
                inputFile >> name;

                // searching for the owner and printing error if we couldn't find him
                auto it = players.find(name);
                if (it == players.end()) {
                    outputFile << "Error caught" << std::endl;
                    continue;
                }

                // calling the showWeapons function according to the owner's type
                Character* character = it->second.get();
                if (character->getCharType() == CharType::F) {
                    static_cast<Fighter*>(character)->showWeapons();
                } else if (character->getCharType() == CharType::A) {
                    static_cast<Archer*>(character)->showWeapons();
                } else {
                    outputFile << "Error caught" << std::endl;
                }

            } else if (object == "potions") {
                std::string name;
                inputFile >> name;

                // searching for the owner and printing error if we couldn't find him
                auto it = players.find(name);
                if (it == players.end()) {
                    outputFile << "Error caught" << std::endl;
                    continue;
                }

                // calling the showPotions function according to the owner's type
                Character* character = it->second.get();
                if (character->getCharType() == CharType::F) {
                    static_cast<Fighter*>(character)->showPotions();
                } else if (character->getCharType() == CharType::A) {
                    static_cast<Archer*>(character)->showPotions();
                } else {
                    static_cast<Wizard*>(character)->showPotions();
                }

            } else {
                std::string name;
                inputFile >> name;

                // searching for the owner and printing error if we couldn't find him
                auto it = players.find(name);
                if (it == players.end()) {
                    outputFile << "Error caught" << std::endl;
                    continue;
                }
                
                // calling the showSpells function according to the owner's type
                Character* character = it->second.get();
                if (character->getCharType() == CharType::W) {
                    static_cast<Wizard*>(character)->showSpells();
                } else if (character->getCharType() == CharType::A) {
                    static_cast<Archer*>(character)->showSpells();
                } else {
                    outputFile << "Error caught" << std::endl;
                }
            }

        } else {
            outputFile << "Error caught" << std::endl;
        }
    }
    inputFile.close();
    outputFile.close();
    return 0;
};
