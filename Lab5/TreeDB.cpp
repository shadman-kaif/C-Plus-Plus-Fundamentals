#include <iostream>
#include "TreeDB.h"

// Default constructor
TreeDB::TreeDB() {
    root = NULL;
}

// Destructor makes use of the clear function
TreeDB::~TreeDB() {
    clear();
}

// Insert function
bool TreeDB::insert(DBentry *newEntry) {
    // Only inserting into an empty tree
    if (root == NULL) {
        root = new TreeNode(newEntry);
        return true;
    } 
    else {
        // If tree isn't empty, make sure of the recursive helper function
        return insertHelper(root, newEntry);
    }
}

// Find function
DBentry *TreeDB::find(string name) {
    // Sets probesCount to 0
    probesCount = 0;
    // Employs the recursive helper function
    return findHelper(root, name);
}

// Remove function
bool TreeDB::remove(string name) {
    TreeNode* parent = NULL; 
    TreeNode* toDelete = findDeleteHelper(root, name, parent);
    // Finding if the ptr to delete exists
    if (toDelete == NULL) {
        return false; 
    }
    // If above test is passed, then ptr exists and we can proceed to check 
    // left and right subtree
    TreeNode* left = toDelete->getLeft(); 
    TreeNode* right = toDelete->getRight(); 
    TreeNode* replace; 
    
    // If both left and right subtree is empty, then ptr is a leaf
    if (left == NULL && right == NULL) {
        replace = NULL;
    } 
    // If right or left are empty, then ptr to delete is the other subtree
    else if (left == NULL || right == NULL) {
        if (left == NULL) {
            replace = right;
        }
        else {
            replace = left;
        }
    } 
    
    // Final case where the ptr to delete can be in either subtree
    else {
        replace = left;
        TreeNode* replaceParent = toDelete; 
        // Using method prescribed in lab handout
        while (replace->getRight() != NULL) {
            replaceParent = replace;
            replace = replace->getRight();
        }
        if (replaceParent != toDelete) {
            replaceParent->setRight(replace->getLeft());
            replace->setLeft(left);
        }
        replace->setRight(right);
    }

    // Deletion of the root
    if (parent == NULL) {
        root = replace;
    } 
    else {
        if (parent->getLeft() == toDelete) {
            parent->setLeft(replace);
        }
        else {
            parent->setRight(replace);
        }
    }

    // Delete ptr before leaving function
    delete toDelete;
    return true;
}

// clear function
void TreeDB::clear() {
    // employs the recursive helper function
    clearHelper(root);
    root = NULL;
}

// prints the probesCount number
void TreeDB::printProbes() const {
    std::cout << probesCount << std::endl;
}

// uses recursive helper function to find the number of active sites
// prints the number of active sites
void TreeDB::countActive() const {
    int count = countHelper(root);
    std::cout << count << std::endl;
}

// employs the print helper function to print the tree
ostream &operator<<(ostream &out, const TreeDB &rhs) {
    rhs.printHelper(out, rhs.root);
    return out;
}

// Count active database entries
int TreeDB::countHelper(TreeNode *ptr) const {
    if (ptr == NULL) {
        return 0;
    }
    int count;
    // Adds one to the count if there is a database entry
    if (ptr->getEntry()->getActive() == true) {
        count = 1;
    }
    // Does not add one to the count if there are no entries
    else if (ptr->getEntry()->getActive() == false) {
        count = 0;
    }
    // The total is the total recursive entries
    return count + countHelper(ptr->getLeft()) + countHelper(ptr->getRight());
}
// Find ptr in tree recursively
DBentry* TreeDB::findHelper(TreeNode *ptr, const string& name) {
    // Increment probesCount by one
    probesCount++;
    if (ptr == NULL) {
        return NULL;
    }
    if (name < ptr->getEntry()->getName()) {
        return findHelper(ptr->getLeft(), name);
    }
    if (ptr->getEntry()->getName() == name) {
        return ptr->getEntry();
    }
    else {
        return findHelper(ptr->getRight(), name);
    }
}

// this function finds the ptr to delete
TreeNode* TreeDB::findDeleteHelper(TreeNode *ptr, const string &name, TreeNode *&parent) {
    if (ptr == NULL) {
        return NULL;
    }
    if (ptr->getEntry()->getName() == name) {
        return ptr;
    }
    parent = ptr;
    if (name < ptr->getEntry()->getName()) {
        return findDeleteHelper(ptr->getLeft(), name, parent);
    }
    else {
        // apply recursion
        return findDeleteHelper(ptr->getRight(), name, parent);
    }
}

// Printing using in-order traversal
void TreeDB::printHelper(ostream &out, TreeNode *ptr) const {
    if (ptr == NULL) {
        return;
    }
    printHelper(out, ptr->getLeft());
    out << ptr;
    printHelper(out, ptr->getRight());
}

// Insertion recursively
bool TreeDB::insertHelper(TreeNode *ptr, DBentry *entry) {
    if (ptr->getEntry()->getName() == entry->getName()) {
        return false; 
    }
    // Left subtree
    if (entry->getName() < ptr->getEntry()->getName()) {
        if (ptr->getLeft() == NULL) {
            ptr->setLeft(new TreeNode(entry));
            return true;
        } 
        else {
            // apply recursion
            return insertHelper(ptr->getLeft(), entry);
        }
    } 
    // Right subtree
    else {
        if (ptr->getRight() == NULL) {
            ptr->setRight(new TreeNode(entry));
            return true;
        } 
        else {
            // apply recursion 
            return insertHelper(ptr->getRight(), entry);
        }
    }
}

// Post-order traversal
void TreeDB::clearHelper(TreeNode *ptr) {
    if (ptr == NULL) {
        return;
    }
    // Recursion for left and right
    clearHelper(ptr->getLeft());
    clearHelper(ptr->getRight());
    delete ptr;
}

// Prints the entry in a tree ptr
ostream &operator<<(ostream &out, TreeNode *rhs) {
    out << *rhs->getEntry();
    return out;
}