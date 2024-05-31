// #include <stdint.h>

// // Function to calculate factorial
// size_t factorial(int n) {
//     size_t result = 1;
//     for (int i = 2; i <= n; ++i) {
//         result *= i;
//     }
//     return result;
// }

// // Function to calculate power
// double power(double base, int exp) {
//     double result = 1.0;
//     for (int i = 0; i < exp; ++i) {
//         result *= base;
//     }
//     return result;
// }

// // Function to calculate cosine using Taylor series
// double my_cos(double x) {
//     const int terms = 10; // Number of terms in the series
//     double sum = 0.0;

//     for (int n = 0; n < terms; ++n) {
//         double term = power(-1, n) * power(x, 2 * n) / factorial(2 * n);
//         sum += term;
//     }

//     return sum;
// }

// // Function to calculate sine using Taylor series
// double my_sin(double x) {
//     const int terms = 10; // Number of terms in the series
//     double sum = 0.0;

//     for (int n = 0; n < terms; ++n) {
//         double term = power(-1, n) * power(x, 2 * n + 1) / factorial(2 * n + 1);
//         sum += term;
//     }

//     return sum;
// }