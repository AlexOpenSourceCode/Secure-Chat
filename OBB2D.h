#pragma once



#ifndef OBB2D_H
#define OBB2D_H


#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>


#include "V2ctor.h"
#include <Eigen/Dense>


using namespace std;


namespace rmp {

	class OBB2D {

		float M_PI = 3.14159265359;


	private:
		/** Corners of the box, where 0 is the lower left. */
		V2ctor       corner[4];

		/** Two edges of the box extended away from corner[0]. */
		V2ctor        axis[2];

		/** origin[a] = corner[0].dot(axis[a]); */
		double          origin[2];

		V2ctor center;

		/** Returns true if other overlaps one dimension of this. */
		bool overlaps1Way(const OBB2D& other) const;


		/** Updates the axes after the corners move.  Assumes the
		corners actually form a rectangle. */
		void computeAxes();

	public:

		OBB2D(V2ctor& _center, const double w, const double h, double degree_angle);




		void update(V2ctor& _center, const double w, const double h, double degree_angle);


		/** For testing purposes. */
		void moveTo(const V2ctor& center);

		/** Returns true if the intersection of the boxes is non-empty. */
		bool overlaps(const OBB2D& other) const;




		V2ctor* get_corner();

		V2ctor* get_center();
	};

}


#endif



