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
char * expbuf = regcomp( reg, â€| );
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
