#include <regex.h>
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <execute_command.h>

void expandWildcardsIfNecessary(char * arg)
{
	//Return if arg does not contain "*" or "?"
	if (arg has neither "*" nor "?" (use strchr) ) {
		Command._currentSimpleCommand->insertArgument(arg);
		return;
	}

	// 1. Convert wildcard to regular expression
	// Convert "*" -> ".*"
	// "?" -> "."
	// "." -> "\." and others you need
	// Also add ^ at the beginning and $ at the end to match
	// the beginning and the end of the word.
	// Allocate enough space for regular expression
	char * reg = (char*)malloc(2*strlen(arg)+10);
	char * a = arg;
	char * r = reg;
	*r = '^'; r++; //match beginning of line
	while (*a) {
		if (*a == '*') { *r='.'; r++; *r='*'; r++; }
		else if (*a == '?') { *r='.' r++; }
		else if (*a == '.') { *r='\\'; r++; *r='.'; r++;}
		else { *r=*a; r++;}
		a++;
	}
	*r='$'; r++; *r=0; //match end of line and add null char
	// 2. compile regular expression. See lab3-src/regular.cc
	char * expbuf = regcomp( reg, REG_EXTENDED|REG_NOSUB );
	if (expbuf==NULL) {
		perror("regcomp");
		return;
	}
	// 3. List directory and add as arguments the entries
	// that match the regular expression
	DIR * dir = opendir(".");
	if (dir == NULL) {
		perror("oopendir");
		return;
	}
	struct dirent * ent;
	while ( (ent = readdir(dir)) != NULL) {
		// Check if name matches
		if (regexec(ent->d_name, re ) == 0 ) {
		//Add argument
		Command._currentSimpleCommand->insertArgument(strdup(ent->d_name));
		}
	}
	closedir(dir);
}

const char * usage = ""
"Usage:\n"
"      regular regular-expresion string\n"
"\n"
"      Tells if \"string\" matches \"regular-expresion\".\n"
"\n"
"      '^' and '$' characters are added at the beginning and\n"
"      end of \"regular-expresion\" to force match of the entire\n"
"      \"string\"\n"
"\n"
"	To know more about regular expresions type \"man ed\"\n"
"Try:\n"
"      bash> ./regular aaa aaa\n"
"      bash> ./regular \"a*\" aaaa\n"
"      bash> ./regular \"a*\" aaa\n"
"      bash> ./regular \"a*\" aaaf\n"
"      bash> ./regular \"a.*\" akjhkljh \n"
"      bash> ./regular \"a.*\" jkjhkj\n"
"      bash> ./regular \"a.*\" aaalklkjlk\n"
"      bash> ./regular \".*\\..*\" kljhkljhlj.lkjhlkj\n"
"      bash> ./regular \".*\\..*\" kljhkljhlj\n\n";

int main(int argc, char ** argv)
{
	if ( argc < 3 ) {
		fprintf( stderr, "%s", usage );
		exit( -1 );
	}

	const char * regularExp = argv[1];
	const char * stringToMatch = argv[2];

	/*
	 *  Add ^ and $ at the beginning and end of regular expression
	 *  to force match of the entire string. See "man ed".
	 */
	char regExpComplete[ 1024 ];
	sprintf(regExpComplete, "^%s$", regularExp );

	regex_t re;	
	int result = regcomp( &re, regExpComplete,  REG_EXTENDED|REG_NOSUB);
	if( result != 0 ) {
		fprintf( stderr, "%s: Bad regular expresion \"%s\"\n",
			 argv[ 0 ], regExpComplete );
		exit( -1 );
      	}

	regmatch_t match;
	result = regexec( &re, stringToMatch, 1, &match, 0 );

	const char * matchResult = "MATCHES";
	if ( result != 0 ) {
		matchResult = "DOES NOT MATCH";
	}

	fprintf( stderr, "\t\"%s\" %s \"%s\"\n", matchResult,
		regExpComplete, stringToMatch);

	regfree(&re);

	return 0;
}
	
