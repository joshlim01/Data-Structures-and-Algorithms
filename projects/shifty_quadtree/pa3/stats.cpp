#include "stats.h"


#include "stats.h"

stats::stats(PNG & im){

/* Your code here!! */
  /**
   * Initialize the private vectors so that, for each color, entry 
   * (x,y) is the sum of that color value in the rectangle from
   * (0,0) to (x,y). Similarly, for each color, the sumSq vector
   * entry (x,y) is the sum of that color squared over all image
   * pixels from (0,0) to (x,y).
   */
  sumRed.resize(im.width(), vector<long>(im.height(), 0));
  sumGreen.resize(im.width(), vector<long>(im.height(), 0));
  sumBlue.resize(im.width(), vector<long>(im.height(), 0));
  sumsqRed.resize(im.width(), vector<long>(im.height(), 0));
  sumsqGreen.resize(im.width(), vector<long>(im.height(), 0));
  sumsqBlue.resize(im.width(), vector<long>(im.height(), 0));

  for (unsigned int x = 0; x < im.width(); x++) {
    for (unsigned int y = 0; y < im.height(); y++) {
      RGBAPixel *pixel = im.getPixel(x, y);

      if (x == 0 && y == 0) {
        sumRed[x][y] = pixel->r;
        sumGreen[x][y] = pixel->g;
        sumBlue[x][y] = pixel->b;
        sumsqRed[x][y] = pixel->r * pixel->r;
        sumsqGreen[x][y] = pixel->g * pixel->g;
        sumsqBlue[x][y] = pixel->b * pixel->b;
      }

      if (x == 0 && y != 0) {
        sumRed[x][y] = pixel->r + sumRed[x][y - 1];
        sumGreen[x][y] = pixel->g + sumGreen[x][y - 1];
        sumBlue[x][y] = pixel->b + sumBlue[x][y - 1];
        sumsqRed[x][y] = pixel->r * pixel->r + sumsqRed[x][y - 1];
        sumsqGreen[x][y] = pixel->g * pixel->g + sumsqGreen[x][y - 1];
        sumsqBlue[x][y] = pixel->b * pixel->b + sumsqBlue[x][y - 1];
      }

      if (y == 0 && x != 0) {
        sumRed[x][y] = pixel->r + sumRed[x - 1][y];
        sumGreen[x][y] = pixel->g + sumGreen[x - 1][y];
        sumBlue[x][y] = pixel->b + sumBlue[x - 1][y];
        sumsqRed[x][y] = pixel->r * pixel->r + sumsqRed[x - 1][y];
        sumsqGreen[x][y] = pixel->g * pixel->g + sumsqGreen[x - 1][y];
        sumsqBlue[x][y] = pixel->b * pixel->b + sumsqBlue[x - 1][y];
      }

      if (x != 0 && y != 0) {
        sumRed[x][y] = pixel->r + sumRed[x - 1][y] + sumRed[x][y - 1] - sumRed[x - 1][y - 1];
        sumGreen[x][y] = pixel->g + sumGreen[x - 1][y] + sumGreen[x][y - 1] - sumGreen[x - 1][y - 1];
        sumBlue[x][y] = pixel->b + sumBlue[x - 1][y] + sumBlue[x][y - 1] - sumBlue[x - 1][y - 1];
        sumsqRed[x][y] = pixel->r * pixel->r + sumsqRed[x - 1][y] + sumsqRed[x][y - 1] - sumsqRed[x - 1][y - 1];
        sumsqGreen[x][y] = pixel->g * pixel->g + sumsqGreen[x - 1][y] + sumsqGreen[x][y - 1] - sumsqGreen[x - 1][y - 1];
        sumsqBlue[x][y] = pixel->b * pixel->b + sumsqBlue[x - 1][y] + sumsqBlue[x][y - 1] - sumsqBlue[x - 1][y - 1];
      }
    }
  }

}


long stats::getSum(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
//  Returns the sums of all pixel values in one color channel.
//  Useful in computing the average color of a rectangle.

  long sum = 0;

  if (ul.first == 0 && ul.second == 0) {
    if (channel == 'r')
        sum = sumRed[w - 1][h - 1];
    if (channel == 'g')
        sum = sumGreen[w - 1][h - 1];
    if (channel == 'b')
        sum = sumBlue[w - 1][h - 1];
  }

  if (ul.first == 0 && ul.second != 0)
  {
    if (channel == 'r')
        sum = sumRed[w - 1][h - 1 + ul.second] - sumRed[w - 1][ul.second - 1];
    if (channel == 'g')
        sum = sumGreen[w - 1][h - 1 + ul.second] - sumGreen[w - 1][ul.second - 1];
    if (channel == 'b')
        sum = sumBlue[w - 1][h - 1 + ul.second] - sumBlue[w - 1][ul.second - 1];
  }

  if (ul.first != 0 && ul.second == 0)
  {
    if (channel == 'r')
        sum = sumRed[w - 1 + ul.first][h - 1] - sumRed[ul.first - 1][h - 1];
    if (channel == 'g')
        sum = sumGreen[w - 1 + ul.first][h - 1] - sumGreen[ul.first - 1][h - 1];
    if (channel == 'b')
        sum = sumBlue[w - 1 + ul.first][h - 1] - sumBlue[ul.first - 1][h - 1];
  }

  if (ul.first != 0 && ul.second != 0)
  {
    if (channel == 'r')
        sum = sumRed[w - 1 + ul.first][h - 1 + ul.second] - sumRed[ul.first - 1][h - 1 + ul.second] - sumRed[ul.first + w - 1][ul.second - 1] + sumRed[ul.first - 1][ul.second - 1];
    if (channel == 'g')
        sum = sumGreen[w - 1 + ul.first][h - 1 + ul.second] - sumGreen[ul.first - 1][h - 1 + ul.second] - sumGreen[ul.first + w - 1][ul.second - 1] + sumGreen[ul.first - 1][ul.second - 1];
    if (channel == 'b')
        sum = sumBlue[w - 1 + ul.first][h - 1 + ul.second] - sumBlue[ul.first - 1][h - 1 + ul.second] - sumBlue[ul.first + w - 1][ul.second - 1] + sumBlue[ul.first - 1][ul.second - 1];
  }
  return sum;
}

long stats::getSumSq(char channel, pair<int,int> ul, int w, int h){
/* Your code here!! */
  long sum = 0;

  if (ul.first == 0 && ul.second == 0) {
    if (channel == 'r')
        sum = sumsqRed[w - 1 + ul.first][h - 1 + ul.second];
    if (channel == 'g')
        sum = sumsqGreen[w - 1 + ul.first][h - 1 + ul.second];
    if (channel == 'b')
        sum = sumsqBlue[w - 1 + ul.first][h - 1 + ul.second];
  }

  else if (ul.first == 0) {
    if (channel == 'r')
        sum = sumsqRed[w - 1 + ul.first][h - 1 + ul.second] - sumsqRed[w - 1 + ul.first][ul.second - 1];
    if (channel == 'g')
        sum = sumsqGreen[w - 1 + ul.first][h - 1 + ul.second] - sumsqGreen[w - 1 + ul.first][ul.second- 1];
    if (channel == 'b')
        sum = sumsqBlue[w - 1 + ul.first][h - 1 + ul.second] - sumsqBlue[w - 1 + ul.first][ul.second- 1];
  }

  else if (ul.second == 0) {
    if (channel == 'r')
        sum = sumsqRed[w - 1 + ul.first][h - 1 + ul.second] - sumsqRed[ul.first- 1][h - 1 + ul.second];
    if (channel == 'g') 
        sum = sumsqGreen[w - 1 + ul.first][h - 1 + ul.second] - sumsqGreen[ul.first - 1][h - 1 + ul.second];
    if (channel == 'b')
        sum = sumsqBlue[w - 1 + ul.first][h - 1 + ul.second] - sumsqBlue[ul.first - 1][h - 1 + ul.second];
  }

  else {
    if (channel == 'r')
      sum = sumsqRed[w - 1 + ul.first][h - 1 + ul.second] - sumsqRed[ul.first - 1][h - 1 + ul.second] - sumsqRed[ul.first + w - 1][ul.second - 1] + sumsqRed[ul.first - 1][ul.second - 1];
    if (channel == 'g')
      sum = sumsqGreen[w - 1 + ul.first][h - 1 + ul.second] - sumsqGreen[ul.first - 1][h - 1 + ul.second] - sumsqGreen[ul.first + w - 1][ul.second - 1] + sumsqGreen[ul.first - 1][ul.second - 1];
    if (channel == 'b')
      sum = sumsqBlue[w - 1 + ul.first][h - 1 + ul.second] - sumsqBlue[ul.first - 1][h - 1 + ul.second] - sumsqBlue[ul.first + w - 1][ul.second - 1] + sumsqBlue[ul.first - 1][ul.second - 1];
  }

  return sum;
}

// given a rectangle, compute its sum of squared deviations from mean, over all color channels.
// see written specification for a description of this function.
double stats::getVar(pair<int,int> ul, int w, int h){
/* Your code here!! */
  double area = w * h;

  if (area == 0)
    return 0.0;

  else {
    double sum_red = getSum('r', ul, w, h);
    double sum_green = getSum('g', ul, w, h);
    double sum_blue = getSum('b', ul, w, h);

    double sumsq_red = getSumSq('r', ul, w, h);
    double sumsq_green = getSumSq('g', ul, w, h);
    double sumsq_blue = getSumSq('b', ul, w, h);

    double var = (sumsq_red - (sum_red * sum_red) / area) + (sumsq_green - (sum_green * sum_green) / area) + (sumsq_blue - (sum_blue * sum_blue) / area);
    return var;
}
}
		
RGBAPixel stats::getAvg(pair<int,int> ul, int w, int h){
/* Your code here!! */
  RGBAPixel pixel;

  long area = w * h;

  if (area == 0)
    return pixel;

  long temp_red;
  long temp_green;
  long temp_blue;

  temp_red = getSum('r', ul, w, h) / area;
  temp_green = getSum('g', ul, w, h) / area;
  temp_blue = getSum('b', ul, w, h) / area;

  pixel.r = temp_red;
  pixel.g = temp_green;
  pixel.b = temp_blue;

  return pixel;
}