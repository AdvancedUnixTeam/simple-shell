%token NOTOKEN PIPE GREAT NEWLINE WORD GREATGREAT AMPERSAND GREATGREATAMPERSAND GREATAMPERSAND LESS

%%
Before:
	argument:WORD{
		Command._currentSimpleCommand->insertArgument($1);
	};

After:
	argument:WORD{
		expandWildcardsIfNecessary($1);
	};

/*goal: command_list;*/

arg_list:
		arg_list WORD
		| /*empty*/
				;

cmd_and_args:
		WORD arg_list
				;

pipe_list:
		pipe_list PIPE cmd_and_args
		| cmd_and_args
				;

io_modifier:
		GREATGREAT Word
		| GREAT Word
		| GREATGREATAMPERSAND Word
		| GREATAMPERSAND Word
		| LESS Word
			;
%%

#include "lex.yy.c"

main() {
	return yyparse();
}
int yyerror( char *s ) { fprintf( stderr, "%s\n", s); }

