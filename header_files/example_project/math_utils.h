#ifndef MATH_UTILS_H
#define MATH_UTILS_H

#include <stdbool.h>

// Mathematical constants
#define PI 3.14159265358979323846
#define E  2.71828182845904523536
#define GOLDEN_RATIO 1.61803398874989484820

// Utility macros
#define SQUARE(x) ((x) * (x))
#define CUBE(x) ((x) * (x) * (x))
#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x) ((x) < 0 ? -(x) : (x))
#define SIGN(x) ((x) > 0 ? 1 : ((x) < 0 ? -1 : 0))

// Angle conversion
#define DEG_TO_RAD(deg) ((deg) * PI / 180.0)
#define RAD_TO_DEG(rad) ((rad) * 180.0 / PI)

// Type definitions
typedef struct {
    double x;
    double y;
} Point2D;

typedef struct {
    double x;
    double y;
    double z;
} Point3D;

typedef struct {
    double real;
    double imag;
} Complex;

// Basic math operations
int gcd(int a, int b);
int lcm(int a, int b);
long factorial(int n);
bool is_prime(int n);
double power(double base, int exponent);

// Geometry functions
double distance_2d(Point2D p1, Point2D p2);
double distance_3d(Point3D p1, Point3D p2);
double circle_area(double radius);
double circle_circumference(double radius);
double triangle_area(double base, double height);

// Complex number operations
Complex complex_add(Complex a, Complex b);
Complex complex_subtract(Complex a, Complex b);
Complex complex_multiply(Complex a, Complex b);
Complex complex_divide(Complex a, Complex b);
double complex_magnitude(Complex c);
double complex_phase(Complex c);

// Statistics functions
double mean(const double *data, int size);
double median(double *data, int size);
double variance(const double *data, int size);
double std_deviation(const double *data, int size);

// Inline utility functions
static inline int int_min(int a, int b) {
    return (a < b) ? a : b;
}

static inline int int_max(int a, int b) {
    return (a > b) ? a : b;
}

static inline double clamp(double value, double min, double max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

static inline bool approximately_equal(double a, double b, double epsilon) {
    return ABS(a - b) < epsilon;
}

#endif // MATH_UTILS_H