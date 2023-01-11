/**
 * @file fib.cpp
 * Contains Fibonacci-number generating functions both utilizing and not
 * utilizing memoization.
 *
 * @author Matt Joras
 * @date Winter 2013
 */

#include "fib.h"
#include <map>

using std::map;

/**
 * Calculates the nth Fibonacci number where the zeroth is defined to be
 * 0.
 * @param n Which number to generate.
 * @return The nth Fibonacci number.
 */
unsigned long fib(unsigned long n)
{
    /* Your code goes here! */

    // Stub value - remove when you are done
    return n<=1? n: fib(n-1)+fib(n-2);
}

/**
 * Calculates the nth Fibonacci number where the zeroth is defined to be
 * 0. This version utilizes memoization.
 * @param n Which number to generate.
 * @return The nth Fibonacci number.
 */
unsigned long memoized_fib(unsigned long n)
{
    /* Your code goes here! */

    // Stub value - remove when you are done
    static map<unsigned long, unsigned long> memo = {{0,0}, {1,1}};
    auto lookup = memo.find(n);

    if(lookup != memo.end())
        return lookup->second;

    unsigned long res = memoized_fib(n-1) + memoized_fib(n-2);
    memo[n] = res;
    return res;
}
