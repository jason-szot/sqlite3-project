#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <sqlite3.h>
#include <iostream>

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for (i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

/*	input format:
	artist: 1|Pink Floyd
	cd: 1|Dark Side of the Moon|1|B000024D4P
	track: 1|1|Speak to me
*/

	///		Function Prototpes		///

void populate_sql(void);
int menu(void);
void del_records(void);
void update_records(void);
void report();
void output(char tbl);

int main(){
	int choice;

	// populate the database
	/*	read file
		create insert statements
		execute sql statements
		close file */
	populate_sql();

	// menu //
	/*	Options
	**	1. Update record
	**	2. Delete all records
	**	3. Quit
	*/
	while (choice = menu()){
		if (choice == 4){
			return 0;
		}
		if (choice == 3){
			report();
		}
		else if (choice == 2){
			del_records();
		}
		else if (choice == 1){
			update_records();
		}
	}

	return 0;
}

void populate_sql(){
	bool eof = false;

	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;
	char* insert;
	char* token;
	std::string line_str;

	// open database
	rc = sqlite3_open("song.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	// part A1 read the file
	std::ifstream input;
	input.open("artist.txt");
	char* line;
	// part A2 create and insert
	sql = "create table artist (" \
		  "id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY," \
		  "name VARCHAR(100) NOT NULL);";
	// execute sql statement
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}

	while (!input.eof()){
		insert = (char*)malloc(1024 * sizeof(char));
		getline(input, line_str);
		line = (char*)line_str.c_str();
		// clear new line
		for (int i = 0; i < strlen(line); i++){
			if (line[i] == '\n'){
				line[i] = '\0';
			}
		}
		// create insert statement
		strcpy(insert, "insert into artist(id, name) values(");
		token = strtok(line, "|");
		strcat(insert, token);
		strcat(insert, ", '");
		token = strtok(NULL,"|");
		strcat(insert, token);
		strcat(insert, "');");

		rc = sqlite3_exec(db, insert, callback, 0, &zErrMsg);
		if (rc != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else{
			fprintf(stdout, "Records created successfully\n");
		}
		free(insert);
	}
	input.close();
	

	//////// CD table stuff

	input.open("cd.txt");
	// part A2 create and insert
	sql = "create table cd ("	\
		"id INTEGER AUTO_INCREMENT NOT NULL PRIMARY KEY,"	\
		"title VARCHAR(70) NOT NULL,"	\
		"artist_id INTEGER NOT NULL,"	\
		"catalogue VARCHAR(30) NOT NULL );";
	// execute sql statement
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}

	while (!input.eof()){
		insert = (char*)malloc(1024 * sizeof(char));
		getline(input, line_str);
		line = (char*)line_str.c_str();
		// create insert statement
		strcpy(insert, "insert into cd(id, title, artist_id, catalogue) values(");
		token = strtok(line,"|");
		strcat(insert, token);
		strcat(insert, ", '");
		token = strtok(NULL, "|");
		strcat(insert, token);
		strcat(insert, "', ");
		token = strtok(NULL, "|");
		strcat(insert, token);
		strcat(insert, ", '");
		token = strtok(NULL, "|");
		strcat(insert, token);
		strcat(insert, "');");

		rc = sqlite3_exec(db, insert, callback, 0, &zErrMsg);
		if (rc != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else{
			fprintf(stdout, "Records created successfully\n");
		}
		free(insert);

	}
	input.close();

	// track table stuff

	input.open("track.txt");
	// part A2 create and insert
	sql = "create table track ("	\
		"cd_id INTEGER NOT NULL,"	\
		"track_id INTEGER NOT NULL,"	\
		"title VARCHAR(70),"	\
		"PRIMARY KEY(cd_id, track_id) );";
	// execute sql statement
	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Table created successfully\n");
	}
	while (!input.eof()){
		insert = (char*)malloc(1024 * sizeof(char));
		getline(input, line_str);
		line = (char*)line_str.c_str();
		// create insert statement
		strcpy(insert,"insert into track(cd_id, track_id, title) values(");
		token = strtok(line, "|");
		strcat(insert, token);
		strcat(insert, ", ");
		token = strtok(NULL, "|");
		strcat(insert, token);
		strcat(insert, ", '");
		token = strtok(NULL, "|");
		strcat(insert, token);
		strcat(insert, "');");

		rc = sqlite3_exec(db, insert, callback, 0, &zErrMsg);
		if (rc != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		else{
			fprintf(stdout, "Records created successfully\n");
		}
		free(insert);

	}
	input.close();

	// tables set up and populated at this point
}

int menu(){
	int choice;
	std::string line;
	size_t bufsize = 0;
	do{
		printf("Menu:\n");
		printf("1. Update record\n");
		printf("2. Delete all records\n");
		printf("3. Report\n");
		printf("4. Quit\n");
		printf("Enter a choice: ");
		getline(std::cin,line);
		choice = atoi(line.c_str());
	} while (choice != 1 && choice != 2 && choice != 3 && choice != 4);
	return choice;
}

void del_records(void){
	char* del_c = "delete from cd;";
	char* del_a = "delete from artist;";
	char* del_t = "delete from track;";
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;

	// open database
	rc = sqlite3_open("song.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	// execute sql commands to delete records
	rc = sqlite3_exec(db, del_a , callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Records deleted successfully\n");
	}

	rc = sqlite3_exec(db, del_c , callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Records deleted successfully\n");
	}

	rc = sqlite3_exec(db, del_t , callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Records deleted successfully\n");
	}
}

void update_records(void){
	char* update = "update track set title = 'Money' where cd_id=1 and track_id=6;";
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	char *sql;

	// open database
	rc = sqlite3_open("song.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	// execute sql update
	rc = sqlite3_exec(db, update , callback, 0, &zErrMsg);
	if (rc != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		sqlite3_free(zErrMsg);
	}
	else{
		fprintf(stdout, "Record updated successfully\n");
	}

}

void report(){
	output('A');
	output('C');
	output('T');
}

void output(char tbl){
	char* type;
	char* query;
	sqlite3 *db;
	char *zErrMsg = 0;
	int rc;
	std::ofstream outputfl;
	switch (tbl){
	case 'A':
	{
		type = "artist";
		outputfl.open("artist.report.txt");
		outputfl << "id | name\n";
		query = "select * from artist";
		break;
	}
	case 'C':
	{
		type = "cd";
		outputfl.open("cd.report.txt");
		outputfl << "id | title | artist_id | catalogue\n";
		query = "select * from cd";
		break;
	}
	case 'T':
	{
		type = "track";
		outputfl.open("track.report.txt");
		outputfl << "cd_id | track_id | title\n";
		query = "select * from track";
		break;
	}
	}	// end of switch

	// open database
	rc = sqlite3_open("song.db", &db);
	if (rc){
		fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		exit(0);
	}
	else{
		fprintf(stdout, "Opened database successfully\n");
	}

	// prepare sql statement and output data to ofstream;

	sqlite3_stmt *statement;

	if (sqlite3_prepare(db, query, -1, &statement, 0) == SQLITE_OK)
	{
		int ctotal = sqlite3_column_count(statement);
		int res = 0;
		while (1)
		{
			res = sqlite3_step(statement);
			if (res == SQLITE_ROW)
			{
				for (int i = 0; i < ctotal; i++)
				{
					std::string s = (char*)sqlite3_column_text(statement, i);

					// output code here with formatting
					outputfl << s;
					if (i < ctotal - 1){
						outputfl << " | ";
					}
				}
				outputfl << "\n";
			}
			if (res == SQLITE_DONE || res == SQLITE_ERROR)
			{
				printf("done with %s table\n", type);
				break;
			}
		}	// end of while
	}	// end of if prepare
	outputfl.close();

}	// end of output