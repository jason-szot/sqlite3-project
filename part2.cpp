#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sqlite3.h>
#include <iostream>
#include <map>
#include "artist.h"
#include "cd.h"
#include "track.h"


static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}



typedef std::pair<int, int> pair_key;
std::map<pair_key, track> track_map;
std::map<int, cd> cd_map;
std::map<int, artist> artist_map;

	// function prototypes
void populate_sql(void);
void update_records(void);
void read_cd(void);
void read_artist(void);
void read_track(void);
void list_artist();
void list_cd();
void list_track();
int menu();
void B4_insert();

	/*	global maps:
		artist_map
		cd_map
		track_map
	*/



int main(){
	int choice;
	// read from database create objects, insert into map if integrity test passed ( internal )
	
	read_artist();
	read_cd();
	read_track();
	B4_insert();

	while (1){
		choice = menu();
		/*
			1. List Artists
			2. List CD's.
			3. List Tracks.
			4. Quit.
		*/

		if (choice == 1){
			list_artist();
		}
		else if (choice == 2){
			list_cd();
		}
		else if (choice == 3){
			list_track();
		}
		else if (choice == 4){
			return 0;
		}

	}

	return 0;
}

void read_cd(){
	// open database
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql = "select * from cd";
	char *ttl, *cat;
	int id, a_id;

	rc = sqlite3_open("song.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	/*
		cd table format:
		id | title | artist_id | catalogue
		1	2		3			4
	*/

	// prepare sql statement, create objects, insert into map

	sqlite3_stmt *statement;

	if (sqlite3_prepare(db, sql, -1, &statement, 0) == SQLITE_OK)
	{
		int ctotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				// read info into data sections
					id = atoi((char*)sqlite3_column_text(statement, 0));
					ttl = (char*)sqlite3_column_text(statement, 1);
					a_id = atoi((char*)sqlite3_column_text(statement, 2));
					cat = (char*)sqlite3_column_text(statement, 3);

				// create cd object
					cd temp(id, ttl, a_id, cat);

				// insert into map if passes integrity check
					if (temp.getArtist(a_id, &artist_map) != NULL){
						cd_map.insert(std::pair<int, cd>(id, temp));
					}



			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				printf("done with cd table\n");
				break;
			}
		}	// end of while
	}	// end of if prepare
}

void read_artist(){
	// open database
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql = "select * from artist";
	char *id, *name;

	rc = sqlite3_open("song.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	/*
	artist table format:
	id | name
	0	1
	*/

	// prepare sql statement, create objects, insert into map

	sqlite3_stmt *statement;

	if (sqlite3_prepare(db, sql, -1, &statement, 0) == SQLITE_OK)
	{
		int ctotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				// read info into data sections
				id = (char*)sqlite3_column_text(statement, 0);
				name = (char*)sqlite3_column_text(statement, 1);

				// create cd object
				artist temp(id, name);

				// insert into map
				artist_map.insert(std::pair<int, artist>(atoi(id), temp));



			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				printf("done with artist table\n");
				break;
			}
		}	// end of while
	}	// end of if prepare
}

void read_track(){
	// open database
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql = "select * from track";

	rc = sqlite3_open("song.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	/*
	cd table format:
	cd_id | track_id | title
	0		1			2
	*/

	// prepare sql statement, create objects, insert into map

	sqlite3_stmt *statement;

	if (sqlite3_prepare(db, sql, -1, &statement, 0) == SQLITE_OK)
	{
		int ctotal = sqlite3_column_count(statement);
		int res = 0;
		char *cd_id, *track_id, *title;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				
				// read info into data sections
				cd_id = (char*)sqlite3_column_text(statement, 0);
				track_id = (char*)sqlite3_column_text(statement, 1);
				title = (char*)sqlite3_column_text(statement, 2);

				// create cd object
				track temp(cd_id, track_id, title);

				// insert into map if it passes integrity test
				if (temp.getCD(atoi(cd_id), &cd_map) != NULL){
					std::pair<int, int> key(atoi(cd_id), atoi(track_id));
					track_map.insert(std::pair<std::pair<int, int>, track>(key, temp));
				}



			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				printf("done with track table\n");
				break;
			}
		}	// end of while
	}	// end of if prepare
}

int menu(){
	int choice;
	std::string input;
	do{
	printf("Menu:\n");
	printf("1. List Artists.\n");
	printf("2. List CD\'s. \n");
	printf("3. List Tracks.\n");
	printf("4. Quit.\n");
	printf("Please enter your selection\n");
	getline(std::cin,input);
	choice = atoi(input.c_str());
	} while (choice != 1 && choice != 2 && choice != 3 && choice != 4);
	return choice;
}

void list_artist(){
	// iterate through the map
	for (std::map<int, artist>::iterator iter = artist_map.begin(); iter != artist_map.end(); iter++){
		iter->second.print();
	}
}

void list_cd(){
	// iterate through the map
	for (std::map<int, cd>::iterator iter = cd_map.begin(); iter != cd_map.end(); iter++){
		iter->second.print();
	}
}

void list_track(){
	// iterate through the map
	for (std::map<std::pair<int, int>, track>::iterator iter = track_map.begin(); iter != track_map.end(); iter++){
		iter->second.print();
	}
}

void B4_insert(){
	/*
	Current data is in integrity. 
	Next update your program to insert a cd record (below) to create an exception.

	7|Northern Star|5|B00004YMST

	This will create an integrity problem for there is no artist id which is 5.
	Run your program to generate the exception report for this case.

	*/

	
	int cd_id = 7;
	char *title = "Northern Star";
	int artist_id = 5;
	char *catalogue = "B00004YMST";

	cd B4(cd_id, title, artist_id, catalogue);
	artist *test;
	test = B4.getArtist(artist_id, &artist_map);
	if (test == NULL){
		printf("\n Error adding new cd\n");
	}
	else{
		cd_map.insert(std::pair<int, cd>(cd_id, B4));
	}

}