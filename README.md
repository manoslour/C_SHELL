# C_SHELL

A simple shell implementation.

- [x] Simple Commands
- [x] Complex Commands
- [x] Built-in Commands
- [x] Redirection
- [x] Pipes
- [ ] Shortcuts


### Shell.c

Shell reads input by line, and calls the appropriate function, based on the delimeter commands are separated with.  
If no delimeter is used, a function for basic shell commands is called.

* INPUT(0), OUTPUT(1), APPEND(2) are defined in functions.h

#### Run Shell:

$ make  
$ ./cs345h