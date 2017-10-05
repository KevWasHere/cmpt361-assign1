#ifndef JSON_PARSE_H_
#define JSON_PARSE_H_

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "./jsmn/jsmn.h"

/*Prototypes*/
int check_request(char *buff);
char *json_data(char *buff, char *key);
#endif