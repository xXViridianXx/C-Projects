# Assignment 3 - Sorting

#Summary:
This program uses a self built sorting library that can be called by the user. The sorts represented are insertion, heap, batcher, and quick. 

#Run:
To the run the program, one can type "./sorting <sorting algorithim> <-r seed> <-n size> <-p elements to print>"

#Expected Output:
The code should print out the array elements, 100 by default if the user doesn't input anything, and the user's inputs.
#Error Handling/Errors:
If the user does not input an algorithim, the program will fail because you need a sorting algorithim to...sort. If the user does not provide any other info, a print message will show that default values were used.

#Scan-build and Valgrind()
This showed no errors on scan-build and no leaks on valgrind.
