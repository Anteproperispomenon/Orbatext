// asdfzxcv

#ifdef  __DEBUG_TEXT_ENTRY__
#define __DEBUG_OUTPUT__
#endif

#include "text-entry.h"

#include "text-box.h"
#include <string>

#ifdef USE_SET
#include <set>
#include <utility>
#else
#include <algorithm>
#endif

#include <ostream>
#include <sstream>
#include <iomanip>

#ifdef USE_IO
#include <ios>
#endif

#ifdef __DEBUG_OUTPUT__
#include <iostream> // for now.
#endif

#include <exception>
#include <stdexcept>

TextEntry::TextEntry(std::string str1                  ):ent(SimpleE),str1(str1),str2("")  ,offs(0),done1(false),done2( true){}

TextEntry::TextEntry(std::string str1, int x           ):ent(ShortE ),str1(str1),str2("")  ,offs(x),done1(false),done2( true){}

TextEntry::TextEntry(std::string str1, std::string str2):ent(FullE  ),str1(str1),str2(str2),offs(0),done1(false),done2(false){}

// tEntry::TextEntry(int ref1          ):ent(RefE),ref1(ref1),ref2(ref1),offs(0),done1(false),done2(false) {}
TextEntry::TextEntry(int ref1, int ref2):ent(RefE),offs(0),ref1(ref1),ref2(ref2),done1(false),done2(false) {} // This is kinda awkward

TextEntry::~TextEntry() {
	// put stuff here.
}

// Return the length of the longer of
// the not-yet-placed strings.
int TextEntry::get_longest() const {
	if (done1 && done2) {
		return 0;
	}
	else if (ent == RefE) {
		return 0;
	}
	else if (done2) {
		return (str1.length());
	}
	else if (done1) {
		return (str2.length());
	}
	int len1 = str1.length();
	int len2 = str2.length();
	if (len1 >= len2) {
		return len1;
	}
	return len2;
}

bool TextEntry::all_done() const {
	return (done1 && done2);
}

EntryType TextEntry::get_type() const {
	return ent;
}

bool TextEntry::add_to_box (TextBox &box) {
	if (ent == SimpleE) {
		if (done1) return false; // asdfzxcv
		int offset = box.add_text(str1);
		int seg    = box.get_segment();
		seg1  = seg;
		seg2  = seg;
		ofs1  = offset;
		ofs2  = offset;
		done1 = true;
		return true;
	}
	else if (ent == ShortE) {
		if (done1) return false; // asdfzxcv
		int offset = box.add_text(str1);
		int seg    = box.get_segment();
		seg1  = seg;
		seg2  = seg;
		ofs1  = offset;
		ofs2  = offset + offs;
		done1 = true;
		return true;
	}
	else if (ent == FullE) {
		if (done1 && done2) return false;
		int len1 = str1.length();
		int len2 = str2.length();
		int seg  = box.get_segment();
		// True: !done1 || !done2
		// At the end of this next one,
		// str2 will have been transfered
		if	(done2 || (!done1 && (len1 >= len2))) {
			int offset = box.add_text(str1);
			seg1  = seg;
			ofs1  = offset;
			done1 = true;
			return  true;
		}
		else if (done1 || (!done2 && (len2 >= len1))) {
			int offset = box.add_text(str2);
			seg2  = seg;
			ofs2  = offset;
			done2 = true;
			return  true;
		}
	}
	else if (ent == RefE) {
		return true;
	}
	return false; // I dunno...
}

// Functions to return private
// variables.
unsigned long TextEntry::get_seg1() const {
	return seg1;
}

unsigned long TextEntry::get_seg2() const {
	return seg2;
}

int TextEntry::get_ofs1() const {
	return ofs1;
}

int TextEntry::get_ofs2() const {
	return ofs2;
}

int TextEntry::get_ref1() const {
	if (ent == RefE) return ref1;
	return (-1);
}

int TextEntry::get_ref2() const {
	if (ent == RefE) return ref2;
	return (-1);
}

// Assign the [final] pointer for the reference entries,
// provided the pointed-to entries have been completed.
bool TextEntry::assign_ref(int len, TextEntry & tnt1, TextEntry & tnt2) {
	if (ent != RefE)             return false;
	if (tnt1.all_done() != true) return false;
	if (tnt2.all_done() != true) return false;
	
	if (ref1 < len) {
		seg1 = tnt1.get_seg1();
		ofs1 = tnt1.get_ofs1();
	}
	else {
		seg1 = tnt1.get_seg2();
		ofs1 = tnt1.get_ofs2();
	}
	
	if (ref2 < len) {
		seg2 = tnt2.get_seg1();
		ofs2 = tnt2.get_ofs1();
	}
	else {
		seg2 = tnt2.get_seg2();
		ofs2 = tnt2.get_ofs2();
	}
	
	done1 = true;
	done2 = true;
	
	return true;
}

// Return
std::string TextEntry::get_long_string() const {
	if (ent == SimpleE) {
		return str1;
	}
	else if (ent == ShortE) {
		return str1;
	}
	else if (ent == FullE) {
		int len1 = str1.length();
		int len2 = str2.length();
		if	(done2 || (!done1 && (len1 >= len2))) {
			return str1;
		}
		else if (done1 || (!done2 && (len2 >= len1))) {
			return str2;
		}
		else if (len1 >= len2) {
			return str1;
		}
		else {
			return str2;
		}
	}
	else if (ent == RefE) {
		return ("<reference entry>"); // This probably won't happen.
	}
	return ("error"); // Might want to do this differently...
}	

// Helper Function
// (I guess?)
std::ostream & output_level(std::ostream & os, int lev, int len) {
	if (lev >= len) os << 'N';
	else            os << 'E';
	os << (lev % len);
	return os;
}

// allowing one to be used both ways
// template <class theType>
#ifdef USE_SET
// /*inline*/ std::pair<std::set<theType>::iterator,bool> cyc_insert(std::set<theType> & cyc, const theType & val) {
inline std::pair<std::set<int>::iterator,bool> cyc_insert(std::set<int> & cyc, const int & val) {
	return (cyc.insert(val));
}
#else
inline void cyc_insert(std::vector<int> & cyc, const int & val) {
	cyc.push_back(val);
}
#endif

//std::vector<TextBox>::iterator
// This can probably be cleaned up.
bool cycle_fix(std::vector<TextEntry> &entries) {
	int len = entries.size();
	// std::vector<std::set<int> > cycs;
	
	for (int i = 0; i < len; i++) {
		TextEntry & ent = entries.at(i);
		if (ent.get_type() != RefE) continue;
		
		#ifdef USE_SET
		std::set<int> cyc; // = cycs.at(i);
		#else
		std::vector<int> cyc;
		#endif
		// This next line is necessary,
		// since otherwise the first element
		// of the sequence won't be redirected.
		cyc_insert(cyc, i);
		
		// "Outcome" value
		// outc =  0 ==> Haven't yet determined whether it ends in text or a loop.
		// outc =  1 ==> The path ends in actual text.
		// outc = -1 ==> The path ends in a loop.
		int outc = 0;
		int off1 = ent.ref1;
		// this works in both (i think) nope
		#ifdef USE_SET
		std::set<int>::iterator it = cyc.end();
		#else
		std::vector<int>::iterator it = cyc.end();
		#endif
		while (outc == 0) {
			// "rent" is the entry that the current entry points to.
			TextEntry & rent = entries.at(off1 % len);
			if (rent.get_type() != RefE) {
				// Here, the end of the link/'cycle'/path has been found.
				outc = 1;
				break;
			}
			// Check and see if that node/reference has already been reached.
			// If it hasn't, the iterator is cyc.end().
			#ifdef USE_SET
			it = cyc.find(off1);
			#else
			it = std::find(cyc.begin(),cyc.end(),off1);
			#endif
			if (it != cyc.end()) {
				outc = -1;
			}
			else {
				// Insert the latest reference node into the reference list/set
				cyc_insert(cyc,off1);
				#ifdef __DEBUG_OUTPUT__
				int offx = off1;
				#endif
				// Update the reference point by looking at the next node.
				if (off1 > len) off1 = rent.ref2;
				else            off1 = rent.ref1;
				#ifdef __DEBUG_OUTPUT__
				std::cout << "off1=" << offx << " >>> off1=" << off1 << std::endl; 
				#endif
			}
			
			
		}
		
		// Here, the path ends in actual text.
		// Thus, we set all reference entries to point directly to the 
		// end node, to make things easier later on.
		if (outc == 1 && !(cyc.empty())) {
			#ifdef __DEBUG_OUTPUT__
			std::cout << "In the cycle redirect loop A for entry #" << i << "." << std::endl;
			#endif
			// Declaring the iterator for the for-loop ahead of time.
			// This is necessary, since the type of the iterator
			// is dependent on whether USE_SET is on.
			#ifdef USE_SET
			std::set<int>::iterator st = cyc.begin();
			#else
			std::vector<int>::iterator st = cyc.begin();
			#endif
			for ( ; st != cyc.end() ; ++st ) {
				#ifdef __DEBUG_OUTPUT__
				std::cout << "off: " << off1          << std::endl;
				std::cout << "st1: " <<  (*st)        << std::endl;
				std::cout << "st2: " << ((*st) % len) << std::endl;
				#endif
				if ((*st) > len) entries.at((*st) % len).ref2 = off1;
				else             entries.at((*st)      ).ref1 = off1;
			}
		}
		
		// If the path ends in a loop...
		if (outc == -1) {
			#ifdef USE_SET
			throw std::runtime_error("Entry list contains a cycle.");
			#else
			std::ostringstream oss;
			oss << "Entry list contains a cycle:" << std::endl;
			oss << std::setw(2) << std::setfill('0');
			std::vector<int>::iterator ct = cyc.begin();
			for ( ; ct != cyc.end(); ++ct) {
				output_level(oss, *ct, len) << " -> ";
			}
			output_level(oss,off1,len);
			throw std::runtime_error(oss.str());
			#endif
		}
		
		// for the second reference
		cyc.clear(); // = cycs.at(i);
		
		// This is basically the same thing, but 
		// with the second reference.
		cyc_insert(cyc,len+i);
		outc = 0;
		off1 = ent.ref2;
		#ifdef USE_SET
		std::set<int>::iterator it2 = cyc.end();
		#else
		std::vector<int>::iterator it2 = cyc.end();
		#endif
		while (outc == 0) {
			TextEntry & rent = entries.at(off1 % len);
			if (rent.get_type() != RefE) {
				outc = 1;
				break;
			}
			#ifdef USE_SET
			it2 = cyc.find(off1);
			#else
			it2 = std::find(cyc.begin(),cyc.end(),off1);
			#endif
			if (it2 != cyc.end()) {
				outc = -1;
			}
			else {
				#ifdef __DEBUG_OUTPUT__
				int offx = off1;
				#endif
				cyc_insert(cyc,off1);
				if (off1 > len) off1 = rent.ref2;
				else            off1 = rent.ref1;
				#ifdef __DEBUG_OUTPUT__
				std::cout << "off1=" << offx << " >>> off1=" << off1 << std::endl; 
				#endif
			}

		}
		
		// Again, here it ends in an actual text entry.
		if (outc == 1 && !(cyc.empty())) {
			#ifdef __DEBUG_OUTPUT__
			std::cout << "In the cycle redirect loop B for entry #" << i << "." << std::endl;
			#endif
			
			#ifdef USE_SET
			std::set<int>::iterator st = cyc.begin();
			#else
			std::vector<int>::iterator st = cyc.begin();
			#endif
			for (  ; st != cyc.end() ; ++st ) {
				#ifdef __DEBUG_OUTPUT__
				std::cout << "off: " << off1          << std::endl;
				std::cout << "st1: " <<  (*st)        << std::endl;
				std::cout << "st2: " << ((*st) % len) << std::endl;
				#endif
				if ((*st) > len) entries.at((*st) % len).ref2 = off1;
				else             entries.at((*st)      ).ref1 = off1;
			}
			
		}
		
		if (outc == -1) {
			#ifdef USE_SET
			throw std::runtime_error("Entry list contains a cycle.");
			#else
			std::ostringstream oss;
			oss << "Entry list contains a cycle:" << std::endl;
			oss << std::setw(2) << std::setfill('0');
			std::vector<int>::iterator ct = cyc.begin();
			for ( ; ct != cyc.end(); ++ct) {
				output_level(oss, *ct, len) << " -> ";
			}
			output_level(oss,off1,len);
			throw std::runtime_error(oss.str());
			#endif
		}
		
		
	}
	
	return true;
	
}

// Render the patch for the level entry
std::string TextEntry::render_patch1() const {
	//int state = 0;
	std::ostringstream ss;
	ss << std::setbase(16) << "$" << std::setw(5) << std::setfill('0') << seg1 << std::setw(3) << ofs1 << "RL";  // << "%patch $" << segment;
	// $1EF80013RL
	return (ss.str());
}

// Render the patch for the level name
std::string TextEntry::render_patch2() const {
	//int state = 0;
	std::ostringstream ss;
	ss << std::setbase(16) << "$" << std::setw(5) << std::setfill('0') << seg2 << std::setw(3) << ofs2 << "RL";  // << "%patch $" << segment;
	// $1EF80013RL
	return (ss.str());
}

// These are for when you output directly to an output stream.
#ifdef USE_IO
// Render the patch for the level entry
std::ostream & render_patch1(std::ostream & os) const {
	//int state = 0;
	//std::ostringstream ss;
	//int  wd = os.width();
	//char fl = os.fill();
	os << std::setbase(16) << "$" << std::setw(5) << std::setfill('0') << seg1 << std::setw(3) << ofs1 << "RL";  // << "%patch $" << segment;
	//os << std::setw(wd) << std::setfill(fl);
	// $1EF80013RL
	return (os);
}

// Render the patch for the level name
std::ostream & render_patch2(std::ostream & os) const {
	//int state = 0;
	//std::ostringstream ss;
	//int  wd = os.width();
	//char fl = os.fill();
	os << std::setbase(16) << "$" << std::setw(5) << std::setfill('0') << seg2 << std::setw(3) << ofs2 << "RL";  // << "%patch $" << segment;
	//os << std::setw(wd) << std::setfill(fl);
	// $1EF80013RL
	return (os);
}
#endif





// 
void TextEntry::fix_refs(int len) {
	
	if (ent != RefE) return;
	
	//    y == -x-1 == -(x+1)
	//   -y == x+1
	// -1-y == x
	
	#ifdef __DEBUG_OUTPUT__
	bool x = (ref1 < 0) || (ref2 < 0);
	
	if (x) {
		std::cout << "before:"        << std::endl;
		std::cout << "ref1: " << ref1 << std::endl;
		std::cout << "ref2: " << ref2 << std::endl;
	}
	#endif
	
	if (ref1 < 0) ref1 = len + (-1-ref1);
	if (ref2 < 0) ref2 = len + (-1-ref2);

	#ifdef __DEBUG_OUTPUT__
	if (x) {
		std::cout << "after:"         << std::endl;
		std::cout << "ref1: " << ref1 << std::endl;
		std::cout << "ref2: " << ref2 << std::endl;
	}
	#endif
	
}





