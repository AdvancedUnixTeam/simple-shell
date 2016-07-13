%token NOTOKEN, GREAT, NEWLINE, WORD, GREATGREAT, PIPE, AMPERSAND

Before:
	argument:WORD{
		Command._currentSimpleCommand->insertArgument($1);
	};

After:
	argument:WORD{
		expandWildcardsIfNecessary($1);
	};
