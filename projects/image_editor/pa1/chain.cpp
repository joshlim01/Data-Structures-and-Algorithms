#include "chain.h"
#include "chain_given.cpp"
#include <cmath>
#include <iostream>

// PA1 functions

/**
 * Destroys the current Chain. This function should ensure that
 * memory does not leak on destruction of a chain.
 */
Chain::~Chain()
{
    clear();
    if(empty()){
        delete head_;
        head_ = NULL;
    }
}

/**
 * Inserts a new node at the back of the List.
 * This function **SHOULD** create a new ListNode.
 *
 * @param ndata The data to be inserted.
 */
void Chain::insertBack(const Block &ndata)
{
    Node *temp = head_;
    while(temp->next != head_){
        temp = temp->next;
    }

    Node *toInsert = new Node(ndata);
    temp->next = toInsert;
    toInsert->next = head_;
    length_++;
    temp = NULL;
}


/**
 * Swaps the two nodes at the indexes "node1" and "node2".
 * The indexes of the nodes are 1 based.
 * assumes i and j are valid (in {1,length_} inclusive)
 * 
 */
void Chain::swap(int i, int j)
{
    if(i == j){
        return;
    }

    if(i > j){
        i ^= j;
        j ^= i;
        i ^= j;
    }

    // Finds the nodes needed to swap
    Node * node_i_prev = walk(head_, i-1);
    Node * node_i = walk(head_, i);

    Node * node_j_prev = walk(head_, j-1);
    Node * node_j = walk(head_, j);
    Node * node_j_next= node_j->next;

    if(j-i==1){
        node_i_prev->next = node_j;
        node_j->next= node_i;
        node_i->next= node_j_next;
        return;
    }

    // Swaps
    node_i_prev->next = node_j;
    node_j->next = node_i->next;

    node_j_prev->next = node_i;
    node_i->next = node_j_next;
}

/**
 * Reverses the chain
 */
void Chain::reverse()
{
int i = 1;
int j = length_;
while(i < j){
    swap(i, j);
    i++;
    j--;
}
}

/*
* Modifies the current chain by "rotating" every k nodes by one position.
* In every k node sub-chain, remove the first node, and place it in the last 
* position of the sub-chain. If the last sub-chain has length less than k,
* then don't change it at all. 
* Some examples with the chain a b c d e:
*   k = 1: a b c d e
*   k = 2: b a d c e
*   k = 3: b c a d e
*   k = 4: b c d a e
*/
void Chain::rotate(int k)
{
    int section = k;
    int swaps = section-1;
    while(section <= length_){
        for(int i = 0; i< swaps; i++){
            swap(section-swaps, section-i);
        }
        section +=k;
    }
}


/**
 * Destroys ALL dynamically allocated memory associated with the
 * current Chain class.
 */
void Chain::clear()
{
    Node* current = head_->next;

    while(current != head_) {
        Node* temp = current;
        current = current->next;
        delete temp;
        temp = NULL;
        length_--;
    }
    

    head_->next = head_;
    current = NULL;
}

/* makes the current object into a copy of the parameter:
 * All member variables should have the same value as
 * those of other, but the memory should be completely
 * independent. This function is used in both the copy
 * constructor and the assignment operator for Chains.
 */
void Chain::copy(Chain const &other)
{
    head_ = new Node(other.head_->data);
    head_->next = head_;

    width_ = other.width_;
    height_ = other.height_;
    length_ = 0;

    Node * temp = other.head_->next;
    for(int i=0; i < other.size(); i++){
        insertBack(temp->data);
        temp = temp->next;
    }
}