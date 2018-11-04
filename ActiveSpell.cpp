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



#include "Spell.h"
#include "ActiveSpell.h";

//#include "base.h";

#include <Eigen/Dense>


using namespace std;
using namespace Eigen;


namespace rmp {


	ActiveSpell::ActiveSpell(Vector3f _center, Spell* _spell) : Object(_center),
		damage{ damage },
		spell{ _spell }
	{
		//width = 0.5;
		//height = 0.5;
		//change_x(5);



		//set_x(0);
		//set_y(0);
		//set_z(0);
		
	};



	ActiveSpell::ActiveSpell(Vector3f _center, Spell* _spell, float _dx, float _dy, float _dz) : Object(_center),
		damage{ damage },
		spell{ _spell }
	{
		//width = 0.5;
		//height = 0.5;
		//change_x(5);
		dx = _dx;
		dy = _dy;
		dz = _dz;


		//set_x(0);
		//set_y(0);
		//set_z(0);

	};
	


	ActiveSpell::ActiveSpell(float _x, float _y, float _z, Spell* _spell) : Object(_x, _y, _z),
		damage{ damage },
		spell{ _spell }
	{};
	

	void ActiveSpell::draw(){
		
		////cout << "DRAWING ACTIVE SPELL (x = " << get_x() << ", y = " << get_y() << ")" << endl;


		Object::draw();
	}



	//void ActiveSpell::draw(){


	//	glPushMatrix();


	//	//draw bounding box
	//	glBegin(GL_QUADS);
	//	glColor3f(0.7, 0.5, 0.9);


	//	V2ctor* center = obb.get_center();
	//	V2ctor* corners = obb.get_corner();


	//	glVertex3f(corners[0].x, 2, corners[0].y);


	//	glVertex3f(corners[1].x, 2, corners[1].y);


	//	glVertex3f(corners[2].x, 2, corners[2].y);


	//	glVertex3f(corners[3].x, 2, corners[3].y);

	//	glEnd();
	//	glPopMatrix();

	//	glPushMatrix();
	//	glTranslated(get_x(), get_y() + height / 2, get_z()); //translate the cube
	//	glRotatef(dx, 1.0, 0.0, 0.0);
	//	glRotatef(dy * -1, 0.0, 1.0, 0.0);
	//	glRotatef(dz, 0.0, 0.0, 1.0);



	//	/* Ceiling */
	//	glColor3f(0.2, 0.0, 0.5);
	//	glVertex3f(-width, height, -width);
	//	glVertex3f(width, height, -width);
	//	glVertex3f(width, height, width);
	//	glVertex3f(-width, height, width);


	//	///* Walls */

	//	glBegin(GL_QUADS);
	//		glColor3f(1.0, 0.5, 0.0);
	//		//glVertex3f(-floor_size, -floor_size, floor_size);
	//		//glVertex3f(floor_size, -floor_size, floor_size);
	//		//glVertex3f(floor_size, floor_size, floor_size);
	//		//glVertex3f(-floor_size, floor_size, floor_size);

	//		glVertex3f(-width, 0, width);
	//		glVertex3f(width, 0, width);
	//		glVertex3f(width, width, width);
	//		glVertex3f(-width, width, width);


	//		glColor3f(1.0, 0.0, 0.4);
	//		glVertex3f(-width, -0, -width);
	//		glVertex3f(width, -0, -width);
	//		glVertex3f(width, width, -width);
	//		glVertex3f(-width, width, -width);


	//		glColor3f(1.0, 0.0, 0.0);
	//		glVertex3f(width, width, width);
	//		glVertex3f(width, -0, width);
	//		glVertex3f(width, -0, -width);
	//		glVertex3f(width, width, -width);



	//		glColor3f(0.5, 0.2, 0.4);
	//		glVertex3f(-width, width, width);
	//		glVertex3f(-width, -0, width);
	//		glVertex3f(-width, -0, -width);
	//		glVertex3f(-width, width, -width);
	//	glEnd();








	//	glutSolidCube(width); //draw the cube
	//	glPopMatrix();
	//	
	//}



}










