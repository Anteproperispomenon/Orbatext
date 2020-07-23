//

#include "writer.h"

#include <ostream>
#include <iomanip>

std::ostream & write_warp(std::ostream & os, int ep_num, int levels){
	
	int levp, txtp;
	
	if      (ep_num == 4) { levp = 0x77E4; txtp = 0x2F418; }
	else if (ep_num == 5) { levp = 0x7799; txtp = 0x30864; }
	else if (ep_num == 6) { levp = 0x75B9; txtp = 0x31250; }
	
	
	os << "#Levels F10 + W accesses" << std::endl;
	os << std::setfill('0') << std::setbase(16) << std::setw(2);
	os << "%patch $" << levp << " $" << levels << std::endl;
	os << std::endl;
	os << "# Warp Text Patch" << std::endl;
	os << "%patch $" << txtp << std::setbase(10);
	os << " \" Warp to which level(1-" << levels << "):\" $00" << std::endl;
	
	return os;
}


