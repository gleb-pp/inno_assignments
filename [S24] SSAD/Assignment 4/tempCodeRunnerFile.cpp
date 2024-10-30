#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

class User {
private:
    bool premium;
    string name;
public:
    User() = default;
    User(const bool& p, const string& n) : premium(p), name(n) {}

    bool hasPremium() const {
        return premium;
    }

    string getName() const {
        return name;
    }
};

// OBSERVER PATTERN
class Observer {
public:
    virtual void notify(const string& bookTitle, const string& newPrice) = 0;
};

class UserObserver : public Observer {
private:
    vector<const User*> users;
public:
    void notify(const string& bookTitle, const string& newPrice) override {
        for (auto& user : users) {
            cout << user->getName() << " notified about price update for " << bookTitle << " to " << newPrice << endl;
        }
    }

    void addUser(const User* user) {
        users.push_back(user);
    }

    bool containsUser(const User* user) const {
        for (int i = 0; i < users.size(); i++) {
            if (users[i]->getName() == user->getName()) {
                return true;
            }
        }
        return false;
    }

    void removeUser(const User* user) {
        for (int i = 0; i < users.size(); i++) {
            if (users[i]->getName() == user->getName()) {
                users.erase(users.begin() + i);
                return;
            }
        }
    }
};

// FACTORY PATTERN
class UserFactory {
public:
    static User* createUser(const string& type, const string& name) {
        if (type == "premium") {
            return new User(true, name);
        } else {
            return new User(false, name);
        }
    }
};

class IBook {
public:
    virtual string getTitle() const = 0;
    virtual string getAuthor() const = 0;
    virtual void read(User* user) = 0;
    virtual void listen(User* user) = 0;
};

class BookProxy {
private:
    IBook* book;
    User* user;
public:
    BookProxy(IBook* b, User* u) : book(b), user(u) {}

    void readBook() {
        cout << user->getName() << " reading " << book->getTitle() << " by " << book->getAuthor() << endl;
    }

    void listenBook() {
        if (user->hasPremium()) {
            cout << user->getName() << " listening " << book->getTitle() << " by " << book->getAuthor() << endl;
        } else {
            cout << "No access" << endl;
        }
    }
};

class Book : public IBook {
private:
    string title;
    string author;
    string price;
    static UserObserver* obs;
public:
    Book() = default;
    Book(const string& t, const string& a, const string& p) : title(t), author(a), price(p) {}

    void setPrice(const string& p) {
        price = p;
        obs->notify(title, p);
    }

    string getAuthor() const {
        return author;
    }

    string getTitle() const {
        return title;
    }
    
    static void subscribe(const User* user) {
        if (obs->containsUser(user)) {
            cout << "User already subscribed" << endl;
        } else {
            obs->addUser(user);
        }
    }

    static void unsubscribe(const User* user) {
        if (obs->containsUser(user)) {
            obs->removeUser(user);
        } else {
            cout << "User is not subscribed" << endl;
        }
    }

    void read(User* user) {
        BookProxy proxy(this, user);
        proxy.readBook();
    }

    void listen(User* user) {
        BookProxy proxy(this, user);
        proxy.listenBook();
    }
};

UserObserver* Book::obs = new UserObserver();

int main() {
    map<string, Book> bookshell;
    map<string, User> readers;

    while (true) {
        string command;
        cin >> command;
        if (command == "createBook") {
            string title;
            string author;
            string price;
            cin >> title >> author >> price;
            auto it = bookshell.find(title);
            if (it == bookshell.end()) {
                bookshell.emplace(title, Book(title, author, price));
            } else {
                cout << "Book already exists" << endl;
            }
        } else if (command == "createUser") {
            string type;
            string name;
            cin >> type >> name;
            auto it = readers.find(name);
            if (it == readers.end()) {
                auto user = UserFactory::createUser(type, name);
                readers.emplace(name, *user);
            } else {
                cout << "User already exists" << endl;
            }
        } else if (command == "subscribe") {
            string user;
            cin >> user;
            Book::subscribe(&readers[user]);
        } else if (command == "unsubscribe") {
            string user;
            cin >> user;
            Book::unsubscribe(&readers[user]);
        } else if (command == "updatePrice") {
            string title;
            string newPrice;
            cin >> title >> newPrice;
            bookshell[title].setPrice(newPrice);
        } else if (command == "readBook") {
            string username;
            string title;
            cin >> username >> title;
            bookshell[title].read(&readers[username]);
        } else if (command == "listenBook") {
            string username;
            string title;
            cin >> username >> title;
            bookshell[title].listen(&readers[username]);
        } else if (command == "end") {
            break;
        } else {
            cout << "Something went wrong" << endl;
        }
    }
}