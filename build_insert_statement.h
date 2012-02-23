#include <stdio.h>
#include <math.h>

char* insert(int num) {

	// TODO: This is quite fragile be careful if modifying
	const char* dstmt      = "insert into data()";
	const char* vstmt      = " values()";
	const int   dlen       = strlen(dstmt) - 1;
	const int   vlen       = strlen(vstmt) - 1;
	const int   boiler_len = dlen + vlen;
	const int   dyn_factor = 1 + (int) log10(num);
	const int   dyn_len    = dyn_factor * num; // represents the size of the column-number text (minus formatting) (overshoots estimate by log(n) factor)
	char*       dyn_part   = malloc(sizeof(char) * dyn_factor + 3); // constant is 'c' ',' '\0'
	char*       result     = malloc(sizeof(char) * (boiler_len + (3 * num) + dyn_len));
	char*       p          = result; // movable pointer for printing

	memcpy(p, dstmt, dlen);
	p += dlen;

	int i;
	for(i = 1; i <= num; i++) {
		sprintf(dyn_part, "c%d,", i);
		sprintf(p,        "%s",   dyn_part);

		p += ((int) log10(i)) + 1 + 2; // constant is the length of 'c' ',' the first digit, and one more to advance to next position (where '\0' is)
	}
	p[-1] = ')';

	memcpy(p, vstmt, vlen);
	p += vlen;

	for(i = 0; i < num; i++) {
		*p = '?'; p++;
		*p = ','; p++;
	}

	p[-1] = ')';
	p[ 0] = '\0';

	return result;
}
