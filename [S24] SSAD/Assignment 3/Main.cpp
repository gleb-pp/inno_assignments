#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <memory>
#include <map>
#include <tuple>

using namespace std;
ostringstream output;  // stream for collecting output data

float typeToPercent(string type) {
    if (type == "Savings") {
        return 1.5;
    } else if (type == "Checking") {
        return 2;
    } else {
        return 2.5;
    }
};

// STATE design pattern — active / inactive state of the account
class AccountState {
public:
    virtual void view(const string& name, const string& type, const float& balance, const string& tr) = 0;
    virtual void deactivate(const string& name) = 0;
    virtual void activate(const string& name) = 0;
    virtual void withdraw(const string& name, const float& summ, float& balance, const string& type, ostringstream& tr) = 0;

    // AccountState isn't allowed to send money, so "transfer" will return the sum, and Account will send it by himself
    virtual float transfer(const string& name, const string& acc, const float& summ, float& balance, const string& type, ostringstream& tr) = 0;
};

// implementation for active accounts
class ActiveState : public AccountState {
public:

    // printing the info about the account
    void view(const string& name, const string& type, const float& balance, const string& tr) override {
        output << name << "'s Account: Type: " << type << ", Balance: $" << balance;
        output << ", State: Active, Transactions: [" << tr << "]." << endl;
    }

    // successful deactivating of an active account
    void deactivate(const string& name) override {
        output << name << "'s account is now deactivated." << endl;
    }

    // error when trying to activate an active account
    void activate(const string& name) override {
        output << "Error: Account " << name << " is already activated." << endl;
    }

    void withdraw(const string& name, const float& summ, float& balance, const string& type, ostringstream& tr) override {
        if (summ > balance) {
            output << "Error: Insufficient funds for " << name << "." << endl;
            return;
        }

        float percent = typeToPercent(type);
        balance -= summ;  // balance is a reference to the account's balance, so we can change it right here
        float cash = (summ * (1 - percent / 100));  // the transferred amount of money

        output << name << " successfully withdrew $" << cash;
        output << ". New Balance: $" << balance;
        output << ". Transaction Fee: $" << (summ - cash);
        output << fixed << setprecision(1);  // fixing the format with 1 decimal places (since we print the percent)
        output << " (" << percent << "%) in the system." << endl;
        output << fixed << setprecision(3);  // return to the usual 3 decimal places

        tr << ", Withdrawal $" << summ;  // recording a transaction
    }

    // AccountState isn't allowed to send money, so it will return the sum, and Account will send it by itself
    float transfer(const string& name, const string& acc, const float& summ, float& balance, const string& type, ostringstream& tr) override {
        if (summ > balance) {
            output << "Error: Insufficient funds for " << name << "." << endl;
            return 0;  // there is an error, we don't send anything
        }

        float percent = typeToPercent(type);
        float cash = (summ * (1 - percent / 100));  // the transferred amount of money
        balance -= summ;        // balance is a reference to the account's balance, so we can change it right here
        if (name == acc) {
            balance += cash;    // if sender and reciever are the same account, then we add money right here
        }

        output << name << " successfully transferred $" << cash << " to " << acc << ". New Balance: $" << balance;
        output << ". Transaction Fee: $" << (summ - cash);
        output << fixed << setprecision(1);  // fixing the format with 1 decimal places (since we print the percent)
        output << " (" << percent << "%) in the system." << endl;
        output << fixed << setprecision(3);  // return to the usual 3 decimal places

        tr << ", Transfer $" << summ;  // recording a transaction

        if (name == acc) return 0;  // if sender and reciever are the same account, we have already sent money
        return cash;                // in other cases, we need to send cash to the reciever
    }

};

// implementation for inactive accounts
class InactiveState : public AccountState {
public:

    // printing the info about the account
    void view(const string& name, const string& type, const float& balance, const string& tr) override {
        output << name << "'s Account: Type: " << type << ", Balance: $" << balance;
        output << ", State: Inactive, Transactions: [" << tr << "]." << endl;
    }

    // error when trying to deactivate an inactive account
    void deactivate(const string& name) override {
        output << "Error: Account " << name << " is already deactivated." << endl;
    }

    // successful activating of an inactive account
    void activate(const string& name) override {
        output << name << "'s account is now activated." << endl;
    }

    // error — withdraw is not allowed for inactive accounts
    void withdraw(const string& name, const float& summ, float& balance, const string& type, ostringstream& tr) override {
        output << "Error: Account " << name << " is inactive." << endl;
    }

    // error — transfer is not allowed for inactive accounts
    float transfer(const string& name, const string& acc, const float& summ, float& balance, const string& type, ostringstream& tr) override {
        output << "Error: Account " << name << " is inactive." << endl;
        return 0;  // since we dont't need to send anything, we can send 0
    }

};

ActiveState active;
InactiveState inactive;

class Account {
private:
    const string type;
    const string name;
    float balance;
    AccountState* state = &active;  // initial state is active
    ostringstream transactions;     // stream for collecting information about the transactions
public:
    Account(string t, string n, float b) : type(t), name(n), balance(b) {
        transactions << fixed << setprecision(3);  // fixing the format with 3 decimal places
        transactions << "Initial Deposit $" << b;
    }

    // deposit function doesn't depend on the state, so we can write it right here
    void deposit(float summ) {
        balance += summ;
        output << name << " successfully deposited $" << summ << ". New Balance: $" << balance << "." << endl;
        transactions << ", Deposit $" << summ;  // recording a transaction
    }

    // the functions below call the corresponding functions of the AccountState class
    // because their implementations depend on state

    void view() const {
        state->view(name, type, balance, transactions.str());
    }

    void deactivate() {
        state->deactivate(name);
        state = &inactive;
    }

    void activate() {
        state->activate(name);
        state = &active;
    }

    void withdraw(float summ) {
        state->withdraw(name, summ, balance, type, transactions);
    }

    void transfer(float summ, Account* acc) {
        // we get the transfered amount of money and send it to the reciever
        acc->sendMoney(state->transfer(name, acc->getName(), summ, balance, type, transactions));
    }

protected:
    // these functions used only by transfer fucntion, so I made them protected
    void sendMoney(float summ) {
        balance += summ;
    }

    const string& getName() const {
        return name;
    }
};

// SINGLETON design pattern
// database containing information about the accounts
class Database {
private:
    static Database* instance;      // pointer to a single instance
    map<string, Account> accounts;  // map for storing accounts

    Database() {}                   // private constructor to prohibit direct instantiation

public:
    // a function for accessing a single instance
    static Database* getInstance() {
        if (!instance) {
            instance = new Database();
        }
        return instance;
    }

    // a function for creating account
    void createAccount(const string& type, const string& name, const float& dep) {
        accounts.emplace(name, Account(type, name, dep));
        output << "A new " << type << " account created for " << name << " with an initial balance of $" << dep << "." << endl;
    }

    // a function for account access
    Account* getAccount(const string& name) {
        auto it = accounts.find(name);
        if (it == accounts.end()) {
            output << "Error: Account " << name << " does not exist." << endl;
            return nullptr;
        } else {
            return &(it->second);
        }
    }
};

// initializing a static Database class member
Database* Database::instance = nullptr;

// SINGLETON design pattern + FACADE design pattern
// banking system working with accounts
class BankFacade {
private:
    static BankFacade* instance;               // pointer to a single instance
    Database* db = Database::getInstance();    // database (also singleton)

    BankFacade() {}                            // private constructor to prohibit direct instantiation

public:
    // a function for accessing a single instance
    static BankFacade* getInstance() {
        if (!instance) {
            instance = new BankFacade();
        }
        return instance;
    }

    // creates a new account
    void createAccount(const string& type, const string& name, const float& dep) {
        db->createAccount(type, name, dep);
    }

    // triggers crediting the deposit or returns nullptr if the account is not found
    void deposit(const string& name, const float& dep) {
        Account* acc = db->getAccount(name);
        if (acc == nullptr) return; 
        acc->deposit(dep);
    }

    // triggers a withdrawal or returns nullptr if the account is not found
    void withdraw(const string& name, const float& dep) {
        Account* acc = db->getAccount(name);
        if (acc == nullptr) return; 
        acc->withdraw(dep);
    }

    // triggers a transfer or returns nullptr if at least one account is not found
    void transfer(const string& from, const string& to, const float& dep) {
        Account* sender = db->getAccount(from);
        if (sender == nullptr) return;
        Account* recipient = db->getAccount(to);
        if (recipient == nullptr) return;
        sender->transfer(dep, recipient);
    }

    // triggers a view function or returns nullptr if the account is not found
    void view(const string& name) const {
        const Account* acc = db->getAccount(name);
        if (acc == nullptr) return;
        acc->view();
    }

    // triggers a deactivate function or returns nullptr if the account is not found
    void deactivate(const string& name) {
        Account* acc = db->getAccount(name);
        if (acc == nullptr) return;
        acc->deactivate();
    }

    // triggers an activate function or returns nullptr if the account is not found
    void activate(const string& name) {
        Account* acc = db->getAccount(name);
        if (acc == nullptr) return;
        acc->activate();
    }

};

// initializing a static BankFacade class member
BankFacade* BankFacade::instance = nullptr;

int main() {
    BankFacade* bank = BankFacade::getInstance();
    output << fixed << setprecision(3);  // fixing the format with 3 decimal places

    int n;
    cin >> n;
    for (int i = 0; i < n; i++) {
        string word;
        cin >> word;

        if (word == "Create") {
            cin >> word;
            string type;
            string name;
            float dep;
            cin >> type >> name >> dep;
            bank->createAccount(type, name, dep);

        } else if (word == "Deposit") {
            string name;
            float dep;
            cin >> name >> dep;
            bank->deposit(name, dep);

        } else if (word == "Withdraw") {
            string name;
            float dep;
            cin >> name >> dep;
            bank->withdraw(name, dep);

        } else if (word == "Transfer") {
            string from;
            string to;
            float dep;
            cin >> from >> to >> dep;
            bank->transfer(from, to, dep);

        } else if (word == "View") {
            string name;
            cin >> name;
            bank->view(name);

        } else if (word == "Deactivate") {
            string name;
            cin >> name;
            bank->deactivate(name);

        } else if (word == "Activate") {
            string name;
            cin >> name;
            bank->activate(name);

        } else {
            output << "Something went wrong" << endl;
        }
    }
    cout << output.str();
};