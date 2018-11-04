#include "base.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;


//Determines how many lines of text there would be of a string in a given width
int Base::compute_line_count(const std::string& str, float width, float char_width){
	size_t str_length = str.length();
	int line_count = ceil((str_length * char_width) / width);
	return line_count;
}



//Splits string at delimeter and returns 
//vector<string> of each part
vector<string> Base::split(const string &s, char delim) {
	vector<string> elems;
	stringstream ss(s);
	string number;
	while (getline(ss, number, delim)) {
		elems.push_back(number);
	}
	return elems;
}



template<typename T>
void Base::remove_substring(basic_string<T>& s, const basic_string<T>& p) {
	basic_string<T>::size_type n = p.length();

	for (basic_string<T>::size_type i = s.find(p);
		i != basic_string<T>::npos;
		i = s.find(p))
		s.erase(i, n);
}




void Base::print_vector(const vector<string>& v){
	//cout << " vector<";
	for (size_t x = 0; x < v.size(); x++) {
		//cout << v[x];
		if (x + 1 < v.size()){
			//cout << ", ";
		}
	}
	//cout << ">" << endl;
}


//wraps number to degrees of a circle
float Base::circle_number(float num){
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


string Base::to_lower_case(string& str){
	std::transform(str.begin(), str.end(), str.begin(), ::tolower);
	return str;
}

size_t Base::writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
	data->append((char*)ptr, size * nmemb);
	return size * nmemb;
}


bool Base::file_exists(const string& fileName)
{
	std::ifstream infile(fileName);
	return infile.good();
}




int Base::current_time_ms(){
	timeb tb;
	ftime(&tb);
	int nCount = tb.millitm + (tb.time & 0xfffff) * 1000;
	return nCount;
}

int Base::ms_span(int nTimeStart){
	int nSpan = current_time_ms() - nTimeStart;
	if (nSpan < 0)
		nSpan += 0x100000 * 1000;
	return nSpan;
}