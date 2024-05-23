#pragma once

class vector2 {

	public:
		double x;
		double y;

		vector2();
		vector2(double _x, double _y);
		vector2 operator+(vector2 b);
		vector2 operator-(vector2 b);
		vector2 operator*(double b);
		vector2 operator/(double b);
		vector2 operator*(vector2 b);
		
		double getMagnitude();
		vector2 normalized();
};