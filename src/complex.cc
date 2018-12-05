//
// Created by brian on 11/20/18.
// Updated by John on 12/4/18.
//

#include "complex.h"

#include <cmath>

const float PI = 3.14159265358979f;

Complex::Complex() : real(0.0f), imag(0.0f) {}

Complex::Complex(float r) : real(r), imag(0.0f) {}

Complex::Complex(float r, float i) : real(r), imag(i) {}

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
	// Type should be float
	// sqrt(imag^2 + real ^2) = mag
    return sqrt(pow(imag, 2) + pow(real, 2));
}

float Complex::angle() const {
	// arctan(imag / real) = theta
    return atan(imag / real);
    // TODO check for real = 0; return pi/2 or 3pi/2
    // TODO range should be 0 to 2pi
    // TODO check if both negative; add + pi if so
    // TODO check if should return degrees, currently returns in radians
}

Complex Complex::conj() const {
    return Complex(real, -1 * imag);
}

std::ostream& operator<< (std::ostream& os, const Complex& rhs) {
    Complex c(rhs);
    if(fabsf(rhs.imag) < 1e-10) c.imag = 0.0f;
    if(fabsf(rhs.real) < 1e-10) c.real = 0.0f;

    if(c.imag == 0) {
        os << c.real;
    }
    else {
        os << "(" << c.real << "," << c.imag << ")";
    }
    return os;
}
