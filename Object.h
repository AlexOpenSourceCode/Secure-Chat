#pragma once



#ifndef OBJECT_H
#define OBJECT_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <math.h>

#include "V2ctor.h"
#include "OBB2D.h"
#include <Eigen/Dense>
#include <chrono>

using namespace std::chrono;


using namespace std;
using namespace Eigen;


namespace rmp {

	class Object{
		float M_PI = 3.14159265359;
	protected:
		

		



	public:
		//float x;
		//float y;
		//float z;
		float dx = 0;
		float dy = 0;
		float dz = 0;


		


		Eigen::Vector3f direction_v;
		float xrot;
		float width;
		float height;
		Eigen::Vector3f center;
		V2ctor center2d;
		OBB2D obb;


		float speed = 0.5;



		Object(float _x = 0.0, float _y = 0.0, float _z = 0.0, float _width = 1, float _height = 1);
		

		Object(Vector3f _center, float _width = 1, float _height = 1);
		


		float get_x();

		float get_y();

		float get_z();


		OBB2D get_obb();



		float get_dx();

		float get_dy();

		float get_dz();


		void set_x(float _x);
		void set_y(float _y);
		void set_z(float _z);


		void set_dx(float _dx);

		void set_dy(float _dy);

		void set_dz(float _dz);




		void change_x(float diff_x);

		void change_y(float diff_y);

		void change_z(float diff_z);



		void change_dx(float direction_difference_x);




		void change_dy(float direction_difference_y);

		void change_dz(float direction_difference_z);





		float get_width();

		float get_height();

		//dx = direction x
		void draw(float _x, float _y, float _z, float _dx, float _dy, float _dz);

		void draw(float _x, float _y, float _z);

		void draw();



		string print();


		void move(milliseconds time_delta);



		//Virtual destructors are useful when you can delete an instance of a derived class through a pointer to base class:

		//class Base 
		//{
		//	// some virtual methods
		//};

		//class Derived : public Base
		//{
		//	~Derived()
		//	{
		//		// Do some important cleanup
		//	}
		//}
		//Here, you'll notice that I didn't declare Base's destructor to be virtual. Now, let's have a look at the following snippet:

		//Base *b = new Derived();
		//// use b
		//delete b; // Here's the problem!
		//Since Base's destructor is not virtual and b is a Base* pointing to a Derived object, delete b has undefined behaviour. In most implementations, the call to the destructor will be resolved like any non-virtual code, meaning that the destructor of the base class will be called but not the one of the derived class, resulting in resources leak.

		//To sum up, always make base classes' destructors virtual when they're meant to be manipulated polymorphically.

		//If you want to prevent the deletion of an instance through a base class pointer, you can make the base class destuctor protected and nonvirtual; by doing so, the compiler won't let you call delete on a base class pointer.

		//You can learn more about virtuality and virtual base class destructor in this article from Herb Sutter.

		virtual ~Object();




	private:

		//draw objects
		void draw_function(float _x, float _y, float _z, float _dx, float _dy, float _dz);
	};

}


#endif



