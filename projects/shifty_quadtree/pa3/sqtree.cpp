/**
 *
 * shifty quadtree (pa3)
 * sqtree.cpp
 * This file will be used for grading.
 *
 */

#include "sqtree.h"
#include <iostream>
#include <algorithm>

// First Node constructor, given.
SQtree::Node::Node(pair<int,int> ul, int w, int h, RGBAPixel a, double v)
  :upLeft(ul),width(w),height(h),avg(a),var(v),NW(NULL),NE(NULL),SE(NULL),SW(NULL)
{}

// Second Node constructor, given
SQtree::Node::Node(stats & s, pair<int,int> ul, int w, int h)
  :upLeft(ul),width(w),height(h),NW(NULL),NE(NULL),SE(NULL),SW(NULL) {
  avg = s.getAvg(ul,w,h);
  var = s.getVar(ul,w,h);
}

// SQtree destructor, given.
SQtree::~SQtree() {
  clear();
}

// SQtree copy constructor, given.
SQtree::SQtree(const SQtree & other) {
  copy(other);
}

// SQtree assignment operator, given.
SQtree & SQtree::operator=(const SQtree & rhs) {
  if (this != &rhs) {
    clear();
    copy(rhs);
  }
  return *this;
}

/**
 * SQtree constructor given tolerance for variance.
 */
SQtree::SQtree(PNG & imIn, double tol) {
  // Your code here.
  stats s = stats(imIn);
  pair<int, int> ul = make_pair(0,0);
  root = buildTree(s, ul, imIn.width(), imIn.height(), tol);
  cout<< "end "<<endl;
}


/**
 * Helper for the SQtree constructor.
 */
SQtree::Node * SQtree::buildTree(stats & s, pair<int,int> & ul,
				 int w, int h, double tol) {
  if(w*h == 0) return NULL;

  Node * current = new Node(ul, w, h, s.getAvg(ul, w, h), s.getVar(ul, w, h));
  if (current->var < tol) return current;
  pair<int,int> split = bestSplit(s, ul , w, h);

  pair<int,int> ne = make_pair(split.first, ul.second);
  pair<int, int> sw = make_pair(ul.first, split.second);

  if (split.first == ul.first && split.second == ul.second) return current;
  else if(split.first != ul.first && split.second != ul.second){ // four quadrant split
    current->NW  =  buildTree(s, ul, split.first-ul.first, split.second-ul.second, tol); // ptr to NW subtree
    current->NE  =  buildTree(s, ne,  w-split.first+ul.first, split.second-ul.second, tol); // ptr to NE subtree
    current->SW  =  buildTree(s, sw, split.first-ul.first, h-split.second+ul.second, tol); // ptr to SW subtree
    current->SE  =  buildTree(s, split, w-split.first+ul.first, h-split.second+ul.second, tol); // ptr to SE subtree
  }
  else if (split.first == ul.first){ // y line, horizontal spit
    current->NW  =  buildTree(s, ul, w, split.second-ul.second, tol); // ptr to NW subtree
    current->SW  =  buildTree(s, split, w, h-split.second+ul.second, tol); // ptr to SW subtree
  }
  else if (split.second == ul.second){ // x line, vertical split
    current->SE  =  buildTree(s, split, w-split.first+ul.first, h, tol); // ptr to SE subtree
    current->SW  =  buildTree(s, ul, split.first-ul.first, h, tol); // ptr to SW subtree
  }
  // current->NW  =  buildTree(s, ul, split.first+1, split.second+1, tol); // ptr to NW subtree
  // current->NE  =  buildTree(s, ne,  w-split.first+1, split.second+1, tol); // ptr to NE subtree
  // current->SW  =  buildTree(s, sw, split.first+1, h-split.second+1, tol); // ptr to SW subtree
  // current->SE  =  buildTree(s, split, w-split.first+1, h-split.second+1, tol); // ptr to SE subtree
 
  return current; 
}
  
/**
 * Render SQtree and return the resulting image.
 */
PNG SQtree::render() {
  PNG img(root->width, root->height);
  helpRender(root, img);
  return img;
}

/**
 * Delete allocated memory.
 */
void SQtree::clear() {
  clearNode(root);
  root = NULL;
  delete root;
}

void SQtree::copy(const SQtree & other) {
  root = copyNode(other.root);
}

int SQtree::size() {
  return countNode(root);
}

/**
 * Wants minimum max variablity
*/
std::pair<int,int> SQtree::bestSplit(stats & s, pair<int,int> & ul , int w, int h) {
  double minVar = -1.0;
  pair<int,int> split = ul;

  for (int y = ul.second; y < (ul.second+h); y++){
    for (int x = ul.first; x < (ul.first+w); x++){
      if (validSplit(ul,x,y,w,h)){
        double a = s.getVar(ul, x-ul.first, y-ul.second);
        double b = s.getVar(make_pair(x, ul.second), w+ul.first-x, y-ul.second); // inital y, new x
        double c = s.getVar(make_pair(ul.first, y), x-ul.first, h+ul.second-y); //inital x, new y, 
        double d = s.getVar(make_pair(x, y), w+ul.first-x, h+ul.second-y);
        
        if ( minVar == -1.0 || maxDouble(a, b, c, d) <= minVar){
          minVar = maxDouble(a, b, c, d);
          split = make_pair(x,y);
        }
      }
    }
  }
  return split;
}

bool SQtree::areSame(double a, double b)
{
    return fabs(a - b) < 0.00001;
}

int SQtree::countNode(Node* &node){
  if(node == NULL) return 0;
  return (1+countNode(node->NW)+countNode(node->NE)+countNode(node->SW)+countNode(node->SE));
}

SQtree::Node * SQtree::copyNode(Node * node){
  if(node == NULL) return NULL;

  Node* newNode = new Node(node->upLeft, node->width, node->height, node->avg, node->var);
  newNode->NW = copyNode(node->NW);
  newNode->NE = copyNode(node->NE);
  newNode->SE = copyNode(node->SE);
  newNode->SW = copyNode(node->SW);
  return newNode;
}


void SQtree::clearNode(Node *& node){
  if (node != NULL){
    clearNode(node->NE); 
    clearNode(node->NW);
    clearNode(node->SE); 
    clearNode(node->SW);

    node->NE = NULL; 
    node->NW = NULL;
    node->SE = NULL; 
    node->SW = NULL;

    delete(node);
    node = NULL;
  }
}

/**
 * Recursive call to render image
*/
void SQtree::helpRender(Node * node, PNG & im) {
  if (node == NULL) return; // empty node

  if (node->NW == NULL && node->NE == NULL && node->SW == NULL && node->SE == NULL) { // no children
    for (int y = 0; y < node->height; y++) {
      for (int x = 0; x < node->width; x++) {
        RGBAPixel * pixel = im.getPixel(node->upLeft.first + x, node->upLeft.second + y);
        *pixel = node->avg;
      }
    }
  }
  else {
    helpRender(node->NW, im); 
    helpRender(node->NE, im); // recursive calls
    helpRender(node->SW, im); 
    helpRender(node->SE, im);
  }
}


bool SQtree:: validSplit(pair<int, int> ul, int x, int y, int w, int h){
  if (x == ul.first && y == ul.second) return false;
  if (x ==(w+ul.first) || y == (h+ul.second)) return false; // may violate condition
  else return true; 
}

double SQtree:: maxDouble(double a, double b, double c, double d){
  double maxDouble;
  if (a > b) maxDouble = a;
  else maxDouble = b;

  if (c > maxDouble) maxDouble = c;

  if (d > maxDouble) maxDouble = d;

  return maxDouble;
}