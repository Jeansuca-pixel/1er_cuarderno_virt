#include <iostream>
#include <math.h>
using namespace std;
int main()
{
    int a,b,c,U,D,C; 
    a=123;
    U=a % 10 ;
    D=a / 10 ;
    D=D%10;
    C=a/100;
    
    cout<<"res:"<<D+U+C;
    return 0;
}  