all: part1 part2


part1:	part1.cpp
	g++ part1.cpp -o ./build/part1  -l sqlite3

part2:	artist.h cd.h track.h artist.cpp cd.cpp track.cpp part2.cpp
	g++ artist.cpp cd.cpp track.cpp part2.cpp -o ./build/part2 -l sqlite3
clean:
	rm -f songs.db *~; rm -f part1 *~; rm -f part2 *~

