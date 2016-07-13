hell.l: simple lexical analyzer for the shell.
 */

%{

#include <string.h>
#include "y.tab.h"

%}

%%

\n {
        return NEWLINE;

   }

[ \t]   {
                /* Discard spaces and tabs */
        }

">"     {
                return GREAT;
        }

.       {
                /*Invalid character in input*/
                return NOTOKEN
        }

%%

