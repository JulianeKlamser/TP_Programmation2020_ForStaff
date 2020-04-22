#include <stdio.h>

typedef int MyBool; //I define my own variable type. MyBool is used like int and double. Here I defiend that MyBool is an int.
#define True 1
#define False 0
#define ExtraOption -1
#define AnOtherExtra 99875

int main() {
   
    printf("My own clear boolean definition without <stdbool.h>\n :) \n");

    MyBool testBool = True;
    if ( testBool == True ) printf("testBool == %d means True\n", testBool);

    testBool = False;
    if ( testBool == False ) printf("testBool == %d means False\n", testBool);

    testBool = ExtraOption;
    if ( testBool == ExtraOption ) printf("testBool == %d means ExtraOption\n", testBool);
    
    testBool = AnOtherExtra;
    if ( testBool == AnOtherExtra ) printf("testBool == %d means AnOtherExtra\n", testBool);
    
    return 0;
}
