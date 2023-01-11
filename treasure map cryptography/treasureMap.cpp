#include "treasureMap.h"
#include "queue.h"
#include <string>
using namespace std;

treasureMap::treasureMap(const PNG & baseim, const PNG & mazeim, pair<int,int> s)
{

/* YOUR CODE HERE */
base = baseim;
maze = mazeim;
start = s;
}

void treasureMap::setGrey(PNG & im, pair<int,int> loc){

/* YOUR CODE HERE */
    RGBAPixel * pixel = im.getPixel(loc.first, loc.second);
    pixel->r = 2 * (pixel->r / 4);
    pixel->b = 2 * (pixel->b / 4);
    pixel->g = 2 * (pixel->g / 4);
}

void treasureMap::setLOB(PNG & im, pair<int,int> loc, int d){
	unsigned char dmod = d % 64;
	RGBAPixel *pixel = im.getPixel(loc.first, loc.second);

	unsigned char encodedRed = (dmod >> 4) & 3; 
	unsigned char encodedGreen = (dmod >> 2) & 3;
	unsigned char encodedBlue = dmod & 3;

	pixel->r = (pixel->r & 252) + encodedRed;
	pixel->g = (pixel->g & 252) + encodedGreen;
	pixel->b = (pixel->b & 252) + encodedBlue;
}

PNG treasureMap::renderMap(){

/* YOUR CODE HERE */
    // 0. Make a copy of the base image. You will change its pixels corresponding to 
    //      locations within the maze.
	PNG renderedMap = base;

    // 1. Initialize working vectors: 
    //      a. Define a 2d vector whose dimensions are the same as the image, and whose 
    //      values are boolean. Use this vector to indicate whether or not a location
    //      has been visited by the traversal.
    //      b. Define a 2d vector whose dimensions are the same as the image, and whose
    //      values are integers. This structure will be used to store the length of the
    //      shortest path between the current location and the start location.
	vector<vector<bool>> visited(base.height(), vector<bool>(base.width(), false)); 
	vector<vector<int>> location(base.height(), vector<int>(base.width(), -1));

    // 2. Initialize a queue whose purpose is to orchestrate the traversal. As you search
    //      the maze, you will have to keep a list of locations to explore. The queue maintains
    //      that list.
	Queue<pair<int,int>> BFStraversal;

    // 3. For the start location, do the following:
    //      a. mark the start location as visited (structure in step 1a)
    //      b. set the distance to the start to be 0 (structure in step 1b)
    //      c. encode the distance within the start location's lower order bits of the treasure map 
    //          (structure in step 0)
    //      d. enqueue the start location. (structure in step 2)
    visited[start.second][start.first] = true;
    location[start.second][start.first] = 0;
    setLOB(renderedMap, start, location[start.second][start.first]);
    BFStraversal.enqueue(start);

    // 4. While the queue is not empty:
    //      a. set the current location, curr, to the value at the front of the queue (and dequeue). 
    //      b. for each compass neighbor, p, of curr in the order of left, below, right, above:
    //          i. if p is "good" (defined below) then
    //              1) mark p's location as visited
    //              2) set p's distance to the start to be curr's distance, + 1. 
    //              3) encode the distance in the lower order bits of the treasure map
    //              4) enqueue p
    //      
    while(!BFStraversal.isEmpty()) {
        pair<int,int> curr = BFStraversal.dequeue();
        
        for(int i = 0; i < neighbors(curr).size(); i++){
            // initializing compass neighbor p
            pair<int, int> p = neighbors(curr)[i];

            if(good(visited, curr, p)) {
                visited[p.second][p.first] = true;
                location[p.second][p.first] = location[curr.second][curr.first] + 1;
                setLOB(renderedMap, p, location[p.second][p.first]);
                BFStraversal.enqueue(p);               
            }
        }
    }

    // 5. Return the image
	return renderedMap;
}


PNG treasureMap::renderMaze(){

/* YOUR CODE HERE */
    // 0. Make a copy of the base image. You will change its pixels corresponding to 
    //      locations within the maze.
    PNG renderedMaze = base;

    // 1. Initialize working vectors: 
    //      a. Define a 2d vector whose dimensions are the same as the image, and whose 
    //      values are boolean. Use this vector to indicate whether or not a location
    //      has been visited by the traversal.
    vector<vector<bool>> visited(base.height(), vector<bool>(base.width(), false));

    // 2. Initialize a queue whose purpose is to orchestrate the traversal. As you search
    //      the maze, you will have to keep a list of locations to explore. The queue maintains
    //      that list.
    Queue<pair<int,int>> BFStraversal;
    
    // 3. For the start location, do the following:
    //      a. mark the start location as visited (structure in step 1a)
    //      b. set the start location to 50% of the original color
    //      c. enqueue the start location. (structure in step 2)
    visited[start.second][start.first] = true;
    setGrey(renderedMaze, start);
    BFStraversal.enqueue(start);

    // 4. While the queue is not empty:
    //      a. set the current location, curr, to the value at the front of the queue (and dequeue). 
    //      b. for each compass neighbor, p, of curr in the order of left, below, right, above:
    //          i. if p is "good" (defined below) then
    //              1) mark p's location as visited
    //              2) set p to 50% of the original color
    //              3) enqueue p
    //     
    while(!BFStraversal.isEmpty()) {
        pair<int,int> curr = BFStraversal.dequeue();
        
        for(int i = 0; i < neighbors(curr).size(); i++){
            // initializing compass neighbor p
            pair<int, int> p = neighbors(curr)[i];

            if(good(visited, curr, p)) {
                visited[p.second][p.first] = true;
                setGrey(renderedMaze, p);
                BFStraversal.enqueue(p);              
            }
        }
    }

    // 5. Draw a red box around the start location
    drawRedBox(renderedMaze, start);

    // 6. Return the image
    return renderedMaze;
}

bool treasureMap::good(vector<vector<bool>> & v, pair<int,int> curr, pair<int,int> next){

/* YOUR CODE HERE */
	if(!(next.first < maze.width() && (next.first >= 0) && (next.second < maze.height()) && (next.second >= 0)))
        return false;
    
    if(v[next.second][next.first])
        return false;

	RGBAPixel *currColor = maze.getPixel(curr.first, curr.second);
	RGBAPixel *nextColor = maze.getPixel(next.first, next.second);

    if(!(*currColor == *nextColor))
        return false;
    
    return true;
}

vector<pair<int,int>> treasureMap::neighbors(pair<int,int> curr) {

/* YOUR CODE HERE */
	vector<pair<int,int>> neighbors;

	neighbors.push_back(make_pair(curr.first - 1, curr.second));
	neighbors.push_back(make_pair(curr.first, curr.second + 1));
	neighbors.push_back(make_pair(curr.first + 1, curr.second));
	neighbors.push_back(make_pair(curr.first, curr.second - 1));

	return(neighbors);
}

// int encode(int rgb, int d, string color) {
//     int new_rgb;
//     char n1, n2;
//     string new_color = to_string(rgb);
//     string ds = to_string(d%64);
//     new_color.pop_back();
//     new_color.pop_back();

//     if(color == "red") {
//         n1 = ds[0];
//         n2 = ds[1];
//     }

//     else if (color == "green") {
//         n1 = ds[2];
//         n2 = ds[3];
//     }

//     else {
//         n1 = ds[4];
//         n2 = ds[5];
//     }
//     new_color = new_color + n1 + n2;
//     new_rgb = stoi(new_color);
//     return(new_rgb);
// }

void treasureMap::drawRedBox(PNG & im, pair<int,int> pos) { 
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