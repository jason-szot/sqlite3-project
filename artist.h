/*
	create table artist(
	id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,
	name VARCHAR(100) NOT NULL
	);
*/

#ifndef ARTIST_H
#define ARTIST_H

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <map>



class artist{
public:
	int artist_id;
	char *name;

	// report function
	void report(void);
	void print();

	// constructor
	artist(char *id, char *nam);
};



#endif