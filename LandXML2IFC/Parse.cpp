// LandXML2IFC.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include "parse.h"

#include "assert.h"


#define	LIST_SIZE	4096//65536
char	list[1024], _list[LIST_SIZE + 10];
int		listIndex = 0,
		current_index,
		TOTAL_LINES_READ,
		TOTAL_CHARS_READ,
		readPos;
size_t	numread;
bool	eof;
FILE	* global_fp;
static	char				returned_when_eof = ';';

void	InitGetByte(
				FILE	* fp
			)
{
	numread = fread( _list, sizeof( char ), LIST_SIZE, fp );
	if (numread) {
		listIndex = 0;
		list[0] = _list[listIndex++];
		while (list[0] != '<' && listIndex < LIST_SIZE) {
			list[0] = _list[listIndex++];
		}
		numread--;
	}
	current_index = 0;
	readPos = 0;
	eof = false;

	global_fp = fp;
}

char	* GetByte(
			)
{
	if (current_index >= 1) {
		if (numread == 0 || (listIndex < 0  ||  listIndex >= LIST_SIZE)) {
			numread = fread( _list, sizeof( char ), LIST_SIZE, global_fp );
			listIndex = 0;
			if (numread == 0) {
				eof = true;
				current_index = 1;
				return  &returned_when_eof;
			}
			assert(numread <= LIST_SIZE);
		}
		list[0] = _list[listIndex++];
		numread--;
		current_index = 0;
	}

	if (list[current_index] == 10) {
		TOTAL_LINES_READ++;
		TOTAL_CHARS_READ = 0;
	}
	else {
		TOTAL_CHARS_READ++;
	}

	return	&list[current_index++];
}

void	UndoGetByte(
			)
{
	assert(current_index > 0);
	current_index--;
}

