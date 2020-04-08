#include <stdio.h>

typedef enum {
    False, // now false means 0
    True, // now true means 1
    ExtraOption, // ExtraOption means 2
    AnOtherExtra, // AnOtherExtra means 3
}MyBool;

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
