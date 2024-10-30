#include <vector>
#include <algorithm>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <memory>

std::ofstream outputFile("output.txt");

enum CharType {
    F,
    A,
    W
};

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

class Character {
private:
    int hp;
    std::string name;
   CharType type;

public:
    Character(int h, std::string n, CharType t) : hp(h), name(n), type(t) {}

    std::string getName() const {
        return name;
    }

    int getHP() const {
        return hp;
    }

    CharType getCharType() const {
        return type;
    }

    bool damage(int d) {
        hp -= d;
        return hp <= 0;
    }

    void heal(int h) {
        hp += h;
    }
};

class PhysicalItem {
private:
    Character* owner;
    std::string name;
public:
    PhysicalItem(Character* o, const std::string& n) : owner(o), name(n) {}

    std::string getName() const {
        return name;
    }
    Character* getOwner() const {
        return owner;
    }
};

class Weapon : public PhysicalItem {
private:
    int damage;
public:
    Weapon(Character* owner, const std::string& name, int d) : PhysicalItem(owner, name), damage(d) {}

    int getDamage() const {
        return damage;
    }
};

class Potion : public PhysicalItem {
private:
    int healValue;
public:
    Potion(Character* owner, const std::string& name, int hv) : PhysicalItem(owner, name), healValue(hv) {}

    int getHealValue() const {
        return healValue;
    }
};

class SpellSheet : public PhysicalItem {
private:
    std::vector<Character*> allowedTargets;
public:
    SpellSheet(Character* owner, const std::string& name, std::vector<Character*> ch) : PhysicalItem(owner, name), allowedTargets(ch) {}

    int getNumAllowedTargets() const {
        return allowedTargets.size();
    }

    bool targetAllowed(Character* c) const {
        for (Character* check : allowedTargets) {
            if (c->getName() == check->getName()) {
                return true;
            }
        }
        return false;
    }
};

template<typename T>
class Container {
protected:
    std::vector<T> elements;
    int maxSize = 0;
public:
    Container(int n) : maxSize(n) {}

    void addItem(const T& item) {
        elements.push_back(item);
    }

    void removeItem(const std::string& n) {
        for (auto iter = elements.begin(); iter != elements.end(); iter++) {
            if (iter->getName() == n) {
                iter = elements.erase(iter);
                return;
            }
        }
    }

    T* findItem(const std::string& n) const {
        for (auto& element : elements) {
            if (element.getName() == n) {
                return &element;
            }
        }
        return nullptr;
    }

    bool contains(const std::string& n) const {
        for (auto& element : elements) {
            if (element.getName() == n) {
                return true;
            }
        }
        return false;
    }

    bool isFull() const {
        return elements.size() == maxSize;
    }

    virtual void show() = 0;
};

class Arsenal : public Container<Weapon> {
public:
    Arsenal(int n) : Container(n) {}

    void show() {
        std::sort(elements.begin(), elements.end(), [](Weapon a, Weapon b) {
            return a.getName() < b.getName();
        });

        for (Weapon w : elements) {
            outputFile << w.getName() << ":" << w.getDamage() << " ";
        }
        outputFile << std::endl;
    }
};

class MedicalBag  : public Container<Potion> {
public:
    MedicalBag(int n) : Container(n) {}

    void show() {
        std::sort(elements.begin(), elements.end(), [](Potion a, Potion b) {
            return a.getName() < b.getName();
        });

        for (Potion w : elements) {
            outputFile << w.getName() << ":" << w.getHealValue() << " ";
        }
        outputFile << std::endl;
    }
};

class SpellBook : public Container<SpellSheet> {
public:
    SpellBook(int n) : Container(n) {}

    void show() {
        std::sort(elements.begin(), elements.end(), [](SpellSheet a, SpellSheet b) {
            return a.getName() < b.getName();
        });

        for (SpellSheet w : elements) {
            outputFile << w.getName() << ":" << w.getNumAllowedTargets() << " ";
        }
        outputFile << std::endl;
    }
};

class WeaponUser : public Character {
private:
    Arsenal arsenal;
public:
    WeaponUser(int hp, const std::string& name, CharType t, int a) : Character(hp, name, t), arsenal(Arsenal(a)) {}

    void giveWeapon(const std::string& n, int d) {
        if (arsenal.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            arsenal.addItem(Weapon(this, n, d));
            outputFile << this->getName() << " just obtained a new weapon called " << n << "." << std::endl;
        } 
    }

    bool attack(const std::string& n, Character* ch) {
        if (arsenal.contains(n)) {
            outputFile << this->getName() << " attacks " << ch->getName() << " with their " << n << "!" << std::endl;
            return ch->damage(arsenal.findItem(n)->getDamage());
        } else {
            outputFile << "Error caught" << std::endl;
            return false;
        }
    }

    void showWeapons() {
        arsenal.show();
    }
};

class PotionUser : public Character {
private:
    MedicalBag medicalBag;
public:
    PotionUser(int hp, const std::string& name, CharType t, int a) : Character(hp, name, t), medicalBag(MedicalBag(a)) {}

    void givePotion(const std::string& n, int hv) {
        if (medicalBag.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            medicalBag.addItem(Potion(this, n, hv));
            outputFile << this->getName() << " just obtained a new potion called " << n << "." << std::endl;
        }
    }

    void drink(const std::string& n, Character* ch) {
        if (medicalBag.contains(n)) {
            ch->heal(medicalBag.findItem(n)->getHealValue());
            outputFile << ch->getName() << " drinks " << n << " from " << this->getName() << "." << std::endl;
            medicalBag.removeItem(n);
        } else {
            outputFile << "Error caught" << std::endl;
        }
    }

    void showPotions() {
        medicalBag.show();
    }
};

class SpellUser : public Character {
private:
    SpellBook spellBook;
public:
    SpellUser(int hp, const std::string& name, CharType t, int a) : Character(hp, name, t), spellBook(SpellBook(a)) {}

    void giveSpell(const std::string& n, std::vector<Character*> ch) {
        if (spellBook.isFull()) {
            outputFile << "Error caught" << std::endl;
        } else {
            spellBook.addItem(SpellSheet(this, n, ch));
            outputFile << this->getName() << " just obtained a new spell called " << n << "." << std::endl;
        }
    }

    bool cast(const std::string& n, Character* ch) {
        if (spellBook.contains(n)) {
            if (spellBook.findItem(n)->targetAllowed(ch)) {
                spellBook.removeItem(n);
                outputFile << this->getName() << " casts " << n << " on " << ch->getName() << "!" << std::endl;
                return true;
            } else {
                outputFile << "Error caught" << std::endl;
            }
        }
        return false;
    }

    void showSpells() {
        spellBook.show();
    }

};

class Fighter : public virtual Character, public WeaponUser, public PotionUser {
public:
    Fighter(const std::string& name, int hp) : Character(hp, name, CharType::F), WeaponUser(hp, name, CharType::F, 2), PotionUser(hp, name, CharType::F, 5) {
        outputFile << "A new fighter came to town, " << name << "." << std::endl;
    };
};

class Archer : public virtual Character, public WeaponUser, public PotionUser, public SpellUser {
public:
    Archer(const std::string& name, int hp) : Character(hp, name, CharType::A), WeaponUser(hp, name, CharType::F, 2), PotionUser(hp, name, CharType::F, 3), SpellUser(hp, name, CharType::F, 2) {
        outputFile << "A new archer came to town, " << name << "." << std::endl;
    }
};

class Wizard : public virtual Character, public PotionUser, public SpellUser {
public:
    Wizard(const std::string& name, int hp) : Character(hp, name, CharType::W), PotionUser(hp, name, CharType::F, 10), SpellUser(hp, name, CharType::F, 10) {
        outputFile << "A new wizard came to town, " << name << "." << std::endl;
    }
};

int main() {
    std::ifstream inputFile("input.txt");
    std::map<std::string, std::unique_ptr<Character>> players;

    int n;
    inputFile >> n;
    for (int i = 0; i < n; i++) {
        std::string word;
        inputFile >> word;

        if (word == "Create") {
            inputFile >> word;

            if (word == "character") {
                std::string type, name;
                int hp;
                inputFile >> type >> name >> hp;
                if (type == "fighter") {
                    players[name] = std::make_unique<Fighter>(name, hp);
                } else if (type == "wizard") {
                    players[name] = std::make_unique<Wizard>(name, hp);
                } else {
                    players[name] = std::make_unique<Archer>(name, hp);
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
                    auto it = players.find(ownerName);
                    if (it == players.end()) {
                        outputFile << "Error caught" << std::endl;
                        continue;
                    }
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
                    auto it = players.find(ownerName);
                    if (it == players.end()) {
                        outputFile << "Error caught" << std::endl;
                        continue;
                    }
                    Character* character = it->second.get();
                    if (character->getCharType() == CharType::F) {
                        static_cast<Fighter*>(character)->givePotion(potionName, healValue);
                    } else if (character->getCharType() == CharType::A) {
                        static_cast<Archer*>(character)->givePotion(potionName, healValue);
                    } else {
                        static_cast<Wizard*>(character)->givePotion(potionName, healValue);
                    }

                } else {
                    std::string ownerName, spellName;
                    int m;
                    inputFile >> ownerName >> spellName >> m;
                    std::vector<Character*> chars;
                    bool full = true;
                    for (int i = 0; i < m; i++) {
                        std::string target;
                        inputFile >> target;
                        auto it = players.find(target);
                        if (it == players.end()) {
                            outputFile << "Error caught" << std::endl;
                            full = false;
                            break;
                        }
                        chars.push_back(it->second.get());
                    }
                    if (full) {
                        auto it = players.find(ownerName);
                        if (it == players.end()) {
                            outputFile << "Error caught" << std::endl;
                        }
                        Character* character = it->second.get();
                        if (character->getCharType() == CharType::W) {
                            static_cast<Wizard*>(character)->giveSpell(spellName, chars);
                        } else if (character->getCharType() == CharType::A) {
                            static_cast<Archer*>(character)->giveSpell(spellName, chars);
                        } else {
                            outputFile << "Error caught" << std::endl;
                        }
                    }
                }
            }

        } else if (word == "Attack") {
            std::string attackerName, targetName, weaponName;
            inputFile >> attackerName >> targetName >> weaponName;
            auto it1 = players.find(attackerName);
            auto it2 = players.find(targetName);
            if (it1 == players.end() || it2 == players.end()) {
                outputFile << "Error caught" << std::endl;
                continue;
            }
            Character* character = it1->second.get();
            if (character->getCharType() == CharType::F) {
                if (static_cast<Fighter*>(character)->attack(weaponName, it2->second.get())) {
                    outputFile << (it2->second.get())->getName() << " has died..." << std::endl;
                    it2 = players.erase(it2);
                }
            } else if (character->getCharType() == CharType::A) {
                if (static_cast<Archer*>(character)->attack(weaponName, it2->second.get())) {
                    outputFile << (it2->second.get())->getName() << " has died..." << std::endl;
                    it2 = players.erase(it2);
                }
            } else {
                outputFile << "Error caught" << std::endl;
            }

        } else if (word == "Cast") {
            std::string casterName, targetName, spellName;
            inputFile >> casterName >> targetName >> spellName;
            auto it1 = players.find(casterName);
            auto it2 = players.find(targetName);
            if (it1 == players.end() || it2 == players.end()) {
                outputFile << "Error caught" << std::endl;
                continue;
            }
            Character* character = it1->second.get();
            if (character->getCharType() == CharType::W) {
                if (static_cast<Wizard*>(character)->cast(spellName, it2->second.get())) {
                    outputFile << (it2->second.get())->getName() << " has died..." << std::endl;
                    it2 = players.erase(it2);
                }
            } else if (character->getCharType() == CharType::A) {
                if (static_cast<Archer*>(character)->cast(spellName, it2->second.get())) {
                    outputFile << (it2->second.get())->getName() << " has died..." << std::endl;
                    it2 = players.erase(it2);
                }
            } else {
                outputFile << "Error caught" << std::endl;
            }

        } else if (word == "Drink") {
            std::string supplierName, drinkerName, potionName;
            inputFile >> supplierName >> drinkerName >> potionName;
            auto it1 = players.find(supplierName);
            auto it2 = players.find(drinkerName);
            if (it1 == players.end() || it2 == players.end()) {
                outputFile << "Error caught" << std::endl;
                continue;
            }
            Character* character = it1->second.get();
            if (character->getCharType() == CharType::W) {
                static_cast<Wizard*>(character)->drink(potionName, it2->second.get());
            } else if (character->getCharType() == CharType::A) {
                static_cast<Archer*>(character)->drink(potionName, it2->second.get());
            } else {
                static_cast<Fighter*>(character)->drink(potionName, it2->second.get());
            }

        } else if (word == "Dialogue") {
            std::string speaker;
            inputFile >> speaker;
            if (speaker == "Narrator") {
                outputFile << "Narrator:";
            } else {
                auto it = players.find(speaker);
                if (it == players.end()) {
                    outputFile << "Error caught" << std::endl;
                    continue;
                } else {
                    outputFile << speaker << ":";
                }
            }
            int m;
            inputFile >> m;
            for (int i = 0; i < m; i++) {
                std::string speech;
                inputFile >> speech;
                outputFile << " " << speech;
            }
            outputFile << std::endl;

        } else if (word == "Show") {
            std::string object;
            inputFile >> object;
            if (object == "characters") {
                std::vector<Character*> sortedCharacters;
                for (auto& pair : players) {
                    sortedCharacters.push_back(pair.second.get());
                }

                std::sort(sortedCharacters.begin(), sortedCharacters.end(), [](Character* a, Character* b) {
                    return a->getName() < b->getName();
                });

                for (auto* character : sortedCharacters) {
                    outputFile << character->getName() << ":" << charTypeToString(character->getCharType()) << ":" << character->getHP() << " ";
                }
                outputFile << std::endl;

            } else if (object == "weapons") {
                std::string name;
                inputFile >> name;
                auto it = players.find(name);
                if (it == players.end()) {
                    outputFile << "Error caught" << std::endl;
                    continue;
                }
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
                auto it = players.find(name);
                if (it == players.end()) {
                    outputFile << "Error caught" << std::endl;
                    continue;
                }
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
                auto it = players.find(name);
                if (it == players.end()) {
                    outputFile << "Error caught" << std::endl;
                    continue;
                }
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
            outputFile << "Something went wrong" << std::endl;
        }
    }
    inputFile.close();
    outputFile.close();
    return 0;
};