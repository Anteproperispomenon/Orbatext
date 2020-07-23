// optimise.cpp

#ifdef  __DEBUG_OPTIMISE__
#define __DEBUG_OUTPUT__
#endif

#include <string>
#include <vector>

// #include <set>

#include <sstream>

#include <exception>
#include <stdexcept>

#include "optimise.h"

#include "text-box.h"
#include "text-entry.h"

#ifdef __DEBUG_OUTPUT__
#include <iostream>
#endif

int get_box(int min_length, std::vector<TextBox> & boxes) {
	#ifdef  __DEBUG_OPTIMISE__
	std::cout << "min_length: " << min_length << std::endl;
	#endif
	
	int current_min = -1; // 0x7FFF;
	int current_ind = -1;
	
	int lng = boxes.size();

	// std::vector<TextBox>::iterator i = entries.begin();
	// std::vector<TextBox>::iterator j; // = entries.begin();
	
	int len = 0;
	
	for (int i = 0; i < lng; i++) {
		len = boxes.at(i).get_remaining();
		if (/*(current_min == -1) || */ (len >= min_length && (current_min == -1 || len < current_min))) {
			current_ind = i;
			current_min = len;
			//std::cout << "current_ind: " << current_ind << std::endl;
			//std::cout << "current_min: " << current_min << std::endl;
		}
	}
	return current_ind;
}

/*
TextBox & get_box_alt(int min_length, std::vector<TextBox> & entries) {
	unsigned int current_min = 0xFFFFFF;
	// int current_ind = -1;
	
	//int len = entries.length();

	std::vector<TextBox>::iterator i = entries.begin();
	std::vector<TextBox>::iterator j = entries.begin();
	
	int len = 0;
	bool found = false;
	
	for (i ; i != entries.end(); ++i) {
		len = i->get_remaining();
		if (len >= min_length && len < current_min) {
			j = i;
			current_min = len;
			found = true;
		}
	}
	
	if (!found) {
		throw std::runtime_error("Could not find large enough text box.");
	}
	return (*j);
}

*/

// Returns -1 if all entries have "length" 0.
int get_longest_entry(std::vector<TextEntry> &vec) {
	int x = -1;
	int len = vec.size();
	int mxl = 0; // max     length
	int crl = 0; // current length
	
	for (int i = 0; i < len; i++) {
		crl = vec.at(i).get_longest();
		if (crl > mxl) {
			mxl = crl;
			x   = i;
		}
	}
	return x;
}




bool optimise_entries(std::vector<TextBox> &boxes, std::vector<TextEntry> &entries) {
	int nt = -1;
	int bx = -1;
	
	//bool done1 = false;
	//bool done2 = false;
	
	// #ifdef __DEBUG_OUTPUT__
	int count = 0;
	// #endif
	
	while (true) {
		#ifdef __DEBUG_OUTPUT__
		std::cout << "Optimising " << count << "-th entry." << std::endl;
		#endif
		nt = get_longest_entry(entries);
		if (nt == -1) break;
		#ifdef __DEBUG_OUTPUT__
		std::cout << "nt=" << nt << std::endl;
		#endif
		TextEntry & ent = entries.at(nt);
		int el = ent.get_longest();
		if (el <= 0) break;
		#ifdef __DEBUG_OUTPUT__
		std::cout << "el=" << el << std::endl;
		#endif
		
		bx = get_box(el,boxes);
		if (bx == -1) { // break;
			std::string blah = ent.get_long_string();
			for (std::string::iterator it = blah.begin(); it != blah.end(); ++it) {
				if (*it == '\n') { // it = blah.erase(it);
				it = blah.erase (it);
				it = blah.insert(it,' ');
				}
			}
			std::ostringstream oss;
			oss << "Could not fit the following string (the " << (count+1) << "-th longest string) into a box:" << std::endl;
			oss << blah << std::endl << "(newlines removed)"; // << std::endl;
			throw (std::runtime_error(oss.str()));
		}
		#ifdef __DEBUG_OUTPUT__
		std::cout << "bx=" << bx << std::endl;
		std::cout << std::endl; // extra newline
		#endif
		TextBox & box = boxes.at(bx);
		
		// Actually adding to the box...
		bool z = ent.add_to_box(box);
		if (!z) {
			throw std::runtime_error("Could not add to box for some reason.");
		}
		count++;
		
	}
	
	// Resolving References
	int len = entries.size();
	std::vector<TextEntry>::iterator it;
	for (it = entries.begin(); it != entries.end(); ++it) {
		if (it->get_type() != RefE) continue;
		TextEntry &ent1 = entries.at(it->get_ref1() % len);
		TextEntry &ent2 = entries.at(it->get_ref2() % len);
		#ifdef __DEBUG_OUTPUT__
		std::cout << "ent1 = " << (it->get_ref1()) << " / " << (it->get_ref1() % len) << std::endl; 
		std::cout << "ent2 = " << (it->get_ref2()) << " / " << (it->get_ref2() % len) << std::endl; 
		#endif
		it->assign_ref(len, ent1, ent2);
	}
	
	return true;
	
}


