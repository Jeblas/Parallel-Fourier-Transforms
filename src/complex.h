//
// Created by brian on 11/20/18.
//


#ifndef COMPLEX_H
#define COMPLEX_H

#include <iostream>
#include <cmath>

class Complex {
public:
    Complex();
    Complex(float r, float i);
    Complex(float r);
    Complex(const Complex & rhs);
    Complex operator+(const Complex& b) const;
    Complex operator-(const Complex& b) const;
    Complex operator*(const Complex& b) const;

    // Complex mag() const;
    // Complex angle() const;
    float mag() const;
    float angle() const;
    Complex conj() const;

    float real;
    float imag;
};

std::ostream& operator<<(std::ostream& os, const Complex& rhs);

#endif
