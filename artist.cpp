#include "artist.h"
#include <string>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <fstream>
/*
int artist_id;
char *name;
*/


void artist::report(){	// report to file
	std::ofstream output;
	output.open("artist.p2.report.txt", std::ios::app);	// open output file in append mode
	output << "id = " << artist_id << "\n";
	output << "name = " << name << "\n";
	output << "-------------------------------\n";
	output.close();

}

void artist::print(){
	printf("artist_id = %d\n", artist_id);
	printf("name = %s\n", name);
	printf("________________________\n");
}
	//// constructor ////
artist::artist(char *id, char *nam){
	artist_id = atoi(id);
	std::string temp(nam);
	char *foo = new char[temp.length() + 1];
	for (int i = 0; i < temp.length(); i++){
		foo[i] = temp.c_str()[i];
		foo[i + 1] = '\0';
	}
	name = foo;
	this->report();
}