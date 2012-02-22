
#include <stdio.h>

char* insert(int num) {

	const char* statement = "insert into data values(";

	int boiler_len = strlen(statement);

	int bind_len =  2 * num;

	int full_len = boiler_len + bind_len;

	char* result = malloc(sizeof(char) * full_len);

	strncpy(result, statement, boiler_len);

	int i;
	for(i = boiler_len; i < full_len; i++) {
		result[i]   = '?';
		result[i+1] = ',';
		i++;
	}

	result[full_len-1] = ')';
	result[full_len]   = '\0';

	return result;
}
