#pragma once



#ifndef V2CTOR_H
#define V2CTOR_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>




namespace rmp {

	class V2ctor {
	public:
		float x, y;
		//members
		
		//ctor
		//V2ctor(float xVal = 0.0, float yVal = 0.0);

		//methods
		//float LengthSq() const;
		//float Length() const;
		//const V2ctor & Skew();
		//const V2ctor & Normalize();

		//assingment operators
		V2ctor & V2ctor::operator = (const V2ctor & v);
		V2ctor & V2ctor::operator = (const float & s);
		V2ctor & V2ctor::operator - (void);

		//equality operators
		bool V2ctor::operator == (const V2ctor & v) const;
		bool V2ctor::operator != (const V2ctor & v) const;

		//comparison operators
		bool V2ctor::operator < (const V2ctor & v) const;
		bool V2ctor::operator >(const V2ctor & v) const;

		//V2ctor to this operators
		V2ctor & V2ctor::operator += (const V2ctor & v);
		V2ctor & V2ctor::operator -= (const V2ctor & v);
		V2ctor & V2ctor::operator *= (const V2ctor & v);
		V2ctor & V2ctor::operator /= (const V2ctor & v);

		//V2ctor to V2ctor operators
		const V2ctor V2ctor::operator + (const V2ctor & v) const;
		const V2ctor V2ctor::operator - (const V2ctor & v) const;
		const V2ctor V2ctor::operator * (const V2ctor & v) const;
		const V2ctor V2ctor::operator / (const V2ctor & v) const;

		//scaler to this operators
		V2ctor & V2ctor::operator += (float s);
		V2ctor & V2ctor::operator -= (float s);
		V2ctor & V2ctor::operator *= (float s);
		V2ctor & V2ctor::operator /= (float s);

		//scaler to V2ctor operators
		const V2ctor V2ctor::operator + (float s) const;
		const V2ctor V2ctor::operator - (float s) const;
		const V2ctor V2ctor::operator * (float s) const;
		const V2ctor V2ctor::operator / (float s) const;


		////static arithmetic
		//static const V2ctor Add(const V2ctor & v1, const V2ctor & v2);
		//static const V2ctor Sub(const V2ctor & v1, const V2ctor & v2);
		//static const V2ctor Mul(const V2ctor & v1, const V2ctor & v2);
		//static const V2ctor Div(const V2ctor & v1, const V2ctor & v2);

		////static methods
		//static float DistanceSq(const V2ctor & v1, const V2ctor & v2);

		//static float Distance(const V2ctor & v1, const V2ctor & v2);

		//static float Dot(const V2ctor & v1, const V2ctor & v2);

		//static const V2ctor Min(const V2ctor & v1, const V2ctor & v2);

		//static const V2ctor Max(const V2ctor & v1, const V2ctor & v2);

		//static const V2ctor Clamp(const V2ctor & v, const V2ctor & min, const V2ctor & max);


		////static arithmetic
		static const V2ctor Add(const V2ctor & v1, const V2ctor & v2) { return v1 + v2; }
		static const V2ctor Sub(const V2ctor & v1, const V2ctor & v2) { return v1 - v2; }
		static const V2ctor Mul(const V2ctor & v1, const V2ctor & v2) { return v1 * v2; }
		static const V2ctor Div(const V2ctor & v1, const V2ctor & v2) { return v1 / v2; }

		//static methods
		static float DistanceSq(const V2ctor & v1, const V2ctor & v2)
		{
			return ((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y));
		}

		static float Distance(const V2ctor & v1, const V2ctor & v2)
		{
			return sqrt(DistanceSq(v1, v2));
		}

		static float Dot(const V2ctor & v1, const V2ctor & v2)
		{
			return ((v1.x * v2.x) + (v1.y * v2.y));
		}

		static const V2ctor Min(const V2ctor & v1, const V2ctor & v2)
		{
			return V2ctor(v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y);
		}

		static const V2ctor Max(const V2ctor & v1, const V2ctor & v2)
		{
			return V2ctor(v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y);
		}

		static const V2ctor Clamp(const V2ctor & v, const V2ctor & min, const V2ctor & max)
		{
			return Min(Max(v, min), max); /*because lifes too short*/
		}






		//friend operators
		friend std::ostream & V2ctor::operator << (std::ostream & os, const V2ctor & v);



		//V2ctor ctor
		V2ctor::V2ctor();
		V2ctor::V2ctor(float xVal, float yVal);

		//V2ctor methods
		float LengthSq() const;

		float Length() const;

		V2ctor Skew();

		V2ctor Normalize();
		//test V2ctor
		void TestV2ctor();



	};

}


#endif



