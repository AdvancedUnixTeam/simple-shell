/*shell.l: simple lexical analyzer for the shell.
*/

%{

#include <string.h>
#include "y.tab.h"
#include "commands.h"

%}

%%

\n 	{
        	return NEWLINE;

   	}

[ \t]   {
            /* Discard spaces and tabs */
        }

">"     {
            return GREAT;
        }

">>"	{
        	return GREATGREAT;
	}

">&"	{
		return GREATAMPERSAND;
	}

">>&"	{
		return GREATGREATAMPERSAND;
	}

"<"	{
		return LESS;
	}

"&"	{
		return AMPERSAND;
	}

"|"	{
		return PIPE;
	}

[^ \t\n][^ \t\n]*	{
 			/* Assume that file names have only alpha chars */ 
				yylval.string_val = strdup(yytext);
				return WORD;
			}

.       {
            /*Invalid character in input*/
            return NOTOKEN;
        }

%%

int yywrap() { return 1; }

