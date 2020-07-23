// text-box.cpp

#ifdef  __DEBUG_TEXT_BOX__
#define __DEBUG_OUTPUT__
#endif

#include "text-box.h"

#include <string>
#include <exception>
#include <stdexcept>

#include <ostream>
#include <sstream>
#include <iomanip>

// //-@ -> /*-@-*/
#ifdef __DEBUG_OUTPUT__
#include <iostream>
#endif

TextBox::TextBox(int sz, unsigned long seg):segment(seg),avail(sz),size(sz),sw(false) {
	// text  = new char [sz];
	text2.reserve(sz);
	//avail = sz;
	// There's probably a better way to do this
	for (int i = 0; i < sz; i++) {
		// text[i] = 0;
		text2.push_back(0);
	}
	avail--; // to ensure a null-terminator
	//text2.reserve(sz);
	// for (std::string::iterator it = text2.begin(); it != text2.end(); ++it) {
	// 	*it = 0;
	// }
	
}

TextBox::TextBox(int sz, unsigned long seg,bool sw):segment(seg),avail(sz),size(sz),sw(sw) {
	// text  = new char [sz];
	text2.reserve(sz);
	//avail = sz;
	// There's probably a better way to do this
	for (int i = 0; i < sz; i++) {
		// text[i] = 0;
		text2.push_back(0);
	}
	avail--; // to ensure a null-terminator
	//text2.reserve(sz);
	// for (std::string::iterator it = text2.begin(); it != text2.end(); ++it) {
	// 	*it = 0;
	// }
	
}

TextBox::~TextBox() {
	// delete [] text;
	text2.clear();
}

int TextBox::get_remaining() const {
	return avail;
}

// in case I need to change the functionality
int TextBox::get_pointer() const {
	return avail;
}

unsigned long TextBox::get_segment() const {
	return segment;
}


bool TextBox::is_sw() const{
	return sw;
}


int TextBox::add_text(const std::string & str) {
	int len = str.length();
	int avl = get_remaining();
	#ifdef __DEBUG_OUTPUT__
	std::cout << "len: " << len << ", avl: " << avl    << std::endl;
	std::cout << "str: " << str << std::endl << "----" << std::endl;
	#endif
	if (avl < len) {
		throw std::runtime_error("Not enough room in text box.");
	}
	
	int ofs = 0; // offset
	
	if (avl == len) {
		for (int i = 0; i < len; i++) {
			// text[i]     = str.at(i);
			text2.at(i) = str.at(i);
		}
		avail = 0;
	}
	else if (len < avl) {
		ofs = avl - len;
		for (int i = 0; i < len; i++) {
			// text[ofs+i]     = str.at(i);
			text2.at(ofs+i) = str.at(i);
		}
		avail = ofs - 1;
	}
	#ifdef __DEBUG_OUTPUT__
	std::cout << "Current state of box: " << std::endl;
	std::cout << render_patch() << std::endl;
	#endif
	return ofs;
}

// example:
// 10-char  array
// 10 chars avail
// 7 char string
//
//     0000000000
// --> 000xxxxxxx
// --> yy0xxxxxxx
// therefore,
// 2 chars available
// offset is 3
// ofs = avail - len

std::string TextBox::render_segfile() const {
	std::ostringstream ss;
	ss << std::setbase(16) << std::setw(5) << std::setfill('0') << "%patchfile $" << segment;
	return (ss.str());
}

std::string TextBox::render_patch() const {
	int state = 0;
	std::ostringstream ss;
	ss << std::setbase(16) << std::setw(5) << std::setfill('0') << "%patch $" << segment;
	char x = 0;
	for (int i = 0; i < size; i++) {
		// x = text[i];
		x = text2.at(i);
		if (state == 0) {
			if      (x == '\0') ss << " $00";
			else if (x == '\n') ss << " $0A";
			else if (x == '\"') ss << " $22";
			else if (x == '#' ) ss << " $23";
			else if (x == '\\') ss << " $5C";
			else {
				ss << " \"" << x;
				state = 1;
			}
		}
		else if (state == 1) {
			if      (x == '\0') { ss << "\" $00"; state = 0; }
			else if (x == '\n') { ss << "\" $0A"; state = 0; }
			else if (x == '\"') { ss << "\" $22"; state = 0; }
			else if (x == '#' ) { ss << "\" $23"; state = 0; }
			else if (x == '\\') { ss << "\" $5C"; state = 0; }
			else                { ss << x ; }
		}
	}
	if (state == 1) ss << "\""; // since the quotation marks haven't been ended.
	return (ss.str());
}

std::ostream & TextBox::render_patch_file(std::ostream & os) const {
	if (sw) return os.write(text2.c_str(),size);
	return os;
}


