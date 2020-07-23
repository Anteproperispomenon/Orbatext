
#ifndef __OPTIMISE_H__
#define __OPTIMISE_H__

#include <string>
#include <vector>

#include "text-box.h"
#include "text-entry.h"


int get_box(int min_length, std::vector<TextBox> & entries);

// TextBox & get_box_alt(int min_length, std::vector<TextBox> & entries);

int get_longest_entry(std::vector<TextEntry> &vec);

bool optimise_entries(std::vector<TextBox> &boxes, std::vector<TextEntry> &entries);



#endif

