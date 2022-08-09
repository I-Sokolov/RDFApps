#pragma once


#define		CASE_LOWER_CASE  case  'a': case  'b': case  'c': case  'd': case  'e': case  'f': case  'g': case  'h': case  'i': case  'j': case  'k': case  'l': case  'm': case  'n': case  'o': case  'p': case  'q': case  'r': case  's': case  't': case  'u': case  'v': case  'w': case  'x': case  'y': case  'z':
#define		CASE_UPPER_CASE  case  'A': case  'B': case  'C': case  'D': case  'E': case  'F': case  'G': case  'H': case  'I': case  'J': case  'K': case  'L': case  'M': case  'N': case  'O': case  'P': case  'Q': case  'R': case  'S': case  'T': case  'U': case  'V': case  'W': case  'X': case  'Y': case  'Z':
#define		CASE_SPECIAL_CASE  case  '_': case  '\\': case  '.':
#define		CASE_NUMBER		 case  '0': case  '1': case  '2': case  '3': case  '4': case  '5': case  '6': case  '7': case  '8': case  '9':
#define		CASE_SPACE		 case  9:   case  10:  case  13:  case  ' ':
#define		CASE_ENTITY_CHAR case  '_': case  '-':
#define     CASE_HEXADECIMAL case  '0': case  '1': case  '2': case  '3': case  '4': case  '5': case  '6': case  '7': case  '8': case  '9': case  'A': case  'B': case  'C': case  'D': case  'E': case  'F':


void	InitGetByte(
				FILE	* fp
			);

char	* GetByte(
			);

void	UndoGetByte(
			);
