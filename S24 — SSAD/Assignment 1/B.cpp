#include <iostream>
#include <vector>

// Defining class DataStructure with 3 virtual functions
class DataStructure {
public:
    virtual void insert(int x) = 0;
    virtual void remove() = 0;
    virtual bool search(int x) = 0;

};

// Defining class SequentialDataStructure inherited from DataStructure with 4 new virtual functions
class SequentialDataStructure : public DataStructure {
public:
    virtual void pushBack(int x) = 0;
    virtual void pushFront(int x) = 0;
    virtual void popBack() = 0;
    virtual void popFront() = 0;

};

// Defining class DynamicDataStructure inherited from DataStructure with 2 new virtual functions
class DynamicDataStructure : public DataStructure {
public:
    virtual void resize(int new_size) = 0;
    virtual void clear() = 0;

};

// Defining class List inherited from SequentialDataStructure and DynamicDataStructure
class List : public SequentialDataStructure, public DynamicDataStructure {
private:
    std::vector<int> lst;
public:

    // to insert element we should push it in the end
    void insert(int x) override {
        lst.push_back(x);
    }

    // to remove the element we should check if the list is not empty and then pop the element from the end
    void remove() override {
        if (!lst.empty()) {
            lst.pop_back();
        }
    }

    // to search the element we go through the list and return true if we could find it and false in other case
    bool search(int x) override {
        for (int element : lst) {
            if (element == x) {
                return true;
            }
        }
        return false;
    }

    // push back new element
    void pushBack(int x) override {
        lst.push_back(x);
    }

    // adding new element at the begining of the list
    void pushFront(int x) override {
        lst.insert(lst.begin(), x);
    }

    // checking if the list is not empty and pop the element from the end
    void popBack() override {
        if (!lst.empty()) {
            lst.pop_back();
        }
    }

    // checking if the list is not empty and pop the element from the begining
    void popFront() override {
        if (!lst.empty()) {
            lst.erase(lst.begin());
        }
    }

    // resizing the list
    void resize(int new_size) override {
        lst.resize(new_size);
    }

    // clearing the list
    void clear() override {
        lst.clear();
    }

    // defining iterator for the correct printing 
    std::vector<int>::iterator begin() {
        return lst.begin();
    }
    std::vector<int>::iterator end() {
        return lst.end();
    }
};

int main() {

    // Creating the list
    List lst;

    // Input number of commands
    int n;
    std::cin >> n;

    // Defining the array with "search" results
    bool res[n];
    bool* resptr = &res[0];

    for (int i = 0; i < n; i++) {

        // Input command
        std::string command;
        std::cin >> command;
        if (command == "search") {
            int x;
            std::cin >> x;
            *(resptr++) = lst.search(x);     // searching for the element x in the list and putting the result to the array res
        } else if (command == "insert") {
            int x;
            std::cin >> x;
            lst.insert(x);                   // inserting the element x to the list
        } else if (command == "remove") {
            lst.remove();                    // removing the last element of the list
        }
    }

    // Printing the results of searching
    bool* startarr = &res[0];
    while (startarr < resptr) {
        if (*(startarr++)) {
            std::cout << "YES" << std::endl;
        } else {
            std::cout << "NO" << std::endl;
        }
    }

    // Printing the list
    for (int element : lst) {
        std::cout << element << " ";
    }
    std::cout << std::endl;
}
