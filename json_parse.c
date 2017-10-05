/*-------------------------------------------------------------------------
  # Student's Name: Kevin Ho
  # CMPT 361 Assignment #1
  # json_server.c
  # Functions that parse the data received
*-----------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include "./jsmn/jsmn.h"

//Check if HTTP Request was valid or not
int check_request(char *buff){
	//Check if valid HTTP Request. Parse data if valid.
	if (strstr(buff, "HTTP/1.1 200 OK\r\n") != NULL){
		return 0;
	}
	//HTTP Request wasn't valid. Return status code
	else {
		return 1;
	}
}


//Parse JSON data. Return parsed JSON
char *json_data(char *buff, char *key){
	char *token;
	int result;
	int i;
	jsmn_parser parser;
	jsmntok_t tokens[500];
	jsmntok_t current_key;	
	
	char *data = malloc(100000 * sizeof(char));
	if (data == NULL){
		fprintf(stderr, "Malloc Failed\n");
		exit(1);
	}
	
	token = strtok(buff, "{");
	token = strtok(NULL, "\0");
	
	strcat(data, "{");
	strcat(data, token);
	strcat(data, "\0");
	
	jsmn_init(&parser);
	
	result = jsmn_parse(&parser, data, strlen(data), tokens,
			sizeof(tokens)/sizeof(tokens[0]));
	if (result < 0) {
		printf("Failed to parse JSON: %d\n", result);
		exit(1);
	}

	/*Refernece taken from to convert jsmn tokens
	https://stackoverflow.com/questions/14388692/how-to-parse-a-small-json-file-with-jsmn-on-an-embedded-system
	*/
	//Loop over all tokens and compare with key
	for (i = 1; i < result; i++){
		current_key = tokens[i];
		unsigned int length = current_key.end - current_key.start;
		char keyString[length + 1];    
		memcpy(keyString, &data[current_key.start], length);
		keyString[length] = '\0';
		
		//Key found. Move to next token for value
		if ((strcmp(keyString, key) == 0)){
			current_key = tokens[i + 1];
			unsigned int length = current_key.end - current_key.start;
			char keyString[length + 1];    
			memcpy(keyString, &data[current_key.start], length);
			keyString[length] = '\0';
			strcpy(key, keyString);
			return key;
		}
	}
	
	//If out of for loop, key not found
	key = "\0";
	return key;
}