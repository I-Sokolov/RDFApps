 #include "stdafx.h"
#include "generic.h"


bool	equals(wchar_t * txtI, wchar_t * txtII)
{
	int_t i = 0;
	if (txtI && txtII) {
		while (txtI[i]) {
			if (txtI[i] != txtII[i]) {
				return	false;
			}
			i++;
		}
		if (txtII[i]) {
			return	false;
		}
	} else if (txtI || txtII) {
		return	false;
	}
	return	true;
}
/*
wchar_t	* __copy__(wchar_t * txt)
{
	if	(txt) {
		int_t	i = 0;
		while  (txt[i]) { i++; }
		wchar_t	* rValue = new wchar_t[i+1];
		i = 0;
		while  (txt[i]) {
			rValue[i] = txt[i];
			i++;
		}
		rValue[i] = 0;

		return	rValue;
	} else {
		return	0;
	}
}*/
