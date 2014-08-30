#ifndef POINT_H_INCLUDED
#define POINT_H_INCLUDED

/* A 2-dimensional real vector.
 */
struct point {
    double x;
    double y;

    /* Set both coordinates to 0
     */
    void reset();

    /* Return length of the vector
     */
    double length();
};


/* Standard 2D vector operations
 */
point operator-(point a);
point operator+(point a, point b);
point operator-(point a, point b);
point& operator+=(point &a, point b);
point& operator-=(point &a, point b);
point operator*(double scalar, point p);

/* Distance between two points
 */
double dist(point a, point b);


#endif // POINT_H_INCLUDED
