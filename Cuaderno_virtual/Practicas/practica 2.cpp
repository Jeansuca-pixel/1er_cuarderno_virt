#include <iostream>
#include <math.h>
using namespace std;
int main()
{
   int a,b,c,U,D,;
   cout<<"ingr"<<endl;
   cin>>a;
   cin>>b;
   c=a/10;
   U=a%10; //U=4
   c=(c*10)+b;//c=1230+5
   c=(c*10)+U; //c=12350+4
  
  cout<<"res"<<c;
    return 0;
}  