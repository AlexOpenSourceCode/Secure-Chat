#pragma once

#ifndef BASE_H
#define BASE_H

#include <vector>
#include <string>
#include <math.h> /* ceil */
#include <algorithm>
#include <string> 


#include <iostream>

#include <cstdlib>
#include <sys/timeb.h>

#define DEFAULT_PORT "9999"
#define DEFAULT_BUFFER_LENGTH	512

using namespace std;

class Base {
public:




	static int current_time_ms();

	static int ms_span(int nTimeStart);



	static bool file_exists(const string& fileName);


	//Determines how many lines of text there would be of a string in a given width
	static int compute_line_count(const std::string& str, float width, float char_width = 9.0);


	//Splits string at delimeter and returns 
	//vector<string> of each part
	static std::vector<std::string> split(const std::string &s, char delim);


	template<typename T>
	static void remove_substring(basic_string<T>& s, const basic_string<T>& p);



	static void print_vector(const vector<string>& v);

	//wraps number to degrees of a circle
	static float circle_number(float num);

	static int blank_function();

	static string to_lower_case(string& str);

	static string get_request(string url);	


	static size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data);


};



#endif