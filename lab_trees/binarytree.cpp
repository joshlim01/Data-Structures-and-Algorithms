/**
 * @file binarytree.cpp
 * Definitions of the binary tree functions you'll be writing for this lab.
 * You'll need to modify this file, as well as binarytree.h.
 */

#include <iostream>

using namespace std;

/**
 * @return The height of the binary tree. Recall that the height of a binary
 *  tree is just the length of the longest path from the root to a leaf, and
 *  that the height of an empty tree is -1.
 */
template <typename T>
int BinaryTree<T>::height() const
{
    // Call recursive helper function on root
    return height(root);
}

/**
 * Private helper function for the public height function, with an additional
 * Node* parameter to allow for recursive calls. NOTE: use this as an example
 * for writing your own private helper functions.
 * @param subRoot
 * @return The height of the subtree.
 */
template <typename T>
int BinaryTree<T>::height(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL)
        return -1;

    // Recursive definition
    return 1 + max(height(subRoot->left), height(subRoot->right));
}

/**
 * Prints out the values of the nodes of a binary tree in order.
 * That is, everything to the left of a node will be printed out before that
 * node itself, and everything to the right of a node will be printed out after
 * that node.
 */
template <typename T>
void BinaryTree<T>::printLeftToRight() const
{
    // Your code here
    printLefttoRight(root);

    // Do not remove this line - used for correct print output
    cout << endl;
}

/**
 * Private helper function for the public printLefttoRight function, with an additional
 * Node* parameter to allow for recursive calls.
 * @param subRoot
 * 
 */
template <typename T>
void BinaryTree<T>::printLefttoRight(const Node* subRoot) const
{
    // Base case
    if (subRoot == NULL) {
      return;
    }

    // Recursive definition
    printLefttoRight(subRoot->left);
    cout << subRoot->elem << " ";
    printLefttoRight(subRoot->right);
}

/**
 * Flips the tree over a vertical axis, modifying the tree itself
 * (i.e. not creating a flipped copy).
 */
template <typename T>
void BinaryTree<T>::mirror()
{
    // Your code here
    mirror(root);
}

/**
 * Private helper function for the public mirror function, with an additional
 * Node* parameter to allow for recursive calls.
 * @param subRoot
 * 
 */
template <typename T>
void BinaryTree<T>::mirror(Node* subRoot)
{
    // Base case
    if (subRoot == NULL) {
      return;
    }

    // Recursive definition
    mirror(subRoot->left);
    mirror(subRoot->right);
 
    /* swap the pointers in this node */
    Node * temp = subRoot->left;
    subRoot->left = subRoot->right;
    subRoot->right = temp;
}

/**
 * Prints out all the possible paths from the root of the tree to any leaf node.
 * That is, all sequences starting at the root node and continuing downwards,
 * ending at a leaf node. Paths ending in a left node should be printed before
 * paths ending in a node further to the right.
 */
template <typename T>
void BinaryTree<T>::printPaths() const
{
  // Your code here
  printPaths(root, "Path: ");
}

/**
 * Private helper function for the public printPaths function, with an additional
 * Node* parameter to allow for recursive calls.
 * @param subRoot
 * 
 */
template <typename T>
void BinaryTree<T>::printPaths(const Node* subRoot, string path) const
{
    // Base case
    if (subRoot == NULL) {
      return;
    }

    // Recursive definition
    path += to_string(subRoot->elem) + " ";

    if(!subRoot->left && !subRoot->right)
      cout << path << endl;
    
    printPaths(subRoot->left, path);
    printPaths(subRoot->right, path);
}


/**
 * Each node in a tree has a distance from the root node - the depth of that
 * node, or the number of edges along the path from that node to the root. This
 * function returns the sum of the distances of all nodes to the root node (the
 * sum of the depths of all the nodes). Your solution should take O(n) time,
 * where n is the number of nodes in the tree.
 * @return The sum of the distances of all nodes to the root.
 */
template <typename T>
int BinaryTree<T>::sumDistances() const
{
  // Your code here
  return sumDistances(root, 0);
}

/**
 * Private helper function for the public sumDistances function, with an additional
 * Node* parameter to allow for recursive calls. NOTE: use this as an example
 * for writing your own private helper functions.
 * @param subRoot
 * @return The sum of the distances of all nodes to the root.
 */
template <typename T>
int BinaryTree<T>::sumDistances(const Node* subRoot, int depth) const
{
    // Base case
    if (subRoot == NULL)
        return 0;

    // Recursive definition
    return depth + sumDistances(subRoot->left, depth + 1) + sumDistances(subRoot->right, depth + 1);
}

/**
 * @return True if an in-order traversal of the tree would produce a
 *  nondecreasing list output values, and false otherwise. This is also the
 *  criterion for a binary tree to be a binary search tree.
 */
template <typename T>
bool BinaryTree<T>::isOrdered() const
{
  // Your code here

  // TA's Solution VERSION #1 (ugly one)
  return isOrdered(root, numeric_limits<T>::min(), numeric_limits<T>::max());
  
  // TA's Solution VERSION #2
  // vector<T> result
  // vectorize(root, result);
  // return is_sorted(result.begin(), result.end());

  // TA's Solution VERSION #3
  // return isOrdered2(root, NULL, NULL);
}

/**
 * Private helper function for the public isOrdered function, with an additional
 * Node* parameter to allow for recursive calls. NOTE: use this as an example
 * for writing your own private helper functions.
 * @param subRoot
 * @return The sum of the distances of all nodes to the root.
 */
template <typename T>
bool BinaryTree<T>::isOrdered(const Node* subRoot, T lower, T upper) const
{
    // Base case
    if (subRoot == NULL)
        return true;

    // Recursive definition
    return(subRoot->elem >= lower)
          && (subRoot->elem <= upper)
          //bound on left branch is between the current lower bound
          // and the current node value
          && isOrdered(subRoot->left, lower, subRoot->elem)
          //bound on right branch is between the current node value
          // and the current upper bound
          && isOrdered(subRoot->right, subRoot->elem, upper);
}

/**
 * Private helper function for the public isOrdered2 function, with an additional
 * Node* parameter to allow for recursive calls. NOTE: use this as an example
 * for writing your own private helper functions.
 * @param subRoot
 * @return The sum of the distances of all nodes to the root.
 */
template <typename T>
bool BinaryTree<T>::isOrdered2(const Node* subRoot, const Node* left, const Node* right) const
{
    // Base case
    if (subRoot == NULL)
        return true;

    // Recursive definition
    return !(left && subRoot->elem < left->elem)
        && !(right && subRoot->elem > right->elem)
        && isOrdered2(subRoot->left, left, subRoot)
        && isOrdered2(subRoot->right, subRoot, right);
}