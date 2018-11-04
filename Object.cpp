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
#include "Object.h";
#include "V2ctor.h";

#include "base.h";

#include <Eigen/Dense>


using namespace std;
using namespace Eigen;


namespace rmp {


	Object::Object(float _x, float _y, float _z, float _width, float _height) :
		width(_width),
		height(_height),
		center(_x, _y, _z),
		center2d(_x, _z),
		obb(center2d, width, height, dy)
	{

	};

	Object::Object(Vector3f _center, float _width, float _height) :
		width(_width),
		height(_height),
		center(_center),
		center2d(_center[0], _center[2]),
		obb(center2d, width, height, dy)
	{

	};


	float Object::get_x(){
		return center[0];
	}

	float Object::get_y(){
		return center[1];
	}

	float Object::get_z(){
		return center[2];
	}


	OBB2D Object::get_obb(){
		obb.update(center2d, width, height, dy);
		return obb;
	}



	float Object::get_dx(){
		return dx;
	}

	float Object::get_dy(){
		return dy;
	}

	float Object::get_dz(){
		return dz;
	}


	void Object::set_dx(float _dx){
		dx = _dx;

	}

	void Object::set_dy(float _dy){
		dy = _dy;
		obb.update(center2d, width, height, dy);
	}

	void Object::set_dz(float _dz){
		dz = _dz;
	}





	void Object::change_x(float diff_x){
		center[0] += diff_x;
		center2d.x = center[0];
		obb.update(center2d, width, height, dy);
	}

	void Object::change_y(float diff_y){
		center[1] += diff_y;
	}

	void Object::change_z(float diff_z){
		center[2] += diff_z;
		center2d.y = center[2];
		obb.update(center2d, width, height, dy);
	}



	float circle_number(float num){
		if (num >= 360.0){
			num = 0.0;
		}
		else if (num <= 0.0){
			num = 360 + num; //Add because # is negative except 0
			//or
			//num = 360;

		}

		return num;
	}



	void Object::change_dx(float direction_difference_x){
		dx += direction_difference_x;

		dx = circle_number(dx);
	}




	


	void Object::change_dy(float direction_difference_y){
		dy += direction_difference_y;
		dy = circle_number(dy);

		obb.update(center2d, width, height, dy);
	}

	void Object::change_dz(float direction_difference_z){
		dz += direction_difference_z;
	}





	float Object::get_width(){
		return width;
	}

	float Object::get_height(){
		return height;
	}

	//dx = direction x
	void Object::draw(float _x, float _y, float _z, float _dx, float _dy, float _dz){
		draw_function(_x, _y, _z, _dx, _dy, _dz);
	}

	void Object::draw(float _x, float _y, float _z){
		draw_function(_x, _y, _z, dx, dy, dz);
	}

	void Object::draw(){
		draw_function(center[0], center[1], center[2], dx, dy, dz);
	}



	string Object::print(){
		return "";
	}



	//draw objects
	void Object::draw_function(float _x, float _y, float _z, float _dx, float _dy, float _dz){

		glPushMatrix();


		//draw bounding box
		glBegin(GL_QUADS);
		glColor3f(0.7, 0.5, 0.9);
		V2ctor* center = obb.get_center();
		V2ctor* corners = obb.get_corner();


		glVertex3f(corners[0].x, 2, corners[0].y);


		glVertex3f(corners[1].x, 2, corners[1].y);


		glVertex3f(corners[2].x, 2, corners[2].y);


		glVertex3f(corners[3].x, 2, corners[3].y);

		glEnd();
		glPopMatrix();

		glPushMatrix();
		glTranslated(_x, _y + height / 2, _z); //translate the cube
		glRotatef(_dx, 1.0, 0.0, 0.0);
		glRotatef(_dy * -1, 0.0, 1.0, 0.0);
		glRotatef(_dz, 0.0, 0.0, 1.0);



		/* Ceiling */
		glColor3f(0.2, 0.0, 0.5);
		glVertex3f(-width, height, -width);
		glVertex3f(width, height, -width);
		glVertex3f(width, height, width);
		glVertex3f(-width, height, width);


		///* Walls */

		glBegin(GL_QUADS);
		glColor3f(1.0, 0.5, 0.0);
		//glVertex3f(-floor_size, -floor_size, floor_size);
		//glVertex3f(floor_size, -floor_size, floor_size);
		//glVertex3f(floor_size, floor_size, floor_size);
		//glVertex3f(-floor_size, floor_size, floor_size);

		glVertex3f(-width, 0, width);
		glVertex3f(width, 0, width);
		glVertex3f(width, width, width);
		glVertex3f(-width, width, width);


		glColor3f(1.0, 0.0, 0.4);
		glVertex3f(-width, -0, -width);
		glVertex3f(width, -0, -width);
		glVertex3f(width, width, -width);
		glVertex3f(-width, width, -width);


		glColor3f(1.0, 0.0, 0.0);
		glVertex3f(width, width, width);
		glVertex3f(width, -0, width);
		glVertex3f(width, -0, -width);
		glVertex3f(width, width, -width);



		glColor3f(0.5, 0.2, 0.4);
		glVertex3f(-width, width, width);
		glVertex3f(-width, -0, width);
		glVertex3f(-width, -0, -width);
		glVertex3f(-width, width, -width);
		glEnd();








		glutSolidCube(width); //draw the cube
		glPopMatrix();
	}



	//void Object::change_x(float diff_x){
	//	center[0] += diff_x;
	//	center2d.x = center[0];
	//	obb.update(center2d, width, height, dy);
	//}



	//void Object::change_x(float diff_x){
	//	center[0] += diff_x;
	//	center2d.x = center[0];
	//	obb.update(center2d, width, height, dy);
	//}

	//void Object::change_y(float diff_y){
	//	center[1] += diff_y;
	//}

	//void Object::change_z(float diff_z){
	//	center[2] += diff_z;
	//	center2d.y = center[2];
	//	obb.update(center2d, width, height, dy);
	//}




	void Object::set_x(float _x){
		center[0] = _x;
		center2d.x = center[0];
		obb.update(center2d, width, height, dy);
	}

	void Object::set_y(float _y){
		center[1] = _y;
	}

	void Object::set_z(float _z){
		center[2] = _z;
		center2d.y = center[2];
		obb.update(center2d, width, height, dy);
	}

	//There are 360 degrees in a circle.And that 360 degrees is equivalent to 2 * pi radians.

	//So, converting and angle x degrees to radians is 2 * pi * (x / 360).


	float degree_to_radian(float _degrees){
		float M_PI = 3.14159265359;
		return 2 * M_PI * (_degrees / 360.0);
	}

	void Object::move(milliseconds time_delta){
		//deltaTime = currentTime - timeLastFrame    // time sins last frame
		//velocityDelta = deltaTime * gravityRate    // velocity change 

		// calculate new position with only half of the velocity change this frame
		//objectPosition.y -= (velocity.y + (velocityDelta / 2)) * deltaTime
		//velocity.y += velocityDelta

		


		//X += Speed * Math.Cos(angle);
		//Y += speed * Math.Sin(angle);


		////cout << "D: (" << dx << "," << dy << "," << dz << ")" << endl;
		////cout << "DY: (" << dy << endl;


		//Open gl when yrot is set to 0 points towards
		// the 3 and 4 quadrant
		//
		//     4  |  1
		//	  ____|____
		//		  |  
		//     3  |  2
		// 3rd on left 4th on right

		//when yrot = 0, cos(0) = 1, sin(0) = 0
		//so xy moves (1,0)
		//but since we arent facing 4(left),1(right)
		//that movement goes to the right
		// - 90 to shift

		float dy_radian = degree_to_radian(dy - 90);
		//float dy_radian = degree_to_radian(dy);

		////cout << "DY radians: (" << dy_radian << endl;


		//distance = rate * time
		float distance = speed * time_delta.count();
		distance = speed;


		//hypotenuse = distance

		// cos(x) = a / h;
		// a = h * cos(x) = dx

		// sin(x) = o / h;
		// o = h * sin(x) = dy

		change_x(distance * cos(dy_radian));
		change_z(distance * sin(dy_radian));



	}
		


	Object::~Object(){

	}



}






