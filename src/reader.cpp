// reader.cpp

// #define __DEBUG_UNESCAPE__

#ifdef  __DEBUG_READER__
#define __DEBUG_OUTPUT__
#endif

#ifdef __DEBUG_UNESCAPE__
#include <iostream> // for now
#endif

// #ifdef __DEBUG_OUTPUT__
// #define __DEBUG_UNESCAPE__
// #include <iostream>
// #endif

#include "reader.h"
#include "text-entry.h"

#include <string>
#include <sstream>
#include <istream>

#ifdef __DEBUG_OUTPUT__
#include <iostream> // for now
#endif


//Create an entry from an input string.
//(Will have to check that this string
// is not just comments first)
TextEntry create_entry(std::string istr) {
	
	int nposx = (int) istr.npos;
	
	//std::string str = istr; // copy input string.
	unsigned int com1 = istr.find_first_of('#'); // start of comments
	// std::string str = istr.substr(0,com1); // copy the non-comment part of the input string.
	if (com1 != istr.npos) com1--;
	unsigned int com2 = istr.find_last_not_of (" \n\t", com1); // strip away ending whitespace.
	
	// Unnecessary; whitespace is stripped in the input phase.
	//unsigned int com3 = istr.find_first_not_of(" \t"  ); // Strip away leading whitespace.
	
	#ifdef __DEBUG_OUTPUT__
	std::cout << "Working on the following string:" << std::endl;
	std::cout << istr << std::endl << std::endl;
	std::cout << "com1: " << com1 << std::endl;
	std::cout << "com2: " << com2 << std::endl;
	// std::cout << "com3: " << com3 << std::endl;
	#endif
	
	std::string str = istr.substr(0,com2+1); // copy the content part of the string
	
	#ifdef __DEBUG_OUTPUT__
	std::cout << "Resulting substring:" << std::endl;
	// std::cout << "str : " << str  << std::endl;
	std::cout << str << std::endl << std::endl;
	#endif
	
	// Unescaping characters that won't have an effect
	// on program logic at this point
	// (So, '#' can be unescaped since comments
	//  have already been removed, but '|' can't
	//  be unescaped because that would interfere
	//  with the splitting logic.)
	unescape_chars(str);
	
	// Actually modifying the string
	
	// If the entry is a reference:
	if (str.at(0) == '*') {
		#ifdef __DEBUG_OUTPUT__
		std::cout << "This is a reference entry." << std::endl;
		#endif
		std::istringstream iss(str);
		iss.get(); // to get the '*' out of the stream
		skip_whitespace(iss);
		int x; // seems like it should be char, but iss.peek() returns an int. 
		int y; // same
		int en; // entrance number
		int nn; // name     number
		
		// Get the entrance value for
		// this level's entry text.
		x = iss.peek();
		if (x == 'e' || x == 'E') {      // This points to the entry text
			iss.get();
			iss >> en;
		} 
		else if (x == 'n' || x == 'N') { // This points to the level name
			iss.get();
			iss >> en;
			en = -1 * (en+1);
		}
		else {                           // This points to the entry text
			iss >> en;
		}
		
		skip_whitespace(iss);
		
		y = iss.peek();
		if (y == 'e' || y == 'E') {      // This points to the entry text
			iss.get();
			iss >> nn;
		} 
		else if (y == 'n' || y == 'N') { // This points to the level name
			iss.get();
			iss >> nn;
			nn = -1 * (nn+1);
		}
		else {                           // This points to the level name
			iss >> nn;
			nn = -1 * (nn+1);
		}
		
		#ifdef __DEBUG_OUTPUT__
		std::cout << "en : " << en << std::endl;
		std::cout << "nn : " << nn << std::endl;
		#endif
		
		// No need to unescape stuff, since
		// this is a reference entry.
		
		return (TextEntry(en,nn));
		
	}
	
	// Unescaping the first star, if necessary...
	if (str.at(0) == '\\' && str.at(1) == '*') {
		str.erase(0,1);
	}
	
	// Now, other entry types...
	int pos;
	
	// If a full entry...
	pos = str.find("||");
	// extra variable to deal with the
	// case where you have "\||"
	// TODO: Check if this still works with "\|||"
	// Needs to be fixed: "\|||" seems to work,
	// but "\||" doesn't.
	bool ext = true;
	if (pos != 0 && pos != /*str.npos*/ nposx) {
		// It should be that str.at(pos) == '|' and str.at(pos+1) == '|'.
		if (str.at(pos-1) == '\\') {
			if ((pos+2 != /*str.npos*/ nposx) && str.at(pos+2) == '|' ) {
				pos++;
			} else {
				ext = false;
			}
		}
		
	}
	if (pos != /*str.npos*/ nposx && ext) {
		//std::string::iterator it = str.begin()+pos;
		// it = str.erase(it);
		// it = str.erase(it);
		// leave "it" be for now.
		
		#ifdef __DEBUG_OUTPUT__
		std::cout << "Managing a Full Entry" << std::endl;
		#endif
		
		// Split the string into the level name and the entry text.
		std::string str1 = str.substr(0,pos); // try changing this if the entry text/name is cut off.
		std::string str2 = str.substr(pos+2,str.npos);
		
		#ifdef __DEBUG_OUTPUT__
		// Temporary
		std::cout << "str1:" << std::endl << str1 << std::endl << std::endl;
		std::cout << "str2:" << std::endl << str2 << std::endl << std::endl;
		#endif
		
		
		// Replace escaped newlines
		// with actual newlines
		// (and also unescape pipes)
		unescape_chars2(str1);
		unescape_chars2(str2);
		
		// I guess that's it?
		return (TextEntry(str1,str2));
		
	}
	
	// Short Entries...
	int  lng = str.length();
	bool   z = true;
	char   c;
	int  ind = -1;
	
	#ifdef __DEBUG_OUTPUT__
	std::cout << "In the \"Short Entries\" area." << std::endl;
	#endif
	
	
	// Finding the first '|' NOT preceded by
	// a backslash.
	for (int i = 0; i < lng; i++) {
		c = str.at(i);
		if (c == '\\') {
			z = false;
			continue;
		}
		else if (z && (c == '|')) {
			ind = i;
			break;
		}
		else {
			z = true;
		}
	}
	
	if (ind != -1) {
		
		#ifdef __DEBUG_OUTPUT__
		std::cout << "This entry is a short entry." << std::endl;
		#endif
		
		// To ease unescaping characters, this splits the
		// string into two here, then recombines them later.
		std::string str1 = str.substr(0,ind); // need to check for +-1 errors
		std::string str2 = str.substr(ind+1,str.npos);
		
		#ifdef __DEBUG_OUTPUT__
		// Temporary
		std::cout << "str1:" << std::endl << str1 << std::endl << std::endl;
		std::cout << "str2:" << std::endl << str2 << std::endl << std::endl;
		#endif
		
		// Replace escaped newlines
		// with actual newlines
		// And escaped pipes with
		// pipes.
		unescape_chars2(str1);
		unescape_chars2(str2);
		
		// I guess that's it?
		return (TextEntry((str1+str2),str1.length()));
		
	}
	
	// std::string str1 = str.substr(0,ind); // need to check for +-1 errors
	// std::string str2 = str.substr(pos+1,str.npos);
	
	#ifdef __DEBUG_OUTPUT__
	std::cout << "This is a simple entry." << std::endl;
	#endif
	// Temporary
	//std::cout << "str1 : " << str1 << std::endl;
	//std::cout << "str2 : " << str2 << std::endl;
	
	// This is the "Simple" Entry part
	
	// Replace escaped newlines
	// with actual newlines
	unescape_chars2(str);
	
	// I guess that's it?
	return (TextEntry(str));
}




// Skip horizontal whitespace
// (but not newlines)
void skip_whitespace(std::istream & is) {
	int x = is.peek();
		while (x == ' ' || x == '\t') {
			is.get();
			x = is.peek();
		}
	// is.sync();
}


std::string get_string (std::istream & is) {
	std::string str;
	int x = is.peek();
	while (x != ' ' && x != '\t' && x != '\n') {
		str.push_back(x);
		is.get();
		x = is.peek();
	}
	//std::cout << "In get_string. str is " << str << std::endl;
	// is.sync();
	return str;
}

std::string get_string2(std::istream & is) {
	std::string str;
	int x = is.peek();
	while (x != ' ' && x != '\t' && x != '\n' && x != '=') {
		str.push_back(x);
		is.get();
		x = is.peek();
	}
	//std::cout << "In get_string. str is " << str << std::endl;
	// is.sync();
	return str;
}

// For getting a possibly quoted string
std::string get_string_quote (std::istream & is) {
	std::string str;
	int x = is.peek();
	if (x == '\"') {
		is.get();
		x = is.peek();
		while (x != '"') {
			str.push_back(x);
			is.get();
			x = is.peek();
		}
		is.get(); // to get the last quotation mark.
	}
	else {
		while (x != ' ' && x != '\t' && x != '\n') {
		str.push_back(x);
		is.get();
		x = is.peek();
		}	
	}
	//std::cout << "In get_string. str is " << str << std::endl;
	// is.sync();
	return str;
}

std::string get_line(std::istream & is) {
	std::string str;
	int x = is.peek();
	while (x != '\n' && !is.eof()) {
		str.push_back(x);
		is.get();
		x = is.peek();
	}
	if (x == '\n') is.get();
	// is.sync();
	return str;
}

void skip_line(std::istream & is) {
	//string str;
	int x = is.peek();
	while (x != '\n' && !is.eof()) {
		//str.push_back(x);
		is.get();
		x = is.peek();
	}
	if (x == '\n') is.get();
	// is.sync();
	//return str;
}

// Get a single character.
bool get_maybe(std::istream & is, char c) {
	char x = is.peek();
	if (x == c) {
		is.get();
		return true;
	}
	return false;
}

InputInfo make_info(std::istream &iss) {
	// skip_whitespace(iss);
	std::string str1; // = get_string(iss);
	//
	
	// std::cout << "In the \"make_info\" function." << std::endl;
	// std::cout << "str1: " << str1 << std::endl;
	
	int  ep   = -1;
	int  vr   = -1;
	bool sw   = false;
	bool full = false;
	std::string swo;
	
	int x = iss.peek();
	
	while (x != '\n' && x != EOF && iss) {
		skip_whitespace(iss);
		str1 = get_string2(iss);
		// int z;
		if (  (str1.compare("--episode") == 0) 
		   || (str1.compare("--ep")      == 0) 
		   || (str1.compare("ep")        == 0) 
		   || (str1.compare("episode")   == 0) 
		   || (str1.compare("e")         == 0)
		   ) {
			if (!(get_maybe(iss,'='))) skip_whitespace(iss);
			iss >> ep;
		} else if (  (str1.compare("--virtual") == 0)
			  || (str1.compare("--virt")    == 0)
			  || (str1.compare("virt")      == 0)
			  || (str1.compare("virtual")   == 0)
			  || (str1.compare("-v")        == 0)
			  || (str1.compare("v")         == 0)
			  ) {
				if (!(get_maybe(iss,'='))) skip_whitespace(iss);
				iss >> vr;
		} else if (  (str1.compare("--starwars") == 0)
			  || (str1.compare("--sw")       == 0)
			  || (str1.compare("starwars")   == 0)
			  || (str1.compare("sw")         == 0)
			  ) {
				sw = true;
		} else if (  (str1.compare("--starwarsout") == 0)
			  || (str1.compare("--swo")         == 0)
			  || (str1.compare("swo")           == 0)
			  || (str1.compare("starwarsout")   == 0)
			  ) {
				if (!(get_maybe(iss,'='))) skip_whitespace(iss);
				sw = true;
				swo = get_string_quote(iss);
		} else if (  (str1.compare("--full-file") == 0)
			  || (str1.compare("--full")      == 0)
			  || (str1.compare("full-file")   == 0)
			  || (str1.compare("full")        == 0)
			  ) {
			full = true;
		}
		
		skip_whitespace(iss);
		x = iss.peek();
		
		// std::cout << "peeked char is " << ((char) x) << " (" << ((int) x) << ")" << std::endl;
		// std::cout << std::endl;
	}
	if (swo.empty()) {
		return (InputInfo(ep,vr,sw,full));
	}
	return (InputInfo(ep,vr,swo,full));
	
}

InputInfo::InputInfo(int ep, int virt, std::string swo,bool full):ep_num(ep),virt_cutoff(virt),star_wars(true),swo(swo),full_file(full) {}

InputInfo::InputInfo(int ep, int virt, bool swars, bool full):ep_num(ep),virt_cutoff(virt),star_wars(swars),full_file(full ) {}
InputInfo::InputInfo(int ep, int virt                       ):ep_num(ep),virt_cutoff(virt),star_wars(false),full_file(false) {}
InputInfo::InputInfo(int ep                                 ):ep_num(ep),virt_cutoff( -1 ),star_wars(false),full_file(false) {}


// Note: unescaping backslashes must be
//       done in the absolute last phase;
//       otherwise stuff like "\\n" might
//       be interpreted as a newline.

// Use this to fix general escaped chars.
// In the first phase, after stripping
// comments.
void unescape_chars(std::string & str) {
	#ifdef __DEBUG_UNESCAPE__
	std::cout << "String before un-escaping:" << std::endl << str << std::endl << std::endl;
	#endif
	int flag = 0;
	char x   = '\0';
	for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
		x = *it;
		if (flag == 0 && x == '\\') {
			flag = 1;
		}
		else if (flag == 1) {
			if (flag == 1 && x == '\\') {
				// it = str.erase(it); //not yet
				flag = 0;
			}
			else if (x == 'h') {
				#ifdef __DEBUG_UNESCAPE__
				std::cout << "In the \"replacing h\" section of stuff." << std::endl;
				std::cout << "previous char was '" << *(it-1) << "', I guess..." << std::endl;
				#endif
				--it; // hmm...
				it = str.erase(it);      // erase backslash
				it = str.erase(it);      // erase char
				it = str.insert(it,'#'); // insert proper char
				flag = 0;
			}
			else if (x == 'q' || x == '\"') {
				--it; // hmm...
				it = str.erase(it);
				it = str.erase(it);
				it = str.insert(it,'\"');
				flag = 0;
			}
			else if (x == 'w') {
				--it; // hmm...
				it = str.erase(it);
				it = str.erase(it);
				it = str.insert(it,'~');
				flag = 0;
			}
			else {
				flag = 0;
			}
		}
	}

	#ifdef __DEBUG_UNESCAPE__
	std::cout << "String after un-escaping:" << std::endl << str << std::endl << std::endl;
	#endif
		
}

// Use this to fix general escaped chars.
void unescape_chars2(std::string & str) {
	#ifdef __DEBUG_UNESCAPE__
	std::cout << "String before un-escaping:" << std::endl << str << std::endl << std::endl;
	#endif
	int flag = 0;
	char x   = '\0';
	for (std::string::iterator it = str.begin(); it != str.end(); ++it) {
		x = *it;
		if (flag == 0 && x == '\\') {
			flag = 1;
		}
		else if (flag == 1) {
			if (x == '\\') { 
				it = str.erase(it);
				flag = 0;
			}
			else if (x == 'n') {
				--it; // hmm...
				it = str.erase(it);
				it = str.erase(it);
				it = str.insert(it,'\n');
				flag = 0;
			}
			else if (x == '|') {
				--it; // hmm...
				it = str.erase(it);
				//it = str.erase(it);
				//it = str.insert(it,'|');
				flag = 0;
			}
			else if (x == 'p') { // Since it works differently from the above.
				--it;
				it = str.erase(it);
				it = str.erase(it);
				it = str.insert(it,'|');
				flag = 0;
			}
			else if (x == 't') {
				--it; // hmm...
				it = str.erase(it);
				it = str.erase(it);
				it = str.insert(it,'\t');
				flag = 0;
			}
			else if (x == 's') {
				--it; // hmm...
				it = str.erase(it);
				it = str.erase(it);
				it = str.insert(it,' ');
				flag = 0;
			}
			else {
				flag = 0;
			}
		}
	}

	#ifdef __DEBUG_UNESCAPE__
	std::cout << "String after un-escaping:" << std::endl << str << std::endl << std::endl;
	#endif
		
}




