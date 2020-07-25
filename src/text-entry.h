// Text Entry

#ifndef __TEXT_ENTRY_H__
#define __TEXT_ENTRY_H__

#include <string>
#include <vector>

#ifdef USE_IO
#include <ostream>
#endif


enum /*class*/ EntryType {SimpleE, ShortE, FullE, RefE}; // asdfzxcv

class TextBox;

class TextEntry
{
	//private :
	
		/*const*/ EntryType ent;
	
		/*const*/ std::string str1;
		/*const*/ std::string str2;
	
		/*const*/ int offs; // Offset if ent == ShortE
		
		int ref1; // if ref1 < length, refers to entry text.
		int ref2; // if     >= length, refers to level text.
		
		bool done1;
		bool done2;
	
		unsigned long seg1;
		unsigned long seg2;
		
		int ofs1;
		int ofs2;
	
	public  :
	
		TextEntry(std::string str1);
		TextEntry(std::string str1, int x);
		TextEntry(std::string str1, std::string str2);
		
		//TextEntry(int ref1);
		TextEntry(int ref1, int ref2);
		
		~TextEntry();
		
		int get_longest() const;
		
		bool add_to_box (TextBox &box);
		
		bool all_done() const;
		
		EntryType get_type() const;
		
		unsigned long get_seg1() const;
		unsigned long get_seg2() const;
		
		int get_ofs1() const;
		int get_ofs2() const;
		
		int get_ref1() const;
		int get_ref2() const;
		
		bool assign_ref(int len, TextEntry & tnt1, TextEntry & tnt2);
		
		std::string get_long_string() const;
		
		std::string render_patch1() const; //hmm...
		std::string render_patch2() const; //hmm...
		
		#ifdef USE_IO
		std::ostream & render_patch1(std::ostream & os) const; //hmm...
		std::ostream & render_patch2(std::ostream & os) const; //hmm...
		#endif
		
		friend bool cycle_fix(std::vector<TextEntry> &entries);
		
		void fix_refs(int len); 
		
};

#endif

