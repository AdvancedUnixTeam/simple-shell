// Use extern for values used in command table

// Command Data Structure 
// Describes a simple command and arguments 
struct *SimpleCommand { 
        // Available space for arguments currently preallocated 
        int _numberOfAvailableArguments; 
        // Number of arguments 
        int _numberOfArguments; 
        // Array of arguments 
        char ** _arguments; 
        SimpleCommand(); // not implemented
        void insertArgument( char * argument ); // not implemented
}; 

// Describes a complete command with the multiple pipes if any 
// and input/output redirection if any. 
struct *Command { 
        int _numberOfAvailableSimpleCommands; 
        int _numberOfSimpleCommands; 
        SimpleCommand ** _simpleCommands; 
        char * _outFile; 
        char * _inputFile; 
        char * _errFile; 
        int _background; 
        void *prompt();  // not implemented
        void *print(); 	// not implemented
        void execute(); 
        void clear(); // not implemented
        Command(); // not implemented
        void insertSimpleCommand( SimpleCommand * simpleCommand ); // not implemented
        static Command _currentCommand; 
        static SimpleCommand *_currentSimpleCommand; 
}; 
