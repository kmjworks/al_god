#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "math_utils.h"
#include "string_utils.h"

// Demonstrate math utilities
void demo_math_utils() {
    printf("\n=== Math Utils Demo ===\n");
    
    // Basic operations
    printf("GCD(48, 18) = %d\n", gcd(48, 18));
    printf("LCM(12, 15) = %d\n", lcm(12, 15));
    printf("5! = %ld\n", factorial(5));
    printf("Is 17 prime? %s\n", is_prime(17) ? "Yes" : "No");
    printf("2^10 = %.0f\n", power(2, 10));
    
    // Geometry
    Point2D p1 = {0, 0}, p2 = {3, 4};
    printf("\nDistance between (0,0) and (3,4) = %.2f\n", distance_2d(p1, p2));
    printf("Area of circle with radius 5 = %.2f\n", circle_area(5));
    
    // Complex numbers
    Complex c1 = {3, 4}, c2 = {1, 2};
    Complex sum = complex_add(c1, c2);
    printf("\n(3+4i) + (1+2i) = %.0f+%.0fi\n", sum.real, sum.imag);
    printf("Magnitude of 3+4i = %.2f\n", complex_magnitude(c1));
    
    // Statistics
    double data[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    printf("\nMean of [1..10] = %.2f\n", mean(data, 10));
    printf("Std deviation = %.2f\n", std_deviation(data, 10));
}

// Demonstrate string utilities (implementation would be in string_utils.c)
void demo_string_utils() {
    printf("\n=== String Utils Demo ===\n");
    
    // These would work if string_utils.c was implemented
    printf("is_digit('5') = %s\n", is_digit('5') ? "true" : "false");
    printf("is_alpha('A') = %s\n", is_alpha('A') ? "true" : "false");
    printf("is_whitespace(' ') = %s\n", is_whitespace(' ') ? "true" : "false");
    
    const char *test = "Hello, World!";
    printf("\nString: \"%s\"\n", test);
    printf("Length (safe): %zu\n", SAFE_STRLEN(test));
    printf("Is empty: %s\n", STR_EMPTY(test) ? "Yes" : "No");
}

// Demonstrate header features
void demo_header_features() {
    printf("\n=== Header Features Demo ===\n");
    
    // Using macros
    int a = 10, b = 20;
    printf("MIN(%d, %d) = %d\n", a, b, MIN(a, b));
    printf("MAX(%d, %d) = %d\n", a, b, MAX(a, b));
    printf("SQUARE(%d) = %d\n", a, SQUARE(a));
    printf("ABS(-15) = %d\n", ABS(-15));
    
    // Angle conversion
    double angle_deg = 45.0;
    double angle_rad = DEG_TO_RAD(angle_deg);
    printf("\n%g degrees = %g radians\n", angle_deg, angle_rad);
    printf("%g radians = %g degrees\n", angle_rad, RAD_TO_DEG(angle_rad));
    
    // Using inline functions
    printf("\nint_min(15, 7) = %d\n", int_min(15, 7));
    printf("clamp(150, 0, 100) = %.0f\n", clamp(150, 0, 100));
    printf("approximately_equal(3.14159, PI, 0.001) = %s\n", 
           approximately_equal(3.14159, PI, 0.001) ? "true" : "false");
}

int main() {
    printf("HEADER FILES EXAMPLE PROJECT\n");
    printf("============================\n");
    
    demo_math_utils();
    demo_string_utils();
    demo_header_features();
    
    return 0;
}