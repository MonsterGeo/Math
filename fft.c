#include <stdio.h>
#include <stdlib.h>
#include <complex.h>
#include <math.h>
#include <string.h>

#define PI 3.1415926


void fft(complex double *x, int n) {
    if (n <= 1) return;
    
    complex double *even = (complex double *)malloc(n / 2 * sizeof(complex double));
    complex double *odd = (complex double *)malloc(n / 2 * sizeof(complex double));
    
    int j = 0;
    for (int i = 0; i < n; i += 2) {
        even[j] = x[i];
        odd[j] = x[i + 1];
        j++;
    }
    
    fft(even, n / 2);
    fft(odd, n / 2);
    
    for (int k = 0; k < n / 2; k++) {
        complex double t = cexp(2 * PI * I * k / n) * odd[k];
        x[k] = even[k] + t;
        x[k + n / 2] = even[k] - t;
    }
    
    free(even);
    free(odd);
}


char* complex_to_string(complex double z) {
    static char buffer[100];
    double real = creal(z);
    double imag = cimag(z);
    
    if (imag == 0) {
        sprintf(buffer, "%.4f", real);
    } else if (real == 0) {
        if (imag == 1) {
            strcpy(buffer, "i");
        } else if (imag == -1) {
            strcpy(buffer, "-i");
        } else {
            sprintf(buffer, "%.4fi", imag);
        }
    } else {
        if (imag > 0) {
            if (imag == 1) {
                sprintf(buffer, "%.4f + i", real);
            } else {
                sprintf(buffer, "%.4f + %.4fi", real, imag);
            }
        } else {
            if (imag == -1) {
                sprintf(buffer, "%.4f - i", real);
            } else {
                sprintf(buffer, "%.4f - %.4fi", real, fabs(imag));
            }
        }
    }
    
    return buffer;
}

void print_frequency_domain(complex double *data, int n) {
    printf("result:\n");
    for (int i = 0; i < n; i++) {
        printf("X[%d] = %s\n", i, complex_to_string(data[i]));
    }
}


void print_polynomial(complex double *coeffs, int degree) {
    printf("poly: ");
    for (int i = 0; i <= degree; i++) {
        if (i > 0) printf(" + ");
        printf("%s x^%d", complex_to_string(coeffs[i]), i);
    }
    printf("\n");
}

int main() {
    //  A(x) = 1 + 2x + 3x^2
    // B(x) = 4 + 5x
    double A_coeffs[] = {1.0, 2.0, 3.0};
    double B_coeffs[] = {4.0, 5.0};
    int A_degree = sizeof(A_coeffs) / sizeof(A_coeffs[0]) - 1;
    int B_degree = sizeof(B_coeffs) / sizeof(B_coeffs[0]) - 1;
    
    
    int product_degree = A_degree + B_degree;
    int n = 1;
    while (n <= product_degree) n *= 2; //find nth root of numbers

    complex double *A = (complex double *)calloc(n, sizeof(complex double));
    complex double *B = (complex double *)calloc(n, sizeof(complex double));
    
    for (int i = 0; i <= A_degree; i++) A[i] = A_coeffs[i];
    for (int i = 0; i <= B_degree; i++) B[i] = B_coeffs[i];
    
    printf("A: ");
    print_polynomial(A, A_degree);
    printf("B: ");
    print_polynomial(B, B_degree);
    
    complex double *A_fft = (complex double *)malloc(n * sizeof(complex double));
    complex double *B_fft = (complex double *)malloc(n * sizeof(complex double));
    
    for (int i = 0; i < n; i++) {
        A_fft[i] = A[i];
        B_fft[i] = B[i];
    }
    
    fft(A_fft, n);
    fft(B_fft, n);
    
    printf("\nA_FFT:\n");
    print_frequency_domain(A_fft, n);
    
    printf("\nB_FFT:\n");
    print_frequency_domain(B_fft, n);
    
    complex double *C_fft = (complex double *)malloc(n * sizeof(complex double));
    for (int i = 0; i < n; i++) {
        C_fft[i] = A_fft[i] * B_fft[i];
    }
    
    printf("\nA*B_FFT:\n");
    print_frequency_domain(C_fft, n);
    
    free(A);
    free(B);
    free(A_fft);
    free(B_fft);
    free(C_fft);
    
    return 0;
}    