#pragma once
#include <cmath>
#include <iostream>
class Complex
{
public:
	double re, im;

	Complex(double _re = 0, double _im = 0) :re(_re), im(_im)
	{
		
	}

	double abs()
	{
		return sqrt(re*re + im*im);
	}

	Complex operator+(const Complex &c)
	{
		return Complex(re + c.re, im + c.im);
	}

	Complex operator*(const Complex &c)
	{
		return Complex(re*c.re - im*c.im, re*c.im + im*c.re);
	}

	Complex operator*(double a)
	{
		return Complex(re*a, im*a);
	}
	
};