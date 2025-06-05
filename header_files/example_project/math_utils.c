#include "math_utils.h"
#include <math.h>
#include <stdlib.h>

// Basic math operations
int gcd(int a, int b) {
    a = ABS(a);
    b = ABS(b);
    while (b != 0) {
        int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}

int lcm(int a, int b) {
    if (a == 0 || b == 0) return 0;
    return ABS(a * b) / gcd(a, b);
}

long factorial(int n) {
    if (n < 0) return -1;  // Error
    if (n <= 1) return 1;
    long result = 1;
    for (int i = 2; i <= n; i++) {
        result *= i;
    }
    return result;
}

bool is_prime(int n) {
    if (n <= 1) return false;
    if (n <= 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;
    
    for (int i = 5; i * i <= n; i += 6) {
        if (n % i == 0 || n % (i + 2) == 0) {
            return false;
        }
    }
    return true;
}

double power(double base, int exponent) {
    if (exponent == 0) return 1.0;
    
    double result = 1.0;
    int abs_exp = ABS(exponent);
    
    for (int i = 0; i < abs_exp; i++) {
        result *= base;
    }
    
    return (exponent < 0) ? 1.0 / result : result;
}

// Geometry functions
double distance_2d(Point2D p1, Point2D p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    return sqrt(SQUARE(dx) + SQUARE(dy));
}

double distance_3d(Point3D p1, Point3D p2) {
    double dx = p2.x - p1.x;
    double dy = p2.y - p1.y;
    double dz = p2.z - p1.z;
    return sqrt(SQUARE(dx) + SQUARE(dy) + SQUARE(dz));
}

double circle_area(double radius) {
    return PI * SQUARE(radius);
}

double circle_circumference(double radius) {
    return 2 * PI * radius;
}

double triangle_area(double base, double height) {
    return 0.5 * base * height;
}

// Complex number operations
Complex complex_add(Complex a, Complex b) {
    Complex result = {
        .real = a.real + b.real,
        .imag = a.imag + b.imag
    };
    return result;
}

Complex complex_subtract(Complex a, Complex b) {
    Complex result = {
        .real = a.real - b.real,
        .imag = a.imag - b.imag
    };
    return result;
}

Complex complex_multiply(Complex a, Complex b) {
    Complex result = {
        .real = a.real * b.real - a.imag * b.imag,
        .imag = a.real * b.imag + a.imag * b.real
    };
    return result;
}

Complex complex_divide(Complex a, Complex b) {
    double denominator = SQUARE(b.real) + SQUARE(b.imag);
    Complex result = {
        .real = (a.real * b.real + a.imag * b.imag) / denominator,
        .imag = (a.imag * b.real - a.real * b.imag) / denominator
    };
    return result;
}

double complex_magnitude(Complex c) {
    return sqrt(SQUARE(c.real) + SQUARE(c.imag));
}

double complex_phase(Complex c) {
    return atan2(c.imag, c.real);
}

// Statistics functions
double mean(const double *data, int size) {
    if (size <= 0) return 0.0;
    
    double sum = 0.0;
    for (int i = 0; i < size; i++) {
        sum += data[i];
    }
    return sum / size;
}

// Helper function for median
int compare_double(const void *a, const void *b) {
    double diff = *(const double*)a - *(const double*)b;
    return (diff > 0) - (diff < 0);
}

double median(double *data, int size) {
    if (size <= 0) return 0.0;
    
    // Create a copy to avoid modifying original data
    double *sorted = (double*)malloc(size * sizeof(double));
    for (int i = 0; i < size; i++) {
        sorted[i] = data[i];
    }
    
    qsort(sorted, size, sizeof(double), compare_double);
    
    double result;
    if (size % 2 == 0) {
        result = (sorted[size/2 - 1] + sorted[size/2]) / 2.0;
    } else {
        result = sorted[size/2];
    }
    
    free(sorted);
    return result;
}

double variance(const double *data, int size) {
    if (size <= 1) return 0.0;
    
    double m = mean(data, size);
    double sum_sq_diff = 0.0;
    
    for (int i = 0; i < size; i++) {
        sum_sq_diff += SQUARE(data[i] - m);
    }
    
    return sum_sq_diff / (size - 1);
}

double std_deviation(const double *data, int size) {
    return sqrt(variance(data, size));
}