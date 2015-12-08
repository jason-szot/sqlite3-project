/*
	create table cd(
	id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,
	title VARCHAR(70) NOT NULL,
	artist_id INTEGER NOT NULL,
	catalogue VARCHAR(30) NOT NULL
	);
*/

#ifndef CD_H
#define CD_H

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include "artist.h"
#include <map>


class cd{
public:
	int cd_id;
	char *title;
	int artist_id;
	char *catalogue;

	// report function
	void report(void);
	artist *getArtist(int artistID, std::map<int,artist> *foo);
	void print();


	// constructor
	cd(int cd, char *ttl, int artist, char *cat);
};


#endif