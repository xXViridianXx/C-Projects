# Assignment 4 - Game of Life

#Summary:
This program uses abstract data types in order to create a 2d grid where cells live in and either live or die based on certain requirements. The requirements are: Any live cell with 2 or 3 neighbors survives to the next generation, Any dead cell with exactly 3 neighbors becomes alive, and all other cells die due to overcrowding or lonliness. 

#Run:
To the run the program, one can type "./life <-i file name> <-s silence ncurses animation> <-o choose output> <-n generations>"
-i is the only needed flag, while the others have default values set. Use make to compile, make clean to remove .o files, and make format to format code. 

#Expected Output:
The code should show an animation of all the generations the cells underwent if ncurses wasn't silenced. The final generation should be printed on the output file regardless. 
#Error Handling/Errors:
If the user does not input a generation number, then the default will be 100. If the user, doesn't input a file, then an error appears telling the user that no file was inputted. The input file is stdin by default and the output file is stdout by default as well.

#Scan-build and Valgrind()
Scan-build make showed no possible or existing errors, and valgrind showed no possible memory leaks.
