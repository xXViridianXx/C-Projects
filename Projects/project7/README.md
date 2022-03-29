# Assignment 7 - Author Identification

#Summary:
This program takes in a user inputed message compares it with the diction of other writers in a database. The program then guesses who the unknown author most likely is.
#Run:
To the run the program, one can type "make clean all" in order to compile the whole program. You can either use the terminal or another file to encode. Either use < to use a file or type something into stdin. If using stdin, finish the message with ctr+d to signify end file. Use -h for help and other options.

#Expected Output:
The code should output a list of likely authors starting from most likely to least likely.

#Error Handling/Errors:
If the user inputs a "NULL" file , then the program spits out an error saying the file was empty. Otherwise defaults are used.
#Scan-build
scan-build gives 2 warnings and 1 error. The first warning is: warning: Result of 'calloc' is converted to a pointer of type 'Node *', which is incompatible with sizeof operand type 'Node' and second is: Result of 'calloc' is converted to a pointer of type 'PQEntry *', which is incompatible with sizeof operand type 'PQEntry' which I don't understand why because that is how I normally allocate memory. The error seems to be a false memory leak because I have no leaks when unsing valgrind.

#Valgrind
Gives no Leaks
