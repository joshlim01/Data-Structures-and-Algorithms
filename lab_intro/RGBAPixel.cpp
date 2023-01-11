#include <iostream>
#include <cmath>
#include <cstdlib>

#include "RGBAPixel.h"

namespace cs221util {

RGBAPixel::RGBAPixel() {
    r = 255;
    g = 255;
    b = 255;

    a = 1.0;
   
}

RGBAPixel::RGBAPixel (char red, char green, char blue) {
    r = red;
    g = green;
    b = blue;

    a = 1.0;

}

RGBAPixel::RGBAPixel (char red, char green, char blue, double opacity) {
    r = red;
    g = green;
    b = blue;

    a = opacity;
}

}