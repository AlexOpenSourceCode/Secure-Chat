#pragma once



#ifndef GUIGLOBALS_H
#define GUIGLOBALS_H


#include <string>
#include <math.h> /* ceil */

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "base.h"

class gui_globals {
public:
	static int draw_text(std::string& s, float x, float y, float width = NULL, float height = NULL, bool bottom_to_top = true){
		glClearColor(0, 0, 0, 0);


		//Must put color before glRasterPos2i or else color wont be applied to drawn character
		glColor3d(1, 1, 1);
		glRasterPos2i(x, y);






		void * font = GLUT_BITMAP_9_BY_15;

		int font_width = 9;
		int font_height = 15;

		int lines_of_text = 1;

		int row_count = 0;


		int message_line_count = Base::compute_line_count(s, width);

		float message_height = NULL;


		int start_drawing_line_number = 1;
		if (height){


			message_height = message_line_count * font_height;

			if (message_height > height){
				start_drawing_line_number = ceil((message_height - height) / font_height);
			}

			float to_be_rendered_message_height = (message_line_count - start_drawing_line_number) * font_height;
			if (to_be_rendered_message_height >= height - font_height){
				//y -= 10;
				start_drawing_line_number += 1;
			}

		}
		float drawn_text_height = 0;

		for (char& c : s) {


			//if (height){
			//	//float drawn_text_height = lines_of_text * font_height;
			//	if (drawn_text_height > height){
			//		//height that was passed in has been used up
			//		return lines_of_text;
			//	}
			//}

			//If a defined width was set
			if (width){
				float row_pixels = row_count * font_width;
				////cout << "ROW PIXELS" << endl;
				////cout << row_pixels << endl;

				if (row_pixels >= width){
					if (lines_of_text >= start_drawing_line_number){
						y += font_height;
					}

					row_count = 0;
					lines_of_text++;

					glRasterPos2i(x, y);
				}
			}



			if (lines_of_text >= start_drawing_line_number){
				drawn_text_height += font_height;

				glutBitmapCharacter(font, c);
			}
			else{

			}




			row_count++;
		}

		//for (size_t i = 0; i < s.length(); i++){
		//	try {
		//		char c = s.at(i);
		//		glColor3d(0.0, 0.0, 0.0);
		//		glutBitmapCharacter(font, c);
		//	} catch (exception& e) {
		//		//cout << e.what() << '\n';
		//	}
		//}

		//for (std::string::iterator i = s.begin(); i != s.end(); ++i)
		//{
		//	try {
		//		char c = *i;
		//		glColor3d(0.0, 0.0, 0.0);
		//		glutBitmapCharacter(font, c);
		//	}
		//	catch (exception& e)
		//	{
		//		//cout << e.what() << '\n';
		//	}
		//	
		//}


		return lines_of_text;
	}

};



#endif