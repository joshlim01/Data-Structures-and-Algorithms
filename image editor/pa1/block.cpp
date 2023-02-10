#include "block.h"
#include <cmath>
#include <iostream>

int Block::width() const
{
/* YOUR CODE HERE */
return data.size();
}
int Block::height() const
{
/* YOUR CODE HERE */
return data[0].size();
}

void Block::render(PNG &im, int column, int row) const
{
/* YOUR CODE HERE */
for(int x = column; x < column + width(); x++) {
    for(int y = row; y < row + height(); y++) {
        *im.getPixel(x,y) = data[x-column][y-row];
    }
}
}

void Block::build(PNG &im, int column, int row, int width, int height)
{
/* YOUR CODE HERE */
for(int x = column; x < column + width; x++) {
    vector<RGBAPixel> y_block;

    for(int y = row; y < row + height; y++) {
        y_block.push_back(*im.getPixel(x,y));
    }
    data.push_back(y_block);
}
}

void Block::flipVert()
{
/* YOUR CODE HERE */
int top = 0;
int bottom = height() - 1;
    while(top <= bottom) {
        for(int x = 0; x < width(); x++) {
            RGBAPixel temp = data[x][top];
            data[x][top] = data[x][bottom];
            data[x][bottom] = temp;
        }
        top++;
        bottom--;
    }
}

void Block::flipHoriz()
{
/* YOUR CODE HERE */
int left = 0;
int right = width() - 1;
    while(left < right) {        
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
/* YOUR CODE HERE */
vector<vector<RGBAPixel>> original_data;

for(int x = 0; x < width(); x++) {
    vector<RGBAPixel> y_block;

    for(int y = 0; y < height(); y++) {
        y_block.push_back(data[y][x]);
    }
    original_data.push_back(y_block);
}

for(int x = 0; x < width(); x++) {
    for(int y = 0; y < height(); y++) {
        data[y][x] = original_data[height() - 1- y][x];
    }
}
}
