#include "block.h"
#include <cmath>
#include <iostream>

int Block::width() const
{
return data.size();
}

int Block::height() const
{
return data[0].size();
}

void Block::render(PNG &im, int column, int row) const
{
for ( int x = column; x < column + width(); x++){
    for ( int y = row; y < row + height(); y++){
        *im.getPixel(x,y) = data[x-column][y-row];
    }
}
}

void Block::build(PNG &im, int column, int row, int width, int height)
{
/* YOUR CODE HERE */
// Make zero before if non-zero

for ( int x = column; x < column + width; x++){
    vector<RGBAPixel> y_block;
    for ( int y = row; y < height+row; y++){
        y_block.push_back(*im.getPixel(x,y));
    }
    data.push_back(y_block);
}
}

void Block::flipVert(){
    // Iterates until top row is equal to or less than bottom
    int top = 0;
    int bottom = height()-1;

    while (top < bottom){
        for (int x = 0; x < width(); x++){
            RGBAPixel temp = data[x][top];
            data[x][top] = data[x][bottom];
            data[x][bottom] = temp;
        }

        bottom--;
        top++;    
    }
}

void Block::flipHoriz()
{
    int left =0;
    int right = width()-1;
    while (left < right){
            
        for(int y = 0; y < height(); y++) {
            RGBAPixel temp = data[left][y];
            data[left][y] = data[right][y];
            data[right][y] = temp;
        }
        
        left++;
        right--;
    }
}

void Block::rotateRight()
{
    
    for(int x = 0; x < width(); x++){
        for(int y = 0; y < height() ; y++){
            cout << data[y][x];
        }
            cout << "." << endl;
    }

    int size = width()- 1;
    for(int x = 0; x < width()/ 2; x++){
     for(int y = x; y < size - x; y++){
        // Stores the value of the current pixel
        RGBAPixel temp = data[x][y];
       
       //Now loops around, shifting 90 degrees
       data[x][y] = data[y][size - x];
       data[y][size -x] = data[size - x][size - y];
       data[size - x][size - y] = data[size -y][x];
 
       data[size - y][x] = temp;
     }
   }

    cout << "_______________" << endl;
    for(int x = 0; x < width(); x++){
        for(int y = 0; y < height() ; y++){
            cout << data[y][x];
        }
        cout << "." << endl;
    }

}