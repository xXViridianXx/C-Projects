# Assignment 2 - Numerical Integration

#Summary:
This program uses a self built math library called mathlib.c that gets used by the main function integrate.c The user is prompted to enter some values in order to integrate a certain function. This integration is done using Simpson's 1/3 rule with a lower and upper bound, along with partitions. Exp(), Sin(), and Cos were integrated using taylor series. Sqrt() and Log() were integrated using Newton's method. 

#Run:
To the run the program, one can type "./integrate <-a-j> <-p #> <-q #> <-n #>" if one has the make file or run it manually by using "clang -Wall -Wextra -Werror -Wpedantic -o plot plot.sh.

#Expected Output:
The code should output the user's chosen filters for the integration and then integrate every 2 partitions. The goal is to show that as the number of partitions increases, so does the accuracy. This, however, isn't true for Log(Log(x)) -- which obtains it's maximal accuracy using 100 partitions.

#Error Handling/Errors:
If the user inputs a number outside of the functions domain, he/she will be promted with an error -- text will pop up showing the proper domains for the functions. If the user doesn't input an equation to integrate, he/she will be told to enter a function. If the user doesn't input a lower or upper bound, they will also be prompted with an error. The user will also get an error if he/she inputs a keyword not inlcluded, such as -z. Typing -H brings the help menu.
