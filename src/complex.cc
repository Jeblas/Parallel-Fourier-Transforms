#include "complex.h"
#include <cmath>

const float PI = 3.14159265358979f;

Complex::Complex() : real(0.0f), imag(0.0f) {}

Complex::Complex(float r) : real(r), imag(0.0f) {}

Complex::Complex(float r, float i) : real(r), imag(i) {}

Complex::Complex(const Complex & rhs) : real(rhs.real), imag(rhs.imag) {}

Complex Complex::operator+(const Complex &b) const {
    return Complex(real + b.real, imag + b.imag);
}

Complex Complex::operator-(const Complex &b) const {
    return Complex(real - b.real, imag - b.imag);
}

Complex Complex::operator*(const Complex &b) const {
    float real_comp = (real * b.real) - (imag * b.imag);
    float imag_comp = (real * b.imag) + (imag * b.real);
    return Complex(real_comp, imag_comp);
}

float Complex::mag() const {
    return sqrt(pow(imag, 2) + pow(real, 2));
}

float Complex::angle() const {
    return atan2(imag, real);
}

Complex Complex::conj() const {
    return Complex(real, -1 * imag);
}

std::ostream& operator<< (std::ostream& os, const Complex& rhs) {
    Complex c = rhs;
    if(fabsf(rhs.imag) < 1e-10) c.imag = 0.0f;
    if(fabsf(rhs.real) < 1e-10) c.real = 0.0f;

/* TODO Will this affect the output file
    if(rhs.imag < 1e-10) {
        os << c.real;
    }
*/

        os << "(" << c.real << "," << c.imag << ")";

    return os;
}
