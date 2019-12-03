#include <iostream>
#include <string>
#include "DBentry.h"
#include "TreeDB.h"

using namespace std;

// Command Functions:
void insertCommand(TreeDB* tree);
void findCommand(TreeDB* tree);
void removeCommand(TreeDB* tree);
void commandPrintAll(TreeDB* tree);
void printProbesCommand(TreeDB* tree);
void removeAllCommand(TreeDB* tree);
void countActiveCommand(TreeDB* tree);
void updateStatusCommand(TreeDB* tree);
bool activeBool (const string& status);

int main() {
    // the binary search tree database that will be passed into every command function
    TreeDB* tree = new TreeDB;
    
    while (!cin.eof()) {
        cout << "> ";
        string command;
        cin >> command;

        if (command == "insert") {
            insertCommand(tree);
        }
        else if (command == "find") {
            findCommand(tree);
        }
        else if (command == "remove") {
            removeCommand(tree);
        }
        else if (command == "printall") {
            commandPrintAll(tree);
        }
        else if (command == "removeall") {
            removeAllCommand(tree);
        }
        else if (command == "countactive") {
            countActiveCommand(tree);
        }
        else if (command == "printprobes") {
            printProbesCommand(tree);
        }
        else if (command == "updatestatus") {
            updateStatusCommand(tree);
        }
    }

    // Delete binary search tree database to avoid memory leaks
    delete tree;
    
    return 0;
}

// returns status equating to the active string
bool activeBool (const string& status) {
    return status == "active";
}

// updates the status
void updateStatusCommand(TreeDB* tree) {
    string name;
    string status;
    cin >> name >> status;
    DBentry* entry = tree->find(name);
    // if there is no entry, there is nothing to update
    if (entry == NULL) {
        cout << "Error: entry does not exist" << endl;
    }
    // if there are entries, link entry to active
    else {
        entry->setActive(activeBool(status));
        cout << "Success" << endl;
    }
}

// prints the probes
void printProbesCommand(TreeDB* tree) {
    string name;
    // reads the name
    cin >> name;
    DBentry* entry = tree->find(name);
    // no entries means nothing to do
    if (entry == NULL) {
        cout << "Error: entry does not exist" << endl;
    }
    // link tree to the printProbes function in TreeDB
    else {
        tree->printProbes();
    }
}

// Finds the amount of active entries
void countActiveCommand(TreeDB* tree) {
    tree->countActive();
}

// prints all the entries
void commandPrintAll(TreeDB* tree) {
    cout << *tree;
}

// insert command
void insertCommand(TreeDB* tree) {
    string name;
    unsigned int IPaddress;
    string status;
    // read the inputs
    cin >> name >> IPaddress >> status;
    // create new entry
    DBentry* entry = new DBentry(name, IPaddress, activeBool(status));
    // check whether the insertion is validful using the TreeDB insert bool
    bool valid = tree->insert(entry);
    // if validful print out Success
    if (valid == true) {
        cout << "Success" << endl;
    }
    // If not validful, print out the entry already exists and 
    // delete the entry to avoid memory leaks
    else {
        cout << "Error: entry already exists" << endl;
        delete entry; 
    }
}

// command for remove
void removeCommand(TreeDB* tree) {
    string name;
    // reads the name
    cin >> name;
    // checks if the remove is valid
    bool valid = tree->remove(name);
    // prints success if valid
    if (valid == true) {
        cout << "Success" << endl;
    }
    // prints doesn't exist if entry doesn't exist
    else {
        cout << "Error: entry does not exist" << endl;
    }
}

// goes through the entries to find the desired entry
void findCommand(TreeDB* tree) {
    string name;
    cin >> name;
    DBentry* entry = tree->find(name);
    // Checking if entry doesn't exist
    if (entry == NULL) {
        cout << "Error: entry does not exist" << endl;
    }
    // If entry exists, then proceed to print out the entry
    else {
        cout << *entry << endl;
    }
}

// removes all the entries and prints valid
void removeAllCommand(TreeDB* tree) {
    tree->clear();
    cout << "Success" << endl;
}
