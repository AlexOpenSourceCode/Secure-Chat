#pragma once

#ifndef TEST_H
#define TEST_H


#include <string>
#include <math.h> /* ceil */

class BaseTest {
public:
	//Determines how many lines of text there would be of a string in a given width
	static int compute_line_count(const std::string& str, float width, float char_width = 9.0){
		size_t str_length = str.length();
		int line_count = ceil((str_length * char_width) / width);
		return line_count;
	}

};

#endif