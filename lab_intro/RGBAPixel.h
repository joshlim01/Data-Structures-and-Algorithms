#ifndef CS221UTIL_RGBAPixel_h
#define CS221UTIL_RGBAPixel_h
//header guards

#include <iostream>
#include <cmath>
#include <cstdlib>

namespace cs221util {
    class RGBAPixel {       // The class
        public:             // Access specifier
            unsigned char r, g, b;
            double a;

            RGBAPixel(); //constructor
            RGBAPixel(char red, char blue, char green);
            RGBAPixel(char red, char blue, char green, double opacity);
            
    };
}

#endif