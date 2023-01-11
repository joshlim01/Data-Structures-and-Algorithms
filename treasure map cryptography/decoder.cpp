#include "decoder.h"
#include "queue.h"
#include "stack.h"
using namespace std;

decoder::decoder(const PNG & tm, pair<int,int> s)
   :start(s),mapImg(tm) {

/* YOUR CODE HERE */
    // 0. Make a copy of the base image. You will change its pixels corresponding to 
    //      locations within the maze.
	PNG DecodedMap = tm;

	// 1. Initialize working vectors: 
    //      a. Define a 2d vector whose dimensions are the same as the image, and whose 
    //      values are boolean. Use this vector to indicate whether or not a location
    //      has been visited by the traversal.
    //      b. Define a 2d vector whose dimensions are the same as the image, and whose
    //      values are integers. This structure will be used to store the length of the
    //      shortest path between the current location and the start location.
    //      c. Define the additional working vector whose purpose is to encode,  
    // 		for every location, the location from which it was first discovered. (prevLocation) 
    // 		This working structure will help you determine the path from any location to the start 
    // 		(and thus from the start to any location).
	vector<vector<bool>> visited(tm.height(), vector<bool>(tm.width(), false)); 
	vector<vector<int>> location(tm.height(), vector<int>(tm.width(), -1));
	vector<vector<pair<int,int>>> prevLocation(tm.height(), vector<pair<int,int>>(tm.width(), make_pair(-1,-1)));

    // 2. Initialize a queue whose purpose is to orchestrate the traversal. As you search
    //      the maze, you will have to keep a list of locations to explore. The queue maintains
    //      that list.
	Queue<pair<int,int>> BFStraversal;

    // 3. For the start location, do the following:
    //      a. mark the start location as visited (structure in step 1a)
    //      b. set the distance to the start to be 0
    //      c. enqueue the start location. (structure in step 2)
	visited[start.second][start.first] = true;
	location[start.second][start.first] = 0;
	BFStraversal.enqueue(start);

	// 4. Define the current location, curr
	pair<int,int> curr;

    // 5. While the queue is not empty:
    //      a. Assign curr to the value at the front of the queue (and dequeue). 
    //      b. for each compass neighbor, p, of curr in the order of left, below, right, above:
    //          i. if p is "good" (defined below) then
    //              1) mark p's location as visited
    //              2) set p's distance to the start to be curr's distance, + 1
    //              3) assign p's previous location to curr
	//				4) enqueue p
    //     
	while(!BFStraversal.isEmpty()) {
      curr = BFStraversal.dequeue();

		for (unsigned long i = 0; i < neighbors(curr).size(); i++) {
            // initializing compass neighbor p
            pair<int, int> p = neighbors(curr)[i];

			if (good(visited, location, curr, p)) {
				visited[p.second][p.first] = true;
				location[p.second][p.first] = location[curr.second][curr.first] + 1;
				prevLocation[p.second][p.first] = curr;
				BFStraversal.enqueue(p);
			}
		}
	}

    // 6. Initialize a stack whose purpose is to orchestrate the reverse traversal.
	// (I chose a stack so we can go backwards)
	Stack<pair<int,int>> reverseTraversal;

	// 7. Fill the stack with all previous locations starting with the end and ending with the start location.
	for (pair<int,int> next = curr; next != start; next = prevLocation[next.second][next.first]) {
		reverseTraversal.push(next);
	}
    reverseTraversal.push(start);

	// 8. Fill PathPts from start location to end by popping the stack.
    while(!reverseTraversal.isEmpty()) {
    	pathPts.push_back(reverseTraversal.pop());
    }
}

PNG decoder::renderSolution(){

/* YOUR CODE HERE */
	PNG renderedSolution = mapImg;

	for (int i = 0; i < pathPts.size(); i++) {
		pair<int,int> currPt = pathPts[i];
		RGBAPixel *pixel = renderedSolution.getPixel(currPt.first, currPt.second);
		pixel->r = 255, pixel->g = 0, pixel->b = 0;
	}
	return renderedSolution;
}

PNG decoder::renderMaze(){

/* YOUR CODE HERE */
	PNG renderedMaze = mapImg;
	vector<vector<bool>> visited(mapImg.height(), vector<bool>(mapImg.width(), false)); 
	vector<vector<int>> location(mapImg.height(), vector<int>(mapImg.width(), -1));
	Queue<pair<int,int>> BFStraversal;

	visited[start.second][start.first] = true;
	location[start.second][start.first] = 0;
	setGrey(renderedMaze, start);
	BFStraversal.enqueue(start);

	while(!BFStraversal.isEmpty()) {
		pair<int,int> curr = BFStraversal.dequeue();

		for (int i = 0; i < neighbors(curr).size(); i++) {
            // initializing compass neighbor p
            pair<int, int> p = neighbors(curr)[i];

			if (good(visited, location, curr, p)) {
				visited[p.second][p.first] = true;
				location[p.second][p.first] = location[curr.second][curr.first] + 1;
				setGrey(renderedMaze, p);
				BFStraversal.enqueue(p);
			}
		}
	}

	drawRedBox(renderedMaze, start);
	return renderedMaze;

}

void decoder::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
   RGBAPixel *pixel = im.getPixel(loc.first, loc.second);
	pixel->r = 2 * (pixel->r / 4);
	pixel->g = 2 * (pixel->g / 4);
	pixel->b = 2 * (pixel->b / 4);
}

pair<int,int> decoder::findSpot(){

/* YOUR CODE HERE */
	return pathPts[pathPts.size() - 1];
}

int decoder::pathLength(){

/* YOUR CODE HERE */
	return pathPts.size();
}

bool decoder::good(vector<vector<bool>> & v, vector<vector<int>> & d, pair<int,int> curr, pair<int,int> next){

/* YOUR CODE HERE */
	if(!(next.first < mapImg.width() && (next.first >= 0) && (next.second < mapImg.height()) && (next.second >= 0)))
        return false;
    
    if(v[next.second][next.first])
        return false;

	RGBAPixel *nextColor = mapImg.getPixel(next.first, next.second);

    if(!(compare(*nextColor, d[curr.second][curr.first])))
      return false;
    
    return true;
}

vector<pair<int,int>> decoder::neighbors(pair<int,int> curr) {

/* YOUR CODE HERE */
	vector<pair<int,int>> neighbors;

	neighbors.push_back(make_pair(curr.first - 1, curr.second));
	neighbors.push_back(make_pair(curr.first, curr.second + 1));
	neighbors.push_back(make_pair(curr.first + 1, curr.second));
	neighbors.push_back(make_pair(curr.first, curr.second - 1));

	return neighbors;
}


bool decoder::compare(RGBAPixel p, int d){

/* YOUR CODE HERE */
	return ((p.r % 4) * 16 + (p.g % 4) * 4 + (p.b % 4)) == (d + 1) % 64;
}

void decoder::drawRedBox(PNG & im, pair<int,int> pos) { 
    // checking the top side of the box
    int top_row = pos.second - 3;
    if(top_row < 0)
        top_row = 0;

    // checking the bottom side of the box
	int bottom_row = pos.second + 3;
    if(bottom_row > im.height() - 1)
        bottom_row = im.height() - 1;
    
    // checking the left side of the box
    int left_col = pos.first - 3;
    if(left_col < 0)
        left_col = 0;

    // checking the right side of the box
    int right_col = pos.first + 3;
    if(right_col > im.width() - 1)
        right_col = im.width() - 1;
    
    
    for(int row = top_row; row <= bottom_row; row++) {
        for(int col = left_col; col <= right_col; col++) {
            RGBAPixel * pixel = im.getPixel(col, row);
            pixel->r = 255, pixel->g = 0, pixel->b = 0; 
        }
    }
}