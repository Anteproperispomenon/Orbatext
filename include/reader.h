// reader.h

#ifndef __READER_H__
#define __READER_H__

#include "text-entry.h"

#include <string>

#include <sstream>
#include <istream>

//Create an entry from an input string.
TextEntry create_entry(std::string istr);

void skip_whitespace(std::istream & is);

std::string get_string (std::istream & is);
std::string get_string2(std::istream & is);

std::string get_line(std::istream & is);

void skip_line(std::istream & is);

// Take a single instance of a character,
// or not.
bool get_maybe(std::istream & is, char c);

// Info
struct InputInfo {
	// asfzxcv
	
	const int  ep_num;
	const int  virt_cutoff;
//	const int 
	const bool star_wars;
	const std::string swo;
	
	const bool full_file;
	
	InputInfo(int ep, int virt,std::string swo,bool full);
	InputInfo(int ep, int virt,bool sw        ,bool full);
	InputInfo(int ep, int virt);
	InputInfo(int ep);
		
};


// InputInfo make_info(std::stream str);
InputInfo make_info(std::istream &iss);

// Note: unescaping backslashes must be
//       done in the absolute last phase;
//       otherwise stuff like "\\n" might
//       be interpreted as a newline.
void unescape_chars (std::string & str);
void unescape_chars2(std::string & str);

#endif

