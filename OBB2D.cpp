
#include <GL/glew.h>;
#include <GL/freeglut.h>;
#include <iostream>;


#include <iostream>;
#include <stdio.h>;
#include <stdlib.h>;
#include <fstream>;
#include <vector>;
#include <string>

#include "shader_loader.h";


#include "V2ctor.h";
#include "OBB2D.h";


#include <Eigen/Dense>


namespace rmp {



	/** Returns true if other overlaps one dimension of this. */
	bool OBB2D::overlaps1Way(const OBB2D& other) const {
		for (int a = 0; a < 2; ++a) {

			double t = V2ctor::Dot(other.corner[0], axis[a]);

			// Find the extent of box 2 on axis a
			double tMin = t;
			double tMax = t;

			for (int c = 1; c < 4; ++c) {

				t = V2ctor::Dot(other.corner[c], axis[a]);

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
	void OBB2D::computeAxes() {
		axis[0] = corner[1] - corner[0];
		axis[1] = corner[3] - corner[0];

		// Make the length of each axis 1/edge length so we know any
		// dot product must be less than 1 to fall within the edge.

		for (int a = 0; a < 2; ++a) {
			axis[a] /= axis[a].LengthSq();
			origin[a] = V2ctor::Dot(corner[0], axis[a]);
		}
	}



	OBB2D::OBB2D(V2ctor& _center, const double w, const double h, double degree_angle) {

		double radian_angle = 2 * M_PI * (degree_angle / 360);

		V2ctor X(cos(radian_angle), sin(radian_angle));
		V2ctor Y(-sin(radian_angle), cos(radian_angle));

		X *= w / 2;
		Y *= h / 2;

		corner[0] = _center - X - Y;
		corner[1] = _center + X - Y;
		corner[2] = _center + X + Y;
		corner[3] = _center - X + Y;



		center = _center;
		computeAxes();
	}




	void OBB2D::update(V2ctor& _center, const double w, const double h, double degree_angle) {
		double radian_angle = 2 * M_PI * (degree_angle / 360);

		V2ctor X(cos(radian_angle), sin(radian_angle));
		V2ctor Y(-sin(radian_angle), cos(radian_angle));

		X *= w / 2;
		Y *= h / 2;

		corner[0] = center - X - Y;
		corner[1] = center + X - Y;
		corner[2] = center + X + Y;
		corner[3] = center - X + Y;




		center = _center;
		computeAxes();
	}


	/** For testing purposes. */
	void OBB2D::moveTo(const V2ctor& center) {
		V2ctor centroid = (corner[0] + corner[1] + corner[2] + corner[3]) / 4;

		V2ctor translation = center - centroid;

		for (int c = 0; c < 4; ++c) {
			corner[c] += translation;
		}



		computeAxes();
	}

	/** Returns true if the intersection of the boxes is non-empty. */
	bool OBB2D::overlaps(const OBB2D& other) const {
		return overlaps1Way(other) && other.overlaps1Way(*this);
	}




	V2ctor* OBB2D::get_corner(){
		return corner;
	}

	V2ctor* OBB2D::get_center(){
		return &center;
	}




}











