#include <GL/glew.h>;
#include <GL/freeglut.h>;
#include <iostream>;
#include <stdio.h>;
#include <stdlib.h>;

#include <vector>;
#include <string>

#include "V2ctor.h";


#include <Eigen/Dense>


namespace rmp {

	//ctor
	//V2ctor(float xVal = 0.0, float yVal = 0.0);

	//methods
	//float LengthSq() const;
	//float Length() const;
	//const V2ctor & Skew();
	//const V2ctor & Normalize();

	//assingment operators
	V2ctor & V2ctor::operator = (const V2ctor & v) { x = v.x; y = v.y; return *this; }
	V2ctor & V2ctor::operator = (const float & s) { x = s; y = s; return *this; }
	V2ctor & V2ctor::operator - (void) { x = -x; y = -y; return *this; }

	//equality operators
	bool V2ctor::operator == (const V2ctor & v) const { return (x == v.x) && (y == v.y); }
	bool V2ctor::operator != (const V2ctor & v) const { return !(*this == v); }

	//comparison operators
	bool V2ctor::operator < (const V2ctor & v) const { return (x < v.x) || ((x == v.x) && (y < v.y)); }
	bool V2ctor::operator >(const V2ctor & v) const { return !(*this < v) && !(*this == v); }

	//V2ctor to this operators
	V2ctor & V2ctor::operator += (const V2ctor & v) { x += v.x; y += v.y; return *this; }
	V2ctor & V2ctor::operator -= (const V2ctor & v) { x -= v.x; y -= v.y; return *this; }
	V2ctor & V2ctor::operator *= (const V2ctor & v) { x *= v.x; y *= v.y; return *this; }
	V2ctor & V2ctor::operator /= (const V2ctor & v) { x /= v.x; y /= v.y; return *this; }

	//V2ctor to V2ctor operators
	const V2ctor V2ctor::operator + (const V2ctor & v) const { V2ctor r(*this); return r += v; }
	const V2ctor V2ctor::operator - (const V2ctor & v) const { V2ctor r(*this); return r -= v; }
	const V2ctor V2ctor::operator * (const V2ctor & v) const { V2ctor r(*this); return r *= v; }
	const V2ctor V2ctor::operator / (const V2ctor & v) const { V2ctor r(*this); return r /= v; }

	//scaler to this operators
	V2ctor & V2ctor::operator += (float s) { x += s; y += s; return *this; }
	V2ctor & V2ctor::operator -= (float s) { x -= s; y -= s; return *this; }
	V2ctor & V2ctor::operator *= (float s) { x *= s; y *= s; return *this; }
	V2ctor & V2ctor::operator /= (float s) { x /= s; y /= s; return *this; }

	//scaler to V2ctor operators
	const V2ctor V2ctor::operator + (float s) const { V2ctor r(*this); return r += s; }
	const V2ctor V2ctor::operator - (float s) const { V2ctor r(*this); return r -= s; }
	const V2ctor V2ctor::operator * (float s) const { V2ctor r(*this); return r *= s; }
	const V2ctor V2ctor::operator / (float s) const { V2ctor r(*this); return r /= s; }


	////static arithmetic
	//const V2ctor V2ctor::Add(const V2ctor & v1, const V2ctor & v2) { return v1 + v2; }
	//const V2ctor V2ctor::Sub(const V2ctor & v1, const V2ctor & v2) { return v1 - v2; }
	//const V2ctor V2ctor::Mul(const V2ctor & v1, const V2ctor & v2) { return v1 * v2; }
	//const V2ctor V2ctor::Div(const V2ctor & v1, const V2ctor & v2) { return v1 / v2; }

	////static methods
	//float V2ctor::DistanceSq(const V2ctor & v1, const V2ctor & v2)
	//{
	//	return ((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y));
	//}

	//float V2ctor::Distance(const V2ctor & v1, const V2ctor & v2)
	//{
	//	return sqrt(DistanceSq(v1, v2));
	//}

	//float V2ctor::Dot(const V2ctor & v1, const V2ctor & v2)
	//{
	//	return ((v1.x * v2.x) + (v1.y * v2.y));
	//}

	//const V2ctor V2ctor::Min(const V2ctor & v1, const V2ctor & v2)
	//{
	//	return V2ctor(v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y);
	//}

	//const V2ctor V2ctor::Max(const V2ctor & v1, const V2ctor & v2)
	//{
	//	return V2ctor(v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y);
	//}

	//const V2ctor V2ctor::Clamp(const V2ctor & v, const V2ctor & min, const V2ctor & max)
	//{
	//	return Min(Max(v, min), max); /*because lifes too short*/
	//}

	//friend operators
	std::ostream & operator << (std::ostream & os, const V2ctor & v) { return os << "{" << v.x << ", " << v.y << "}"; }




	//V2ctor ctor
	V2ctor::V2ctor() : x(0.0), y(0.0) { }
	V2ctor::V2ctor(float xVal, float yVal) : x(xVal), y(yVal) { }

	//V2ctor methods
	float V2ctor::LengthSq() const { return x * x + y * y; }

	float V2ctor::Length() const { return sqrt(LengthSq()); }

	V2ctor V2ctor::Skew() { x -= y; y += x; x -= y; return *this; }

	V2ctor V2ctor::Normalize()
	{
		float len = Length();
		if (len < FLT_EPSILON)
		{
			x = y = 0;
			return *this;
		}
		else
		{
			float invLen = 1.0f / len;
			x *= invLen;
			y *= invLen;
			return *this;
		}
	}

	//test V2ctor
	void V2ctor::TestV2ctor()
	{
		
	}




}











