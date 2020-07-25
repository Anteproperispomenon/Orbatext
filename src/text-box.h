// 

#ifndef __TEXT_BOX_H__
#define __TEXT_BOX_H__

#include <string>

#include <ostream>

class TextBox
{
	//ivate :	
		// char text[];
		// char * text ;
		std::string text2;
		/*const*/ unsigned long segment;
		int avail;
		/*const*/ int size;
		/*const*/ bool sw;

	public  :
		TextBox(int sz, unsigned long seg);
		TextBox(int sz, unsigned long seg, bool sw);
		~TextBox();
		int           get_remaining() const;
		int           get_pointer()   const;
		unsigned long get_segment()   const;
		
		bool is_sw() const;
		
		int add_text(const std::string &str); // returns the added pointer to the segment
		
		std::string render_patch()   const; //hmm...
		std::string render_segfile() const;
		
		std::ostream & render_patch_file(std::ostream & os) const;

};

#endif

