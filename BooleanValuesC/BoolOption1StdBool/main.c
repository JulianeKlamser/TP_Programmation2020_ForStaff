#include <stdio.h>
#include <stdbool.h>

int main() {
    
    bool testBool = true;
    if ( testBool == true ) printf("testBool == %d means true\n", testBool);
    
    testBool = false;
    if ( testBool == false ) printf("testBool == %d means false\n", testBool);
    
    int test = 1;
    if ( test == true ) printf("test == %d means true\n", test);
    else if ( test == false) printf("test == %d means false\n", test);
    else printf("test == %d is neither false nor true \n", test);
    
    
    test = 0;
    if ( test == true ) printf("test == %d means true\n", test);
    else if ( test == false) printf("test == %d means false\n", test);
    else printf("test == %d is neither false nor true \n", test);
    
    test = 2;
    if ( test == true ) printf("test == %d means true\n", test);
    else if ( test == false) printf("test == %d means false\n", test);
    else printf("test == %d is neither false nor true \n", test);
    
    double testD = 0;
    if ( testD == true ) printf("testD == %f means true but is dangerous\n", testD);
    else if ( testD == false) printf("testD == %f means false but is dangerous\n", testD);
    else printf("testD == %f is neither false nor true \n", testD);
    
    testD = 1;
    if ( testD == true ) printf("testD == %f means true but is dangerous\n", testD);
    else if ( testD == false) printf("testD == %f means false but is dangerous\n", testD);
    else printf("testD == %f is neither false nor true \n", testD);
    
    
   return 0;
}
