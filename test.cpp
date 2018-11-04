#include <GL/glew.h>;
#include <GL/freeglut.h>;
#include <iostream>;


#include <iostream>;
#include <stdio.h>;
#include <stdlib.h>;
#include <fstream>;
#include <vector>;
#include <string>



//bool collides(const Box &box1, const Box &box2)
//{
//	assert(box1.numberOfDimensions() == box2.numberOfDimensions());
//	for (size_t i = 0; i != box1.numberOfDimensions(); ++i) {
//		float a = max(box1.minForDimension(i), box2.minForDimension(i));
//		float b = min(box1.maxForDimension(i), box2.maxForDimension(i));
//		if (a>b) return false;
//	}
//	return true;
//}

//
//
//bool collidez(){
//
//}
//
//
//
//class Point {
//	public:
//		float x;
//		float y;
//
//		Point(float _x, float _y):
//			x{_x},
//			y{_y}
//		{}
//
//		float get_x(){
//			return x;
//		}
//
//		float get_y(){
//			return y;
//		}
//};
//
//
//class Box {
//	public:
//		Point center;
//		
//
//
//};
//





//private function refresh() :void {
//	var dot10 : Point = box1.getDot(0);
//	var dot11 : Point = box1.getDot(1);
//
//	var dot20 : Point = box2.getDot(0);
//	var dot24 : Point = box2.getDot(4);
//
//	//Actual calculations
//	var axis : Vector2d = new Vector2d(1, -1).unitVector;
//	var C : 
//	Vector2d = new Vector2d(
//		dot20.x - dot10.x,
//		dot20.y - dot10.y
//	)
//	var A : Vector2d = new Vector2d(
//		dot11.x - dot10.x,
//		dot11.y - dot10.y
//	)
//	var B : Vector2d = new Vector2d(
//		dot24.x - dot20.x,
//		dot24.y - dot20.y
//	)
//	var projC : Number = C.dotProduct(axis)
//	var projA : Number = A.dotProduct(axis);
//	var projB : Number = B.dotProduct(axis);
//
//	var gap : Number = projC - projA + projB;	//projB is expected to be a negative value
//	if (gap > 0) t.text = "There's a gap between both boxes"
//	else if (gap > 0) t.text = "Boxes are touching each other"
//	else t.text = "Penetration had happened."
//
//	//drawing the line
//	var stageCenter : Point = new Point(stage.stageWidth >> 1, stage.stageHeight >> 1);
//
//	var toBox1 : Vector2d = new Vector2d(
//		dot10.x - stageCenter.x,
//		dot10.y - stageCenter.y
//		)
//		var toBox2 : Vector2d = new Vector2d(
//		dot20.x - stageCenter.x,
//		dot20.y - stageCenter.y
//		)
//
//		var offset1 : Number = toBox1.dotProduct(axis);
//	var beginPoint : Point = new Point(
//		stageCenter.x + offset1 * axis.x,
//		stageCenter.y + offset1 * axis.y
//		)
//		var offset2 : Number = toBox2.dotProduct(axis);
//	var endPoint : Point = new Point(
//		stageCenter.x + offset2 * axis.x,
//		stageCenter.y + offset2 * axis.y
//		)
//		graphics.clear();
//	graphics.lineStyle(1);
//	graphics.drawPath(new <int>[1, 2],
//		new <Number>[
//			beginPoint.x, beginPoint.y,
//				endPoint.x, endPoint.y,
//		]
//		)
//		graphics.lineStyle(3, 0xff0000);
//			graphics.drawPath(new <int>[1, 2, 1, 2],
//				new <Number>[
//					beginPoint.x, beginPoint.y,
//						beginPoint.x + projA * axis.x, beginPoint.y + projA * axis.y,
//						endPoint.x, endPoint.y,
//						endPoint.x + projB * axis.x, endPoint.y + projB * axis.y
//				]
//				)
//}
//
//


#include <iostream>

//#include <Eigen/Dense>

//using namespace Eigen;
using namespace std;

//using Eigen::Vector2d;


#include <float.h>

class Vector2
{

public:
	//members
	float x, y;

	//ctor
	//Vector2(float xVal = 0.0, float yVal = 0.0);

	//methods
	//float LengthSq() const;
	//float Length() const;
	//const Vector2 & Skew();
	//const Vector2 & Normalize();

	//assingment operators
	Vector2 & Vector2::operator = (const Vector2 & v) { x = v.x; y = v.y; return *this; }
	Vector2 & Vector2::operator = (const float & s) { x = s; y = s; return *this; }
	Vector2 & Vector2::operator - (void) { x = -x; y = -y; return *this; }

	//equality operators
	bool Vector2::operator == (const Vector2 & v) const { return (x == v.x) && (y == v.y); }
	bool Vector2::operator != (const Vector2 & v) const { return !(*this == v); }

	//comparison operators
	bool Vector2::operator < (const Vector2 & v) const { return (x < v.x) || ((x == v.x) && (y < v.y)); }
	bool Vector2::operator >(const Vector2 & v) const { return !(*this < v) && !(*this == v); }

	//vector2 to this operators
	Vector2 & Vector2::operator += (const Vector2 & v) { x += v.x; y += v.y; return *this; }
	Vector2 & Vector2::operator -= (const Vector2 & v) { x -= v.x; y -= v.y; return *this; }
	Vector2 & Vector2::operator *= (const Vector2 & v) { x *= v.x; y *= v.y; return *this; }
	Vector2 & Vector2::operator /= (const Vector2 & v) { x /= v.x; y /= v.y; return *this; }

	//vector2 to vector2 operators
	const Vector2 Vector2::operator + (const Vector2 & v) const { Vector2 r(*this); return r += v; }
	const Vector2 Vector2::operator - (const Vector2 & v) const { Vector2 r(*this); return r -= v; }
	const Vector2 Vector2::operator * (const Vector2 & v) const { Vector2 r(*this); return r *= v; }
	const Vector2 Vector2::operator / (const Vector2 & v) const { Vector2 r(*this); return r /= v; }

	//scaler to this operators
	Vector2 & Vector2::operator += (float s) { x += s; y += s; return *this; }
	Vector2 & Vector2::operator -= (float s) { x -= s; y -= s; return *this; }
	Vector2 & Vector2::operator *= (float s) { x *= s; y *= s; return *this; }
	Vector2 & Vector2::operator /= (float s) { x /= s; y /= s; return *this; }

	//scaler to vector2 operators
	const Vector2 Vector2::operator + (float s) const { Vector2 r(*this); return r += s; }
	const Vector2 Vector2::operator - (float s) const { Vector2 r(*this); return r -= s; }
	const Vector2 Vector2::operator * (float s) const { Vector2 r(*this); return r *= s; }
	const Vector2 Vector2::operator / (float s) const { Vector2 r(*this); return r /= s; }


	//static arithmetic
	static const Vector2 Add(const Vector2 & v1, const Vector2 & v2) { return v1 + v2; }
	static const Vector2 Sub(const Vector2 & v1, const Vector2 & v2) { return v1 - v2; }
	static const Vector2 Mul(const Vector2 & v1, const Vector2 & v2) { return v1 * v2; }
	static const Vector2 Div(const Vector2 & v1, const Vector2 & v2) { return v1 / v2; }

	//static methods
	static float DistanceSq(const Vector2 & v1, const Vector2 & v2)
	{
		return ((v1.x - v2.x) * (v1.x - v2.x)) + ((v1.y - v2.y) * (v1.y - v2.y));
	}

	static float Distance(const Vector2 & v1, const Vector2 & v2)
	{
		return sqrt(DistanceSq(v1, v2));
	}

	static float Dot(const Vector2 & v1, const Vector2 & v2)
	{
		return ((v1.x * v2.x) + (v1.y * v2.y));
	}

	static const Vector2 Min(const Vector2 & v1, const Vector2 & v2)
	{
		return Vector2(v1.x < v2.x ? v1.x : v2.x, v1.y < v2.y ? v1.y : v2.y);
	}

	static const Vector2 Max(const Vector2 & v1, const Vector2 & v2)
	{
		return Vector2(v1.x > v2.x ? v1.x : v2.x, v1.y > v2.y ? v1.y : v2.y);
	}

	static const Vector2 Clamp(const Vector2 & v, const Vector2 & min, const Vector2 & max)
	{
		return Min(Max(v, min), max); /*because lifes too short*/
	}

	//friend operators
	friend std::ostream & Vector2::operator << (std::ostream & os, const Vector2 & v) { return os << "{" << v.x << ", " << v.y << "}"; }




	//vector2 ctor
	Vector2::Vector2() : x(0.0), y(0.0) { }
	Vector2::Vector2(float xVal, float yVal) : x(xVal), y(yVal) { }

	//vector2 methods
	float LengthSq() const { return x * x + y * y; }

	float Length() const { return sqrt(LengthSq()); }

	Vector2 Skew() { x -= y; y += x; x -= y; return *this; }

	Vector2 Normalize()
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

	//test vector2
	void TestVector2()
	{


	}
}; //end vector2





class OBB2D {
private:
	/** Corners of the box, where 0 is the lower left. */
	Vector2         corner[4];

	/** Two edges of the box extended away from corner[0]. */
	Vector2        axis[2];

	/** origin[a] = corner[0].dot(axis[a]); */
	double          origin[2];

	/** Returns true if other overlaps one dimension of this. */
	bool overlaps1Way(const OBB2D& other) const {
		for (int a = 0; a < 2; ++a) {

			double t = Vector2::Dot(other.corner[0], axis[a]);

			// Find the extent of box 2 on axis a
			double tMin = t;
			double tMax = t;

			for (int c = 1; c < 4; ++c) {

				t = Vector2::Dot(other.corner[c], axis[a]);

				if (t < tMin) {
					tMin = t;
				}
				else if (t > tMax) {
					tMax = t;
				}
			}

			// We have to subtract off the origin

			// See if [tMin, tMax] intersects [0, 1]
			if ((tMin > 1 + origin[a]) || (tMax < origin[a])) {
				// There was no intersection along this dimension;
				// the boxes cannot possibly overlap.
				return false;
			}
		}

		// There was no dimension along which there is no intersection.
		// Therefore the boxes overlap.
		return true;
	}


	/** Updates the axes after the corners move.  Assumes the
	corners actually form a rectangle. */
	void computeAxes() {
		axis[0] = corner[1] - corner[0];
		axis[1] = corner[3] - corner[0];

		// Make the length of each axis 1/edge length so we know any
		// dot product must be less than 1 to fall within the edge.

		for (int a = 0; a < 2; ++a) {
			axis[a] /= axis[a].LengthSq();
			origin[a] = Vector2::Dot(corner[0], axis[a]);
		}
	}

public:

	OBB2D(const Vector2& center, const double w, const double h, double angle)
	{
		Vector2 X(cos(angle), sin(angle));
		Vector2 Y(-sin(angle), cos(angle));

		X *= w / 2;
		Y *= h / 2;

		corner[0] = center - X - Y;
		corner[1] = center + X - Y;
		corner[2] = center + X + Y;
		corner[3] = center - X + Y;

		computeAxes();
	}


	/** For testing purposes. */
	void moveTo(const Vector2& center) {
		Vector2 centroid = (corner[0] + corner[1] + corner[2] + corner[3]) / 4;

		Vector2 translation = center - centroid;

		for (int c = 0; c < 4; ++c) {
			corner[c] += translation;
		}

		computeAxes();
	}

	/** Returns true if the intersection of the boxes is non-empty. */
	bool overlaps(const OBB2D& other) const {
		return overlaps1Way(other) && other.overlaps1Way(*this);
	}

	void render() const {
		glBegin(GL_LINES);
		for (int c = 0; c < 5; ++c) {
			glVertex2fv(&corner[c & 3].x);
		}
		glEnd();
	}
};




#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

using Eigen::Vector3f;

#include <string>

using namespace std;
int main2(){



	//	OBB2D(const Vector2& center, const double w, const double h, double angle)


	Vector3f v3(2.0, 5.0, 8.0);
	Vector3f v4(1.0, 2.0, 3.0);


	//cout << v3[2] << endl;



	Vector2 center1(0, 0);
	Vector2 center2(1, 1);

	OBB2D b1(center1, 2, 2, 45);
	OBB2D b2(center2, 1, 1, 45);




	//cout << b1.overlaps(b2) << endl;
	


	system("pause");
	return 1;
}