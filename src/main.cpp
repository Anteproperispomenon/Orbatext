// main.cpp


/* * * * * * * * * * * * * * * * * * * * * * * * * *
 *                                                 *
 *  Orbatext:                                      *
 *  This is a program to ease patching of          *
 *  level names and level entry text in            * 
 *  Commander Keen 4-6.                            *
 *                                                 *
 *  It works by taking an input text file          *
 *  with all the levels' names and entry texts,    *
 *  and fits them into the executible segments     *
 *  that are reserved for the level entries        *
 *  and names.                                     *
 *                                                 *
 *  See FORMAT.txt for info on formatting the      *
 *  input files.                                   *
 *                                                 *
 * * * * * * * * * * * * * * * * * * * * * * * * * *
 */


#include <vector>
#include <string>

#include <iostream>
#include <fstream>

#include "text-box.h"
#include "text-entry.h"
#include "optimise.h"
#include "reader.h"
#include "writer.h"

using namespace std;

int main(int argc, char *argv[]) {
	
	string ifn; // input  filename
	string ofn; // output filename
	
	// Help Stuff
	if (argc <= 1) {
		cout << "Usage: " << endl;
		cout << argv[0] << " [--input] <INFILE> [[--output] <OUTFILE>]" << endl << endl;
		cout << "INFILE is the name of the input file (see FORMAT.txt for"  << endl;
		cout << "formatting information), and OUTFILE is the name of the"   << endl;
		cout << "output file. If no output file name is provided, the"      << endl;
		cout << "program will default to \"<INFILE>.pat\"."                 << endl;
		return (0);
	}
	
	// Argument handling
	for (int i = 1 ; i < argc ; i++) {
		string arg = argv[i];
		if (arg.compare("--input") == 0 || arg.compare("-i") == 0) {
			// Return if no argument passed
			if ((i+1) >= argc) {
				cerr << "No argument given for input." << endl;
				return (1);
			}
			i++;
			ifn = argv[i];
		}
		else if (arg.compare("--output") == 0 || arg.compare("-o") == 0) {
			// return if no argument passed.
			if ((i+1) >= argc) {
				cerr << "No argument given for output." << endl;
				return (1);
			}
			i++;
			ofn = argv[i];
		}
		else if (i == 1 && ifn.empty()) {
			ifn = argv[i];
		}
		else if (i == 2 && ofn.empty()) {
			ofn = argv[i];
		}
		else if (i == 3 && ofn.empty() && !(ifn.empty())) {
			ofn = argv[i];
		}
		else {
			cerr << "Could not understand " << i << "-th argument; \"" << argv[i] << "\"" << endl; 
			return (1);
		}
	}
	
	if (ifn.empty()) {
		cerr << "No input file specified." << endl;
		return (1);
	}
	
	if (ofn.empty()) {
		ofn = ifn + ".pat";
	}
	
	ifstream ifs(ifn.c_str());
	
	if (ifs.fail()) {
		cerr << "Could not open/find file \"" << ifn << "\"" << endl;
		ifs.close();
		return (1);
	}
	
	if (ifs.eof()) {
		cerr << "File opened (\"" << ifn << "\") is empty." << endl;
		ifs.close();
		return (1);
	}
	
	
	//Keen 4
	
	// (1[0-9A-F]{4})H 1[0-9A-F]{4}H ([0-9A-F]{5})H
	// ([0-9A-F]{5})H [0-9A-F]{5}H ([0-9A-F]{5})H
	
	// boxes.push_back\( TextBox\( 0x\2 , 0x\1 \) \); //
	
	InputInfo info = make_info(ifs);
	
	vector<TextBox>   boxes;
	vector<TextEntry> entries;
	
	if (info.ep_num == 4) {
		if (info.star_wars) {
			boxes.reserve(39);
		} else {
			boxes.reserve(38);
		}
		boxes.push_back( TextBox( 0x00010 , 0x1F040 ) ); // seg026             Level 0 name
		boxes.push_back( TextBox( 0x00010 , 0x1F050 ) ); // seg027             Level 1 name
		boxes.push_back( TextBox( 0x00010 , 0x1F060 ) ); // seg028             Level 2 name
		boxes.push_back( TextBox( 0x00020 , 0x1F070 ) ); // seg029             Level 3 name
		boxes.push_back( TextBox( 0x00020 , 0x1F090 ) ); // seg030             Level 4 name
		boxes.push_back( TextBox( 0x00010 , 0x1F0B0 ) ); // seg031             Level 5 name
		boxes.push_back( TextBox( 0x00010 , 0x1F0C0 ) ); // seg032             Level 6 name
		boxes.push_back( TextBox( 0x00010 , 0x1F0D0 ) ); // seg033             Level 7 name
		boxes.push_back( TextBox( 0x00010 , 0x1F0E0 ) ); // seg034             Level 8 name
		boxes.push_back( TextBox( 0x00010 , 0x1F0F0 ) ); // seg035             Level 9 name
		boxes.push_back( TextBox( 0x00010 , 0x1F100 ) ); // seg036             Level 10 name
		boxes.push_back( TextBox( 0x00020 , 0x1F110 ) ); // seg037             Level 11 name
		boxes.push_back( TextBox( 0x00020 , 0x1F130 ) ); // seg038             Level 12 name
		boxes.push_back( TextBox( 0x00030 , 0x1F150 ) ); // seg039             Level 13 name
		boxes.push_back( TextBox( 0x00020 , 0x1F180 ) ); // seg040             Level 14 name
		boxes.push_back( TextBox( 0x00010 , 0x1F1A0 ) ); // seg041             Level 15 name
		boxes.push_back( TextBox( 0x00010 , 0x1F1B0 ) ); // seg042             Level 16 name
		boxes.push_back( TextBox( 0x00010 , 0x1F1C0 ) ); // seg043             Level 17 name
		boxes.push_back( TextBox( 0x00020 , 0x1F1D0 ) ); // seg044             Level 18 name
		boxes.push_back( TextBox( 0x00020 , 0x1F1F0 ) ); // seg045             Level 0 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F210 ) ); // seg046             Level 1 entry text
		boxes.push_back( TextBox( 0x00020 , 0x1F240 ) ); // seg047             Level 2 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F260 ) ); // seg048             Level 3 entry text
		boxes.push_back( TextBox( 0x00040 , 0x1F290 ) ); // seg049             Level 4 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F2D0 ) ); // seg050             Level 5 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F300 ) ); // seg051             Level 6 entry text
		boxes.push_back( TextBox( 0x00020 , 0x1F330 ) ); // seg052             Level 7 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F350 ) ); // seg053             Level 8 entry text
		boxes.push_back( TextBox( 0x00020 , 0x1F380 ) ); // seg054             Level 9 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F3A0 ) ); // seg055             Level 10 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F3D0 ) ); // seg056             Level 11 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F400 ) ); // seg057             Level 12 entry text
		boxes.push_back( TextBox( 0x00040 , 0x1F430 ) ); // seg058             Level 13 entry text
		boxes.push_back( TextBox( 0x00040 , 0x1F470 ) ); // seg059             Level 14 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F4B0 ) ); // seg060             Level 15 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F4E0 ) ); // seg061             Level 16 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F510 ) ); // seg062             Level 17 entry text
		boxes.push_back( TextBox( 0x00040 , 0x1F540 ) ); // seg063             Level 18 entry text
		if (info.star_wars) {
			// asfzxcv
			boxes.push_back( TextBox( 0x00200 , 0x1EE40 , true ) ); // star wars story
		}
	}
	else if (info.ep_num == 5) {
		//Level names:
		if (info.star_wars) {
			boxes.reserve(33);
		} else {
			boxes.reserve(32);
		}
		
		// boxes.push_back\( TextBox\( 0x , 0x\1 \) \); //
		
		boxes.push_back( TextBox( 0x00010 , 0x20040 ) ); // "Omegamatic" $00
		boxes.push_back( TextBox( 0x00020 , 0x20050 ) ); // "Ion Ventilation System" $00
		boxes.push_back( TextBox( 0x00010 , 0x20070 ) ); // "Security Center" $00
		boxes.push_back( TextBox( 0x00020 , 0x20080 ) ); // "Defense Tunnel Vlook" $00
		boxes.push_back( TextBox( 0x00020 , 0x200A0 ) ); // "Energy Flow Systems" $00
		boxes.push_back( TextBox( 0x00020 , 0x200C0 ) ); // "Defense Tunnel Burrh" $00
		boxes.push_back( TextBox( 0x00020 , 0x200E0 ) ); // "Regulation" $0A "Control Center" $00
		boxes.push_back( TextBox( 0x00020 , 0x20100 ) ); // "Defense Tunnel Sorra" $00
		boxes.push_back( TextBox( 0x00020 , 0x20120 ) ); // "Neutrino" $0A "Burst Injector" $00
		boxes.push_back( TextBox( 0x00020 , 0x20140 ) ); // "Defense Tunnel Teln" $00
		boxes.push_back( TextBox( 0x00020 , 0x20160 ) ); // "Brownian" $0A "Motion Inducer" $00
		boxes.push_back( TextBox( 0x00020 , 0x20180 ) ); // "Gravitational" $0A "Damping Hub" $00
		boxes.push_back( TextBox( 0x00020 , 0x201A0 ) ); // "Quantum" $0A "Explosion Dynamo" $00
		boxes.push_back( TextBox( 0x00010 , 0x201C0 ) ); // "Korath III Base" $00
		boxes.push_back( TextBox( 0x00010 , 0x201D0 ) ); // "BWBMegarocket" $00
		boxes.push_back( TextBox( 0x00010 , 0x201E0 ) ); // "High Scores" $00

		//Keen entering levels
		boxes.push_back( TextBox( 0x00030 , 0x201F0 ) ); // "Keen purposefully" $0A "wanders about the" $0A "Omegamatic" $00
		boxes.push_back( TextBox( 0x00030 , 0x20220 ) ); // "Keen investigates the" $0A "Ion Ventilation System" $00
		boxes.push_back( TextBox( 0x00030 , 0x20250 ) ); // "Keen struts through" $0A "the Security Center" $00
		boxes.push_back( TextBox( 0x00030 , 0x20280 ) ); // "Keen invades" $0A "Defense Tunnel Vlook" $00
		boxes.push_back( TextBox( 0x00030 , 0x202B0 ) ); // "Keen engages" $0A "Energy Flow Systems" $00
		boxes.push_back( TextBox( 0x00030 , 0x202E0 ) ); // "Keen barrels into" $0A "Defense Tunnel Burrh" $00
		boxes.push_back( TextBox( 0x00030 , 0x20310 ) ); // "Keen goes nuts in" $0A "the Regulation" $0A "Control Center" $00
		boxes.push_back( TextBox( 0x00030 , 0x20340 ) ); // "Keen regrets entering" $0A "Defense Tunnel Sorra" $00
		boxes.push_back( TextBox( 0x00030 , 0x20370 ) ); // "Keen blows through" $0A "the Neutrino" $0A "Burst Injector" $00
		boxes.push_back( TextBox( 0x00030 , 0x203A0 ) ); // "Keen trots through" $0A "Defense Tunnel Teln" $00
		boxes.push_back( TextBox( 0x00030 , 0x203D0 ) ); // "Keen breaks into" $0A "the Brownian" $0A "Motion Inducer" $00
		boxes.push_back( TextBox( 0x00040 , 0x20400 ) ); // "Keen hurries through" $0A "the Gravitational" $0A "Damping Hub" $00
		boxes.push_back( TextBox( 0x00030 , 0x20440 ) ); // "Keen explodes into" $0A "the Quantum" $0A "Explosion Dynamo" $00
		boxes.push_back( TextBox( 0x00030 , 0x20470 ) ); // "Keen faces danger" $0A "in the secret" $0A "Korath III Base" $00
		boxes.push_back( TextBox( 0x00030 , 0x204A0 ) ); // "Keen will not be" $0A "in the BWBMegarocket" $00
		boxes.push_back( TextBox( 0x00040 , 0x204D0 ) ); // "Keen unexplainedly" $0A "finds himself by" $0A "theHigh Scores" $00
		if (info.star_wars) {
			// asfzxcv
			boxes.push_back( TextBox( 0x00260 , 0x1FDE0 , true ) ); // star wars story
		}
	}
	else if (info.ep_num == 6) {
		if (info.star_wars) {
			boxes.reserve(39);
		} else {
			boxes.reserve(38);
		}
		boxes.push_back( TextBox( 0x00010 , 0x1EF40 ) ); // seg026             Level 0 name
		boxes.push_back( TextBox( 0x00020 , 0x1EF50 ) ); // seg027             Level 1 name
		boxes.push_back( TextBox( 0x00010 , 0x1EF70 ) ); // seg028             Level 2 name
		boxes.push_back( TextBox( 0x00020 , 0x1EF80 ) ); // seg029             Level 3 name
		boxes.push_back( TextBox( 0x00020 , 0x1EFA0 ) ); // seg030             Level 4 name
		boxes.push_back( TextBox( 0x00010 , 0x1EFC0 ) ); // seg031             Level 5 name
		boxes.push_back( TextBox( 0x00020 , 0x1EFD0 ) ); // seg032             Level 6 name
		boxes.push_back( TextBox( 0x00010 , 0x1EFF0 ) ); // seg033             Level 7 name
		boxes.push_back( TextBox( 0x00020 , 0x1F000 ) ); // seg034             Level 8 name
		boxes.push_back( TextBox( 0x00010 , 0x1F020 ) ); // seg035             Level 9 name
		boxes.push_back( TextBox( 0x00010 , 0x1F030 ) ); // seg036             Level 10 name
		boxes.push_back( TextBox( 0x00010 , 0x1F040 ) ); // seg037             Level 11 name
		boxes.push_back( TextBox( 0x00020 , 0x1F050 ) ); // seg038             Level 12 name
		boxes.push_back( TextBox( 0x00020 , 0x1F070 ) ); // seg039             Level 13 name
		boxes.push_back( TextBox( 0x00020 , 0x1F090 ) ); // seg040             Level 14 name
		boxes.push_back( TextBox( 0x00020 , 0x1F0B0 ) ); // seg041             Level 15 name
		boxes.push_back( TextBox( 0x00010 , 0x1F0D0 ) ); // seg042             Level 16 name
		boxes.push_back( TextBox( 0x00020 , 0x1F0E0 ) ); // seg043             Level 17 name
		boxes.push_back( TextBox( 0x00010 , 0x1F100 ) ); // seg044             Level 18 name
		boxes.push_back( TextBox( 0x00020 , 0x1F110 ) ); // seg045             Level 0 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F130 ) ); // seg046             Level 1 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F160 ) ); // seg047             Level 2 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F190 ) ); // seg048             Level 3 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F1C0 ) ); // seg049             Level 4 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F1F0 ) ); // seg050             Level 5 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F220 ) ); // seg051             Level 6 entry text
		boxes.push_back( TextBox( 0x00020 , 0x1F250 ) ); // seg052             Level 7 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F270 ) ); // seg053             Level 8 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F2A0 ) ); // seg054             Level 9 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F2D0 ) ); // seg055             Level 10 entry text
		boxes.push_back( TextBox( 0x00050 , 0x1F300 ) ); // seg056             Level 11 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F350 ) ); // seg057             Level 12 entry text
		boxes.push_back( TextBox( 0x00040 , 0x1F380 ) ); // seg058             Level 13 entry text
		boxes.push_back( TextBox( 0x00040 , 0x1F3C0 ) ); // seg059             Level 14 entry text
		boxes.push_back( TextBox( 0x00040 , 0x1F400 ) ); // seg060             Level 15 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F440 ) ); // seg061             Level 16 entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F470 ) ); // seg062             BWB entry text
		boxes.push_back( TextBox( 0x00030 , 0x1F4A0 ) ); // seg063             High scores entry text
		if (info.star_wars) {
			// asfzxcv
			boxes.push_back( TextBox( 0x001F0 , 0x1ED50 , true ) ); // star wars story
		}
	}
	else {
		cerr << "Error: episode " << info.ep_num << " is invalid/not (yet) supported." << endl;
		ifs.close();
		return (1);
	}
	
	skip_whitespace(ifs);
	char x = ifs.peek();
	if (x == '\n') ifs.get();
	while (ifs) { // (x != EOF) {
		skip_whitespace(ifs);
		x = ifs.peek();
		if (x == '#' || x == '\n') {
			skip_line(ifs);
			//cout << "About to continue." << endl;
			continue;
		}
		if (!ifs) break; // this seems to make it work...
		string str1 = get_line(ifs);
		entries.push_back(create_entry(str1));
	}
	
	ifs.close();
	
	// Convert negative-format references to 
	// len+x format references.
	// int len = entries.size();
	// for ( vector<TextEntry>::iterator it = entries.begin() ; it != entries.end() ; ++it ) {
	// 	it->fix_refs(len);
	// }
	
	// Actually doing stuff...
	try {
		// Convert negative-format references to 
		// len+x format references.
		int len = entries.size();
		for ( vector<TextEntry>::iterator it = entries.begin() ; it != entries.end() ; ++it ) {
			it->fix_refs(len);
		}
		cycle_fix(entries);
		optimise_entries(boxes, entries);
	} catch (exception& e) {
		cerr << "Error:" << endl;
		cerr << e.what() << endl;
		entries.clear();
		boxes.clear();
		return (1);
	}
	
	
	// Set the maximum, which is
	// < length if using the virtual
	// variable
	int mx = entries.size();
	if (info.virt_cutoff != -1) mx = info.virt_cutoff;
	
	// optimise_entries(boxes, entries);
	
	//ofstream ofs("output.txt");
	ofstream ofs;
	//ofs.open("output.txt");
	ofs.open(ofn.c_str());
	
	if (info.full_file) {
		ofs << "%ext ck" << info.ep_num << endl << endl;
		write_warp(ofs, info.ep_num, (mx-1));
		ofs << endl;
	}
	
	// If using a separate file for star wars output.
	
	ofs << "# Level Entry/Name Text" << endl;
	if (info.star_wars && !(info.swo.empty())) {
		for (vector<TextBox>::iterator it = boxes.begin() ; it != boxes.end() ; ++it) {
			if (it->is_sw()) {
				ofstream swf(info.swo.c_str(), (ofstream::out | ofstream::binary));
				if (swf.fail()) {
					swf.close();
					cerr << "There was an issue when trying to output the" << endl;
					cerr << "Star Wars text to file: " << endl;
					cerr << "\"" << info.swo << "\"" << endl;
					cerr << "Will instead fall back to writing the text to" << endl;
					cerr << "the patch file instead." << endl;
					ofs << it->render_patch() << endl;
				} else {
					it->render_patch_file(swf);
					swf.close();
					ofs << it->render_segfile() << " \"" << info.swo << "\"" << endl;
				}	
			} else {
				ofs << it->render_patch() << endl;
			}
		}
	}
	else {
		for (vector<TextBox>::iterator it = boxes.begin() ; it != boxes.end() ; ++it) {
			ofs << it->render_patch() << endl;
		}
	}
	
	ofs << endl;
	
	
	// Level entry text
	int i = 0;
	#ifdef __DEBUG_OUTPUT__
	cout << "Level entry values:" << endl;
	#endif
	ofs << "# Level Entry Texts" << endl;
	for (vector<TextEntry>::iterator it = entries.begin() ; (it != entries.end() && i < mx) ; ++it) {
		#ifdef __DEBUG_OUTPUT__
		string str = it->render_patch1();
		#endif
		if (it != entries.begin()) ofs << "              ";
		else if (info.ep_num == 4) ofs << "%patch $307C8 ";
		else if (info.ep_num == 5) ofs << "%patch $31B18 ";
		else if (info.ep_num == 6) ofs << "%patch $32516 ";
		#ifndef __DEBUG_OUTPUT__
		ofs << it->render_patch1() << "  # level entry " << i  << endl;
		#else
		cout << str << endl;
		ofs  << str << "  # level entry " << i << endl;
		#endif
		i++;
		// if (i >= mx) break;
	}
	
	ofs << endl;
	
	// Level Names
	i = 0;
	#ifdef __DEBUG_OUTPUT__
	cout << "Level name values:" << endl;
	#endif
	ofs << "# Level Names" << endl;
	for (vector<TextEntry>::iterator it = entries.begin() ; (it != entries.end() && i < mx) ; ++it) {
		#ifdef __DEBUG_OUTPUT__
		string str = it->render_patch2();
		#endif
		if (it != entries.begin()) ofs << "              ";
		else if (info.ep_num == 4) ofs << "%patch $30764 ";
		else if (info.ep_num == 5) ofs << "%patch $31AB4 ";
		else if (info.ep_num == 6) ofs << "%patch $324B2 ";
		#ifndef __DEBUG_OUTPUT__
		ofs << it->render_patch2() << "  # level name " << i << endl;
		#else
		cout << str << endl;
		ofs  << str << "  # level entry " << i << endl;
		#endif
		i++;
		// if (i >= mx) break;
	}
	
	if (info.full_file) {
		ofs << endl << "%end" << endl;
	}
	
	ofs.close();
	entries.clear();
	boxes.clear();
	
	
	return(0);
	
}


