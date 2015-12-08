/*
create table track(
cd_id INTEGER NOT NULL,
track_id INTEGER NOT NULL,
title VARCHAR(70),
PRIMARY KEY(cd_id, track_id)
);
*/
#ifndef TRACK_H
#define TRACK_H

#include <string>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <map>
#include "cd.h"



class track{
public:
	int cd_id;
	int track_id;
	char *title;

	// report function
	void report(void);
	cd *getCD(int cdID, std::map<int,cd> *foo);
	void print();

	// constructor
	track(char *cd, char *track, char *ttl);
};



#endif