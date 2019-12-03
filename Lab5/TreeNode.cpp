#include "TreeNode.h"

// Setting left and right to null and entry pointer
TreeNode::TreeNode(DBentry *_entryPtr) {
    entryPtr = _entryPtr;
    left = NULL;
    right = NULL;
}

// Destructor
TreeNode::~TreeNode() {
    delete entryPtr;
}

// Setting left 
void TreeNode::setLeft(TreeNode *newLeft) {
    left = newLeft;
}

// Setting right
void TreeNode::setRight(TreeNode *newRight) {
    right = newRight;
}

// Returns left
TreeNode *TreeNode::getLeft() const {
    return left;
}

// Returns right
TreeNode *TreeNode::getRight() const {
    return right;
}

// Returns the entry pointer
DBentry *TreeNode::getEntry() const {
    return entryPtr;
}