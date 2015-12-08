#include "track.h"
#include "artist.h"
#include "cd.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
/*
int cd_id;
int track_id;
char *title;
*/


cd *track::getCD(int cdID, std::map<int, cd> *foo){
	std::map<int, cd>::iterator iter = (*foo).find(cdID);
	if (iter != (*foo).end()){	// found
		return &(*iter).second;
	}
	else	// not found
	{
		std::ofstream error;
		error.open("exceptions.txt", std::ios::app);
		error << "exception thrown:\n" << "insert track - cd not found\n";
		error << "cd_id = " << cd_id << "\n";
		error << "track_id = " << track_id << "\n";
		error << "title = " << title << "\n\n\n";
		error.close();
		return NULL;
	}
}

void track::report(){	// report to file
	std::ofstream output;
	output.open("track.p2.report.txt", std::ios::app);	// open output file in append mode
	output << "cd_id = " << cd_id << "\n";
	output << "track_id = " << track_id << "\n";
	output << "title = " << title << "\n";
	output << "-------------------------------\n";
	output.close();
}

void track::print(){
	printf("cd_id = %d\n", cd_id);
	printf("track_id = %d\n", track_id);
	printf("title = %s\n", title);
	printf("________________________\n");
}

	//// constructor ////
track::track(char *cd, char *track, char *ttl){
	cd_id = atoi(cd);
	track_id = atoi(track);
	std::string temp(ttl);
	char *foo = new char[temp.length() + 1];
	for (int i = 0; i < temp.length(); i++){
		foo[i] = temp.c_str()[i];
		foo[i + 1] = '\0';
	}
	title = foo;
	this->report();
}