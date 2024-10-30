#include <vector>
#include <string>
#include <iostream>
#include <map>

using namespace std;

// class User represetns the users of online book store
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

// OBSERVER pattern
class Observer {
public:
    virtual void notify(const string& bookTitle, const string& newPrice) = 0;
};

// implementation of Observer pattern for the store users
// notifies the subscribed users when the book's price is changing
class UserObserver : public Observer {
private:
    vector<const User*> users;  // the list of subscribed users
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

// FACTORY pattern
// helps to create users depending on their type (premium / usual)
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

// BookProxy uses some functions from Book, so we need to declare some of them before BookProxy 
class IBook {
public:
    virtual string getTitle() const = 0;
    virtual string getAuthor() const = 0;
    virtual void read(User* user) = 0;
    virtual void listen(User* user) = 0;
};

// PROXY pattern
// helps to restrict acess to listening the books for non-premium users
class BookProxy {
private:
    IBook* book;  // the book
    User* user;   // the user who will read/listen the book
public:
    BookProxy(IBook* b, User* u) : book(b), user(u) {}

    // everyone can read the book
    void readBook() {
        cout << user->getName() << " reading " << book->getTitle() << " by " << book->getAuthor() << endl;
    }

    // only Premimum users can listen the book
    void listenBook() {
        if (user->hasPremium()) {
            cout << user->getName() << " listening " << book->getTitle() << " by " << book->getAuthor() << endl;
        } else {
            cout << "No access" << endl;
        }
    }
};

// class Book represents the books from online store
class Book : public IBook {
private:
    string title;
    string author;
    string price;
    static UserObserver* obs;  // the observer (one for entire class)
public:
    Book() = default;
    Book(const string& t, const string& a, const string& p) : title(t), author(a), price(p) {}

    void setPrice(const string& p) {
        price = p;
        obs->notify(title, p); // when the price is changing, we notify all the subscribed users
    }

    string getAuthor() const {
        return author;
    }

    string getTitle() const {
        return title;
    }
    
    // subscribing users
    static void subscribe(const User* user) {
        if (obs->containsUser(user)) {
            cout << "User already subscribed" << endl;
        } else {
            obs->addUser(user);
        }
    }

    // unsubscribing users
    static void unsubscribe(const User* user) {
        if (obs->containsUser(user)) {
            obs->removeUser(user);
        } else {
            cout << "User is not subscribed" << endl;
        }
    }

    // reading and listening the books are described in the Proxy class

    void read(User* user) {
        BookProxy proxy(this, user);
        proxy.readBook();
    }

    void listen(User* user) {
        BookProxy proxy(this, user);
        proxy.listenBook();
    }
};

// creating and observer for the entire Book class
UserObserver* Book::obs = new UserObserver();

int main() {
    map<string, Book> bookshell;  // bookshell with books
    map<string, User> readers;    // store users

    while (true) {
        string command;
        cin >> command;
        if (command == "createBook") {
            string title;
            string author;
            string price;
            cin >> title >> author >> price;

            // searching for a book
            auto it = bookshell.find(title);

            // if the book is not found, add it
            if (it == bookshell.end()) {
                bookshell.emplace(title, Book(title, author, price));

            // if the book is found, printing error
            } else {
                cout << "Book already exists" << endl;
            }

        } else if (command == "createUser") {
            string type;
            string name;
            cin >> type >> name;

            // searching for a user
            auto it = readers.find(name);

            // if the user is not found, create it by the Factory and add it to "readers"
            if (it == readers.end()) {
                auto user = UserFactory::createUser(type, name);
                readers.emplace(name, *user);
            
            // if the user is found, printing error
            } else {
                cout << "User already exists" << endl;
            }

        } else if (command == "subscribe") {
            string user;
            cin >> user;
            Book::subscribe(&readers[user]);      // subscribe function is the same for entire Book class

        } else if (command == "unsubscribe") {
            string user;
            cin >> user;
            Book::unsubscribe(&readers[user]);    // subscribe function is the same for entire Book class

        } else if (command == "updatePrice") {
            string title;
            string newPrice;
            cin >> title >> newPrice;
            bookshell[title].setPrice(newPrice);  // all the notifications are in the observer

        } else if (command == "readBook") {
            string username;
            string title;
            cin >> username >> title;
            bookshell[title].read(&readers[username]);

        } else if (command == "listenBook") {
            string username;
            string title;
            cin >> username >> title;
            bookshell[title].listen(&readers[username]);  // all the restrictions regarding premium are in the BookProxy

        } else if (command == "end") {
            break;
        } else {
            cout << "Something went wrong" << endl;
        }
    }
}