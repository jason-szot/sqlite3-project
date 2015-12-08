#include "cd.h"
#include "artist.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
/*
int cd_id;
char *title;
int artist_id;
char *catalogue;
*/

void cd::report(){	// report to file
	std::ofstream output;
	output.open("cd.p2.report.txt", std::ios::app);	// open output file in append mode
	output << "cd_id = " << cd_id << "\n";
	output << "title = " << title << "\n";
	output << "artist_id = " << artist_id << "\n";
	output << "catalogue = " << catalogue << "\n";
	output << "-------------------------------\n";
	output.close();
}


artist *cd::getArtist(int artistID, std::map<int, artist> *foo){
	std::map<int,artist>::iterator iter = (*foo).find(artistID);
	if (iter != (*foo).end()){	// found
		return &(*iter).second;
	}
	else	// not found
	{
		std::ofstream error;
		error.open("exceptions.txt", std::ios::app);
		error << "exception thrown:\n" << "insert cd - artist not found\n";
		error << "cd_id = " << cd_id << "\n";
		error << "title = " << title << "\n";
		error << "artist_id = " << artist_id << "\n";
		error << "catalogue = " << catalogue << "\n\n\n";
		error.close();
		return NULL;
	}
}

void cd::print(){
	printf("cd_id = %d\n", cd_id);
	printf("title = %s\n", title);
	printf("artist_id = %d\n", artist_id);
	printf("catalogue = %s\n", catalogue);
	printf("________________________\n");
}

//// constructor ////
cd::cd(int cd, char *ttl, int artist, char *cat){
	cd_id = cd;
	std::string temp(ttl);
	char *foo = new char[temp.length() + 1];
	for (int i = 0; i < temp.length(); i++){
		foo[i] = temp.c_str()[i];
		foo[i + 1] = '\0';
	}
	title = foo;
	artist_id = artist;
	std::string ctemp(cat);
	char *cfoo = new char[ctemp.length() + 1];
	for (int i = 0; i < ctemp.length(); i++){
		cfoo[i] = ctemp.c_str()[i];
		cfoo[i + 1] = '\0';
	}
	catalogue = cfoo;
	this->report();
}