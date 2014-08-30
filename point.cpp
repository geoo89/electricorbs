#include "point.h"

#include <math.h>

void point::reset() {
    x = 0;
    y = 0;
}

double point::length() {
    return sqrt(x*x + y*y);
}


point operator-(point a) {
    point c;
    c.x = -a.x;
    c.y = -a.y;
    return c;
}

point operator+(point a, point b) {
    point c;
    c.x = a.x + b.x;
    c.y = a.y + b.y;
    return c;
}

point operator-(point a, point b) {
    return a + (-b);
}

point& operator+=(point &a, point b) {
    a.x += b.x;
    a.y += b.y;
    return a;
}

point& operator-=(point &a, point b) {
    a += -b;
    return a;
}

point operator*(double scalar, point p) {
    point c;
    c.x = scalar * p.x;
    c.y = scalar * p.y;
    return c;
}

double dist(point a, point b) {
    return (b-a).length();
}
