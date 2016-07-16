// Use extern for values used in command table

#include <commands.h>

// Command Data Structure 
// Describes a simple command and arguments 
struct Command { 
        // Available space for arguments currently preallocated 
        int _numberOfAvailableArguments; 
        // Number of arguments 
        int _numberOfArguments; 
        // Array of arguments 
        char ** _arguments; 
        void (*Command)(); // constructor -- Not Implemented
        void (*insertArgument)(char * argument); // Inserts Arguments into a Command -- Not Implemented
}; 

// Describes a complete command with the multiple pipes if any 
// and input/output redirection if any. 
struct CommandTable { 
        int _numberOfAvailableSimpleCommands; 
        int _numberOfSimpleCommands; 
        SimpleCommand ** _simpleCommands; 
        char * _outFile; 
        char * _inputFile; 
        char * _errFile; 
        int _background; 
        void (*prompt)(void);  // Not Sure -- Not Implemented
        void (*print)(void); 	// Not Sure -- Not Implemented
        void (*execute)(void);  // Executes the command table -- Needs to be refactored
        void (*clear)(void); // Not Sure -- Not Implemented
        Command(); // not implemented
        void (*insertCommand)(Command * command ); // Inserts a Command into the Command Table -- Not Implemented
        static CommandTable _currentCommandTable; 
        static Command *_currentCommand; 
}; 







